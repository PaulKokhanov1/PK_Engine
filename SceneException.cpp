#include "SceneException.h"

SceneException::SceneException(std::string msg) : message(msg) {}

SceneException::~SceneException()
{
}

const char* SceneException::what() const noexcept
{
	std::string errorInfo = "[Scene Error] " + message;
	return errorInfo.c_str();
}
