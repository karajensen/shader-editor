########################################################################################
## Kara Jensen - mail@karajensen.com - replace_with_saved.py
########################################################################################

import os

currentDirectory = os.path.dirname(__file__)

files = ["Emitters", 
         "Lights", 
		 "Meshes", 
		 "PostProcessing"]

for name in files:
	filepath = currentDirectory + "\\" + name
	oldFile = filepath + ".xml";
	newFile = filepath + "_Saved.xml";
	
	if os.path.exists(oldFile):
		if os.path.exists(newFile):
			os.remove(oldFile)
			os.rename(newFile, oldFile)