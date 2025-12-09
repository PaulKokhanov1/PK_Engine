#include "Logging.h"

void Log(LogLevel type, const string& msg)
{
	// _DEBUG automatically defined in Visual Studio
#if defined(_DEBUG)
	switch (type)
	{
	case LogLevel::Trace:
		cout << "[TRACE] " << msg << endl;
		break;
	case LogLevel::Info:
		cout << "[INFO] " << msg << endl;
		break;
	case LogLevel::Warn:
		cout << "[WARN] " << msg << endl;
		break;
	case LogLevel::Error:
		cerr << "[ERROR] " << msg << endl;
		break;
	case LogLevel::Critical:
		cerr << "[CRITICAL] " << msg << endl;
		break;
	default:
		break;
	}

#endif
}
