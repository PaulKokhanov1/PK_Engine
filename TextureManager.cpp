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
	texMap.clear();
}

void TextureManager::initFallback()
{
	std::vector<unsigned char> white = { 255,255,255,255 };
	std::vector<unsigned char> gray = { 128,128,128,255 };
	std::vector<unsigned char> black = { 0,0,0,255 };
	std::vector<unsigned char> blue = { 0,0,255,255 };

	TextureDescriptor whiteDesc(1, 1, GL_RGBA, "white");
	TextureDescriptor grayDesc(1, 1, GL_RGBA, "gray");
	TextureDescriptor blackDesc(1, 1, GL_RGBA, "black");
	TextureDescriptor blueDesc(1, 1, GL_RGBA, "blue");

	if (texMap.find(whiteDesc) == texMap.end()) {
		texMap[whiteDesc] = std::make_unique<Texture>(white.data(), "Fallback", texTypeToUnit["Fallback"]);
	}	
	
	if (texMap.find(grayDesc) == texMap.end()) {
		texMap[grayDesc] = std::make_unique<Texture>(gray.data(), "Fallback", texTypeToUnit["Fallback"]);
	}	
	
	if (texMap.find(blackDesc) == texMap.end()) {
		texMap[blackDesc] = std::make_unique<Texture>(black.data(), "Fallback", texTypeToUnit["Fallback"]);
	}	
	
	if (texMap.find(blueDesc) == texMap.end()) {
		texMap[blueDesc] = std::make_unique<Texture>(blue.data(), "Fallback", texTypeToUnit["Fallback"]);
	}

}

Texture* TextureManager::load(TextureDescriptor texDesc, const char* filepath, const char* texType, GLenum format, GLenum pixelType, GLenum texTarget)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	auto it = texTypeToUnit.find(texType);
	if (it == texTypeToUnit.end()) {
		LogTextureManagerError("Error finding texType: " + texType + " within texTypeToUnit Map");
		return getFallback("white");
	}

	int unit = it->second;

	texMap[texDesc] = std::make_unique<Texture>(filepath, texType, unit, format, pixelType, texTarget);
	return texMap[texDesc].get();
}

Texture* TextureManager::loadCubeMap(TextureDescriptor texDesc, std::array<std::string, 6> paths, const char* texType, GLenum texTarget, GLenum format, GLenum pixelType)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	auto it = texTypeToUnit.find(texType);
	if (it == texTypeToUnit.end()) {
		LogTextureManagerError("Error finding texType: " + texType + " within texTypeToUnit Map");
		return getFallback("white");
	}

	int unit = it->second;

	texMap[texDesc] = std::make_unique<Texture>(paths, texType, texTarget, unit, format, pixelType);
	return texMap[texDesc].get();
}

Texture* TextureManager::loadRenderedTexture(TextureDescriptor texDesc, unsigned int width, unsigned int height, const char* texType, GLenum texTarget, GLenum format, GLenum pixelType)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	auto it = texTypeToUnit.find(texType);
	if (it == texTypeToUnit.end()) {
		LogTextureManagerError("Error finding texType: " + texType + " within texTypeToUnit Map");
		return getFallback("white");
	}

	int unit = it->second;

	texMap[texDesc] = std::make_unique<Texture>(width, height, texType, unit, format, pixelType, texTarget);
	return texMap[texDesc].get();
}

Texture* TextureManager::getOrLoad(TextureDescriptor texDesc)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	LogTextureManagerError("Cannot find Texture (" + texDesc.path + ") , attempting to create new texture with default parameters" );

	return load(texDesc, texDesc.path.c_str());

}

Texture* TextureManager::getFallback(std::string texture)
{
	TextureDescriptor desc(1, 1, GL_RGBA, texture);

	if (texMap.find(desc) == texMap.end()) {
		LogTextureManagerError("Error finding Fallback Texture(" + texture + ")");
		return nullptr;
	}

	return texMap[desc].get();
}

int TextureManager::getUnitForType(std::string texType)
{
	auto it = texTypeToUnit.find(texType);
	if (it == texTypeToUnit.end()) {
		LogTextureManagerError("Error finding texType: " + texType + " within texTypeToUnit Map");
		return 0; // Maybe return something else? Not sure if this is the best to return, dont want to return -1 as that will throw an error and crash program
	}

	return it->second;
}
