import serial
import pyautogui
from PIL import Image
import numpy as np
import time

# Set up serial communication with Arduino Uno
ser = serial.Serial('COM3', 115200)

# Define LED strip parameters
NUM_LEDS = 32

# Define screen capture parameters
SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

while True:
    # Capture screen and resize to LED strip resolution
    screen = pyautogui.screenshot(region=(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT))
    screen = screen.resize((NUM_LEDS, 1), resample=Image.BILINEAR)
    #print(screen)

    # Convert image to RGB values
    screen_array = np.array(screen)
    screen_array = screen_array.reshape((NUM_LEDS, 3))
    #print(screen_array)
    screen_array = screen_array.tolist()
    #print(screen_array) # [[45, 46, 46], [49, 48, 48], [49, 48, 48]]

    # Send RGB values to Arduino Uno
    pixel_str = ' '.join([f'{p[0]} {p[1]} {p[2]}' for p in screen_array])
    #print(pixel_str) # 45 46 46 49 48 48 49 48 48
    matrix_str = f'[{pixel_str}]'
    #print(matrix_str) # [45 46 46 49 48 48 49 48 48]

    # Send the matrix string to the Arduino
    ser.write(matrix_str.encode())
    print(matrix_str)
    time.sleep(0.0001)

