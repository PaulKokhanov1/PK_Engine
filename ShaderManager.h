#ifndef SHADERMANAGER_CLASS_H
#define SHADERMANAGER_CLASS_H

#include"iostream"
#include <unordered_map>
#include "Shader.h"


class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void load(std::string programName, const char* vertexShader, const char* fragmentShader);
	bool remove(std::string programName);
	bool bind(std::string programName);

	Shader* get(std::string programName);

	void reloadAll();


private:
	
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaderPrograms;
};


#endif

