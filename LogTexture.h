#pragma once

#include "Logging.h"

#define LogTextureInfo(msg) Log(LogLevel::Info, string("[Texture] ") + msg);
#define LogTextureWarn(msg) Log(LogLevel::Warn, string("[Texture] ") + msg);
#define LogTextureError(msg) Log(LogLevel::Error, string("[Texture] ") + msg);