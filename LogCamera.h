#pragma once

#include "Logging.h"

#define LogCameraInfo(msg) Log(LogLevel::Info, string("[Camera] ") + msg);
#define LogCameraWarn(msg) Log(LogLevel::Warn, string("[Camera] ") + msg);
#define LogCameraError(msg) Log(LogLevel::Error, string("[Camera] ") + msg);