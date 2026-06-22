#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include<glad/glad.h>

// Used for setting parameters for renderbuffer creation
struct RenderBufferDescriptor
{
    unsigned int width;
    unsigned int height;

    GLenum internalFormat;
    GLenum target;

    RenderBufferDescriptor(
        unsigned int w = 0,
        unsigned int h = 0,
        GLenum texInternalFormat = GL_DEPTH_COMPONENT,
        GLenum texTarget = GL_RENDERBUFFER
    )
        : width(w),
        height(h),
        internalFormat(texInternalFormat),
        target(texTarget)
    {}

    bool operator==(const RenderBufferDescriptor& other) const noexcept {
        return width == other.width &&
            height == other.height &&
            internalFormat == other.internalFormat &&
            target == other.target;
    }
};

// Hash specialization must be in std namespace
namespace std {
    template<>
    struct hash<RenderBufferDescriptor>
    {
        size_t operator()(const RenderBufferDescriptor& k) const noexcept
        {
            size_t seed = 0;

            auto hashCombine = [&seed](size_t value) {
                seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

            hashCombine(std::hash<unsigned int>{}(k.width));
            hashCombine(std::hash<unsigned int>{}(k.height));
            hashCombine(std::hash<unsigned int>{}(k.internalFormat));
            hashCombine(std::hash<unsigned int>{}(k.target));

            return seed;
        }
    };
}