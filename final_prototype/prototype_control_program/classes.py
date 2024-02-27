# classes used for plotting, serial communication, and logging to .csv
# written by Graeme Dockrill - 2024

import serial
import threading
import queue
import time
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import struct
import csv

# class for plotting a graph in real-time based on values placed in a queue
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

        # create a figure and axis for plotting
        self.fig = Figure(figsize=(6, 3))
        self.ax = self.fig.add_subplot(111)
        self.line1, = self.ax.plot([], [], label='Line 1')
        self.line2, = self.ax.plot([], [], label='Line 2')
        if self.y_sets == 4:
            self.line3, = self.ax.plot([], [], label='Line 3')
            self.line4, = self.ax.plot([], [], label='Line 4')

        # create a canvas to display the plot
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        if self.y_sets != 4:
            self.canvas.get_tk_widget().grid(row=1, column=6, sticky="nsew", rowspan=7)
        else:
            self.canvas.get_tk_widget().grid(row=9, column=6, sticky="nsew", rowspan=7)

    def run(self):
        while self.running:
            if not self.data_queue.empty():
                data = self.data_queue.get()

                # add values to list
                self.x_data.append(data[0])
                self.y1_data.append(data[1])
                self.y2_data.append(data[2])
                if self.y_sets == 4:
                    self.y3_data.append(data[3])
                    self.y4_data.append(data[4])

                
                # truncate to keep last values
                self.x_data = self.x_data[-self.truncate_limit:]
                self.y1_data = self.y1_data[-self.truncate_limit:]
                self.y2_data = self.y2_data[-self.truncate_limit:]
                if self.y_sets == 4:
                    self.y3_data = self.y3_data[-self.truncate_limit:]
                    self.y4_data = self.y4_data[-self.truncate_limit:]

                # draw lines using last truncated values
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

                # print("Load Cell Queue: ", data)

                # do something with the data
            else:
                time.sleep(0.1)

    def close(self):
        self.running = False

# class for reading from serial and adding data to data queues
class SerialReaderThread(threading.Thread):
    def __init__(self, port, baudrate, packet_size, load_cell_queue, encoder_queue, logger_queue):
        super(SerialReaderThread, self).__init__()
        self.port = port
        self.baudrate = baudrate
        self.packet_size = packet_size
        self.load_cell_queue = load_cell_queue
        self.encoder_queue = encoder_queue
        self.logger_queue = logger_queue
        self.running = True
        self.counter = 0
        self.counter_delay = 2

    # loop for opening, and continually reading serial port
    def run(self):
        # open serial port
        try:
            self.serial_port = serial.Serial(self.port, self.baudrate)

            # continuous reading loop
            while self.running:
                if self.serial_port.in_waiting > 0:
                    data = self.serial_port.read(self.packet_size)

                    # print("data: " + str(data[0]) + ", " + str(data[1]) + ", " + str(data[2]) + ", " + str(data[3]) + ", " + str(data[4]) + ", " + str(data[5]) + ", " + str(data[6]) + ", " + str(data[7]) + ", " + str(data[8]) + ", " + str(data[9]) + ", " + str(data[10]) + ", " + str(data[11]) + ", " + str(data[12]) + ", " + str(data[13]) + ", " + str(data[14]) + ", " + str(data[15]) + ", " + str(data[16]) + ", " + str(data[17]) + ", " + str(data[18]) + ", " + str(data[19]) + ", " + str(data[20]) + ", " + str(data[21]))

                    # # convert bytes into load cell data and time stamp
                    # load_cell1_float = struct.unpack('<f', data[1:5])[0]
                    # load_cell2_float = struct.unpack('<f', data[5:9])[0]
                    # time_stamp_sec = (struct.unpack('<l', data[9:13])[0]) / 1000

                    # parsing incoming data bytes
                    time_stamp_sec = (struct.unpack('<l', data[1:5])[0]) / 1000
                    load_cell1_float = struct.unpack('<f', data[5:9])[0]
                    load_cell2_float = struct.unpack('<f', data[9:13])[0]
                    encoder1_pos = struct.unpack('<H', data[13:15])[0]
                    encoder2_pos = struct.unpack('<H', data[15:17])[0]
                    encoder3_pos = struct.unpack('<H', data[17:19])[0]
                    encoder4_pos = struct.unpack('<H', data[19:21])[0]
                    
                    # print(time_stamp_sec)

                    # creating and adding logger message to queue
                    logger_message = (time_stamp_sec, load_cell1_float, load_cell2_float, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)
                    self.logger_queue.put(logger_message)

                    # only plot every 5th data point
                    if(self.counter >= self.counter_delay):
                        # creating messages to plot
                        load_cell_message = (time_stamp_sec, load_cell1_float, load_cell2_float)
                        encoder_message = (time_stamp_sec, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)

                        # queueing messages
                        self.load_cell_queue.put(load_cell_message)
                        self.encoder_queue.put(encoder_message)

                        self.counter = 0
                    
                    # increment counter
                    self.counter = self.counter + 1
                    
                else:
                    time.sleep(0.1)

        except Exception as e:
            print("Error opening serial port")
            print(e)

    # writing to serial port
    def write_to_serial(self, COM_message):
        if self.isOpen:
            try:
                print("Wrote " + str(COM_message) + " to serial port!")
                self.serial_port.write(COM_message)     # write byte array to serial
            except Exception as e:
                print("Error writing to serial port!")
                print(e)


    # returns if port is open
    def isOpen(self):
        return self.serial_port.is_open

    # deals with closing the thread safely
    def close(self):
        self.running = False
        self.serial_port.close()

# class for logging data in real-time
class DataLoggerThread(threading.Thread):
    def __init__(self, file_name, data_queue):
        super().__init__()
        self.file_name = file_name
        self.data_queue = data_queue
        self.running = True

        # write header to file
        header = ['Time [s]', 'Load Cell 1 [kg]', 'Load Cell 2 [kg]', 
                            'Motor 1 Position [mm]', 'Motor 2 Position [mm]', 
                            'Motor 3 Position [mm]', 'Motor 4 Position [mm]']
        
        self.create_csv(header)

    def run(self):            
        # continuous loop for logging data
        while self.running:
            data = self.data_queue.get()
            # print(data)

            # if a "None" is added to the queue, quit
            if data is None:
                break;
    
            # log the data
            self.write_to_csv(data)

    def create_csv(self, data):
        with open(self.file_name, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(data)

    # function to write to csv
    def write_to_csv(self, data):
        with open(self.file_name, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(data)
    
    # method to close thread
    def close(self):
        self.running = False