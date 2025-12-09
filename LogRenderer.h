#pragma once

#include "Logging.h"

#define LogRendererInfo(msg) Log(LogLevel::Info, string("[Renderer] ") + msg);
#define LogRendererWarn(msg) Log(LogLevel::Warn, string("[Renderer] ") + msg);
#define LogRendererError(msg) Log(LogLevel::Error, string("[Renderer] ") + msg);