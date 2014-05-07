
- section shader text into editable/non-editable
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

SHADER ANALYZER COMMAND:
GPUShaderAnalyzer.exe Diffuse_glsl_vert.fx -I Diffuse_glsl_vert.asm -ASIC IL -profile glsl_vs -function main
GPUShaderAnalyzer.exe Diffuse_glsl_frag.fx -I Diffuse_glsl_frag.asm -ASIC IL -profile glsl_fs -function main