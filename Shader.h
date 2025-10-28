#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

enum errorType
{
	VERTEX,
	FRAGMENT,
	PROGRAM
};

std::string get_file_contents(const char* filename);

class Shader {

public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	// Error checker
	bool checkCompileErrors(GLuint id, errorType type);
	
};

#endif

