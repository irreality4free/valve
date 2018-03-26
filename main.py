import sys
# Импортируем наш интерфейс из файла
from valveQt import *
from PyQt5 import QtCore, QtGui, QtWidgets
from time import sleep
# import serial


class MyWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Здесь прописываем событие нажатия на кнопку
        self.ui.pushButton.clicked.connect(self.ON)
        self.ui.pushButton_2.clicked.connect(self.OFF)

        self.ui.horizontalSlider.valueChanged.connect(self.lcdDisp)
        self.ui.lineEdit.textChanged.connect(self.texx)



    # Пока пустая функция которая выполняется
    # при нажатии на кнопку

    # parse text      self.ui.lineEdit.text()
    def ON(self):
        print('start')
        # ser.write('start')
        pass

    def OFF(self):
        print('finish')
        # ser.write('finish')


    def SendHPL(self):
        print('HPL:'+self.ui.lineEdit.text())
        # ser.write('HPL:'+self.ui.lineEdit.text())

    def SendLPL(self):
        print('LPL:'+self.ui.lineEdit_2.text())
        # ser.write('LPL:'+self.ui.lineEdit_2.text())

    def SendSlider(self):
        print(self.ui.horizontalSlider.value())

    def lcdDisp(self):
        self.ui.lcdNumber.display(self.ui.horizontalSlider.value())

    def texx(self):
        self.ui.label_5.setText(self.ui.lineEdit.text())


if __name__=="__main__":
    app = QtWidgets.QApplication(sys.argv)
    myapp = MyWin()
    myapp.show()
    # ser = serial.Serial('/dev/cu.usbmodem1411', 9600)
    sys.exit(app.exec_())