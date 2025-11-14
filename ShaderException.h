#ifndef SHADER_EXCEPTION_CLASS_H
#define SHADER_EXCEPTION_CLASS_H

#include<iostream>
#include"ShaderCommon.h"

class ShaderException : public std::exception
{
public:
	ShaderException(std::string msg, errorType errType, const char* filePath = "");
	~ShaderException();


	const char* what() const noexcept;

private:

	std::string message;
	errorType type;
	const char* filePath;
	
};

#endif
