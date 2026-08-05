#pragma once

#include "Logging.h"

#define LogInputManagerInfo(msg) Log(LogLevel::Info, std::string("[Input Manager] ") + msg);
#define LogInputManagerWarn(msg) Log(LogLevel::Warn, std::string("[Input Manager] ") + msg);
#define LogInputManagerError(msg) Log(LogLevel::Error, std::string("[Input Manager] ") + msg);