import sys
# Импортируем наш интерфейс из файла
from valveQt import *
from PyQt5 import QtCore, QtGui, QtWidgets
from analog_plot import AnalogPlot
from time import sleep
import serial


class MyWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Здесь прописываем событие нажатия на кнопку
        self.ui.pushButton.clicked.connect(self.ON)
        self.ui.pushButton_2.clicked.connect(self.OFF)
        self.ui.pushButton_3.clicked.connect(self.Plot)

        self.ui.horizontalSlider.valueChanged.connect(self.lcdDisp)
        self.ui.lineEdit.textChanged.connect(self.texx)
        self.ser =serial.Serial('/dev/cu.usbmodem1421', 9600)



    # Пока пустая функция которая выполняется
    # при нажатии на кнопку

    # parse text      self.ui.lineEdit.text()
    def ON(self):
        print('start')
        self.ser.write('start'.encode('utf-8'))

    def OFF(self):
        print('finish')
        self.ser.write('finish'.encode('utf-8'))


    def SendHPL(self):
        print('HPL:'+self.ui.lineEdit.text())
        self.ser.write(('HPL:'+self.ui.lineEdit.text()).encode('utf-8'))

    def SendLPL(self):
        print('LPL:'+self.ui.lineEdit_2.text())
        self.ser.write(('LPL:'+self.ui.lineEdit_2.text()).encode('utf-8'))

    def SendSlider(self):
        print(self.ui.horizontalSlider.value())
        self.ser.write(('SL:' + self.ui.horizontalSlider.value()).encode('utf-8'))

    def lcdDisp(self):
        self.ui.lcdNumber.display(self.ui.horizontalSlider.value())

    def texx(self):
        self.ui.label_5.setText(self.ui.lineEdit.text())

    def Plot(self):
        analogPlot = AnalogPlot('/dev/cu.usbmodem1421', 340)
        analogPlot.Run()


if __name__=="__main__":
    app = QtWidgets.QApplication(sys.argv)
    myapp = MyWin()
    myapp.show()
    sys.exit(app.exec_())

# analogPlot = AnalogPlot('/dev/cu.usbmodem1421', 100)
# analogPlot.Run()
