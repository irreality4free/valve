

import sys, serial, argparse
import numpy as np
from time import sleep
from collections import deque

import matplotlib.pyplot as plt
import matplotlib.animation as animation


# plot class
class AnalogPlot:
    # constr
    def __init__(self, strPort, maxLen):
        # open serial port
        self.ser = serial.Serial(strPort, 9600)

        self.ax = deque([0.0] * maxLen)
        self.ay = deque([0.0] * maxLen)
        self.maxLen = maxLen
        self.strPort = strPort

    # add to buffer
    def addToBuf(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.pop()
            buf.appendleft(val)

    # add data
    def add(self, data):
        assert (len(data) == 2)
        self.addToBuf(self.ax, data[0])
        self.addToBuf(self.ay, data[1])

    # update plot
    def update(self, frameNum, a0, a1):
        try:
            line = self.ser.readline()
            print(line)
            data = [float(val) for val in line.split()]
            # print data
            if (len(data) == 2):
                self.add(data)
                a0.set_data(range(self.maxLen), self.ax)
                a1.set_data(range(self.maxLen), self.ay)
        except KeyboardInterrupt:
            print('exiting')

        return a0,

        # clean up

    def close(self):
        # close serial
        self.ser.flush()
        self.ser.close()

        print('exiting.')

    # main() function

    def Run(self):



        print('reading from serial port %s...' % self.strPort)

        # plot parameters


        print('plotting data...')

        # set up animation
        fig = plt.figure()


        # host = fig.add_subplot(111)
        # par1 = host.twinx()
        # par1.set_ylim(0, 4)
        # par1.set_ylabel("Temperature")
        # color2 = plt.cm.viridis(0.5)
        # p2, = par1.plot([0, 1, 2], [0, 3, 2], color=color2, label="Temperature")
        # lns = [p2]
        # host.legend(handles=lns, loc='best')
        # par1.yaxis.label.set_color(p2.get_color())



        ax = plt.axes(xlim=(0, 340), ylim=(0, 400))
        ax2 =ax.twinx()
        ax2.set_ylim((0, 400))
        ax2.set_xlim((0, 340))

        # ax2 =plt.axes(xlim=(0, 300), ylim=(-100, 300))

        a0, = ax.plot([], [], color='blue')
        a1, = ax2.plot([], [], color='red')
        # ax.plot(secondary_y=True, style='g')
        ax2.set_ylabel('Flow mL/s')
        ax.set(xlabel='cycles - samples/beat', ylabel='Pressure mmHg',
               title='Analysis')

        ax.grid()
        anim = animation.FuncAnimation(fig, self.update, fargs=(a0, a1), interval=50)

        # show plot
        plt.show()

        # clean up
        self.close()



    # def Exit(self):





