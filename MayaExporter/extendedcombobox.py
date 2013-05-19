from PyQt4 import QtGui

class ExtendedComboBox(QtGui.QComboBox):
    def __init__(self, parent=None):
        super(ExtendedComboBox, self).__init__(parent)
        self.keyPressed = False
    def mousePressEvent(self,e):
        self.keyPressed = True
        QtGui.QComboBox.mousePressEvent(self,e)
    def mouseReleaseEvent(self,e):
        self.keyPressed = False
        QtGui.QComboBox.mouseReleaseEvent(self,e)
