#pragma once

#include "Logging.h"

#define LogLightInfo(msg) Log(LogLevel::Info, string("[Light] ") + msg);
#define LogLightWarn(msg) Log(LogLevel::Warn, string("[Light] ") + msg);
#define LogLightError(msg) Log(LogLevel::Error, string("[Light] ") + msg);