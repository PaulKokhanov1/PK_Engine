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

    GLint internalFormat;
   	GLenum format;
    GLenum pixelType;
    GLenum target;

    std::vector<std::pair<GLenum, GLint>> parameters;

	std::string path;

    TextureDescriptor(
        unsigned int w = 0,
        unsigned int h = 0,
        GLint texInternalFormat = GL_RGBA8,
        GLenum textureFormat = GL_RGBA,
        GLenum texPixelType = GL_UNSIGNED_BYTE,
        GLenum texTarget = GL_TEXTURE_2D,
        std::vector<std::pair<GLenum, GLint>> texParameters = {},
        std::string filepath = ""
    )
        : width(w),
        height(h),
        internalFormat(texInternalFormat),
        format(textureFormat),
        pixelType(texPixelType),
        target(texTarget),
        parameters(texParameters),
        path(filepath)
    {}

    bool operator==(const TextureDescriptor& other) const noexcept {
        return width == other.width &&
            height == other.height &&
            internalFormat == other.internalFormat &&
            format == other.format &&
            pixelType == other.pixelType &&
            target == other.target &&
            parameters == other.parameters &&
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

            auto hashCombine = [&seed](size_t value) {
                seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

            // Basic fields
            hashCombine(std::hash<unsigned int>{}(k.width));
            hashCombine(std::hash<unsigned int>{}(k.height));
            hashCombine(std::hash<int>{}(k.internalFormat));
            hashCombine(std::hash<unsigned int>{}(k.format));
            hashCombine(std::hash<unsigned int>{}(k.pixelType));
            hashCombine(std::hash<unsigned int>{}(k.target));

            // Parameters (vector of pairs)
            for (const auto& p : k.parameters) {
                hashCombine(std::hash<unsigned int>{}(p.first));
                hashCombine(std::hash<int>{}(p.second));
            }

            // Strings
            hashCombine(std::hash<std::string>{}(k.path));

            return seed;
        }
    };
}