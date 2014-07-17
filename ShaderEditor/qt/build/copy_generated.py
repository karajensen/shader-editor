########################################################################################
## Kara Jensen - mail@karajensen.com - copy_generated.py
########################################################################################

import os
import shutil

buildDirectory = os.path.dirname(__file__)
projectDirectory = buildDirectory.replace("build", "project")
generatedDirectory = projectDirectory + "\\Generated\\"
debugFolder = "\\debug\\"
mocFolder = "\\moc\\"

files = ["moc_editor.cpp", 
         "moc_tweaker.cpp", 
		 "moc_tweakable_box.cpp", 
		 "moc_tweakable_value.cpp", 
		 "ui_editor.h", 
		 "ui_tweaker.h"]

for filename in files:
	if os.path.exists(buildDirectory + filename):
		source = buildDirectory + filename
		dest = generatedDirectory + filename
		print("Copying " + source + " to " + dest + "\n\n")
		shutil.copyfile(source, dest)
	elif os.path.exists(buildDirectory + debugFolder + filename):
		source = buildDirectory + debugFolder + filename
		dest = generatedDirectory + mocFolder + filename
		print("Copying " + source + " to " + dest + "\n\n")
		shutil.copyfile(source, dest)