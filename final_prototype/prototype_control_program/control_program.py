# Control program for MECH45X cell stretcher capstone
# Written by Graeme Dockrill - 2024

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

class CellStretcherApp:
    def __init__(self, master):
        self.master = master

        # Define the serial port, baudrate and packet size
        self.serial_port = "COM6"
        self.baudrate = 480000000
        self.packet_size = 22

        # Device parameters
        self.lead_mm_per_rev = 5
        self.motor_pulse_per_rev = 800
        self.encoder_pulse_per_rev = 1000
        self.max_steps = 65535              # placeholder value
        self.max_strain_target = 100        # placeholder value
        self.max_strain_rate = 100          # Placeholder value

        # Define default file name
        self.file_name = "testfile.csv"

        # Create shared queues for load cell, encoder and logger data
        self.load_cell_queue = queue.Queue()
        self.encoder_queue = queue.Queue()
        self.logger_queue = queue.Queue()

        # Define states
        self.currently_logging = False
        self.COM_connected = False
        self.parameters_set = False

        # Define widget cosmetics
        self.x_pad = 10
        self.y_pad = 5
        self.custom_font = ("Helvetica", 12, "bold")

        # Start up UI modules
        self.create_ui()
        self.start_graphs()
        
    # Called on start up to create UI
    def create_ui(self):
        # Create COM Port drop down
        self.selected_port = tk.StringVar()
        self.port_dropdown = tk.OptionMenu(root, self.selected_port, "")
        self.port_dropdown.grid(row=0, column=0, sticky="nsew", columnspan=2)
        self.refresh_ports()
        self.selected_port.trace_add('write', self.on_port_selected)

        # Create labels
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

        # Create text entry
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

        # Create buttons
        self.btn_connect = tk.Button(root, text="Connect", command=self.btn_connect_click)
        self.btn_connect.grid(row=0, column=5, sticky="nsew")
        self.btn_home_axes = tk.Button(root, text="Set Zero", command=self.btn_home_axes_click)
        self.btn_home_axes.grid(row=1, column=1, columnspan=2, sticky="nsew")
        self.btn_stop = tk.Button(root, text="STOP", command=self.btn_stop_click)
        self.btn_stop.grid(row=2, column=1, rowspan=2, columnspan=2, sticky="nsew")
        self.btn_set_parameters = tk.Button(root, text="Set Parameters", command=self.btn_set_parameters_click)
        self.btn_set_parameters.grid(row=8, column=1, columnspan=5, sticky="nsew")
        self.btn_move_to_zero = tk.Button(root, text="Move to 0", command=self.btn_move_to_zero_click)
        self.btn_move_to_zero.grid(row=9, column=1, rowspan=2, columnspan=3, sticky="nsew")
        self.btn_move_to_max = tk.Button(root, text="Move to Set Strain", command=self.btn_move_to_max_click)
        self.btn_move_to_max.grid(row=9, column=4, rowspan=2, columnspan=2, sticky="nsew")
        self.btn_start_cyclic_test = tk.Button(root, text="Start Cyclic Test", command=self.btn_start_cyclic_test_click)
        self.btn_start_cyclic_test.grid(row=13, column=1, rowspan=2, columnspan=5, sticky="nsew")
        self.btn_save_to_file = tk.Button(root, text="Start Saving", bg="green", command=self.btn_save_to_file_click)
        self.btn_save_to_file.grid(row=15, column=3, columnspan=3, sticky="nsew")

        # Create sliding scale, initialize to mid-point
        self.scale_manual_movement = tk.Scale(root, from_=0, to=10, orient=tk.HORIZONTAL, command=self.manual_move_axes)
        self.scale_manual_movement.grid(row=2, column=3, rowspan=2, columnspan=3, sticky="nsew")
        self.scale_manual_movement.set((self.scale_manual_movement['from'] + self.scale_manual_movement['to']) / 2)

    # Creates serial port reader
    def start_serial_reader(self):
        # Create and start the serial reader thread
        self.serial_port_thread = classes.SerialPortThread(self.serial_port, self.baudrate, 
                                                self.packet_size, self.load_cell_queue, 
                                                self.encoder_queue, self.logger_queue)
        self.serial_port_thread.name = "serial_reader_thread"
        self.serial_port_thread.daemon = True
        self.serial_port_thread.start()

    # Creates graphs
    def start_graphs(self):
        # Create and start two instances of real time graph thread
        self.load_cell_graph_thread = classes.RealTimeGraphThread(root, self.load_cell_queue, 2)
        self.load_cell_graph_thread.name = "load_cell_graph_thread"
        self.load_cell_graph_thread.daemon = True
        self.load_cell_graph_thread.start()

        self.encoder_graph_thread = classes.RealTimeGraphThread(root, self.encoder_queue, 4)
        self.encoder_graph_thread.name = "encoder_graph_thread"
        self.encoder_graph_thread.daemon = True
        self.encoder_graph_thread.start()

    # Starts logging data
    def start_logger(self):
        # Create logger thread
        self.data_logger_thread = classes.DataLoggerThread(self.file_name, self.logger_queue)
        self.data_logger_thread.name = "data_logger_thread"
        self.data_logger_thread.daemon = True
        self.data_logger_thread.start()
    
    # Refreshes available COM ports
    def refresh_ports(self, event=None):
        # Get a list of available COM ports
        ports = [port.device for port in list_ports.comports()]
        # Update the dropdown menu with the new ports
        self.port_dropdown['menu'].delete(0, 'end')
        for port in ports:
            self.port_dropdown['menu'].add_command(label=port, command=tk._setit(self.selected_port, port))

    # Event on port being selected
    def on_port_selected(self, *args):
        print("Selected port:", self.selected_port.get())

    # Generic button click event
    def button_click(self, button_number):
        print(f"Button {button_number} clicked!")

    # When manual movement scale is changed
    def manual_move_axes(self, value):
        try:
            midpoint = int((self.scale_manual_movement['from'] + self.scale_manual_movement['to']) / 2)
            value = int(value)

            if value == midpoint:
                command = 7     # Stop axes
                manual_speed = 0
            elif value > midpoint:
                command = 1     # Move in positive direction
                manual_speed = 0
            elif value < midpoint:
                command = 2     # Move in negative direction
                manual_speed = 0

            if self.COM_connected:
                COM_message = self.create_message(command, manual_speed, 0)
                self.send_serial(COM_message)

        except Exception as e:
            print("Problem manually moving axes!")
            print(e)

    # When connect button clicked
    def btn_connect_click(self):
        # First check if we're already connected
        if self.COM_connected:
            try:
                self.serial_port_thread.close()
                self.btn_connect.config(text="Connect")
                self.COM_connected = False
                self.parameters_set = False
            except Exception as e:
                print("Error closing serial port!")
                print(e)
        else:
            try:
                self.serial_port = self.selected_port.get()
                self.start_serial_reader()
                time.sleep(0.1)
                if self.serial_port_thread.isOpen():
                    self.btn_connect.config(text="Disconnect")
                    self.COM_connected = True
            except Exception as e:
                print("Error opening serial port!")
                print(e)

    # Called when save to file button clicked
    def btn_save_to_file_click(self):
        # First check if currently logging data
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

    # Sending membrane parameters to the teensy
    def btn_set_parameters_click(self):
        try:
            command = 3

            # Parse text inputs
            target_steps = int(int(self.txt_membrane_size_mm.get()) * (int(self.txt_strain_target.get()) / 100) / self.lead_mm_per_rev *  self.motor_pulse_per_rev)
            strain_rate = int(self.txt_strain_rate.get())

            COM_message = self.create_message(command, target_steps, strain_rate)
            self.send_serial(COM_message)
            self.parameters_set = True
        except Exception as e:
            print("Problem sending parameters")
            print(e)

    # Homing axes
    def btn_home_axes_click(self):
        try:
            command = 0
            COM_message = self.create_message(command, 0, 0)
            self.send_serial(COM_message)
        except Exception as e:
            print("Problem homing axes!")
            print(e)

    # Stop device
    def btn_stop_click(self):
        try:
            command = 7
            COM_message = self.create_message(command, 0, 0)
            self.send_serial(COM_message)
        except Exception as e:
            print("Problem stopping axes!")
            print(e)

    # Move axes to zero stretch
    def btn_move_to_zero_click(self):
        try:
            command = 5
            COM_message = self.create_message(command, 0, 0)
            self.send_serial(COM_message)
        except Exception as e:
            print("Problem moving axes to zero!")
            print(e)

    # Move axes to max stretch
    def btn_move_to_max_click(self):
        try:
            command = 4
            COM_message = self.create_message(command, 0, 0)
            self.send_serial(COM_message)
        except Exception as e:
            print("Problem moving axes to max stretch!")
            print(e)

    # Start cyclic stretching
    def btn_start_cyclic_test_click(self):
        try:
            command = 6
            stretch_cycles = str(self.txt_number_of_cycles.get())       # Parse text inputs
            COM_message = self.create_message(command, stretch_cycles, 0)
            self.send_serial(COM_message)
        except Exception as e:
            print("Problem starting cyclic test!")
            print(e)

    # Function to create byte packet with 1 start, 1 command, 4 data, 1 esc
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

            print("Parsed message as: 255, " + str(command) + ", " + str(data1) + ", " + str(data2) + ", " + str(esc_byte))

            esc_byte = struct.pack('B', esc_byte)

            message = start_byte + command_byte + data1_bytes + data2_bytes + esc_byte

            return message
        except Exception as e:
            print("Problem creating message!")
            print(e)

    # Wrapper function to transmit data over serial
    def send_serial(self, COM_message):
        if self.COM_connected:
            try:
                self.serial_port_thread.write_to_serial(COM_message)
            except Exception as e:
                print("Error writing over serial")
                print(e)
        else:
            print("COM port closed!")

    # Function run when window closed
    def close_program(self):
        self.master.destroy()

if __name__ == "__main__":
    # Initialize tkinter root
    root = tk.Tk()
    root.title("Prototype Control Program")

    # Create app
    app = CellStretcherApp(root)

    # Handle closing the window
    root.protocol("WM_DELETE_WINDOW", app.close_program)
    root.mainloop()