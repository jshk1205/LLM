#바이너리 에디터를 만들어보자
#사용자가 파일을 선택해서 열 수 있어야한다.
#바이너리 파일을 읽어서 헥사데이터를 보여주고 파일을 저장할 수 있게 한다.
#헥사데이터를 수정할 수 있어야한다.
#헥사데이터를 수정하면 파일을 저장할 수 있어야한다.
#바이너리 에디터를 만들기 위해서는 QTableWidget을 사용한다.
#pip install PyQt5 설치해야됨

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
import os

class BinaryEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle("Binary Editor")
        self.setGeometry(100, 100, 800, 600)

     
       
        self.table = QTableWidget(self)
        self.table.setGeometry(0, 0, 800, 500)
        self.table.setColumnCount(16)
        self.table.setRowCount(32)
        self.table.setHorizontalHeaderLabels([str(i) for i in range(16)])
        self.table.setVerticalHeaderLabels([str(i) for i in range(32)])
        self.table.horizontalHeader().setDefaultSectionSize(50)
        self.table.verticalHeader().setDefaultSectionSize(20)
        self.table.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table.setSelectionBehavior(QAbstractItemView.SelectItems)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.itemSelectionChanged.connect(self.on_itemSelectionChanged)
        self.table.setEditTriggers(QAbstractItemView.DoubleClicked | QAbstractItemView.EditKeyPressed)
        self.table.itemChanged.connect(self.itemChanged)
        self.openAction = QAction("Open", self)
        self.openAction.setShortcut("Ctrl+O")
        self.openAction.triggered.connect(self.openFile)

        self.saveAction = QAction("Save", self)
        self.saveAction.setShortcut("Ctrl+S")
        self.saveAction.triggered.connect(self.saveFile)

        menubar = self.menuBar()
        menubar.setNativeMenuBar(False)
        fileMenu = menubar.addMenu("File")
        fileMenu.addAction(self.openAction)
        fileMenu.addAction(self.saveAction)

        self.statusBar = QStatusBar(self)
        self.setStatusBar(self.statusBar)

        self.filename = None
        self.data = None
        self.modified = False

    def openFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.ReadOnly
        self.filename, _ = QFileDialog.getOpenFileName(self, "Open File", "", "All Files (*);;Binary Files (*.bin)", options=options)
        if self.filename:
            with open(self.filename, "rb") as f:
                self.data = bytearray(f.read())  # Change this line
            self.updateTable()  # Change this line
            self.setWindowTitle("Binary Editor - " + os.path.basename(self.filename))
            self.statusBar.showMessage("File opened", 2000)
    
    def itemChanged(self, item):
        if item.text() == "":
            return

        try:
            value = int(item.text(), 16)
        except ValueError:
            item.setText("00")
            self.statusBar.showMessage("Invalid hex value")
            return

        row = item.row()
        col = item.column()
        index = row*16 + col
        if index < len(self.data):
            self.data[index] = value  # Update the data
            self.modified = True

        self.statusBar.showMessage("Ready")

    def saveFile(self):
        options = QFileDialog.Options()
        self.filename, _ = QFileDialog.getSaveFileName(self, "Save File", "", "All Files (*);;Binary Files (*.bin)", options=options)
        if self.filename:
            with open(self.filename, "wb") as f:
                f.write(self.data)
            self.statusBar.showMessage("File saved", 2000)

    def updateTable(self):
        if self.data:
            self.table.clearContents()
            for i in range(32):
                for j in range(16):
                    if 16*i+j < len(self.data):
                        item = QTableWidgetItem("{:02X}".format(self.data[16*i+j]))
                        item.setTextAlignment(Qt.AlignCenter)
                        self.table.setItem(i, j, item)
                    else:
                        break

    def on_itemSelectionChanged(self):
        if self.data:
            selected = self.table.selectedItems()
            if selected:
                row = selected[0].row()
                col = selected[0].column()
                index = row*16 + col
                if index < len(self.data):
                    byte = self.data[index]
                    self.statusBar.showMessage("Offset: 0x{:X}, Value: 0x{:02X}".format(index, byte), 2000)

    def closeEvent(self, event):
        if self.modified:
            reply = QMessageBox.question(self, "Message", "Save changes?", QMessageBox.Yes | QMessageBox.No | QMessageBox.Cancel)
            if reply == QMessageBox.Yes:
                self.saveFile()
            elif reply == QMessageBox.Cancel:
                event.ignore()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    editor = BinaryEditor()
    editor.show()
    sys.exit(app.exec_())