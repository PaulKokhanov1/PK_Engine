#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include<glad/glad.h>

#include"TextureDescriptor.h"
#include"RenderBufferDescriptor.h"

// Separation of Concerns for FBO creation parameters
struct FBODescriptor
{
	std::optional<TextureDescriptor> ColorAttachmentDesc;
	std::optional<TextureDescriptor> DepthTextureDesc;
	std::optional<RenderBufferDescriptor> DepthRenderbufferDesc;
};