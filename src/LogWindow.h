#pragma once

#include "Logging.h"

#define LogWindowInfo(msg) Log(LogLevel::Info, std::string("[Window] ") + msg);
#define LogWindowWarn(msg) Log(LogLevel::Warn, std::string("[Window] ") + msg);
#define LogWindowError(msg) Log(LogLevel::Error, std::string("[Window] ") + msg);