#pragma once

#include<string>

enum errorType
{
	VERTEXSHADER,
	FRAGMENTSHADER,
	PROGRAM
};

inline std::string errorTypeToString(errorType e) {
	switch (e) {
		case errorType::VERTEXSHADER: return "Vertex";
		case errorType::FRAGMENTSHADER: return "Fragment";
		case errorType::PROGRAM: return "Program";
		default:  return "No Specification";
	}
}


