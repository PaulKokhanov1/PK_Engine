#pragma once

#include "Logging.h"

#define LogMeshInfo(msg) Log(LogLevel::Info, string("[Mesh] ") + msg);
#define LogMeshWarn(msg) Log(LogLevel::Warn, string("[Mesh] ") + msg);
#define LogMeshError(msg) Log(LogLevel::Error, string("[Mesh] ") + msg);