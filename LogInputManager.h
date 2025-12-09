#pragma once

#include "Logging.h"

#define LogInputManagerInfo(msg) Log(LogLevel::Info, string("[Input Manager] ") + msg);
#define LogInputManagerWarn(msg) Log(LogLevel::Warn, string("[Input Manager] ") + msg);
#define LogInputManagerError(msg) Log(LogLevel::Error, string("[Input Manager] ") + msg);