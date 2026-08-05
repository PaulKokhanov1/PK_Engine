#pragma once

#include "Logging.h"

#define LogMeshInfo(msg) Log(LogLevel::Info, std::string("[Mesh] ") + msg);
#define LogMeshWarn(msg) Log(LogLevel::Warn, std::string("[Mesh] ") + msg);
#define LogMeshError(msg) Log(LogLevel::Error, std::string("[Mesh] ") + msg);