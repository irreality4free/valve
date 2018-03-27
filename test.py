import matplotlib.pyplot as plt
import numpy as np

# Data for plotting
while(1):
    r=7.0
    t = np.arange(0.0, r, 0.01)
    s = 1 + np.sin(2 * np.pi * t)

    # Note that using plt.subplots below is equivalent to using
    # fig = plt.figure() and then ax = fig.add_subplot(111)
    fig, ax = plt.subplots()
    ax.plot(t, s)

    ax.set(xlabel='cycles - samples/beat', ylabel='Pressure mmHg',
           title='Analysis')
    ax.grid()

    fig.savefig("test.png")
    plt.show()
    r=r+10



