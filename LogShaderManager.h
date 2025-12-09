#pragma once

#include "Logging.h"

#define LogShaderManagerInfo(msg) Log(LogLevel::Info, string("[ShaderManager] ") + msg);
#define LogShaderManagerWarn(msg) Log(LogLevel::Warn, string("[ShaderManager] ") + msg);
#define LogShaderManagerError(msg) Log(LogLevel::Error, string("[ShaderManager] ") + msg);