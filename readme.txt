
- Using ID3D11Debug::ReportLiveDeviceObjects with D3D11_RLDO_DETAIL will help drill into object lifetimes. Objects with ExtRef=0 and IntRef=0 will be eventually destroyed through typical Immediate Context usage. However, if the application requires these objects to be destroyed sooner, ClearState followed by Flush on the Immediate Context will realize their destruction.
- Directx Name="unnamed" change
- DirectX multisampling with render targets
- OpenGL Render targets and multisampling

- Qt window closing when main window does
- Qt passing info to/from app
- Qt text editor/tweak bar

- Fade in-out for opengl/directx switching
- Rendering: specular, attenuation, bump with defines
- Selecting and tweaking mesh/lights/shaders
- Scene Normal/Depth map generation
- Switching on/off post effects and post textures
- Post effects: fog, dof, glow, SSAO, Color correction, shadows
- Optimise shaders

- Create 3D scene
- Transparency
- Particle effects

TO DETERMINE:
- Opengl fps (possibly switch vbo binding with vao)

=================================================================
SHADER EDITOR
=================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/shader-editor.git
=================================================================

INSTALL REQUIREMENTS: GPUShaderAnalyzer 1.59.0.3208
RELEASE REQUIREMENTS: Windows 7
BUILD REQUIREMENTS: Windows 7, Visual Studio 2012

