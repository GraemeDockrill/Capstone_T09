import tkinter as tk
import serial
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation

class SerialReaderApp:
    def __init__(self, master):
        self.master = master
        master.title("Serial Reader")

        self.xData = []
        self.yData = []
        self.xDataTruncated = []
        self.yDataTruncated = []
        self.packetSize = 2  # Adjust this to the size of your packet
        self.truncateLimit = 100

        # Create a figure and axis for plotting
        self.fig = Figure(figsize=(6, 4))
        self.ax = self.fig.add_subplot(111)
        self.line, = self.ax.plot([], [], 'b-')

        # Create a canvas to display the plot
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)


        # Create a label for displaying serial data
        self.label = tk.Label(master, text="")
        self.label.pack()

        # Open the serial port
        self.serial_port = serial.Serial('COM6', 9600)  # Change 'COM1' to your port and 9600 to your baudrate


        # Start reading from the serial port
        self.read_serial()

    def read_serial(self):
        # Read one byte from the serial port
        dataPacket = self.serial_port.read(self.packetSize)

        # add values to list
        self.xData.append(dataPacket[0])
        self.yData.append(dataPacket[1])

        # truncate list to draw on plot
        self.xDataTruncated = self.xData[-self.truncateLimit:]
        self.yDataTruncated = self.yData[-self.truncateLimit:]

        # Update the label with the received data
        self.label.config(text="X: " + str(dataPacket[0]) + ", Y: " + str(dataPacket[1]))

        # print(int(byte))

        # Interpret the packet and plot it
        if len(dataPacket) == self.packetSize:
            # draw line using last truncated values
            self.line.set_xdata(self.xDataTruncated)
            self.line.set_ydata(self.yDataTruncated)

            self.ax.relim()
            self.ax.autoscale_view()

            # Redraw the plot
            self.canvas.draw()            


        # Schedule the next reading after 100 milliseconds
        self.master.after(10, self.read_serial)


    def close(self):
        # Close the serial port when the application is closed
        if self.serial_port.is_open:
            self.serial_port.close()
        self.master.destroy()  # Close the Tkinter window

def main():
    root = tk.Tk()
    app = SerialReaderApp(root)
    root.protocol("WM_DELETE_WINDOW", app.close)    # Handle window close event
    root.mainloop()

if __name__ == "__main__":
    main()
