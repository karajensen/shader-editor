
- update light diagnotics to spin boxes
- update sample 3D scene
- fix bump/attenuation
- post effects (fog, dof)
- post effects diagnostics
- keyed camera

- Create 3D scene
- parallax occlusion mapping
- soft shadows
- water/environment mapping
- post effects (glow, SSAO, Color correction)
- color correction diagnostic

================================================================
GARDEN SHADERS
=================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/shader-scenery.git
=================================================================

LIBRARIES/ENVIRONMENT VARIABLES REQUIRED TO BUILD:
Irrlicht Engine path:   $(IRRLICHT)
Boost path:             $(BOOST)

TAB:    Select next light to edit
DOWN:   Select next light attribute
UP:     Select previous light attribute
+:      Increment light attribute
-:      Decrement light attribute
S:      Save lights to xml
W:      Toggle Shadows
F:      Switch to free camera
T:      Switch to targeted camera
K:      Switch to and play keyed camera
C:      Reload camera from file
D:      Toggle Diagnostics