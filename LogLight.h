#pragma once

#include "Logging.h"

#define LogLightInfo(msg) Log(LogLevel::Info, std::string("[Light] ") + msg);
#define LogLightWarn(msg) Log(LogLevel::Warn, std::string("[Light] ") + msg);
#define LogLightError(msg) Log(LogLevel::Error, std::string("[Light] ") + msg);