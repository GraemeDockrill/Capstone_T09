# control program for MECH45X cell stretcher capstone
# written by Graeme Dockrill - 2024

import tkinter as tk
from tkinter import filedialog
import serial
import threading
import queue
import time
from serial.tools import list_ports
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import struct
import csv
import classes

## files from classes go here

##


class CellStretcherApp:
    def __init__(self, master):
        self.master = master

        # define the serial port, baudrate and packet size
        self.serial_port = "COM6"
        self.baudrate = 480000000
        self.packet_size = 22

        # device parameters
        self.lead_mm_per_rev = 5
        self.motor_pulse_per_rev = 800
        self.encoder_pulse_per_rev = 1000
        self.max_steps = 65535              # placeholder value
        self.max_strain_target = 100        # placeholder value
        self.max_strain_rate = 100          # Placeholder value

        # define default file name
        self.file_name = "testfile.csv"

        # create shared queues for load cell, encoder and logger data
        self.load_cell_queue = queue.Queue()
        self.encoder_queue = queue.Queue()
        self.logger_queue = queue.Queue()

        # definte states
        self.currently_logging = False
        self.COM_connected = False

        # define widget cosmetics
        self.x_pad = 10
        self.y_pad = 5
        self.custom_font = ("Helvetica", 12, "bold")

        # start up UI modules
        self.create_ui()
        self.start_graphs()
        
    # called on start up to create UI
    def create_ui(self):
        # create COM Port drop down
        self.selected_port = tk.StringVar()
        self.port_dropdown = tk.OptionMenu(root, self.selected_port, "")
        self.port_dropdown.grid(row=0, column=0, sticky="nsew", columnspan=2)
        self.refresh_ports()
        self.selected_port.trace_add('write', self.on_port_selected)

        # create labels
        self.lbl_baud = tk.Label(root, text="Baud:", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_baud.grid(row=0, column=3, sticky="nsew")
        self.lbl_load_cell_graph = tk.Label(root, text="Loadcell Force", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_load_cell_graph.grid(row=0, column=6, sticky="nsew")
        self.lbl_manual_movement = tk.Label(root, text="Manual Movement", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_manual_movement.grid(row=1, column=3, columnspan=3, sticky="nsew")
        self.lbl_membrane_parameters = tk.Label(root, text="Membrane Parameters", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_membrane_parameters.grid(row=4, column=1, columnspan=5, sticky="nsew")
        self.lbl_membrane_size_mm = tk.Label(root, text="Membrane Size [mm]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_membrane_size_mm.grid(row=5, column=1, columnspan=3, sticky="nsew")
        self.lbl_strain_target = tk.Label(root, text="Strain Target [%]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_strain_target.grid(row=6, column=1, columnspan=3, sticky="nsew")
        self.lbl_strain_rate = tk.Label(root, text="Strain Rate [%/s]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_strain_rate.grid(row=7, column=1, columnspan=3, sticky="nsew")
        self.lbl_cyclic_stretching = tk.Label(root, text="Cyclic Stretching", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_cyclic_stretching.grid(row=11, column=1, columnspan=5, sticky="nsew")
        self.lbl_number_of_cycles = tk.Label(root, text="Number of Cycles", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_number_of_cycles.grid(row=12, column=1, columnspan=3, sticky="nsew")
        self.lbl_save_to_file = tk.Label(root, text="Save to File", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_save_to_file.grid(row=15, column=1, columnspan=2, sticky="nsew")
        self.lbl_position_graph = tk.Label(root, text="Motor Position", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_position_graph.grid(row=8, column=6, sticky="nsew")
        self.lbl_load_cell1_reading = tk.Label(root, text="Load Cell 1 [kg]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_load_cell1_reading.grid(row=1, column=7, sticky="nsew")
        self.lbl_load_cell2_reading = tk.Label(root, text="Load Cell 2 [kg]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_load_cell2_reading.grid(row=4, column=7, sticky="nsew")
        self.lbl_motor1_reading = tk.Label(root, text="Motor 1 Position [mm]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor1_reading.grid(row=9, column=7, sticky="nsew")
        self.lbl_motor1_position = tk.Label(root, text="", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor1_position.grid(row=9, column=8, sticky="nsew")
        self.lbl_motor2_reading = tk.Label(root, text="Motor 2 Position [mm]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor2_reading.grid(row=11, column=7, sticky="nsew")
        self.lbl_motor2_position = tk.Label(root, text="", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor2_position.grid(row=11, column=8, sticky="nsew")
        self.lbl_motor3_reading = tk.Label(root, text="Motor 3 Position [mm]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor3_reading.grid(row=13, column=7, sticky="nsew")
        self.lbl_motor3_position = tk.Label(root, text="", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor3_position.grid(row=13, column=8, sticky="nsew")
        self.lbl_motor4_reading = tk.Label(root, text="Motor 4 Position [mm]", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor4_reading.grid(row=15, column=7, sticky="nsew")
        self.lbl_motor4_position = tk.Label(root, text="", font=self.custom_font, padx=self.x_pad, pady=self.y_pad)
        self.lbl_motor4_position.grid(row=15, column=8, sticky="nsew")

        # create text entry
        self.txt_baud = tk.Entry(root)
        self.txt_baud.grid(row=0, column=4, sticky="nsew")
        self.txt_membrane_size_mm = tk.Entry(root)
        self.txt_membrane_size_mm.grid(row=5, column=4, columnspan=2, sticky="nsew")
        self.txt_strain_target = tk.Entry(root)
        self.txt_strain_target.grid(row=6, column=4, columnspan=2, sticky="nsew")
        self.txt_strain_rate = tk.Entry(root)
        self.txt_strain_rate.grid(row=7, column=4, columnspan=2, sticky="nsew")
        self.txt_number_of_cycles = tk.Entry(root)
        self.txt_number_of_cycles.grid(row=12, column=4, columnspan=2, sticky="nsew")

        # create buttons
        self.btn_connect = tk.Button(root, text="Connect", command=self.btn_connect_click)
        self.btn_connect.grid(row=0, column=5, sticky="nsew")
        self.btn_set_zero = tk.Button(root, text="Set Zero", command=lambda: self.button_click(1))
        self.btn_set_zero.grid(row=1, column=1, columnspan=2, sticky="nsew")
        self.btn_stop = tk.Button(root, text="STOP", command=lambda: self.button_click(1))
        self.btn_stop.grid(row=2, column=1, rowspan=2, columnspan=2, sticky="nsew")
        self.btn_set_parameters = tk.Button(root, text="Set Parameters", command=self.btn_set_parameters_click)
        self.btn_set_parameters.grid(row=8, column=1, columnspan=5, sticky="nsew")
        self.btn_move_to_zero = tk.Button(root, text="Move to 0", command=lambda: self.button_click(1))
        self.btn_move_to_zero.grid(row=9, column=1, rowspan=2, columnspan=3, sticky="nsew")
        self.btn_move_to_max = tk.Button(root, text="Move to Set Strain", command=lambda: self.button_click(1))
        self.btn_move_to_max.grid(row=9, column=4, rowspan=2, columnspan=2, sticky="nsew")
        self.btn_start_cyclic_test = tk.Button(root, text="Start Cyclic Test", command=lambda: self.button_click(1))
        self.btn_start_cyclic_test.grid(row=13, column=1, rowspan=2, columnspan=5, sticky="nsew")
        self.btn_save_to_file = tk.Button(root, text="Start Saving", bg="green", command=self.btn_save_to_file_click)
        self.btn_save_to_file.grid(row=15, column=3, columnspan=3, sticky="nsew")

        # create sliding scale, initialize to mid-point
        self.scale = tk.Scale(root, from_=0, to=10, orient=tk.HORIZONTAL, command=self.on_scale_change)
        self.scale.grid(row=2, column=3, rowspan=2, columnspan=3, sticky="nsew")
        self.scale.set((self.scale['from'] + self.scale['to']) / 2)

    # creates serial port reader
    def start_serial_reader(self):
        # create and start the serial reader thread
        self.serial_reader_thread = classes.SerialReaderThread(self.serial_port, self.baudrate, 
                                                self.packet_size, self.load_cell_queue, 
                                                self.encoder_queue, self.logger_queue)
        self.serial_reader_thread.name = "serial_reader_thread"
        self.serial_reader_thread.daemon = True
        self.serial_reader_thread.start()

    # creates graphs
    def start_graphs(self):
        # create and start two instances of real time graph thread
        self.load_cell_graph_thread = classes.RealTimeGraphThread(root, self.load_cell_queue, 2)
        self.load_cell_graph_thread.name = "load_cell_graph_thread"
        self.load_cell_graph_thread.daemon = True
        self.load_cell_graph_thread.start()

        self.encoder_graph_thread = classes.RealTimeGraphThread(root, self.encoder_queue, 4)
        self.encoder_graph_thread.name = "encoder_graph_thread"
        self.encoder_graph_thread.daemon = True
        self.encoder_graph_thread.start()

    # starts logging data
    def start_logger(self):
        # create logger thread
        self.data_logger_thread = classes.DataLoggerThread(self.file_name, self.logger_queue)
        self.data_logger_thread.name = "data_logger_thread"
        self.data_logger_thread.daemon = True
        self.data_logger_thread.start()
    
    # refreshes available COM ports
    def refresh_ports(self, event=None):
        # Get a list of available COM ports
        ports = [port.device for port in list_ports.comports()]
        # Update the dropdown menu with the new ports
        self.port_dropdown['menu'].delete(0, 'end')
        for port in ports:
            self.port_dropdown['menu'].add_command(label=port, command=tk._setit(self.selected_port, port))

    # event on port being selected
    def on_port_selected(self, *args):
        print("Selected port:", self.selected_port.get())

    # generic button click event
    def button_click(self, button_number):
        print(f"Button {button_number} clicked!")

    # when manual movement scale is changed
    def on_scale_change(self, value):
        print("Scale Value: " + value)

    # when connect button clicked
    def btn_connect_click(self):
        # first check if we're already connected
        if self.COM_connected:
            try:
                self.serial_reader_thread.close()
                self.btn_connect.config(text="Connect")
                self.COM_connected = False
            except Exception as e:
                print("Error closing serial port!")
                print(e)
        else:
            try:
                self.serial_port = self.selected_port.get()
                self.start_serial_reader()
                time.sleep(0.1)
                if self.serial_reader_thread.isOpen():
                    self.btn_connect.config(text="Disconnect")
                    self.COM_connected = True
            except Exception as e:
                print("Error opening serial port!")
                print(e)

    # called when save to file button clicked
    def btn_save_to_file_click(self):
        # first check if currently logging data
        if self.currently_logging:
            try:
                self.data_logger_thread.close()
                self.btn_save_to_file.config(text="Start Saving", bg="green")
                self.currently_logging = False
            except Exception as e:
                print("Error closing file!")
                print(e)
        else:
            try:
                self.file_name = filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("CSV files", "*.csv")])
                if self.file_name:
                    self.start_logger()
                    self.btn_save_to_file.config(text="Stop Saving", bg="red")
                    self.currently_logging = True
            except Exception as e:
                print("Error opening file!")
                print(e)

    # sending membrane parameters to the teensy
    def btn_set_parameters_click(self):
        # check if COM port connected
        if self.COM_connected:
            try:

                command = 3

                # parse text inputs
                target_steps = int(int(self.txt_membrane_size_mm.get()) * (int(self.txt_strain_target.get()) / 100) / self.lead_mm_per_rev *  self.motor_pulse_per_rev)
                strain_rate = int(self.txt_strain_rate.get())

                COM_message = self.create_message(command, target_steps, strain_rate)

                self.send_serial(COM_message)
            except Exception as e:
                print("Problem sending parameters")
                print(e)

    # function to create byte packet with 1 start, 1 command, 4 data, 1 esc
    def create_message(self, command, data1, data2) -> bytearray:
        try:
            start_byte = 255
            start_byte = struct.pack('B', start_byte)       # Convert int into byte
            
            command_byte = struct.pack('B', command)       # Convert int into byte

            if data1 >= 65535:
                data1 = 65535
            data1_bytes = struct.pack('>H', data1)        # Pack into byte
            data1_bytes = bytearray(data1_bytes)

            if data2 >= 65535:
                data2 = 65535
            data2_bytes = struct.pack('>H', data2)        # Pack into byte
            data2_bytes = bytearray(data2_bytes)

            # Handle if escape byte is necessary
            esc_byte = 0

            if data1_bytes[0] >= 255:
                data1_bytes[0] = 0
                print("data1[0] >= 255")
                esc_byte |= 1 << 3
            if data1_bytes[1] >= 255:
                data1_bytes[1] = 0
                print("data1[1] >= 255")
                esc_byte |= 1 << 2
            if data2_bytes[0] >= 255:
                data2_bytes[0] = 0
                print("data2[0] >= 255")
                esc_byte |= 1 << 1
            if data2_bytes[1] >= 255:
                data2_bytes[1] = 0
                print("data2[1] >= 255")
                esc_byte |= 1 << 0

            esc_byte = struct.pack('B', esc_byte)

            message = start_byte + command_byte + data1_bytes + data2_bytes + esc_byte

            return message
        except Exception as e:
            print("Problem creating message!")
            print(e)

    # wrapper function to transmit data over serial
    def send_serial(self, COM_message):
        if self.COM_connected:
            try:
                self.serial_reader_thread.write_to_serial(COM_message)
            except Exception as e:
                print("Error writing over serial")
                print(e)

    def close_program(self):
        self.master.destroy()

if __name__ == "__main__":
    # initialize tkinter root
    root = tk.Tk()
    root.title("Prototype Control Program")

    # create app
    app = CellStretcherApp(root)

    # handle closing the window
    root.protocol("WM_DELETE_WINDOW", app.close_program)
    root.mainloop()