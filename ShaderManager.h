#pragma once

#include"iostream"
#include<unordered_map>

#include"Shader.h"
#include"LogShaderManager.h"


class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager() = default;

	void load(const std::string& programName, const char* vertexShader, const char* fragmentShader, std::optional<const char*> geometryShader, std::optional<const char*> tcsFile, std::optional<const char*> tesFile);
	bool remove(const std::string& programName);
	bool bind(const std::string& programName);

	Shader* get(const std::string& programName);

	void reloadAll();


private:
	
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaderPrograms;
};


