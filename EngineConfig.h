#pragma once

namespace engineConfig {
	inline constexpr int DEFAULT_WIDTH{ 800 };
	inline constexpr int DEFAULT_HEIGHT{ 800 };
	inline constexpr int REFLECTION_RESOLUTION_WIDTH{ 4096 };
	inline constexpr int REFLECTION_RESOLUTION_HEIGHT{ 4096 };
	
}

namespace textureSlots {
	inline constexpr int FALLBACK{ 0 };
	inline constexpr int DIFFUSE{ 1 };
	inline constexpr int AMBIENT{ 2 };
	inline constexpr int SPECULAR{ 3 };
	inline constexpr int CUBE_MAP{ 4 };
	inline constexpr int RENDER_TEXTURE{ 5 };
}


