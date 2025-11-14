#ifndef MESH_COMPONENT_CLASS_H
#define MESH_COMPONENT_CLASS_H
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include <algorithm>
#include <unordered_map>

#include"Shader.h"
#include"VAO.h"
#include"EBO.h"
#include"Vertex.h"
#include"VertexKey.h"
#include"GLDebug.h"
#include"cyTriMesh.h"


// Used to create obj from .obj files or creating objects manually
class MeshComponent
{
public:
	// Create obj from .obj file
	MeshComponent(const char* filename, std::string shaderName = "default");
	// Create obj manually
	MeshComponent(std::string name, std::vector<VERTEX>& vertices, std::vector<GLuint>& indices, std::string shaderName = "default");
	~MeshComponent();

	// Draw the object
	void Draw();

	// Getters and Setters

	// Get Shader, preventing change of Shader Name
	std::string getShaderName() const;
	std::string getMeshName() const;

private:

	// Instantiate VAO, VBO and EBO
	void CreateMeshObject();

	std::string meshName;
	std::string shaderName;
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;
	VAO vao;
	VBO vbo; // currently only storing position
	EBO ebo;

};

#endif // !MESH_COMPONENT_CLASS_H


#pragma once
