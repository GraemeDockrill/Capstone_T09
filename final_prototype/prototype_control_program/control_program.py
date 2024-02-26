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

def button_click(button_number):
    print(f"Button {button_number} clicked!")

def on_scale_change(value):
    print("Scale Value: " + value)

def refresh_ports(event=None):
    # Get a list of available COM ports
    ports = [port.device for port in list_ports.comports()]
    # Update the dropdown menu with the new ports
    port_dropdown['menu'].delete(0, 'end')
    for port in ports:
        port_dropdown['menu'].add_command(label=port, command=tk._setit(selected_port, port))

def on_port_selected(*args):
    print("Selected port:", selected_port.get())

if __name__ == "__main__":
    # initialize tkinter root
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

    # create COM Port drop down
    selected_port = tk.StringVar()
    port_dropdown = tk.OptionMenu(root, selected_port, "")
    port_dropdown.grid(row=0, column=0, sticky="nsew", columnspan=2)
    refresh_ports()
    selected_port.trace_add('write', on_port_selected)

    # create labels
    lbl_baud = tk.Label(root, text="Baud:")
    lbl_baud.grid(row=0, column=3, sticky="nsew")
    lbl_load_cell_graph = tk.Label(root, text="Loadcell Force")
    lbl_load_cell_graph.grid(row=0, column=6, sticky="nsew")
    lbl_manual_movement = tk.Label(root, text="Manual Movement")
    lbl_manual_movement.grid(row=1, column=3, columnspan=3, sticky="nsew")
    lbl_membrane_parameters = tk.Label(root, text="Membrane Parameters")
    lbl_membrane_parameters.grid(row=4, column=1, columnspan=5, sticky="nsew")
    lbl_membrane_size = tk.Label(root, text="Membrane Size [mm]")
    lbl_membrane_size.grid(row=5, column=1, columnspan=3, sticky="nsew")
    lbl_strain_target = tk.Label(root, text="Strain Target [%]")
    lbl_strain_target.grid(row=6, column=1, columnspan=3, sticky="nsew")
    lbl_strain_rate = tk.Label(root, text="Strain Rate [%/s]")
    lbl_strain_rate.grid(row=7, column=1, columnspan=3, sticky="nsew")
    lbl_cyclic_stretching = tk.Label(root, text="Cyclic Stretching")
    lbl_cyclic_stretching.grid(row=11, column=1, columnspan=5, sticky="nsew")
    lbl_number_of_cycles = tk.Label(root, text="Number of Cycles")
    lbl_number_of_cycles.grid(row=12, column=1, columnspan=3, sticky="nsew")
    lbl_save_to_file = tk.Label(root, text="Save to File")
    lbl_save_to_file.grid(row=15, column=1, columnspan=2, sticky="nsew")
    lbl_position_graph = tk.Label(root, text="Motor Position")
    lbl_position_graph.grid(row=8, column=6, sticky="nsew")
    lbl_load_cell1_reading = tk.Label(root, text="Load Cell 1 [kg]")
    lbl_load_cell1_reading.grid(row=1, column=7, sticky="nsew")
    lbl_load_cell2_reading = tk.Label(root, text="Load Cell 2 [kg]")
    lbl_load_cell2_reading.grid(row=4, column=7, sticky="nsew")
    lbl_motor1_reading = tk.Label(root, text="Motor 1 Position [mm]")
    lbl_motor1_reading.grid(row=9, column=7, sticky="nsew")
    lbl_motor1_position = tk.Label(root, text="")
    lbl_motor1_position.grid(row=9, column=8, sticky="nsew")
    lbl_motor2_reading = tk.Label(root, text="Motor 2 Position [mm]")
    lbl_motor2_reading.grid(row=11, column=7, sticky="nsew")
    lbl_motor2_position = tk.Label(root, text="")
    lbl_motor2_position.grid(row=11, column=8, sticky="nsew")
    lbl_motor3_reading = tk.Label(root, text="Motor 3 Position [mm]")
    lbl_motor3_reading.grid(row=13, column=7, sticky="nsew")
    lbl_motor3_position = tk.Label(root, text="")
    lbl_motor3_position.grid(row=13, column=8, sticky="nsew")
    lbl_motor4_reading = tk.Label(root, text="Motor 4 Position [mm]")
    lbl_motor4_reading.grid(row=15, column=7, sticky="nsew")
    lbl_motor4_position = tk.Label(root, text="")
    lbl_motor4_position.grid(row=15, column=8, sticky="nsew")

    # create text entry
    txt_baud = tk.Entry(root)
    txt_baud.grid(row=0, column=4, sticky="nsew")
    txt_membrane_size = tk.Entry(root)
    txt_membrane_size.grid(row=5, column=4, columnspan=2, sticky="nsew")
    txt_strain_target = tk.Entry(root)
    txt_strain_target.grid(row=6, column=4, columnspan=2, sticky="nsew")
    txt_strain_rate = tk.Entry(root)
    txt_strain_rate.grid(row=7, column=4, columnspan=2, sticky="nsew")
    txt_number_of_cycles = tk.Entry(root)
    txt_number_of_cycles.grid(row=12, column=4, columnspan=2, sticky="nsew")
    

    # create buttons
    btn_connect = tk.Button(root, text="Connect", command=lambda: button_click(1))
    btn_connect.grid(row=0, column=5, sticky="nsew")
    btn_set_zero = tk.Button(root, text="Set Zero", command=lambda: button_click(1))
    btn_set_zero.grid(row=1, column=1, columnspan=2, sticky="nsew")
    btn_stop = tk.Button(root, text="STOP", command=lambda: button_click(1))
    btn_stop.grid(row=2, column=1, rowspan=2, columnspan=2, sticky="nsew")
    btn_set_parameters = tk.Button(root, text="Set Parameters", command=lambda: button_click(1))
    btn_set_parameters.grid(row=8, column=1, columnspan=5, sticky="nsew")
    btn_move_to_zero = tk.Button(root, text="Move to 0", command=lambda: button_click(1))
    btn_move_to_zero.grid(row=9, column=1, rowspan=2, columnspan=3, sticky="nsew")
    btn_move_to_max = tk.Button(root, text="Move to Set Strain", command=lambda: button_click(1))
    btn_move_to_max.grid(row=9, column=4, rowspan=2, columnspan=2, sticky="nsew")
    btn_start_cyclic_test = tk.Button(root, text="Start Cyclic Test", command=lambda: button_click(1))
    btn_start_cyclic_test.grid(row=13, column=1, rowspan=2, columnspan=5, sticky="nsew")
    btn_save_to_file = tk.Button(root, text="Start Saving", command=lambda: button_click(1))
    btn_save_to_file.grid(row=15, column=3, columnspan=3, sticky="nsew")

    # create sliding scale
    scale = tk.Scale(root, from_=0, to=100, orient=tk.HORIZONTAL, command=on_scale_change)
    scale.grid(row=2, column=3, rowspan=2, columnspan=3, sticky="nsew")
    


    # create and start the serial reader thread
    serial_reader_thread = SerialReaderThread(serial_port, baudrate, 
                                              packet_size, load_cell_queue, 
                                              encoder_queue, logger_queue)
    serial_reader_thread.name = "serial_reader_thread"
    serial_reader_thread.daemon = True
    serial_reader_thread.start()

    # create and start two instances of real time graph thread
    load_cell_graph_thread = RealTimeGraphThread(root, load_cell_queue, 2)
    load_cell_graph_thread.name = "load_cell_graph_thread"
    load_cell_graph_thread.daemon = True
    load_cell_graph_thread.start()

    encoder_graph_thread = RealTimeGraphThread(root, encoder_queue, 4)
    encoder_graph_thread.name = "encoder_graph_thread"
    encoder_graph_thread.daemon = True
    encoder_graph_thread.start()

    # create logger thread
    data_logger_thread = DataLoggerThread(file_name, logger_queue)
    data_logger_thread.name = "data_logger_thread"
    data_logger_thread.daemon = True
    data_logger_thread.start()

    # handle closing the window
    root.protocol("WM_DELETE_WINDOW", close_program)

    root.mainloop()

    # wait for threads to finish
    # serial_reader_thread.join()
    # load_cell_graph_thread.join()
    # encoder_graph_thread.join()
    # data_logger_thread.join()