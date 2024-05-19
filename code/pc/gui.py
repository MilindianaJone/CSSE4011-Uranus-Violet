from PIL import Image
import numpy as np
import serial
import time

arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1) 

# Define the width and height of the image
WIDTH = 160
HEIGHT = 120

# Function to convert RGB to 8-bit RRRBBGGG
def convert_rgb_to_8bit(r, g, b):
    r_3bit = (r * 7) // 255   # Map 8-bit red to 3-bit
    g_3bit = (g * 7) // 255   # Map 8-bit green to 3-bit
    b_2bit = (b * 3) // 255   # Map 8-bit blue to 2-bit
    return (r_3bit << 5) | (b_2bit << 3) | g_3bit

# Load the image
img = Image.open("team.jpg")

# Resize the image to 320x240
img = img.resize((WIDTH, HEIGHT))

# Convert the image to RGB
img = img.convert("RGB")

# Get the RGB values as a NumPy array
rgb_array = np.array(img)

for _ in range(8):
    arduino.write(bytes("12345678", 'utf-8'))
    data = arduino.readline().decode('utf-8').strip()
    print(data)


for y in range(HEIGHT):
    for x in range(0, WIDTH, 4):
        r1, g1, b1 = rgb_array[y, x]
        r2, g2, b2 = rgb_array[y, x + 1]
        r3, g3, b3 = rgb_array[y, x + 2]
        r4, g4, b4 = rgb_array[y, x + 3]
        packed_value_1 = (convert_rgb_to_8bit(r1, g1, b1))
        packed_value_2 = (convert_rgb_to_8bit(r2, g2, b2))
        packed_value_3 = (convert_rgb_to_8bit(r3, g3, b3))
        packed_value_4 = (convert_rgb_to_8bit(r4, g4, b4))
        data_to_send = f"{packed_value_1:02X}{packed_value_2:02X}{packed_value_3:02X}{packed_value_4:02X}"
        #data_to_send = f"{packed_value_1[2:]:0<2}{packed_value_2[2:]:0<2}{packed_value_3[2:]:0<2}{packed_value_4[2:]:0<2}"
        arduino.write(bytes(data_to_send, 'utf-8'))
        #time.sleep(0.01) 
        data = arduino.readline().decode('utf-8').strip()
        print(data)
        #time.sleep(0.001)
        #print(data_to_send)
    
    arduino.write(bytes("12345678", 'utf-8'))
    data = arduino.readline().decode('utf-8').strip()
    print(data)
    arduino.write(bytes("12345678", 'utf-8'))
    data = arduino.readline().decode('utf-8').strip()
    print(data)


print("Transmission Complete")
