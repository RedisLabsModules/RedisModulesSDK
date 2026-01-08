import subprocess
import socket
import redis
import time
import os
import sys
import itertools
from contextlib import contextmanager


REDIS_DEBUGGER = os.environ.get('REDIS_DEBUGGER', None)
REDIS_SHOW_OUTPUT = int(os.environ.get('REDIS_VERBOSE', 1 if REDIS_DEBUGGER else 0))


def get_random_port():
    sock = socket.socket()
    sock.listen(0)
    _, port = sock.getsockname()
    sock.close()

    return port


class Client(redis.StrictRedis):
    def __init__(self, disposable_redis, port):
        redis.StrictRedis.__init__(self, port=port)
        self.dr = disposable_redis

    def retry_with_rdb_reload(self):
        yield 1
        self.dr.dump_and_reload()
        yield 2


class DisposableRedis(object):

    def __init__(self, port=None, path='redis-server', **extra_args):
        """
        :param port: port number to start the redis server on.
            Specify none to automatically generate
        :type port: int|None
        :param extra_args: any extra arguments kwargs will
            be passed to redis server as --key val
        """
        self._port = port

        # this will hold the actual port the redis is listening on.
        # It's equal to `_port` unless `_port` is None
        # in that case `port` is randomly generated
        self.port = None
        self.extra_args = []
        for k, v in extra_args.items():
            self.extra_args.append('--%s' % k)
            if isinstance(v, (list, tuple)):
                self.extra_args += list(v)
            else:
                self.extra_args.append(v)

        self.path = path
        self.dumped = False
        self.errored = False

    def _get_output(self):
        return '' if REDIS_SHOW_OUTPUT else self.process.stdout.read()

    def _start_process(self):
        #print("Starting redis process: {}".format(' '.join(self.args)))
        if REDIS_DEBUGGER:
            debugger = REDIS_DEBUGGER.split()
            args = debugger + self.args
        else:
            args = self.args
        stdout = None if REDIS_SHOW_OUTPUT else subprocess.PIPE
        self.process = subprocess.Popen(
            args,
            stdin=sys.stdin,
            stdout=stdout,
            stderr=sys.stderr
        )

        while True:
            try:
                self.client().ping()
                break
            except redis.ConnectionError:
                self.process.poll()
                if self.process.returncode is not None:
                    raise RuntimeError(
                        "Process has exited with code {}\n. Redis output: {}"
                        .format(self.process.returncode, self._get_output()))

                time.sleep(0.1)

    def start(self):
        """
        Start the server. To stop the server you should call stop()
        accordingly
        """
        if self._port is None:
            self.port = get_random_port()
        else:
            self.port = self._port

        self.dumpfile = 'dump.%s.rdb' % self.port
        self.args = [self.path,
                     '--port', str(self.port),
                     '--save', '',
                     '--dbfilename', self.dumpfile] + self.extra_args

        self._start_process()

    def stop(self):
        self.process.terminate()
        if self.dumped:
            try:
                os.unlink(self.dumpfile)
            except OSError:
                pass

    def __enter__(self):
        self.start()
        return self.client()

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()
        if exc_val or self.errored:
            sys.stderr.write("Redis output: {}\n".format(self._get_output()))

    def dump_and_reload(self):
        """
        Dump the rdb and reload it, to test for serialization errors
        """
        conn = self.client()
        conn.save()
        self.dumped = True
        try:
            conn.execute_command('DEBUG', 'RELOAD')
        except redis.RedisError as err:
            self.errored = True
            raise err

    def client(self):
        """
        :rtype: redis.StrictRedis
        """
        return Client(self, self.port)
