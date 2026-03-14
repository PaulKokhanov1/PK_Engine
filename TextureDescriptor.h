#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include<glad/glad.h>

// Used for identifying existing or new textures 
struct TextureDescriptor
{
	unsigned int width;
	unsigned int height;
	GLenum format;
	std::string path;

	TextureDescriptor(unsigned int w = 0, unsigned int h = 0, GLenum textureFormat = GL_RGBA, std::string filepath = "") : width(w), height(h), format(textureFormat), path(filepath) {}

	bool operator==(const TextureDescriptor& other) const noexcept {
		return width == other.width &&
			height == other.height &&
			format == other.format &&
			path == other.path;
	}
};


// Hash specialization must be in std namespace
namespace std {
    template<>
    struct hash<TextureDescriptor>
    {
        size_t operator()(const TextureDescriptor& k) const noexcept
        {
            size_t seed = 0;

            // Hash width
            seed ^= std::hash<unsigned int>{}(k.width)
                + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // Hash height
            seed ^= std::hash<unsigned int>{}(k.height)
                + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // Hash format (GLenum is just an unsigned int)
            seed ^= std::hash<unsigned int>{}(k.format)
                + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // Hash path
            seed ^= std::hash<std::string>{}(k.path)
                + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}