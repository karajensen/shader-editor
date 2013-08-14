########################################################################################
## Kara Jensen - mail@karajensen.com - exporter.py
########################################################################################

from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import uic
import maya.cmds as cmds
import sip, os
import xml.dom.minidom

uiFile = os.path.join(os.path.dirname(__file__), "exporter.ui")

windowTitle = "Scene Exporter"
windowObject = "scene-exporter"
noAttribute = "NoAttribute"
noSelection = "NoSelection"

def getMainWindow():
    import maya.OpenMayaUI as mui
    pointer = mui.MQtUtil.mainWindow()
    return sip.wrapinstance(long(pointer), QtCore.QObject)

class SceneExporter(QtGui.QMainWindow):

    def __init__(self, parent=None):
        super(SceneExporter, self).__init__(parent)
        uic.loadUi(uiFile, self)
        self.setObjectName(windowObject)
        self.setWindowTitle(windowTitle)
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
     
        self.mainWidget = QtGui.QWidget()
        self.setCentralWidget(self.mainWidget)
        self.mainWidget.setLayout(self.gridLayout)
        
        self.textureFolder = os.path.join(os.path.dirname(__file__),"Textures")
        self.meshesFolder = os.path.join(os.path.dirname(__file__),"Meshes")
        self.checkFolderPath(self.textureFolder)
        self.checkFolderPath(self.meshesFolder)

        self.xmlFilename = "Meshes.xml"
        self.emptyTexture = "None"
        self.defaultTexture = "plain.png"
        self.supportedImageTypes = ["png","jpg","jpeg","bmp"]

        # Attribute names for Maya
        self.mattr = self.StringEnum(SpecBlinn="specularRollOff",\
                                     SpecPhong="highlightSize",\
                                     Diffuse="diffuse",\
                                     TexName="fileTextureName",\
                                     Bump="normalCamera",\
                                     BumpTex="outAlpha",\
                                     BumpNodeIn="bumpValue",\
                                     BumpNodeOut="outNormal",\
                                     Color="color",\
                                     ColorSpecTex="outColor",\
                                     Spec="specularColor",\
                                     Notes="notes")
        
        # Attribute names for the fragment linker
        self.attr = self.StringEnum(Specularity="Specularity",\
                                    Parallax="Parallax",\
                                    Flat="Flat",\
                                    Shadow="Shadow",\
                                    Normal="Normal",\
                                    Specular="Specular",\
                                    Culling="Culling",\
                                    Diffuse="Diffuse")
        
        self.scriptJobsFixed = []
        self.scriptJobsFixed.append(cmds.scriptJob(kws=True, event=["Undo",self.fillSceneList]))
        self.scriptJobsFixed.append(cmds.scriptJob(kws=True, event=["Redo",self.fillSceneList]))
        self.scriptJobsFixed.append(cmds.scriptJob(kws=True, event=["DagObjectCreated",self.fillSceneList]))
        self.scriptJobsFixed.append(cmds.scriptJob(kws=True, event=["NameChanged",self.fillSceneList]))
        self.scriptJobs = []

        self.selectedItem = noSelection
        self.SelectedLbl.setText(noSelection)
        self.fillSceneList()

        # Check Box Widgets
        self.ParallaxCB.noteAttr = self.attr.Parallax
        self.CullingCB.noteAttr = self.attr.Culling
        self.ShadowsCB.noteAttr = self.attr.Shadow
        self.ParallaxCB.stateChanged.connect(self.checkBoxChanged)
        self.ShadowsCB.stateChanged.connect(self.checkBoxChanged)
        self.CullingCB.stateChanged.connect(self.checkBoxChanged)

        # List and Button Widgets
        self.SceneView.itemDoubleClicked.connect(self.focusOnNode)
        self.SceneView.currentItemChanged.connect(self.selectNode)
        self.SaveMeshes.clicked.connect(self.saveMeshes)
        self.SelectShader.clicked.connect(self.selectShader)
        self.Update.clicked.connect(self.fillSceneList)
        self.ShaderLE.setReadOnly(True)

        # Combo box Widgets
        self.TextureCB.mattr = self.mattr.Color
        self.SpecularCB.mattr = self.mattr.Spec
        self.NormalCB.mattr = self.mattr.Bump
        self.TextureCB.tex_mattr = self.mattr.ColorSpecTex
        self.SpecularCB.tex_mattr = self.mattr.ColorSpecTex
        self.NormalCB.tex_mattr = self.mattr.BumpTex
        self.TextureCB.activated.connect(self.changeMeshTexture)
        self.NormalCB.activated.connect(self.changeMeshTexture)
        self.SpecularCB.activated.connect(self.changeMeshTexture)

        # Spin box Widgets
        self.SpecularitySB.setSingleStep(0.05) 
        self.SpecularitySB.valueChanged.connect(self.spinBoxChanged)
        self.SpecularitySB.editingFinished.connect(self.spinBoxEditingFinished)
        self.SpecularitySB.lineEdit().textEdited.connect(self.spinBoxEditText)
        self.SpecularitySB.lineEdit().isEditingText = False
        self.isSetup = 1

    ##
    # Enum helper method
    # @return in form of keyword=value
    def StringEnum(self,**enums):
        return type("StringEnum", (), enums)

    ##
    # On new selection in the Scene View list widget
    # @param the new QListWidgetItem selected
    def selectNode(self,item):
        if type(item) is type(QtGui.QListWidgetItem()):
            self.selectedItem = str(item.text())
            self.updateAttributes()

    ##
    # Resets all attribute widgets
    def resetAttributes(self):
        self.ShaderLE.setText("")
        self.ParallaxCB.setChecked(False)
        self.ShadowsCB.setChecked(False)
        self.CullingCB.setChecked(False)
        self.TextureCB.clear()
        self.NormalCB.clear()
        self.SpecularCB.clear()
        self.SpecularitySB.setValue(0.0)

    ##
    # Get the specularity value from the given shader node
    # @param the name of the shader node
    # @return the specularity value
    def getSpecularity(self, shaderNode):
        value = self.getShaderAttribute(shaderNode+"."+self.mattr.SpecPhong)
        if value == noAttribute:
            value = self.getShaderAttribute(shaderNode+"."+self.mattr.SpecBlinn)
            if value == noAttribute:
                value = 0.0
        return value

    ##
    # Updates a combo box widget and sets currently active texture in the shader node
    # @param the combo box to update
    # @param the shader node attached to the mesh
    # @param a list of available textures to fill the box
    def updateComboBox(self, comboBox, shaderNode, textures):
        comboBox.clear()
        comboBox.addItems(textures)
        isBump = (comboBox.mattr == self.mattr.Bump)
        textureName = self.getTextureName(shaderNode+"."+comboBox.mattr, isBump)
        comboBox.setCurrentIndex(comboBox.findText(textureName))

    ##
    # Toggles whether the spin box is currently being edited
    def spinBoxEditText(self):
        self.sender().isEditingText = True

    ##
    # On loss of focus or return of a spin box
    def spinBoxEditingFinished(self):
        self.sender().lineEdit().isEditingText = False
        success = False
        if self.selectedItem != noSelection:
            success = self.spinBoxSetSpecularity(self.sender(), self.selectedItem)
        self.sender().setHidden(success)
        self.SpecularityLbl.setHidden(success)

    ##
    # On value change of spin box
    def spinBoxChanged(self):    
        if not self.sender().lineEdit().isEditingText:
            success = False
            if self.selectedItem != noSelection:
                success = self.spinBoxSetSpecularity(self.sender(), self.selectedItem)
            self.sender().setHidden(success)
            self.SpecularityLbl.setHidden(success)

    ##
    # On state change of a check box add/remove value to the mesh notes
    def checkBoxChanged(self):
        if self.selectedItem != noSelection:
            keyExists = self.meshNotesHaveKey(self.selectedItem, self.sender().noteAttr)
            if self.sender().isChecked() and not keyExists:
                self.addKeyToMeshNotes(self.selectedItem, self.sender().noteAttr)
            elif not self.sender().isChecked() and keyExists:
                self.removeKeyFromMeshNotes(self.selectedItem, self.sender().noteAttr)
            self.ShaderLE.setText(self.getShaderType(self.selectedItem))

    ##
    # Sets the specularity from the spin box to the given mesh shader
    # @param the spin box
    # @param the mesh currently used
    # @param the shader attribute to set
    # @param whether the call failed or not
    def spinBoxSetSpecularity(self, spinbox, mesh):
        value = float(spinbox.value())
        shaderNode = self.getShaderNode(mesh)
        if self.setAttribute(shaderNode+"."+self.mattr.SpecPhong, value) == False:
            if self.setAttribute(shaderNode+"."+self.mattr.SpecBlinn, value) == False:
                return True
        return False

    ##
    # Gets the filename of the texture attached to the texture node
    # @param the shader attribute which has the texture node attached
    # @param whether the shaderAttr is connected to a bump node or not
    # @return the filename of the texture
    def getTextureName(self, attribute, isBump):
        textureNode = self.getTextureNode(attribute, isBump)
        if textureNode != noSelection:
            textureName = cmds.getAttr(textureNode+"."+self.mattr.TexName)
            if len(textureName) > 1:
                return textureName.split(os.path.dirname(textureName)+"/",1)[1]
        return self.emptyTexture

    ##
    # Gets the texture node attached to a mesh shader attribute
    # @param the shader attribute which has the texture node attached
    # @param whether attribute is connected to a bump node or not
    # @return the texture node connected to the shader
    def getTextureNode(self, attribute, isBump):
        textureNode = self.getShaderConnection(attribute)
        if (textureNode != noSelection) and (isBump == True):
            textureNode = self.getShaderConnection(textureNode+"."+self.mattr.BumpNodeIn)
        return textureNode       

    ##
    # @return list of available textures in the current folder
    def getAvailableTextures(self):
        files = os.listdir(self.textureFolder)
        images = [self.emptyTexture]
        for filename in files:
            if os.path.isfile(os.path.join(self.textureFolder, filename)):
                if self.isValidImage(filename):
                    images.append(filename)
        return images

    ## 
    # Checks if extension matches supported image types
    # @param the filename of the image
    # @return whether image is valid or not
    def isValidImage(self, filename):
        splitFile = filename.split(os.extsep, 1)
        if len(splitFile) > 1:
            extension = splitFile[1].lower()
            for imageType in self.supportedImageTypes:
                if extension == imageType:
                    return True
        return False

    ## 
    # Checks if the filename has the given extension type
    # @param the filename of the image
    # @param the extension to check for
    # @return whether image is valid or not
    def hasExtension(self, filename, ext):
        splitFile = filename.split(os.extsep, 1)
        if len(splitFile) > 1:
            if splitFile[1].lower() == ext:
                return True
        return False
        
    ##
    # On push of Select Shader Button
    def selectShader(self):
        if self.selectedItem != noSelection:
            cmds.select(self.selectedItem)
            cmds.hyperShade(smn=True)

    ##
    # Gets the shader node attached to the mesh
    # @param the mesh involved
    # @return the shader node attached to the mesh
    def getShaderNode(self, mesh):
        currentSel = cmds.ls(sl=True)
        cmds.select(mesh)
        cmds.hyperShade(smn=True)
        name = cmds.ls(sl=True)[0]      
        if len(currentSel) > 0:
            cmds.select(currentSel)          
        return name

    ##
    # Connect together two Maya attributes
    # @param the attributes to connect
    # @return whether the call succeeded
    def connectAttributes(self, attr1, attr2):
        try:
            cmds.connectAttr(attr1,attr2,force=True)
            return True
        except (RuntimeError, TypeError, ValueError) as err:
            return False

    ##
    # Sets a string value for a given attribute
    # @param the maya attribute to set
    # @param the value to set the attribute to
    # @return whether the call succeeded
    def setStringAttribute(self, attr, value):
        try:
            cmds.setAttr(attr, value, type="string")
            return True
        except (RuntimeError, TypeError, ValueError) as err:
            return False

    ##
    # Sets a value for a given attribute
    # Note: this may fail for calls that require an explicit type set
    # @param the maya attribute of the Mesh Shader
    # @param the value to set the attribute to
    # @return whether the call succeeded
    def setAttribute(self, attribute, value):
        try:
            cmds.setAttr(attribute,value)
            return True
        except (RuntimeError, TypeError, ValueError) as err:
            return False

    ##
    # Returns the value for a given shader attribute or connected attribute
    # @param the maya attribute of the Mesh Shader
    # @param the attribute connected to the shader attribute
    # @return the value of the attribute or noAttribute if it doesn't exist
    def getShaderAttribute(self, attribute, connectedAttr = noAttribute):
        try:
            if connectedAttr != noAttribute:
                attribute = cmds.listConnections(attribute, destination=False)[0] + "." + connectedAttr
            return cmds.getAttr(attribute)
        except (ValueError,TypeError) as err:
            return noAttribute

    ##
    # Returns the name of the connected node to the given Maya shader attribute
    # @param the maya attribute of the Mesh Shader
    # @return node name connected to the attribute or noSelection if nothing connected
    def getShaderConnection(self, attribute):
        try:
            return cmds.listConnections(attribute, destination=False)[0]
        except (ValueError,TypeError) as err:
            return noSelection

    ##
    # Gets the notes as a string for the mesh transform node
    # @param the mesh involved
    # @return the notes attached to the mesh
    def getMeshNotes(self, mesh):
        noteAttr = mesh + "." + self.mattr.Notes
        try:
            return str(cmds.getAttr(noteAttr))
        except ValueError as err:
            self.addMeshNotesAttr(mesh)
            return str("")

    ##
    # Adds a note attribute to the mesh if it doesn't exist
    # @param the mesh involved
    def addMeshNotesAttr(self, mesh):
        currentSel = cmds.ls(sl=True)     
        cmds.select(mesh)
        cmds.addAttr(longName=self.mattr.Notes, dataType="string")
        if len(currentSel) > 0:
            cmds.select(currentSel)

    ##
    # Searches whether the notes section has the given key
    # @param the mesh involved
    # @param the string key to search
    # @return whether the key exists in the mesh notes or not
    def meshNotesHaveKey(self, mesh, key):
        return ((self.getMeshNotes(mesh)).lower()).count(key.lower()) > 0

    ##
    # Adds a string key to the mesh notes
    # @param the mesh involved
    # @param the string key to add
    def addKeyToMeshNotes(self, mesh, key):
        notes = self.getMeshNotes(mesh) + key
        cmds.setAttr(mesh + "." + self.mattr.Notes, notes, type="string")

    ##
    # Removes a string key from the mesh notes
    # @param the mesh involved
    # @param the string key to remove
    def removeKeyFromMeshNotes(self, mesh, key):
        notes = (self.getMeshNotes(mesh)).replace(key,"")
        cmds.setAttr(mesh + "." + self.mattr.Notes, notes, type="string")

    ##
    # Generates a complilation of shader components for the fragment linker
    # @param the mesh involved
    # @return the compiled shader name
    def getShaderType(self, mesh):
        shaderType = "FX"
        if mesh != noSelection:
            shaderNode = self.getShaderNode(mesh)
            if self.getShaderAttribute(shaderNode+"."+self.mattr.Spec) != noAttribute:
                shaderType += self.attr.Specular
            if self.getShaderAttribute(shaderNode+"."+self.mattr.Diffuse) == noAttribute:
                shaderType += self.attr.Flat
            if self.getShaderAttribute(shaderNode+"."+self.mattr.Bump, self.mattr.BumpNodeOut) != noAttribute:
                shaderType += self.attr.Normal
            if self.meshNotesHaveKey(mesh, self.attr.Parallax):
                shaderType += self.attr.Parallax
        return shaderType

    ##
    # @return all meshes in the scene
    def getScene(self):
        scene = cmds.ls(type="geometryShape")
        if len(scene) > 0:
            return cmds.listRelatives(scene,type="transform",parent=True)
        return scene

    ##
    # On focus on the main window
    def focusInEvent(self,e):
        self.fillSceneList()

    ##
    # On double click of list item in Scene View
    def focusOnNode(self,item):
        itemName = str(item.text())
        if cmds.objExists(itemName):
            cmds.select([itemName])
            cmds.FrameSelected(itemName)

    ##
    # Fills the main list with the Maya scene
    def fillSceneList(self):
        savedJobs = []
        for job in self.scriptJobs:
            savedJobs.append(job)
        self.scriptJobs = []

        sceneItems = self.getScene()
        self.SceneView.clear()
        for item in sceneItems:
            self.SceneView.addItem(item)
            self.scriptJobs.append(cmds.scriptJob(kws=True, nodeDeleted=[item, self.fillSceneList]))
        self.SceneView.sortItems()

        self.deleteScriptJobs(savedJobs)
        self.updateAttributes()

    ##
    # Updates the attribute widgets; this includes changing the list 
    # widget color and resetting the attribute widgets if no selection
    def updateAttributes(self):
        for i in range(self.SceneView.count()):
            self.SceneView.item(i).setBackgroundColor(QtGui.QColor(0,0,0,0))

        SelWidgets = self.SceneView.findItems(self.selectedItem,QtCore.Qt.MatchExactly)
        if len(SelWidgets) == 0:
            self.selectedItem = noSelection
        else:
            SelWidgets[0].setBackgroundColor(QtGui.QColor(103,141,178))

        mesh = self.selectedItem
        self.SelectedLbl.setText(mesh)
        if mesh != noSelection:
            shaderType = self.getShaderType(mesh)
            self.ShaderLE.setText(shaderType)
            self.ParallaxCB.setChecked(self.meshNotesHaveKey(mesh, self.ParallaxCB.noteAttr))
            self.ShadowsCB.setChecked(self.meshNotesHaveKey(mesh, self.ShadowsCB.noteAttr))
            self.CullingCB.setChecked(self.meshNotesHaveKey(mesh, self.CullingCB.noteAttr))
    
            textures = self.getAvailableTextures()
            shaderNode = self.getShaderNode(self.selectedItem)
            self.updateComboBox(self.TextureCB, shaderNode, textures)
            self.updateComboBox(self.NormalCB, shaderNode, textures)
            self.updateComboBox(self.SpecularCB, shaderNode, textures)
            
            noSpec = (shaderType.count(self.attr.Specular) == 0)
            shaderNode = self.getShaderNode(mesh)
            self.SpecularitySB.setValue(self.getSpecularity(shaderNode))
            self.SpecularitySB.setHidden(noSpec)
            self.SpecularityLbl.setHidden(noSpec)
            self.SpecularCB.setHidden(noSpec)
        else:
            self.resetAttributes()

    ##
    # On change of selected combo box item, changes attached Maya texture
    # Will create new textures nodes as needed
    def changeMeshTexture(self):
        if self.sender().keyPressed:
            texture = str(self.sender().currentText())
            if (texture != self.emptyTexture) and (self.selectedItem != noSelection):
                isBump = (self.sender().mattr == self.mattr.Bump)
                shaderAttr = self.getShaderNode(self.selectedItem)+"."+self.sender().mattr
                textureNode = self.getTextureNode(shaderAttr, isBump)
                if textureNode == noSelection:
                    textureNode = self.createTextureNode(self.sender().mattr, self.sender().tex_mattr)
                if len(textureNode) > 0:
                    texture = os.path.join(self.textureFolder, texture)
                    self.setStringAttribute(textureNode+"."+self.mattr.TexName, texture)
                                                         
    ##
    # Create new texture node
    # @param attribute the new node will connect to
    # @param attribute of the new node that will connect to the shader
    # @return the name of the new texture node created
    def createTextureNode(self, shaderMattr, textureMattr):
        textureNode = ""
        shaderNode = self.getShaderNode(self.selectedItem)
        if self.getShaderAttribute(shaderNode+"."+shaderMattr) != noAttribute:
            textureNode = cmds.shadingNode("file", asTexture=True)
            textureOut = textureNode+"."+textureMattr
            shaderIn = shaderNode+"."+shaderMattr
            if shaderMattr == self.mattr.Bump:
                bumpNode = cmds.shadingNode("bump2d", asUtility=True)
                bumpOut = bumpNode+"."+self.mattr.BumpNodeOut
                bumpIn = bumpNode+"."+self.mattr.BumpNodeIn
                self.connectAttributes(bumpOut, shaderIn)
                self.connectAttributes(textureOut, bumpIn)
            else:
                self.connectAttributes(textureOut, shaderIn)
        return textureNode

    ##
    # Checks if folder exists and creates if not
    def checkFolderPath(self, pathname):
        if not os.path.exists(pathname):
            os.makedirs(pathname)

    ##
    # Saves each mesh in the scene as an obj
    def saveMeshesAsOBJ(self):
        scene = self.getScene()
        for mesh in scene:
            filename = os.path.join(self.meshesFolder,mesh+".obj")
            cmds.file(filename, force=True, options="", typ="OBJexport", stx="never", es=True)
        files = os.listdir(self.meshesFolder)
        for f in files:
            if self.isValidImage(f) or self.hasExtension(f,"mtl"):
                os.remove(os.path.join(self.meshesFolder,f))
    ##
    # Saves the scene to an xml
    def saveMeshes(self):
        text = "\n<Meshes>\n"
        sceneItems = self.getScene()
        for mesh in sceneItems:
            text += "\t<Mesh>\n" + self.translateMeshToXML(mesh) + "\t</Mesh>\n"
        text += "</Meshes>"
        print text
        textAsXML = xml.dom.minidom.parseString(text).toxml()
        with open(os.path.join(os.path.dirname(__file__),self.xmlFilename), "w") as fl:
            fl.write(textAsXML)
        self.saveMeshesAsOBJ()

    ##
    # @param the attribute value to insert
    # @param the xml tag for the attribute
    # @return xml in form of <tag>value</tag>
    def translateAttToXML(self, value, tag):
        return "\t\t<" + tag + ">" + str(value) + "</" + tag + ">\n"

    ##
    # @param the attribute connected to the Mesh Shader
    # @param the xml tag for the attribute
    # @param whether to use the default texture if no texture node exists
    # @return a string in form of <tag>textureName</tag> or ""
    def translateTextureToXML(self, attribute, tag, useDefaultTex = False):
        isBump = (tag == self.attr.Normal)
        textureName = self.getTextureName(attribute, isBump)
        if self.isValidImage(textureName):
            return self.translateAttToXML(textureName, tag)
        if useDefaultTex:
            return self.translateAttToXML(self.defaultTexture, tag)
        return ""
        
    ##
    # Returns a string of the mesh components for xml
    # @param the mesh involved
    # @return xml in form of <tag>value</tag>...<tag>value</tag>
    def translateMeshToXML(self, mesh):
        text = self.translateAttToXML(mesh+".obj","Name") + \
               self.translateAttToXML(self.getShaderType(mesh),"Shader")
 
        shaderNode = self.getShaderNode(mesh)
        text += self.translateAttToXML(self.getSpecularity(shaderNode), self.attr.Specularity)

        if self.meshNotesHaveKey(mesh, self.attr.Shadow):
            text += self.translateAttToXML(1, self.attr.Shadow)
        if self.meshNotesHaveKey(mesh, self.attr.Culling):
            text += self.translateAttToXML(1, self.attr.Culling)

        text += self.translateTextureToXML(shaderNode+"."+self.mattr.Bump, self.attr.Normal)
        text += self.translateTextureToXML(shaderNode+"."+self.mattr.Color, self.attr.Diffuse, True)
        text += self.translateTextureToXML(shaderNode+"."+self.mattr.Spec, self.attr.Specular)

        return text

    ##
    # On close of the main window
    def closeEvent(self, event): 
        if hasattr(self, "isSetup"):
            if self.isSetup == 1:
                self.isSetup = 0
                self.deleteScriptJobs(self.scriptJobs)
                self.deleteScriptJobs(self.scriptJobsFixed)
                self.scriptJobsFixed = []
                self.scriptJobs = []
                QtGui.QMainWindow.closeEvent(self, event)

    ##
    # Force kills script jobs in Maya
    # @param list of script job numbers to delete
    def deleteScriptJobs(self,jobList):
        if len(jobList) > 0:
            for job in jobList:
                try:
                    if cmds.scriptJob(exists=job) == True:
                        cmds.scriptJob(kill=job)
                except RuntimeError as err:
                    self.scriptJobsFixed.append(job)

def mayaRun():
    if cmds.window(windowObject, exists=True):
        cmds.deleteUI(windowObject)
    global gui
    gui = SceneExporter(getMainWindow())
    gui.show()
    
mayaRun()
