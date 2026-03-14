#pragma once

#include <iostream>
#include <string>

enum class LogLevel
{
	Trace,
	Info,
	Warn,
	Error,
	Critical

};

void Log(LogLevel type, const std::string& msg);

