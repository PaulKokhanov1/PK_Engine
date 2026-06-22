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

	TextureDescriptor whiteDesc(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, {}, "white");
	TextureDescriptor grayDesc(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, {}, "gray");
	TextureDescriptor blackDesc(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, {}, "black");
	TextureDescriptor blueDesc(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, {}, "blue");

	if (texMap.find(whiteDesc) == texMap.end()) {
		texMap[whiteDesc] = std::make_unique<Texture>();
		texMap[whiteDesc]->CreateFallback(white.data());
	}	
	
	if (texMap.find(grayDesc) == texMap.end()) {
		texMap[grayDesc] = std::make_unique<Texture>();
		texMap[grayDesc]->CreateFallback(gray.data());
	}	
	
	if (texMap.find(blackDesc) == texMap.end()) {
		texMap[blackDesc] = std::make_unique<Texture>();
		texMap[blackDesc]->CreateFallback(black.data());
	}	
	
	if (texMap.find(blueDesc) == texMap.end()) {
		texMap[blueDesc] = std::make_unique<Texture>();
		texMap[blueDesc]->CreateFallback(blue.data());
	}

}

Texture* TextureManager::load(const TextureDescriptor& texDesc)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	texMap[texDesc] = std::make_unique<Texture>();
	texMap[texDesc]->Load2D(texDesc.path.c_str(), texDesc.format, texDesc.pixelType, texDesc.target);
	return texMap[texDesc].get();
}

Texture* TextureManager::loadCubeMap(const TextureDescriptor& texDesc, std::array<std::string, 6> paths)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	texMap[texDesc] = std::make_unique<Texture>();
	texMap[texDesc]->LoadCubeMap(paths, texDesc.target, texDesc.format, texDesc.pixelType);
	return texMap[texDesc].get();
}

Texture* TextureManager::loadRenderedTexture(const TextureDescriptor& texDesc)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	texMap[texDesc] = std::make_unique<Texture>();
	switch (texDesc.target) {

		case (GL_TEXTURE_2D):
			texMap[texDesc]->CreateRenderTarget(texDesc.width, texDesc.height, texDesc.format, texDesc.internalFormat, texDesc.pixelType, texDesc.parameters, texDesc.target);

			break;

		case (GL_TEXTURE_CUBE_MAP):
			texMap[texDesc]->LoadDepthCubeMap(texDesc.width, texDesc.height, texDesc.format, texDesc.internalFormat, texDesc.pixelType, texDesc.parameters, texDesc.target);
			
			break;
	}
	return texMap[texDesc].get();
}

Texture* TextureManager::getOrLoad(const TextureDescriptor& texDesc)
{
	if (texMap.find(texDesc) != texMap.end()) {
		return texMap[texDesc].get();
	}

	LogTextureManagerError("Cannot find Texture (" + texDesc.path + ") , attempting to create new texture with default parameters" );

	return load(texDesc);

}

Texture* TextureManager::getFallback(std::string texture)
{
	TextureDescriptor desc(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, {}, texture);

	if (texMap.find(desc) == texMap.end()) {
		LogTextureManagerError("Error finding Fallback Texture(" + texture + ")");
		return nullptr;
	}

	return texMap[desc].get();
}