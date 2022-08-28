
#pragma once

#define REDISMODULE_XAPI_ENTERPRISE(X) \
    X(int, AvoidReplicaTraffic, ()) \
    X(int, Fork, (RedisModuleForkDoneHandler cb, void *user_data)) \
    X(int, ExitFromChild, (int retcode)) \
    X(int, KillForkChild, (int child_pid))

#define REDISMODULE_XAPI_EXTENSIONS(X) REDISMODULE_XAPI_ENTERPRISE(X)

#include "RedisModuleSDK/5.0/redismodulex.h"
