#ifndef SHADERMANAGER_CLASS_H
#define SHADERMANAGER_CLASS_H

#include"iostream"
#include <unordered_map>
#include "Shader.h"

class shaderManager
{
public:
	shaderManager();
	~shaderManager();

	void load(std::string programName, const char* vertexShader, const char* fragmentShader);
	bool remove(std::string programName);
	bool bind(std::string programName);

	Shader* get(std::string programName);


private:
	
	std::unordered_map<std::string, Shader> shaderPrograms;
};


#endif

