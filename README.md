<img width="798" height="826" alt="Render_to_texture_proj_5" src="https://github.com/user-attachments/assets/8608c707-e71d-4ac4-bd6c-29030128b565" /># PK Engine — A Modern OpenGL Rendering Engine in C++

> A hobby rasterization engine built with OpenGL. My focus is not only on learning the ins and outs of rendering, but also building an engine in a scalable and maintainable way — following and extending [Cem Yuksel's Interactive Computer Graphics](https://graphics.cs.utah.edu/courses/cs6610/) curriculum.

<!-- ADD: A banner GIF or image showing the final rendered scene (Project 8 - tessellation + shadows + normal mapping) -->
<!-- EXAMPLE: ![PK Engine Demo](assets/demo.gif) -->

[![Watch Full Demo](https://img.shields.io/badge/Watch-Full%20Demo-red?style=for-the-badge&logo=youtube)](<!-- ADD YOUR YOUTUBE LINK HERE -->)

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

Does this renderer revolutionize the way we do 3D graphics? ... no. Does this renderer solve any of life's biggest problems? ... no. Does this renderer provide any incentive for an employer to hire me? ... I hope. Regardless of what this engine does or doesnt do for my future, it was a ton of fun to build and learn how to create a maintainable piece of code that can actually be used an interacted with.

This isn't a game. There's no gameplay loop, no assets store, no drag-and-drop editor. It's an engine — simply just a framework that gives a programmer the tools to describe a scene, and then renders it correctly and efficiently. Every class, every shader, every abstraction was designed and debugged by hand, starting from a blank `.cpp` file and a blinking triangle.

The driving motivation was simple: I want to work on engine development. I've always envied the engineers who build the tools that other developers use to make great games. This project was my attempt to take real steps in that direction.

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
3. **Draw Triangle Lines Pass** — Using "lines" shader program, For each shader: bind → upload scene uniforms → for each material: upload material/texture uniforms → draw submeshes.
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

* **Material class** — Holds ambient (Ka), diffuse (Kd), specular (Ks), and shininess parameters; owns shader association; handles uniform uploading
* **Blinn-Phong shading in world space** — Diffuse, specular (half-vector method), and ambient components computed per-fragment in the fragment shader
* **Normal matrix** — Because non-uniform scaling skews normals under the model matrix, normals are transformed using the inverse-transpose of the model matrix: `normalMatrix = inverse(transpose(mat3(modelMatrix)))`
* **Light system** — Point light struct with position, color, and per-component intensity (ambient, diffuse, specular broken out separately to prevent ambient from overpowering)
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
* **MTL file parsing** — Diffuse (map_Kd), specular (map_Ks), and ambient (map_Ka) texture maps extracted and assigned per submesh
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

<!-- ADD: Screenshot of the sphere with cubemap reflections and skybox background -->

* **CubeMap class** — Loads 6 face textures, generates a `GL_TEXTURE_CUBE_MAP`, configures seamless cube map filtering and mipmapping
* **Sky triangle** — A single full-screen triangle (covering the clip space completely) used as the skybox background. Vertices are specified in clip space; the fragment shader converts them to world-space directions via `inverse(viewProjection)` to sample the cubemap correctly
* **Overdraw prevention** — Objects are drawn first, then `glDepthMask(false)` disables depth writes before rendering the sky triangle, re-enabling afterward to prevent the background from occluding geometry
* **Environment reflections** — Cubemap sampled in world space using the reflection vector computed per-fragment; combined with the Blinn-Phong BRDF as an environment specular term
* **Mirror class** — Dedicated abstraction for the reflection plane; manages a second "mirror camera" that renders the scene from the reflected viewpoint into an FBO, which is then sampled as a planar reflection texture
* **Application singleton** — Introduced a formal `Application` class following Cherno's architecture reference, enabling centralized access to global engine objects (TextureManager, ShaderManager, etc.) without passing them everywhere through constructors

Here I realized that the cubemap filepaths MUST be passed in a proper order to then apply them to the cubemap texture appropriately

---

### Project 7 — Shadow Mapping (Spot, Directional, Point)

<!-- ADD: Screenshot showing hard shadows from a spot light -->
<!-- ADD: Screenshot showing soft PCF shadows -->
<!-- ADD: Screenshot showing point light cube shadow map -->

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

<!-- ADD: Screenshot of plane with normal mapping — compare with and without TBN -->
<!-- ADD: GIF showing tessellation level increasing with arrow keys -->
<!-- ADD: GIF of displacement mapping deforming the mesh surface -->

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
* **Blinn-Phong lighting** — Point, directional, and spot lights; per-component intensity (ambient/diffuse/specular separate); all computed in world space
* **Multi-pass renderer** — Collect → Shadow → Draw → Post-process; render layer bitmask filters objects per pass
* **Render-to-Texture** — Full FBO pipeline with color and depth attachments; mipmapped rendered textures
* **Cubemap Environment Mapping** — Sky triangle background, environment reflections on objects
* **Shadow Mapping** — 2D depth texture for spot/directional lights; cube shadow map for point lights; PCF soft shadows; polygon offset shadow acne removal
* **Normal Mapping** — TBN matrix in geometry shader; world-space light/camera data transformed to tangent space
* **GPU Tessellation** — TCS/TES pipeline; runtime-tunable tessellation levels; displacement mapping from height maps
* **Geometry Shader Wireframe** — Line strip overlay visualizing mesh triangulation
* **Quaternion rotations** — Direct quaternion manipulation for object and camera rotations; no gimbal lock
* **Event Dispatcher** — Token-based pub/sub callback system for input and window events
* **TextureManager** — Texture caching by descriptor; fallback 1×1 textures; centralized unit slot policy

[(back to top)](#table-of-contents)

---

## Challenges and Solutions

| Challenge | Solution |
|---|---|
| EBO unbound before VAO destroyed the index buffer link | VAO must be unbound *after* EBO; EBO reference is stored in the VAO, not separately |
| Vertex normals skewed by non-uniform model scaling | Use `inverse(transpose(mat3(modelMatrix)))` as the normal matrix |
| Texture sampler aliasing — two samplers accidentally reading the same unit | Assigned dedicated texture units per texture type in `EngineConfig.h`; sampler uniforms set to specific integer unit slots, not shared |
| Render-to-texture quad moving with the camera | Separated camera control from texture-plane transform; quad stores its own transform with quaternion rotation |
| Shadow acne on lit surfaces | `glPolygonOffset` in shadow pass pushes stored depth values slightly further from light; combined slope-scale factor + constant bias |
| Point light shadow cube map — incorrect depth values | Switched from projected depth to radial depth (`length(fragPos - lightPos) / farPlane`), stored manually via `gl_FragDepth` |
| PCF soft shadows for directional light failing | 2D kernel averaging was incorrect; also required adjusting bias separately from spot light |
| Physics instability at large delta time (RopePlugin) | Fixed-timestep accumulator — accumulate elapsed time, run multiple fixed-step simulation cycles per frame to catch up |
| TBN matrix computed with wrong normals causing distorted normal map | TBN was built from raw positions instead of the transformed normals from `NormalWrld` attribute; switching input fixed the distortion |
| Tessellation + shadow pass mismatch — displaced geometry didn't cast correct shadows | Added TCS and TES stages to the shadow shader program so displaced vertices are captured correctly in the depth pass |
| Light mesh receiving its own light contribution | Introduced render layer bitmask — each object declares layer bits; shadow and draw passes filter with bitwise AND to exclude light meshes from the shadow pass |
| `glActiveTexture` slot confusion during render-to-texture | Removed auto-binding from `Texture::bind()`; texture unit is now explicitly passed at the call site; `texTypeToUnit` removed in favor of `EngineConfig::TextureSlots` namespace |
| Camera movement applying during FBO render incorrectly | Properly separated the "mirror" camera (used for reflection FBO) from the scene camera; ensured normal matrix was uploaded correctly in the reflection pass |

[(back to top)](#table-of-contents)

---

## Key Learning Outcomes

* Internalized the full OpenGL rendering pipeline: how data moves from CPU memory through vertex buffers, vertex shaders, rasterization, fragment shaders, and finally to the framebuffer
* Understood coordinate space transformations end-to-end (Local → World → View → Clip → NDC → Screen) and when each is appropriate for different operations (e.g. lighting in world space, ray intersection in world space, shading in view space)
* Built a complete multi-pass shadow pipeline from scratch — including depth texture FBOs, light-space matrix construction, shadow sampling with PCF, and cube shadow maps for omnidirectional lights
* Learned the TBN matrix construction and its role in transforming lighting calculations into tangent space to correctly apply normal maps
* Extended the OpenGL pipeline with optional geometry shaders and tessellation shaders (TCS/TES), understanding the full VS → TCS → TES → GS → FS data flow
* Designed and iterated on an engine architecture with separable concerns: the renderer doesn't know about scene loading, materials don't manage their own textures, lights own their shadow maps
* Experienced first-hand why a fixed simulation timestep matters for physics stability, and how a timestep accumulator pattern solves it
* Got familiar with GPU debugging using RenderDoc to isolate rendering artifacts and verify depth map correctness

[(back to top)](#table-of-contents)

---

## Rant and Informal Discussion

Now that the formalities are out of the way, I can get into the actual conversation about what it was like to build this.

I'll be real: I started this project because I wanted to understand what engine programmers actually *do*. Not the hand-wavy version, but the actual math, the actual API calls, the actual debugging sessions that end with "wait, I needed to unbind the EBO *after* the VAO??" I've been a game developer, a gameplay programmer, and now I've thrown myself headfirst into the lowest level of the stack I could find while staying above raw Vulkan.

And I have zero regrets.

### The Pipeline

Every time I show someone what this project does, I resist the urge to just play the demo. I want to walk them through what's *actually happening* each frame. There are (in the full Project 8 scene) potentially 4+ render passes before a single pixel hits the screen:

1. Shadow pass from the spot light's POV — rendering scene depth into a 2D texture
2. Shadow pass from the directional light — orthographic depth texture
3. Six shadow passes from the point light — one per face of a cube shadow map
4. The main draw pass — using all of the above to compute shadows, apply Blinn-Phong shading, sample normal maps, and output to an FBO
5. Post-process — copy to screen quad

The fact that this runs in real time still kind of amazes me. The GPU is just an absolute monster.

### Shadow Mapping Nearly Broke Me

The shadow acne problem was particularly humbling. I kept looking at the math, convinced I had the bias applied correctly in the fragment shader, and then it hit me — you can't just subtract a constant from the depth comparison *after* the fact, because the comparison is baked into the depth test. You have to *push the stored values slightly further away* during the shadow pass itself, which is what `glPolygonOffset` does. Once I understood that, the whole thing clicked.

The cube shadow map was a different kind of pain. The key insight that unlocked it: a 2D shadow map stores projected depth (already transformed by a projection matrix, so it lives in `[0,1]` naturally). A cube shadow map doesn't have that luxury — you're indexing it with a 3D direction vector, so you need *actual distance* from the light, not projected depth. Hence storing radial distance normalized by the far plane. That's not something you find in the first tutorial you read; you have to reach a point of confusion and work backwards.

### Tessellation: Worth It?

Honestly, tessellation in OpenGL is kind of a pain to set up. The `VS → TCS → TES → GS → FS` pipeline requires each shader to pass all its data through in an array form — because TCS operates on an entire patch, not individual vertices — and then the data magically reassembles into per-triangle format when it exits TES. Getting that data flow right took a while.

Was it worth it? Absolutely. Watching a flat plane turn into a rippling, displaced surface in real time by just bumping the tessellation level with an arrow key is incredibly satisfying.

### The Architecture Evolves

One thing nobody tells you about writing an engine from scratch: your architecture will be wrong. Multiple times. The version of the renderer I ended up with is the fifth or sixth meaningful refactor. At some point I had the material setting texture units inside the renderer. Then I had the renderer reaching into the TextureManager on every draw call. Then I had textures auto-binding to whatever unit they felt like.

The architecture I landed on — centralized texture unit slots in `EngineConfig`, explicit `bind(unit)` calls at the draw site, `TextureManager` purely responsible for caching and creation — is a product of every one of those mistakes. Good architecture isn't discovered, it's excavated.

### What's Next

The plan is to port this to Vulkan. Yes, I know. But having built this in OpenGL means I understand *why* Vulkan is designed the way it is. Every explicit render pass, every command buffer, every synchronization primitive — they all make more sense now that I've felt the pain of implicit state management in OpenGL. The groundwork is laid.

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
