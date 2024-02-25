import tkinter as tk
import serial
from serial.tools import list_ports
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import struct
import threading

class SerialReaderApp:
    def __init__(self, master):
        self.master = master
        master.title("Serial Reader")

        self.x = 0
        self.y = 0
        self.xData = []
        self.yData = []
        self.xDataTruncated = []
        self.yDataTruncated = []
        self.packetSize = 10  # Adjust this to the size of your packet
        self.truncateLimit = 100
        self.readCOMDelay = 100

        self.selected_port = tk.StringVar()
        self.port_dropdown = tk.OptionMenu(self.master, self.selected_port, "")
        self.port_dropdown.pack(pady=20)

        self.refresh_ports()

        self.selected_port.trace_add('write', self.on_port_selected)

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

        # start thread for reading serial data and plotting to the graph
        self.serialThread = threading.Thread(target=self.read_serial, daemon=True)
        self.serialThread.start()

        # Start reading from the serial port
        # self.read_serial()

    def read_serial(self):

        # Read message from the serial port
        dataPacket = self.serial_port.read(self.packetSize)

        print(dataPacket)

        # self.x = (dataPacket[4] << 24) | (dataPacket[3] << 16) | (dataPacket[2] << 8) | dataPacket[1]

        # self.y = (dataPacket[8] << 24) | (dataPacket[7] << 16) | (dataPacket[6] << 8) | dataPacket[5]

        x_bytes = dataPacket[1:5]
        y_bytes = dataPacket[5:9]

        self.x = struct.unpack('<f', x_bytes)[0]
        self.y = struct.unpack('<f', y_bytes)[0]

        # add values to list
        self.xData.append(self.x)
        self.yData.append(self.y)

        # truncate list to draw on plot
        self.xDataTruncated = self.xData[-self.truncateLimit:]
        self.yDataTruncated = self.yData[-self.truncateLimit:]

        # Update the label with the received data
        self.label.config(text="X: " + str(self.x) + ", Y: " + str(self.y))

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
        self.master.after(self.readCOMDelay, self.read_serial)

    def refresh_ports(self, event=None):
        # Get a list of available COM ports
        ports = [port.device for port in list_ports.comports()]
        # Update the dropdown menu with the new ports
        self.port_dropdown['menu'].delete(0, 'end')
        for port in ports:
            self.port_dropdown['menu'].add_command(label=port, command=tk._setit(self.selected_port, port))

    def on_port_selected(self, *args):
        print("Selected port:", self.selected_port.get())

    def close(self):
        # Close the serial port when the application is closed
        if self.serial_port.is_open:
            self.serial_port.close()
        self.master.destroy()  # Close the Tkinter window

def main():
    root = tk.Tk()
    root.title("Prototype Control Program")
    app = SerialReaderApp(root)
    root.protocol("WM_DELETE_WINDOW", app.close)    # Handle window close event
    root.mainloop()

if __name__ == "__main__":
    main()
