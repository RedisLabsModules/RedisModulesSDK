from subprocess import Popen, PIPE
import os
import os.path
import sys
from argparse import ArgumentParser

from _disposableredis import DisposableRedis

TESTMODULE_SO = os.environ.get('TESTMODULE_SO', '__testmodule.so')
TESTMODULE_SRC = os.path.join(os.path.dirname(__file__), 'testmodule.c')
ap = ArgumentParser()
ap.add_argument('-i', '--source', help='C module source', default=TESTMODULE_SRC)
ap.add_argument('-m', '--module', help='C module name', default=TESTMODULE_SO)
ap.add_argument('-f', '--filter', help='Filter expression for tests', default='*')
ap.add_argument('-N', '--no-compile', help="Don't compile module. Just run tests.",
                default=False, action='store_true')


def compile_module(opts, args):
    # Flags passed to compiler/linker
    flags = [
        '$(CC)', '$(CFLAGS)', '$(CPPFLAGS)', '$(SHOBJ_CFLAGS)',
        '$(SHOBJ_CPPFLAGS)', '-o', '$@', '$^',
        '$(LDFLAGS)', '$(SHOBJ_LDLFAGS)',
    ]

    if sys.platform == 'darwin':
        flags.append('-bundle -undefined dynamic_lookup')

    flags += ['-lc']

    mktest = [
        'all: {0}'.format(opts.module),
        '',
        '{0}: {1} '.format(opts.module, opts.source) + ' '.join(args),
        "\t" + ' '.join(flags),
        ''
    ]
    mktest = "\n".join(mktest)

    po = Popen(['make', '-f', '-'],
               stdin=PIPE, stdout=sys.stdout, stderr=sys.stderr)
    po.communicate(input=mktest)
    if po.returncode != 0:
        raise Exception("Couldn't compile module!")


if __name__ == '__main__':
    opts, args = ap.parse_known_args()

    if not opts.no_compile:
        compile_module(opts, args)

    r = DisposableRedis(loadmodule=opts.module,
                        path=os.environ.get('REDIS_PATH', 'redis-server'))
    r.start()
    try:
        rv = r.client().execute_command('tm.runtests', opts.filter)
        if rv != 0:
            raise Exception('Tests failed!')
    finally:
        pass
