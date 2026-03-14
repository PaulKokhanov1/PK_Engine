#pragma once

#include "Logging.h"

#define LogShaderManagerInfo(msg) Log(LogLevel::Info, std::string("[ShaderManager] ") + msg);
#define LogShaderManagerWarn(msg) Log(LogLevel::Warn, std::string("[ShaderManager] ") + msg);
#define LogShaderManagerError(msg) Log(LogLevel::Error, std::string("[ShaderManager] ") + msg);