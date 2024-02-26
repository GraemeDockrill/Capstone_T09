# control program for MECH45X cell stretcher capstone
# written by Graeme Dockrill - 2024

import tkinter as tk
import serial
import threading
import queue
import time
from serial.tools import list_ports
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import struct
import csv

# class for plotting a graph in real-time based on values placed in a queue
class RealTimeGraphThread(threading.Thread):
    def __init__(self, data_queue):
        super(RealTimeGraphThread, self).__init__()
        self.data_queue = data_queue
        self.running = True

    def run(self):
        while self.running:
            if not self.data_queue.empty():
                data = self.data_queue.get()

                # print("Load Cell Queue: ", data)

                # do something with the data

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

    def run(self):
        # open serial port
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

                # parsing incoming data
                time_stamp_sec = (struct.unpack('<l', data[1:5])[0]) / 1000
                load_cell1_float = struct.unpack('<f', data[5:9])[0]
                load_cell2_float = struct.unpack('<f', data[9:13])[0]
                encoder1_pos = struct.unpack('<H', data[13:15])[0]
                encoder2_pos = struct.unpack('<H', data[15:17])[0]
                encoder3_pos = struct.unpack('<H', data[17:19])[0]
                encoder4_pos = struct.unpack('<H', data[19:21])[0]

                # print(time_stamp_sec)

                # creating messages to add to queues
                load_cell_message = (time_stamp_sec, load_cell1_float, load_cell2_float)
                encoder_message = (time_stamp_sec, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)
                logger_message = (time_stamp_sec, load_cell1_float, load_cell2_float, encoder1_pos, encoder2_pos, encoder3_pos, encoder4_pos)

                # add data to relevant queues
                self.load_cell_queue.put(load_cell_message)
                self.encoder_queue.put(encoder_message)
                self.logger_queue.put(logger_message)

            else:
                time.sleep(0.1)

    def close(self):
        self.running = False

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
        
        self.write_to_csv(header)

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

    def write_to_csv(self, data):
        with open(self.file_name, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(data)
    
    def close(self):
        self.running = False

def close_program():
    load_cell_graph_thread.close()
    encoder_graph_thread.close()
    serial_reader_thread.close()
    data_logger_thread.close()
    root.destroy()

if __name__ == "__main__":

    root = tk.Tk()
    root.title("Prototype Control Program")
    
    # create shared queues for load cell, encoder and logger data
    load_cell_queue = queue.Queue()
    encoder_queue = queue.Queue()
    logger_queue = queue.Queue()

    # define the serial port, baudrate and packet size
    serial_port = "COM6"
    baudrate = 480000000
    packet_size = 22

    # define file name
    file_name = "testfile.csv"

    # create and start the serial reader thread
    serial_reader_thread = SerialReaderThread(serial_port, baudrate, 
                                              packet_size, load_cell_queue, 
                                              encoder_queue, logger_queue)
    serial_reader_thread.start()

    # create and start two instances of real time graph thread
    load_cell_graph_thread = RealTimeGraphThread(load_cell_queue)
    load_cell_graph_thread.start()

    encoder_graph_thread = RealTimeGraphThread(encoder_queue)
    encoder_graph_thread.start()

    # create logger thread
    data_logger_thread = DataLoggerThread(file_name, logger_queue)
    data_logger_thread.start()

    # handle closing the window
    root.protocol("WM_DELETE_WINDOW", close_program)

    root.mainloop()

    # wait for threads to finish
    serial_reader_thread.join()
    load_cell_graph_thread.join()
    encoder_graph_thread.join()
    data_logger_thread.join()