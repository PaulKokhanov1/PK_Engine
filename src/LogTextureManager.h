#pragma once

#include "Logging.h"

#define LogTextureManagerInfo(msg) Log(LogLevel::Info, std::string("[TextureManager] ") + msg);
#define LogTextureManagerWarn(msg) Log(LogLevel::Warn, std::string("[TextureManager] ") + msg);
#define LogTextureManagerError(msg) Log(LogLevel::Error, std::string("[TextureManager] ") + msg);