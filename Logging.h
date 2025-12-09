#ifndef LOGGING_CLASS_H
#define LOGGING_CLASS_H

#include <iostream>
#include <string>

using namespace std;

enum class LogLevel
{
	Trace,
	Info,
	Warn,
	Error,
	Critical

};

void Log(LogLevel type, const string& msg);

#endif // !LOGGING_CLASS_H
