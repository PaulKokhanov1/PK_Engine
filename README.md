# PK Engine — A Modern OpenGL Rendering Engine in C++

> A hobby rasterization engine built with OpenGL. My focus is not only on learning the ins and outs of rendering, but also building an engine in a scalable and maintainable way — following and extending [Cem Yuksel's Interactive Computer Graphics](https://graphics.cs.utah.edu/courses/cs6610/) curriculum.

<!-- ADD: A banner GIF or image showing the final rendered scene (Project 8 - tessellation + shadows + normal mapping) -->
<!-- EXAMPLE: ![PK Engine Demo](assets/demo.gif) -->

[![Watch Full Demo](https://img.shields.io/badge/Watch-Full%20Demo-red?style=for-the-badge&logo=youtube)](https://youtube.com/shorts/q2Ntzx203HA?feature=share)

---

## Table of Contents

* [Goal](#goal)
* [Built With](#built-with)
* [Project Overview](#project-overview)
* [Engine Architecture](#engine-architecture)
* [Project Milestones](#project-milestones)
  * [Project 1 — Core Engine Setup](#project-1--core-engine-setup)
  * [Project 2 — Mesh Loading & Camera System](#project-2--mesh-loading--camera-system)
  * [Project 3 — Blinn-Phong Shading](#project-3--blinn-phong-shading)
  * [Project 4 — Texture System & Multi-Material Meshes](#project-4--texture-system--multi-material-meshes)
  * [Project 5 — Render to Texture & FBO](#project-5--render-to-texture--fbo)
  * [Project 6 — Environment Mapping & Reflections](#project-6--environment-mapping--reflections)
  * [Project 7 — Shadow Mapping (Spot, Directional, Point)](#project-7--shadow-mapping-spot-directional-point)
  * [Project 8 — Normal Mapping, Tessellation & Displacement](#project-8--normal-mapping-tessellation--displacement)
* [Controls](#controls)
* [Render Settings](#render-settings)
* [Core Features](#core-features)
* [Challenges and Solutions](#challenges-and-solutions)
* [Key Learning Outcomes](#key-learning-outcomes)
* [Rant and Informal Discussion](#rant-and-informal-discussion)
* [Next Steps](#next-steps)
* [Contact](#contact)
* [Acknowledgments](#acknowledgments)

---

## Goal

My goal with this project was to build a real-time 3D rendering engine completely from scratch in modern OpenGL and C++.

Does this renderer revolutionize the way we do 3D graphics? ... no. Does this renderer solve any of life's biggest problems? ... no. Does this renderer provide any incentive for an employer to hire me? ... I hope. Regardless of what this engine does or doesnt do for my future, it was a ton of fun to build and learn how to create a maintainable piece of code that can actually be used and interacted with.

This isn't a game... yet. There's no gameplay loop, no assets store, no drag-and-drop editor. It's an engine — simply just a framework that gives a programmer the tools to describe a scene, and then renders it (partially) correctly and efficiently. Every class, every shader, every abstraction was designed and debugged by hand, starting from a blank `.cpp` file and a blinking triangle.

My driving motivation was simple: I want to work on engine development, specifically graphics. I've always envied the engineers who build the tools that other developers use to make great games. This project was my attempt to take real steps in that direction.

[(back to top)](#table-of-contents)

---

## Built With

* **C++17**
* **OpenGL 4.6**
* **GLFW** — Window creation and input handling
* **GLM** — Mathematics library (vectors, matrices, quaternions)
* **cyTriMesh** (Cem Yuksel) — OBJ/MTL file parsing
* **stb_image / LodePNG** — Texture image loading
* **GLAD** — OpenGL function loader
* **RenderDoc** — GPU debugging and profiling

[(back to top)](#table-of-contents)

---

## Project Overview

The engine was built using Cem Yuksel's "Interactive Computer Graphics" course as a guideline. He describes various techniques and concepts that are used in the industry, then asks you to implement some of them in a "mini-project". However, I instead wanted to use each project as a building block to the next one and continue to add on more and more techniques and functionality to make this engine usable!

The engine supports:

* Loading arbitrary `.obj` meshes with multi-material and texturing support
* Transformable meshes with necessary transformation matrix calculations
* A full Blinn-Phong lighting pipeline
* A render to texture pipeline
* Full camera movement and control
* Multi-pass rendering
* Real-time shadow mapping for spot, directional, and point lights (with PCF soft shadows)
* Environment mapping with cubemaps and reflections
* Normal mapping using TBN matrices computed in a geometry shader
* GPU tessellation with displacement mapping
* Shader hot-reload at runtime

Each project builds on the last. The architecture was continuously refactored to accommodate new requirements without collapsing under the weight of the previous ones

<!-- ADD: A screenshot or GIF of the full scene with all features enabled (Project 8) -->

[(back to top)](#table-of-contents)

---

## Engine Architecture

The engine follows a layered architecture where responsibilities are explicitly separated:

```
Application (singleton entry point)
│
├── Window              — GLFW window, GL context, input callbacks
├── InputManager        — Key/mouse state, event dispatcher pattern
├── EventDispatcher     — Token-based publish/subscribe callback system
├── SceneLoader         — Scene setup, object/light creation, feature flags
│
├── Scene               — Owns MeshComponents, Lights, Camera, Mirror
│   ├── Camera          — View/projection matrices, perspective & orthographic, movement and rotation controls
│   ├── Light           — Point / Directional / Spot + shadow matrix support
│   ├── LightController — Runtime light movement and rotation input
│   ├── MeshComponent   — VAO/VBO/EBO management, submesh system, .obj & .mtl parsing and loading
│   ├── MeshFactory     — Procedural mesh generation (quad grids)
│   ├── Transform       — Translation, rotation (quaternion), scale per object
│   ├── Mirror          — Reflection plane + mirrored camera management
│   └── QuadController  — Transform controller for render-to-texture quad
│
├── Renderer            — multi-pass pipeline: Collect → Shadow or Reflections (optional) → Draw → Post (optional) -> Env (optional)
│   ├── ShadowPass      — Depth render from each light's POV (2D or cubemap)
│   ├── ReflectionPass  — Draw Scene from a mirrored perspective
│   ├── DrawPass        — Main forward pass; shader/material/submesh batching
│   ├── PostProcess     — Draw transformable FBO from Rendered texture
│   ├── TriangleLines   — Draw lines indicating triangles (used in tessellation debugging)
│   └── EnvMapPass      — Draw Cube map environment onto triangle covering entire frustrum
│
├── TextureManager      — Caching (TextureDescriptor keyed), fallback textures,
│                         unit slot policy (defined in EngineConfig)
├── ShaderManager       — Shader caching, hot-reload (F6)
├── Shader              — VS + optional GS + optional TCS/TES + FS; uniform cache, handles setting uniforms
├── ShaderCommon        — Shared shader utility functions for error handling
├── FBO                 — Framebuffer abstraction (FBODescriptor + RenderBufferDescriptor)
├── ShadowMap           — 2D depth FBO or cubemap FBO, owned by each Light
├── CubeMap             — GL_TEXTURE_CUBE_MAP creation, binding, lifecycle
├── Material            — Phong parameters (Ka/Kd/Ks/shininess), texture refs,
│                         shader binding, uniform upload
├── Logging             — Per-class log macros (LogMesh, LogRenderer, LogShader, …)
├── GLDebug             — GL_CHECK_ERROR macro, disabled in release builds
└── EngineConfig        — Texture slots, render flags, engine-wide constants
```

**Real source files in this repo:**

| Category | Files |
|---|---|
| Core | `Application`, `Window`, `EngineConfig.h`, `GLDebug.h` |
| Input | `InputManager`, `EventDispatcher` |
| Scene | `Scene`, `SceneLoader`, `SceneException` |
| Camera | `Camera` |
| Mesh | `MeshComponent`, `MeshFactory`, `Vertex.h`, `VertexKey.h`, `VBO`, `VAO`, `EBO` |
| Transform | `Transform` |
| Lighting | `Light`, `LightController`, `ShadowMap` |
| Reflections | `Mirror`, `QuadController` |
| Rendering | `Renderer`, `FBO`, `FBODescriptor.h`, `RenderBufferDescriptor.h` |
| Shaders | `Shader`, `ShaderManager`, `ShaderCommon`, `ShaderException` |
| Materials | `Material`, `Texture`, `TextureManager`, `TextureDescriptor.h` |
| Cubemap | `CubeMap` |
| Math | `MathCommon.h` |
| Logging | `Logging`, `LogCamera.h`, `LogMesh.h`, `LogRenderer.h`, `LogShader*.h`, `LogTexture*.h`, `LogWindow.h`, `LogLight.h`, `LogInputManager.h` |
| Shaders (GLSL) | `default.vert/frag`, `framebuffer.vert/frag`, `shadowMap.vert/frag`, `ShadowMapTess.vert/tesc/tese/frag`, `lines.vert/geom/frag`|
| Shaders cont'd (GLSL) | `tess.vert/tesc/tese/geom/frag`, `skybox.ver/frag`, `planeReflection.vert/frag`, `ShadowCubeMap.vert/frag`, `ShadowCubeMapTess.vert/tesc/tese/frag`,  |
| Assets | `envMaps/cubemap/`, `normalMaps/`, `displacementMaps/` |
| Third-party | `cyTriMesh.h`, `cyCore.h`, `cyVector.h` (Cem Yuksel), `libraries/` |

**Renderer Pipeline — Multiple passes per frame:**

1. **Collection Pass** — Iterates the scene, groups `RenderItem`s (submesh + mesh + model matrix) by shader. Filters by render layer bitmask.
1. **Offscreen Render Pass** — Regular draw pass but drawn to an FBO
2. **Reflection Pass** — For each shader: bind → upload scene uniforms aswell as mirrored camera matrices → for each material: upload material/texture uniforms → draw submeshes.
2. **Draw Pass** — For each shader: bind → upload scene uniforms → for each material: upload material/texture uniforms → draw submeshes.
3. **Post-Process Pass** — Render to quad texture, apply any screen-space effects.
3. **Draw Triangle Lines Pass** — Using "lines" shader program, For each shader: bind → upload scene uniforms → for each material: upload material/texture uniforms → draw submeshes as lines.
3. **Draw Plane With Shader Pass** — Usign Plane Reflection shader program, paste reflected rendered texture → draw plane submesh
3. **Env Map Pass** — Use large triangle to cover viewport of camera and map corresponding environment cubmap position onto triangle uv coordinates

[(back to top)](#table-of-contents)

---

## Project Milestones

### Project 1 — Core Engine Setup

The first step was establishing the foundational scaffolding before touching any rendering at all. This included:

* **Window class** — GLFW window + OpenGL 4.6 context creation
* **Shader class** — Compile/link vertex and fragment shaders, uniform API, custom shader exceptions
* **ShaderManager** — Centralized shader storage and lifetime management, hot-reload support
* **InputManager** — Key and mouse state tracking using an event dispatcher pattern
* **VAO / VBO / EBO classes** — GPU buffer abstractions for vertex data management

All I really did here was lay out the architecture and plan for the future

---

### Project 2 — Mesh Loading & Camera System

| OBJ Mesh Rendered as Points |
|:---------------------------:|
| <img src="https://github.com/user-attachments/assets/bf376d56-daa7-4460-90cf-157b1f389271" width="700"/> |
| *Utah teapot loaded from an OBJ file and rendered using `GL_POINTS` to verify correct vertex parsing before introducing indexed triangle rendering.* |

---

* **OBJ file parsing** using cyTriMesh, with custom vertex deduplication logic to handle cases where the same position is shared with different normals/UVs across faces
* **MeshComponent class** — Manages VAO/VBO/EBO construction from parsed OBJ data, handles the vertex key map for deduplication
* **Camera class** — Perspective and orthographic projection, with a `P` toggle to switch between them at runtime
* **Coordinate transformations** — Model → World → View → Clip space, passed as separate uniforms to maintain clean shader conventions

A subtle but important lesson here was around the EBO unbinding order: the EBO must be unbound *after* the VAO, because the VAO stores the EBO binding. Unbinding the EBO before unbinding the VAO causes the render to not show

---

### Project 3 — Blinn-Phong Shading

<table align="center">
<tr>
<td align="center">
<img src="https://github.com/user-attachments/assets/bbf20715-d474-4bc8-9dc4-a3d62024a484" width="400"><br>
<em>Diffuse texture only</em>
</td>

<td align="center">
<img src="https://github.com/user-attachments/assets/86508caa-b234-4d71-b011-d1c5b740ddba" width="400"><br>
<em>Diffuse and ambient textures applied</em>
</td>
</tr>

<tr>
<td colspan="2" align="center">
<img src="https://github.com/user-attachments/assets/838e5951-f5a3-4010-a285-8d0794e854b5" width="850"><br>
<em>Complete material system demonstrating diffuse, ambient, and specular mapping.</em>
</td>
</tr>
</table>
---

* **Material class** — Holds ambient (Ka), diffuse (Kd), specular (Ks), and shininess parameters; owns shader association; handles uniform uploading
* **Blinn-Phong shading in world space** — Diffuse, specular (half-vector method), and ambient components computed per-fragment in the fragment shader
* **Normal matrix** — Because non-uniform scaling skews normals under the model matrix, normals are transformed using the inverse-transpose of the model matrix: `normalMatrix = inverse(transpose(mat3(modelMatrix)))`
* **Light system** — Point light struct with position, color, and per-component intensity (ambient, diffuse, specular)
* **Light controller** — Ability to hold down CTRL and move light around object by defining fixed position from center always pointing at the center
* **Transform struct** — Translation, rotation, scale per mesh; passed to shader as a model matrix each frame

---

### Project 4 — Texture System & Multi-Material Meshes

<p align="center">
  <img src="https://github.com/user-attachments/assets/7edeab85-af8c-465d-80bf-1e1a1a825393">
  <br>
  <em>Yoda mesh loaded from an OBJ/MTL pair demonstrating multiple materials assigned to individual submeshes.</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/21d6f10f-765a-4546-b93d-87d4ae171c3a">
  <br>
  <em>Utah teapot rendered using Phong lighting with ambient, diffuse, and specular material properties.</em>
</p>

---



* **Submesh system** — OBJ files can define multiple materials per mesh. Each submesh holds an `indexStart`, `indexCount`, and its own `Material`, enabling per-section rendering
* **Texture class** — Handles creation, binding, GPU upload, mipmap generation, and texture parameter configuration
* **TextureManager** — Centralized texture caching (keyed on filepath + descriptor), fallback 1×1 textures for meshes missing ambient/diffuse/specular maps, texture unit slot assignment
* **MTL file parsing** — Diffuse (map_Kd), specular (map_Ks), ambient (map_Ka), and displacement (map_disp) texture maps extracted and assigned per submesh
* **Texture unit policy** — Each texture type (diffuse, specular, ambient, shadow, etc.) is assigned a dedicated texture unit defined in `EngineConfig.h`, preventing sampler aliasing bugs

One thing I learned here: `glUniform1i(samplerUniform, unit)` doesn't *send* texture data — it tells the shader *which unit to read from*. If two samplers point to the same unit, they share the same texture.

---

### Project 5 — Render to Texture & FBO

<table align="center">
  <tr>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/7eb47a1d-95c5-4052-96ed-605477fbc7b0" width="850"><br>
      <em>Render-to-texture reflection showing the mirrored scene projected onto the reflective surface.</em>
    </td>
  </tr>
</table>

<p align="center">
<img src="https://github.com/user-attachments/assets/bad762f4-f499-4e30-8960-9703fa202caf">
<br>
<em>Interactive demonstration of the reflection plane being rotated using <strong>ALT + Drag</strong>, dynamically updating the reflected scene in real time.</em>
</p>
---

* **FBO class** — Abstracts framebuffer creation, configuration (color attachment, depth attachment, or depth-only for shadows), binding/unbinding, and resize handling
* **FBODescriptor / TextureDescriptor / RenderBufferDescriptor** — Separate descriptor structs for clean FBO construction without entangling texture parameters with framebuffer configuration
* **Render-to-texture** — Scene is first rendered into an FBO, then the resulting texture is pasted onto a screen quad in a second pass
* **Screen quad** — Owned by the `Renderer`, always positioned in front of the camera at the beginning, until you move the camera
* **QuadController** — Dedicated class managing the render-to-texture plane's transform (position, rotation, scale) independently from the scene camera; ALT+drag controls route here only
* **Mipmap verification** — Confirmed mipmapping works on rendered textures; resolution scales correctly with distance for both minimization and magnification
* **Event Dispatcher** — Callback registration system (publish/subscribe) using tokens, replacing raw GLFW `glfwSetUserPointer` hacks. Handles resize, input, and custom events
* **Quaternion-based transforms** — Switched from Euler angle extraction to direct quaternion manipulation for rotations, computing pitch/yaw deltas from mouse input and composing them with `qPitch * qYaw`

---

### Project 6 — Environment Mapping & Reflections

<p align="center">
  <img src="https://github.com/user-attachments/assets/0f4ab791-39b9-4164-b34c-297f144350b7">
  <br>
  <em>Interactive demonstration showcasing dynamic environment mapping, planar reflections rendered via render-to-texture, and real-time point light illumination.</em>
</p>

---

* **CubeMap class** — Loads 6 face textures, generates a `GL_TEXTURE_CUBE_MAP`, configures seamless cube map filtering and mipmapping
* **Sky triangle** — A single full-screen triangle (covering the clip space completely) used as the skybox background. Vertices are specified in clip space; the fragment shader converts them to world-space directions via `inverse(viewProjection)` to sample the cubemap correctly
* **Overdraw prevention** — Objects are drawn first, then `glDepthMask(false)` disables depth writes before rendering the sky triangle, re-enabling afterward to prevent the background from occluding geometry
* **Environment reflections** — Cubemap sampled in world space using the reflection vector computed per-fragment; combined with the Blinn-Phong BRDF as an environment specular term
* **Mirror class** — Dedicated abstraction for the reflection plane; manages a second "mirror camera" that renders the scene from the reflected viewpoint into an FBO, which is then sampled as a planar reflection texture
* **Application singleton** — Introduced a formal `Application` class following Cherno's architecture reference, enabling centralized access to global engine objects (TextureManager, ShaderManager, etc.) without passing them everywhere through constructors

Here I realized that the cubemap filepaths MUST be passed in a proper order to then apply them to the cubemap texture appropriately

---

### Project 7 — Shadow Mapping (Spot, Directional, Point)

<p align="center">
  <img src="https://github.com/user-attachments/assets/09b85fa8-cad9-46d9-b412-f3577cbb0203">
  <br>
  <em><strong>Spot Light Shadow Mapping</strong><br>
  Shadows generated using spot light with no Percentage Closer Filtering (PCF).</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/531cbc2c-a65b-4e67-98d5-7a1c244f00e9">
  <br>
  <em><strong>Directional Light Shadows + PCF</strong><br>
  Shadow mapping with PCF to produce softer shadow edges.</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/c14692ae-a2cb-49d7-90b9-2eecff63d815">
  <br>
  <em><strong>Point Light Shadow Mapping</strong><br>
  Omnidirectional shadows generated using a depth cubemap rendered from six light-space views.</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/0e0a7720-26e1-443e-afbf-19b076f9b919">
  <br>
  <em><strong>Light-Space Depth Rendering</strong><br>
  Visualization of the depth texture rendered from the light's perspective.</em>
</p>


---

This was the most technically demanding project. Three light types, three shadow strategies.

**Spot Light Shadows (2D depth texture)**
* Light view matrix computed with `glm::lookAt` from the light's position along its direction
* Light projection matrix uses perspective projection (matching the spot cone angle)
* Shadow pass renders scene depth into a 2D depth texture FBO from the light's POV
* Main pass reconstructs shadow coordinates: `lightSpacePos = lightVP * worldPos`, then divides by `w` and remaps to `[0,1]`
* PCF (Percentage Closer Filtering) implemented by sampling a kernel around the fragment's shadow coordinate, averaging results for soft shadow edges
* Shadow acne eliminated using `glPolygonOffset` in the shadow pass to push depth values slightly farther from the light, preventing self-shadowing artifacts

**Directional Light Shadows**
* Orthographic projection (lights are infinitely far away; no perspective foreshortening)
* A synthetic "position" computed from the light's direction to establish a valid view matrix
* Follows similar 2D depth texture creation as Spot Light

**Point Light Shadows (Cube Shadow Map)**
* 6 render passes — one per face of a `GL_TEXTURE_CUBE_MAP`, essentially re-rendering scene using same process as for spotlight, alternative would be to use geometry shader
* Radial depth stored (distance from light to fragment), not projected depth — because cubemap lookup uses a direction vector, not UV coordinates
* Fragment shader stores `gl_FragDepth` manually as `length(fragPos - lightPos) / farPlane` to normalize into `[0,1]`
* PCF applied by sampling multiple directions around the lookup vector, reducing hard shadow edges

**Render Layers**
* A bitmask system (`uint32_t` enum) lets each scene object declare which render layers it participates in. Shadow passes and light meshes use this to exclude light geometry from self-shadowing.

---

### Project 8 — Normal Mapping, Tessellation & Displacement

<p align="center">
  <img src="https://github.com/user-attachments/assets/8e5a38d2-1c0e-4965-a1e5-d8a0255fbdb8">
</p>

<p align="center">
  <strong>Normal Mapping</strong><br>
  <em>Fine surface detail simulated using tangent-space normal maps without increasing mesh complexity.</em>
</p>

---

<p align="center">
  <img src="https://github.com/user-attachments/assets/4a1266c1-b50b-408b-8f2a-34bda38499f0">
</p>

<p align="center">
  <strong>Dynamic Tessellation</strong><br>
  <em>Tessellation levels adjusted in real time using the arrow keys, demonstrating adaptive subdivision.</em>
</p>

---

<p align="center">
  <img src="https://github.com/user-attachments/assets/f4fd1b60-337e-445a-9363-ee1b64cc0535">
</p>

<p align="center">
  <strong>Tessellation + Shadows</strong><br>
  <em>Tessellated geometry casting and receiving accurate shadows by including tessellation in the shadow rendering pass.</em>
</p>

---

**Normal Mapping**
* Normal maps are sampled in the fragment shader (after remapping from `[0,1]` to `[-1,1]` with `sampledNormal * 2.0 - 1.0`)
* TBN (Tangent-Bitangent-Normal) matrix computed in the **geometry shader**, transforming light positions, camera position, and light direction into local tangent space before passing them to the fragment shader — keeping all shading calculations consistent in the same space
* Optional geometry shader design using `std::optional` in the Shader class, avoiding the need for a separate shader program per VS+FS combination

**Geometry Shader Wireframe**
* A secondary geometry shader takes triangle primitives as input and emits line strips offset slightly along the surface normal, visualizing the mesh triangulation
* Toggled at runtime with the spacebar

**Tessellation (TCS + TES)**
* Shader class extended to support optional Tessellation Control Shader (TCS) and Tessellation Evaluation Shader (TES) using `std::optional`
* Renderer updated to use `GL_PATCHES` draw mode and `glPatchParameteri(GL_PATCH_VERTICES, 4)` when tessellation is enabled
* VS outputs in object space (not clip space) when tessellation is active; clip space projection deferred to TES
* TCS sets inner/outer tessellation levels (runtime-adjustable with arrow keys); TES interpolates control point positions
* `MeshFactory` added to generate quad grids, alternative to making multiple .obj files
* `RenderBatch` added to `Renderer` to specify topology (triangles vs patches) per shader

**Displacement Mapping**
* Displacement texture sampled in TES; vertex position displaced along the surface normal scaled by height map intensity
* Independent controls for tessellation level and displacement scale to separately verify each feature
* Tessellation pipeline fed into the shadow shader as well — a dedicated `ShadowMapTess.vert/.tesc/.tese/.frag` shader program handles the tessellation + shadow depth pass so displaced geometry casts correct shadows

**Feature Flags in EngineConfig**
* `USING_SHADOWS`, `USING_TESSELLATION`, `USING_NORMALMAPPING` flags control which render passes and shader variants activate, keeping the engine flexible without branching chaos in the main loop

Still struggling finding a correct TBN matrix calculation as my current TBN results in poor lighting as tessellation increases

[(back to top)](#table-of-contents)

---

## Controls

| Control | Action |
|---|---|
| `F6` | Hot-reload all shaders |
| `P` | Toggle perspective / orthographic projection |
| `W` `A` `S` `D` | Move camera |
| `LEFT DRAG` | Camera look around |
| `RIGHT DRAG` | Zoom camera |
| `CTRL` + `W` `A` `S` `D` | Move active light |
| `CTRL` + `LEFT DRAG` | Rotate light direction |
| `ALT` + `RIGHT DRAG` | Scale render-to-texture mesh |
| `ALT` + `LEFT DRAG` | Rotate render-to-texture mesh |
| `LEFT ARROW` / `RIGHT ARROW` | Adjust tessellation level |
| `SPACEBAR` | Toggle wireframe/triangulation overlay |

[(back to top)](#table-of-contents)

---

## Render Settings

Render features are toggled through `EngineConfig.h`. To activate a specific project's scene, use the corresponding `RenderFrame` function in the `Application` class and enable the appropriate flags:

```cpp
// In EngineConfig.h
#define USING_SHADOWS        // Enable shadow map generation pass
#define USING_TESSELLATION   // Enable TCS/TES shader stage + GL_PATCHES
...
```

Normal and displacement maps are configured in `SceneLoader.cpp`.

[(back to top)](#table-of-contents)

---

## Core Features

* **VAO / VBO / EBO abstraction** — Clean GPU buffer management; VAO owns attribute layout and EBO reference
* **OBJ/MTL mesh loading** — Full submesh support with per-face material assignment and vertex deduplication
* **Modular Shader class** — Supports VS + optional GS + optional TCS/TES + FS; runtime hot-reload via F6; uniform cache to avoid redundant `glGetUniformLocation` calls each frame
* **Blinn-Phong lighting** — Point, directional, and spot lights; per-component intensity (ambient/diffuse/specular separate)
* **Multi-pass renderer** — Collect → Pre-process → Draw → Post-process; render layer bitmask filters objects per pass
* **Render-to-Texture** — Full FBO pipeline with color and depth attachments; mipmapped rendered textures
* **Cubemap Environment Mapping** — Sky triangle background, environment reflections on objects
* **Shadow Mapping** — 2D depth texture for spot/directional lights; cube shadow map for point lights; PCF soft shadows; polygon offset shadow acne removal
* **Normal Mapping** — TBN matrix in geometry shader; world-space light/camera data transformed to tangent space
* **GPU Tessellation** — TCS/TES pipeline; runtime-tunable tessellation levels; displacement mapping from height maps
* **Geometry Shader Wireframe** — Line strip overlay visualizing mesh triangulation
* **Quaternion rotations** — Direct quaternion manipulation for object and camera rotations; no gimbal lock
* **Event Dispatcher** — Token-based pub/sub callback system for input and window events
* **TextureManager** — Texture caching by descriptor; fallback 1×1 textures; centralized unit slot policy
* **Internal Logging System** — Internal messaging system, filter based on TRACE, INFO, WARN, ERROR & CRITICAL


[(back to top)](#table-of-contents)

---

## Challenges and Solutions

| Challenge | Solution |
|---|---|
| EBO unbound before VAO destroyed the index buffer link | VAO must be unbound *after* EBO; EBO reference is stored in the VAO, not separately |
| Vertex normals skewed by non-uniform model scaling | Use `inverse(transpose(mat3(modelMatrix)))` as the normal matrix |
| Texture sampler aliasing — two samplers accidentally reading the same unit | Assigned dedicated texture units per texture type in `EngineConfig.h`; sampler uniforms set to specific integer unit slots, not shared |
| Render-to-texture Reflection texture drifted with camera movement | Implemented projective texture mapping using the reflected camera's perspective instead of mesh UVs |
| Shadow acne on lit surfaces | `glPolygonOffset` in shadow pass pushes stored depth values slightly further from light; combined slope-scale factor + constant bias |
| Point light shadow cube map — incorrect depth values | Switched from projected depth to radial depth (`length(fragPos - lightPos) / farPlane`), stored manually via `gl_FragDepth` |
| PCF soft shadows for directional light failing | 2D kernel averaging was incorrect; also required adjusting bias separately from spot light |
| TBN matrix computed with wrong normals causing distorted normal map | TBN was built from raw positions instead of the transformed normals from `NormalWrld` attribute; switching input fixed the distortion |
| Tessellation + shadow pass mismatch — displaced geometry didn't cast correct shadows | Added TCS and TES stages to the shadow shader program so displaced vertices are captured correctly in the depth pass |
| Light mesh receiving its own light contribution | Introduced render layer bitmask — each object declares layer bits; shadow and draw passes filter with bitwise AND to exclude light meshes from the shadow pass |
| `glActiveTexture` slot confusion during render-to-texture | Removed auto-binding from `Texture::bind()`; texture unit is now explicitly passed at the call site; `texTypeToUnit` removed in favor of `EngineConfig::TextureSlots` namespace |
| Consistent errors in shader not being noticed | Implemented shader compilation system recording if shader was correctly compiled, otherwise outputting message error |
| Meshes missing vertex positions with appropriate normals | Duplicated vertices as needed to build a unique interleaved vertex buffer while reusing identical vertices through the EBO  |


[(back to top)](#table-of-contents)

---

## Key Learning Outcomes

* Internalized the full OpenGL rendering pipeline: how data moves from CPU memory through vertex buffers, vertex shaders, rasterization, fragment shaders, and finally to the framebuffer
* Understood coordinate space transformations end-to-end (Local → World → View → Clip → NDC → Screen) and when each is appropriate for different operations (e.g. lighting in world space, ray intersection in world space, shading in view space)
* Built a complete multi-pass shadow pipeline from scratch — including depth texture FBOs, light-space matrix construction, shadow sampling with PCF, and cube shadow maps for omnidirectional lights
* Designed and iterated on an engine architecture with separable concerns: the renderer doesn't know about scene loading, materials don't manage their own textures, lights own their shadow maps
* Got familiar with GPU debugging using RenderDoc to isolate rendering artifacts and verify depth map correctness
* Learned creation and usage of shaders using GLSL 

[(back to top)](#table-of-contents)

---
## Rant and Informal Discussion

Now that the formalities are out of the way, I can get into the actual conversation about what it was like to build this.

Genuinely I loved this project. Not only because of what I learned, but just everything that was going on with my life during this point and how I was still able to keep up with this project taught me many key lessons.

Beyond those stories (that I wont share here), I kept finding myself comparing this project to something like a Minecraft survivial world. Creating a rendering engine felt exactly like creating a "world" in a sandbox game and building, automating and testing new ideas and things, and having this "entity" just keep growing with all the history of where it started to where it is now.

### So many techniques

As I continue to learn about the different ways graphics programmers "mimic" reality, you get to a point where you have to question, how viable/reasonable of a change/technique is this?

For example, as I learn about soft shadows, I was watching a video on percentage closer filtering and implementing percentage closer soft shadows (PCSS) and you realize how many resources such a technique would require. You have to do your occluder search and then percentage closer filtering on each one. This is all for one light source. Now there is a discussion about how many light sources would even need to be required, but ultimately what I'm getting at, is that it's expensive.

Yet, there were newer techniques that came in order to approximate PCSS, such as Variance Shadow Maps (VSM), Convolution Shadow Maps (CSM), etc... , and sure, they're old and we probably have better techniques now, but once again the point is that there is endless amount of techniques.

Now this brings the question, how do you find the appropriate one for your situation? Genuinely I have no clue. It feels like it'd become an analysis paralysis situation, and your best bet might just be to make up one on your own. And then you may need different techniques for different parts of your application and/or scene. Thus, it comes down to this ever evolving choice of ways to render a scene thats always losing older techniques and creating new ones.

I guess this rant is more about how do you handle finding what you need for your situation, and then how do you keep improving at it while also focusing on other aspects of the render? I guess that's the point of a group of people working on a project, be that a engine, video, game or anything involving graphics

### RenderDoc and PIX

Despite using RenderDoc at times in my project, it was never really extremely useful. 

However, given that I have this phenonmenal opportunity to work at Activision, I get to use PIX quite often.

Seeing it be used in such a huge codebase such as Call of Duty is astounding. From, the simplicity to quickly take a capture and see all the different render passes, to the ridiculously useful debugger, where, you can make an edit to the code, re-compile and have that frame showcase the changes. This tool, isn't only a tremendous help in debugging, but I've already learned so much about directX 12 thanks to it. 

For example, getting to see the GPU time allocated for specific passes or actions within the pass led me to find a bug where we were allocating ~ 1 million threads to write to a buffer. Like imagine trying to debug "why your application is slow" without such a tool, it'd be extremely tedious. I guess it just gives me that much more appreciation for current tooling and more appreciation for all the graphics programmers who found novel ways to debug their application before such tools 


### I Still cant figure out TBN matrices

Sigh... I really still am struggling with my TBN matrices. The ones I use in my engine just simply are not right. I diagnosed this by rendering the normal map on a plane and then rotating it and monitoring the method light interacted with it. In the flat vs rotated position the lighting was entirely different. This led me to an issue regarding the normals. 

I checked a variety of other properties such as: light direction, world space attributes (normal, tangent & bitangent) and even rendering the world normals via:
`vec3 worldNormal = normalize(transpose(TBNMatrix) * Normal_tangentSpace); `

This is how I concluded it had to do with my TBN. Now I think the calculation itself it fine, but the way the normal map is exported and how it interacts with my existing TBN matrix is clearly out of whack. 

So, really all this section is, is a plea for help or a suggestion on how do you calculate the appropriate TBN matrix based on how the normal map is exported, is there even a way to check how the normal map is exported in order to determine the appropriate way to calculate the TBN. OR is it simply that all TBN's should be the same regardless of the exportation of a normal map and its just my calculations are wrong?

If anyone ever reads this and feels willing to help a clueless graphics programmer, I'd be in your debt. :D 


### My thoughts on Architecture

One goal coming into this project was to focus on architecting a system to be maintainable and scalable. Despite the refactors and existing "questionable" systems, I'd say it was accomplished. It taught me quite a bit about how little I know regarding design patterns and when to use what type of architecture.

For example, in my input handling system, I now have an event dispatcher to allow the Observer pattern to be used, however, initially I naively just had to throw around bool's and have each modules "update" function monitor them continously. Or when I had to rearchitect my application class to avoid creating and handling multiple key modules like all the shader's, the different render passes, or the inputs. 

The funny thing is, is that it still worked previously, and potentially would've continued to still work as I continued to complete the future projects. Which is a really interesting idea to me, specifically that, you can create a large system just using a TON of if else statments. Would it be fast or easy to read, I doubt it, but it just highlights how there really is no "right" way to go about programming (as long as the application works), but just ways that might save you (or others) a headache in the future.

### What's Next

Pretty simple: Port Engine to Vulkan and make it better

[(back to top)](#table-of-contents)

---

## Next Steps

* [ ] Port rendering backend to Vulkan
* [ ] Add a DirectX 12 backend as an alternative path
* [ ] Implement a proper Entity-Component-System (ECS) for scene management
* [ ] Physically Based Rendering (PBR) — GGX BRDF, metallic-roughness workflow
* [ ] SSAO (Screen-Space Ambient Occlusion)
* [ ] GPU-based particle system
* [ ] Cascaded Shadow Maps for directional lights at scale
* [ ] Command-pattern InputManager redesign

[(back to top)](#table-of-contents)

---

## Contact

Paul Kokhanov — [Website](<!-- ADD YOUR WEBSITE URL -->) — paul.kokhanov@gmail.com

Project Link: [https://github.com/PaulKokhanov1/PK_Engine](https://github.com/PaulKokhanov1/PK_Engine)

[(back to top)](#table-of-contents)

---

## Acknowledgments

* [Cem Yuksel — Interactive Computer Graphics Course](https://graphics.cs.utah.edu/courses/cs6610/) — The curriculum this engine is built on top of
* [LearnOpenGL](https://learnopengl.com) — Indispensable reference throughout
* [Scratchapixel](https://www.scratchapixel.com) — Deep mathematical explanations for rendering concepts
* [TheCherno — C++ Engine Architecture Series](https://github.com/TheCherno/Architecture) — Reference for the Application singleton design
* [Polyhaven](https://polyhaven.com) — Normal and displacement map textures used in Project 8
* [RenderDoc](https://renderdoc.org) — GPU debugging tool, genuinely irreplaceable

[(back to top)](#table-of-contents)
