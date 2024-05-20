import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk
import numpy as np
import serial
import time
import threading
import random

from paho.mqtt import client as mqtt_client

# Set up the Arduino connection
arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)

# Define the width and height of the image
WIDTH = 160
HEIGHT = 120


broker = 'broker.emqx.io'
port = 1883
topic = "csse4011-ir"
# Generate a Client ID with the publish prefix.
client_id = f'publish-{random.randint(0, 1000)}'
# username = 'emqx'
# password = 'public'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def publish(client):
    msg_count = 1
    while True:
        time.sleep(10)
        msg = "Transmission Complete"
        result = client.publish(topic, msg)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
        msg_count += 1
        if msg_count > 5:
            break


def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)
    client.loop_stop()

# Function to convert RGB to 8-bit RRRBBGGG
def convert_rgb_to_8bit(r, g, b):
    r_3bit = (r * 7) // 255   # Map 8-bit red to 3-bit
    g_3bit = (g * 7) // 255   # Map 8-bit green to 3-bit
    b_2bit = (b * 3) // 255   # Map 8-bit blue to 2-bit
    return (r_3bit << 5) | (b_2bit << 3) | g_3bit

# Function to process the image and send data to Arduino
def process_image(image_path):
    global processing
    processing = True
    
    # Load the image
    img = Image.open(image_path)

    # Resize the image to 160x120
    img = img.resize((WIDTH, HEIGHT))

    # Convert the image to RGB
    img = img.convert("RGB")

    # Get the RGB values as a NumPy array
    rgb_array = np.array(img)

    for _ in range(8):
        if not processing:
            print("Processing stopped.")
            return
        arduino.write(bytes("12345678", 'utf-8'))
        data = arduino.readline().decode('utf-8').strip()
        print(data)

    for y in range(HEIGHT):
        if not processing:
            print("Processing stopped.")
            return
        for x in range(0, WIDTH, 4):
            if not processing:
                print("Processing stopped.")
                return
            r1, g1, b1 = rgb_array[y, x]
            r2, g2, b2 = rgb_array[y, x + 1]
            r3, g3, b3 = rgb_array[y, x + 2]
            r4, g4, b4 = rgb_array[y, x + 3]
            packed_value_1 = (convert_rgb_to_8bit(r1, g1, b1))
            packed_value_2 = (convert_rgb_to_8bit(r2, g2, b2))
            packed_value_3 = (convert_rgb_to_8bit(r3, g3, b3))
            packed_value_4 = (convert_rgb_to_8bit(r4, g4, b4))
            data_to_send = f"{packed_value_1:02X}{packed_value_2:02X}{packed_value_3:02X}{packed_value_4:02X}"
            arduino.write(bytes(data_to_send, 'utf-8'))
            data = arduino.readline().decode('utf-8').strip()
            print(data)
        
        arduino.write(bytes("12345678", 'utf-8'))
        data = arduino.readline().decode('utf-8').strip()
        print(data)
        arduino.write(bytes("12345678", 'utf-8'))
        data = arduino.readline().decode('utf-8').strip()
        print(data)

    print("Transmission Complete")
    run()

# Function to stop processing
def stop_processing():
    global processing
    processing = False

# GUI setup
def select_image():
    file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.jpg *.jpeg *.png")])
    if file_path:
        img = Image.open(file_path)
        img.thumbnail((320, 240))
        img = ImageTk.PhotoImage(img)
        image_label.config(image=img)
        image_label.image = img
        image_label.file_path = file_path

def start_processing():
    if hasattr(image_label, 'file_path'):
        threading.Thread(target=process_image, args=(image_label.file_path,)).start()
    else:
        messagebox.showwarning("No Image Selected", "Please select an image file first.")




root = tk.Tk()
root.title("Image Processing")

frame = tk.Frame(root)
frame.pack(padx=10, pady=10)

select_button = tk.Button(frame, text="Select Image", command=select_image)
select_button.pack(side=tk.LEFT, padx=5)

start_button = tk.Button(frame, text="Start", command=start_processing)
start_button.pack(side=tk.LEFT, padx=5)

stop_button = tk.Button(frame, text="Stop", command=stop_processing)
stop_button.pack(side=tk.LEFT, padx=5)

image_label = tk.Label(root)
image_label.pack(padx=10, pady=10)

root.mainloop()
