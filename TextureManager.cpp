#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	Shutdown();
}

void TextureManager::Shutdown()
{
	for (auto t : texMap) {
		delete t.second;
	}

	texMap.clear();
}

void TextureManager::initFallback()
{
	vector<unsigned char> white = { 255,255,255,255 };
	vector<unsigned char> gray = { 128,128,128,255 };
	vector<unsigned char> black = { 0,0,0,255 };

	if (texMap.find("white") == texMap.end()) {
		texMap["white"] = new Texture(white.data(), "Fallback", texTypeToUnit["Fallback"]);
	}	
	
	if (texMap.find("gray") == texMap.end()) {
		texMap["gray"] = new Texture(gray.data(), "Fallback", texTypeToUnit["Fallback"]);
	}	
	
	if (texMap.find("black") == texMap.end()) {
		texMap["black"] = new Texture(black.data(), "Fallback", texTypeToUnit["Fallback"]);
	}

}

Texture* TextureManager::load(const char* filepath, const char* texType, GLenum format, GLenum pixelType, GLenum texTarget)
{
	if (texMap.find(filepath) != texMap.end()) {
		return texMap[filepath];
	}

	texMap[filepath] = new Texture(filepath, texType, texTypeToUnit[texType], format, pixelType, texTarget);
	return texMap[filepath];
}

Texture* TextureManager::getOrLoad(string texture)
{
	if (texMap.find(texture) != texMap.end()) {
		return texMap[texture];
	}

	cout << "[OpenGL] Cannot find Texture (" << texture << ") , creating new texture with default parameters \n";

	load(texture.c_str());

}

Texture* TextureManager::getFallback(string texture)
{
	if (texMap.find(texture) == texMap.end()) {
		std::cerr << "[OpenGL] Error finding Fallback Texture (" << texture << ")\n";
	}

	return texMap[texture];
}
