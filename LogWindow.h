#pragma once

#include "Logging.h"

#define LogWindowInfo(msg) Log(LogLevel::Info, string("[Window] ") + msg);
#define LogWindowWarn(msg) Log(LogLevel::Warn, string("[Window] ") + msg);
#define LogWindowError(msg) Log(LogLevel::Error, string("[Window] ") + msg);