
ERROR:  Vulkan: Error:DSCannot submit cmd buffer using image (0xc) 
[sub-resource: aspectMask 0x2 array layer 0, mip level 0], with layout 
VK_IMAGE_LAYOUT_UNDEFINED when first use is VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL. [7]

- Add vulkan API
- Update release and test

===================================================================
SHADER EDITOR
===================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/shader-editor.git
===================================================================

INSTALL REQUIREMENTS: GPUShaderAnalyzer 1.59.0.3208
RELEASE REQUIREMENTS: Windows 8+
BUILD REQUIREMENTS: Windows 8+, Visual Studio 2015
LIBRARIES: Assimp 3.0, Boost 1.6, Qt 5.6.1-1 for Windows 32-bit
CAUSTIC GENERATOR: http://www.dualheights.se/caustics/

===================================================================

1 -> 7     Render a specific layer
0          Toggle Wireframe
F1         Toggle Camera Auto Move
F2         Switch the engine
WASDQE     Move the camera
LMC        Rotate the camera

===================================================================