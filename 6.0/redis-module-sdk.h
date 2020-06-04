
#pragma once

#define REDISMODULE_XAPI_ENTERPRISE(X) \
    X(int, AvoidReplicaTraffic, ())

#define REDISMODULE_XAPI_EXTENSIONS(X) REDISMODULE_XAPI_ENTERPRISE(X)

#include "RedisModuleSDK/6.0/redismodulex.h"
