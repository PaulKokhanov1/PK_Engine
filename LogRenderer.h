#pragma once

#include "Logging.h"

#define LogRendererInfo(msg) Log(LogLevel::Info, std::string("[Renderer] ") + msg);
#define LogRendererWarn(msg) Log(LogLevel::Warn, std::string("[Renderer] ") + msg);
#define LogRendererError(msg) Log(LogLevel::Error, std::string("[Renderer] ") + msg);