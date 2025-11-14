#ifndef SHADER_COMMON_H
#define SHADER_COMMON_H

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


#endif // !SHADER_COMMON_H

