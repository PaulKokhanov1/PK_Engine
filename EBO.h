#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<iostream>
#include<vector>

class EBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Default constructor to create EBO's to default values
	EBO();
	~EBO();

	// Generates a Element Buffer Object and links it to vertices
	void Construct(std::vector<GLuint>& indices);
	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif
