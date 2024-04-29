import tkinter as tk
from tkinter.ttk import *
from tkinter import *
import numpy as np
import serial
import threading

import tkinter as tk

# serial_port = serial.Serial('/dev/ttyACM0', 115200)
# serial_data = ""
capture_command = "capture"

class CaptureGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("CSSE4011 Project GUI")
        
        self.create_widgets()

    def create_widgets(self):
        # Create a canvas for the grid
        cell_size=100
        self.canvas = tk.Canvas(self.master, width=1920, height=1080, bg="white")
        self.canvas.pack(side=tk.RIGHT, padx=10, pady=10)

        self.canvas.create_line(400, 150, 1100, 150)
        self.canvas.create_line(400, 600, 1100, 600)
        self.canvas.create_line(400, 150, 400, 600)
        self.canvas.create_line(1100, 150, 1100, 600)

        self.capture_button = tk.Button(self.master, text="Capture Image", command=self.capture_image)
        self.capture_button.pack(side=tk.BOTTOM, pady=10)
        self.canvas.create_window(750,650,window=self.capture_button)
    
    def capture_image(self):
        serial_port.write(capture_command.encode() + b'\n')

def data_thread():
    while True:
        serial_data = serial_port.readline().decode()
        print(serial_data)
        serial_data = ""

def main():
    root = tk.Tk()
    global app 
    app = CaptureGUI(root)
    # communication_process = threading.Thread(target=data_thread, args=())
    # communication_process.start()
    root.mainloop()

if __name__ == "__main__":
    main()