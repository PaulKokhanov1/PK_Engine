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
	inline constexpr int NORMAL{ 4 };
	inline constexpr int DISPLACEMENT{ 5 };
	inline constexpr int CUBE_MAP{ 6 };
	inline constexpr int RENDER_TEXTURE{ 7 };
}

namespace renderSettings {

	// Runtime Toggles
	inline bool USING_SHADOWS{ false };
	inline bool USING_ENV{ false };
	inline bool USING_TESSELLATION{ false };
	inline bool USING_NORMALMAPPING{ false };
	inline bool USING_REFLECTIONS{ false };
	inline bool USING_RENDER_TO_TEXTURE_SIMPLE{ false };
	inline bool DISPLAY_TRIANGULATION{ false };
	inline float tessellationLevel = 1.0f;
	inline float displacementScale = 0.02f;
}

namespace renderTypes {
	enum class PrimitiveTopology {
		Patches,
		Triangles,
		Lines,
		Points
	};
}

namespace Project {
	enum class ProjectNumber {
		PROJECT5,
		PROJECT6,
		PROJECT7,
		PROJECT8
	};
}

