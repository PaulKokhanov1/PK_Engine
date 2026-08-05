#pragma once

#include<string>

enum errorType
{
	VERTEXSHADER,
	FRAGMENTSHADER,
	GEOMETRYSHADER,
	TESSELLATIONCONTROLSHADER,
	TESSELLATIONEVALUTATIONSHADER,
	PROGRAM
};

inline std::string errorTypeToString(errorType e) {
	switch (e) {
		case errorType::VERTEXSHADER: return "Vertex";
		case errorType::FRAGMENTSHADER: return "Fragment";
		case errorType::GEOMETRYSHADER: return "Geometry";
		case errorType::TESSELLATIONCONTROLSHADER: return "Tessellation Control";
		case errorType::TESSELLATIONEVALUTATIONSHADER: return "Tessellation Evaluation";
		case errorType::PROGRAM: return "Program";
		default:  return "No Specification";
	}
}


