#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
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

	unsigned int getUniformLocation(const std::string& name);


private:
	
	// Used for "hot-reload" of shader's via F6
	const char* vertexFile;
	const char* fragmentFile;

	// cache shader uniform location
	std::unordered_map<std::string, int> shaderUniformLocations;
	
};

#endif

