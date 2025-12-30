#pragma once

#include<iostream>

class SceneException : public std::exception
{
public:
	SceneException(std::string msg);
	~SceneException();


	const char* what() const noexcept;

private:

	std::string message;

};