#include"ShaderException.h"

ShaderException::ShaderException(std::string msg, errorType errType, const char* filePath) : message(msg), type(errType), filePath(filePath)
{}

ShaderException::~ShaderException()
{
}

const char* ShaderException::what() const noexcept
{
	std::string errorInfo = "[Shader Error] " + message + " of errorType: " + errorTypeToString(type);
	return errorInfo.c_str();
}
