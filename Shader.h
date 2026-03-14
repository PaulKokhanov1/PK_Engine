#pragma once

#include<glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<unordered_map>

#include"ShaderCommon.h"
#include"ShaderException.h"
#include"GLDebug.h"



std::string get_file_contents(const char* filename);

class Shader {

public:
	// Reference ID of the Shader Program
	GLuint ID;

	// Default constructor
	Shader();
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	// Error checker
	bool checkCompileErrors(GLuint id, errorType type);
	bool isValid() const;

	const char* getVertexFile();
	const char* getFragmentFile();

	int getUniformLocation(const std::string& name) const;

	// Setting Uniforms
	void setUniform1i(const std::string& name, int val);
	void setUniform1f(const std::string& name, float val);

	void setUniform2fv(const std::string& name, const glm::vec2& val);
	void setUniform3fv(const std::string& name, const glm::vec3& val);
	void setUniform4fv(const std::string& name, const glm::vec4& val);

	void setUniformMat3fv(const std::string& name, const glm::mat3& matrix);
	void setUniformMat4fv(const std::string& name, const glm::mat4& matrix);

private:
	
	// Used for "hot-reload" of shader's via F6
	const char* vertexFile;
	const char* fragmentFile;

	// cache shader uniform location
	mutable std::unordered_map<std::string, int> shaderUniformLocations;
	
};

