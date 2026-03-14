#pragma once

#include "Logging.h"

#define LogTextureInfo(msg) Log(LogLevel::Info, std::string("[Texture] ") + msg);
#define LogTextureWarn(msg) Log(LogLevel::Warn, std::string("[Texture] ") + msg);
#define LogTextureError(msg) Log(LogLevel::Error, std::string("[Texture] ") + msg);