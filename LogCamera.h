#pragma once

#include "Logging.h"

#define LogCameraInfo(msg) Log(LogLevel::Info, std::string("[Camera] ") + msg);
#define LogCameraWarn(msg) Log(LogLevel::Warn, std::string("[Camera] ") + msg);
#define LogCameraError(msg) Log(LogLevel::Error, std::string("[Camera] ") + msg);