

GPUShaderAnalyzer.exe ./Diffuse.vert -I ./Test.txt -ASIC IL -profile glsl_vs -function main
GPUShaderAnalyzer.exe ./Diffuse.frag -I ./Test.txt -ASIC IL -profile glsl_ps -function main

- Add call to GPU Shader analyzer
- Fix boost exception

- Qt textbox for viewing shader code/assembly
- Runtime editing of shaders
- Rendering: diffuse specular, attenuation, transparency, texture, bump mapping
- Create 3D scene

- Render to texture
- Normal map generation for post
- Switching on/off post effects and post textures
- Post effects: fog, dof, glow, SSAO, Color correction, shadows

TO DETERMINE:
- Fix opengl fps: using vao instead of always setting vbo/attributes

=================================================================
SHADER EDITOR
=================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/shader-editor.git
=================================================================

RELEASE REQUIREMENTS: Windows 7
BUILD REQUIREMENTS: Windows 7, Visual Studio 2012