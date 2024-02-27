# Classes used for plotting, serial communication, and logging to .csv
# Written by Graeme Dockrill - 2024

import serial
import threading
import queue
import time
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import struct
import csv

# Class for plotting a graph in real-time based on values placed in a queue
class RealTimeGraphThread(threading.Thread):
    def __init__(self, master, data_queue, y_sets):
        super(RealTimeGraphThread, self).__init__()
        self.data_queue = data_queue
        self.y_sets = y_sets
        self.running = True
        self.master = master
        self.x_data = []
        self.y1_data = []
        self.y2_data = []
        if self.y_sets == 4:
            self.y3_data = []
            self.y4_data = []

        self.truncate_limit = 10

        # Create a figure and axis for plotting
        self.fig = Figure(figsize=(7, 4))
        self.ax = self.fig.add_subplot(111)
        self.ax.set_xlabel('Time [s]')
        if y_sets == 4:
            self.ax.set_ylabel('Motor Position [mm]')
        else:
            self.ax.set_ylabel('Load Cell FOrce [kg]')
        self.line1, = self.ax.plot([], [], label='Line 1')
        self.line2, = self.ax.plot([], [], label='Line 2')
        if self.y_sets == 4:
            self.line3, = self.ax.plot([], [], label='Line 3')
            self.line4, = self.ax.plot([], [], label='Line 4')

        # Add legend
        self.ax.legend(loc='upper right', bbox_to_anchor=(1, 1))

        # Create a canvas to display the plot
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        if self.y_sets != 4:
            self.canvas.get_tk_widget().grid(row=1, column=6, sticky="nsew", rowspan=7)
        else:
            self.canvas.get_tk_widget().grid(row=9, column=6, sticky="nsew", rowspan=7)

    def run(self):
        while self.running:
            if not self.data_queue.empty():
                data = self.data_queue.get()

                # Add values to list
                self.x_data.append(data[0])
                self.y1_data.append(data[1])
                self.y2_data.append(data[2])
                if self.y_sets == 4:
                    self.y3_data.append(data[3])
                    self.y4_data.append(data[4])

                # Truncate to keep last values
                self.x_data = self.x_data[-self.truncate_limit:]
                self.y1_data = self.y1_data[-self.truncate_limit:]
                self.y2_data = self.y2_data[-self.truncate_limit:]
                if self.y_sets == 4:
                    self.y3_data = self.y3_data[-self.truncate_limit:]
                    self.y4_data = self.y4_data[-self.truncate_limit:]

                # Draw lines using last truncated values
                self.line1.set_xdata(self.x_data)
                self.line1.set_ydata(self.y1_data)

                self.line2.set_xdata(self.x_data)
                self.line2.set_ydata(self.y2_data)

                if self.y_sets == 4:
                    self.line3.set_xdata(self.x_data)
                    self.line3.set_ydata(self.y3_data)

                    self.line4.set_xdata(self.x_data)
                    self.line4.set_ydata(self.y4_data)

                self.ax.relim()
                self.ax.autoscale_view()

                # Redraw the plot
                self.canvas.draw()

            else:
                time.sleep(0.1)

    def close(self):
        self.running = False

# Class for reading from serial and adding data to data queues
class SerialPortThread(threading.Thread):
    def __init__(self, port, baudrate, packet_size, load_cell_queue, encoder_queue, logger_queue):
        super(SerialPortThread, self).__init__()
        self.port = port
        self.baudrate = baudrate
        self.packet_size = packet_size
        self.load_cell_queue = load_cell_queue
        self.encoder_queue = encoder_queue
        self.logger_queue = logger_queue
        self.running = True
        self.counter = 0
        self.counter_delay = 2

    # Loop for opening, and continually reading serial port
    def run(self):
        # Open serial port
        try:
            self.serial_port = serial.Serial(self.port, self.baudrate)

            # Continuous reading loop
            while self.running:
                if self.serial_port.in_waiting > 0:
                    data = self.serial_port.read(self.packet_size)

                    # Parsing incoming data bytes
                    time_stamp_sec = (struct.unpack('<l', data[1:5])[0]) / 1000
                    load_cell1_float = struct.unpack('<f', data[5:9])[0]
                    load_cell2_float = struct.unpack('<f', data[9:13])[0]
                    encoder1_pos = struct.unpack('<H', data[13:15])[0]
                    encoder2_pos = struct.unpack('<H', data[15:17])[0]
                    encoder3_pos = struct.unpack('<H', data[17:19])[0]
                    encoder4_pos = struct.unpack('<H', data[19:21])[0]
                    
                    # Print(time_stamp_sec)

                    # Creating and adding logger message to queue
                    logger_message = (time_stamp_sec, load_cell1_float, load_cell2_float, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)
                    self.logger_queue.put(logger_message)

                    # Only plot every 5th data point
                    if(self.counter >= self.counter_delay):
                        # Creating messages to plot
                        load_cell_message = (time_stamp_sec, load_cell1_float, load_cell2_float)
                        encoder_message = (time_stamp_sec, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)

                        # Queueing messages
                        self.load_cell_queue.put(load_cell_message)
                        self.encoder_queue.put(encoder_message)

                        self.counter = 0
                    
                    # Increment counter
                    self.counter = self.counter + 1
                    
                else:
                    time.sleep(0.1)

        except Exception as e:
            print("Error opening serial port")
            print(e)

    # Writing to serial port
    def write_to_serial(self, COM_message):
        if self.isOpen:
            try:
                print("Wrote " + str(COM_message) + " to serial port!")
                self.serial_port.write(COM_message)     # Write byte array to serial
            except Exception as e:
                print("Error writing to serial port!")
                print(e)


    # Returns if port is open
    def isOpen(self):
        return self.serial_port.is_open

    # Deals with closing the thread safely
    def close(self):
        self.running = False
        self.serial_port.close()

# Class for logging data in real-time
class DataLoggerThread(threading.Thread):
    def __init__(self, file_name, data_queue):
        super().__init__()
        self.file_name = file_name
        self.data_queue = data_queue
        self.running = True

        # Write header to file
        header = ['Time [s]', 'Load Cell 1 [kg]', 'Load Cell 2 [kg]', 
                            'Motor 1 Position [mm]', 'Motor 2 Position [mm]', 
                            'Motor 3 Position [mm]', 'Motor 4 Position [mm]']
        
        self.create_csv(header)

    def run(self):            
        # Continuous loop for logging data
        while self.running:
            data = self.data_queue.get()

            # If a "None" is added to the queue, quit
            if data is None:
                break;
    
            # Log the data
            self.write_to_csv(data)

    def create_csv(self, data):
        with open(self.file_name, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(data)

    # Function to write to csv
    def write_to_csv(self, data):
        with open(self.file_name, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(data)
    
    # Method to close thread
    def close(self):
        self.running = False