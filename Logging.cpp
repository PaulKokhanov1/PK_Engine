#include "Logging.h"

void Log(LogLevel type, const std::string& msg)
{
	// _DEBUG automatically defined in Visual Studio
#if defined(_DEBUG)
	switch (type)
	{
	case LogLevel::Trace:
		std::cout << "[TRACE] " << msg << std::endl;
		break;
	case LogLevel::Info:
		std::cout << "[INFO] " << msg << std::endl;
		break;
	case LogLevel::Warn:
		std::cout << "[WARN] " << msg << std::endl;
		break;
	case LogLevel::Error:
		std::cerr << "[ERROR] " << msg << std::endl;
		break;
	case LogLevel::Critical:
		std::cerr << "[CRITICAL] " << msg << std::endl;
		break;
	default:
		break;
	}

#endif
}
