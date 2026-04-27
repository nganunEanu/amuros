import pyvisa as visa
import time
from classess import classNames
from ultralytics import YOLO
import threading
import subprocess 

loop_jalan = False
program_running = True
total_detected_objects = 0
speed = 0

# Function to read and process serial data
def read_serial():
    global loop_jalan, program_running, model1, model2  # Declare models as global variables
    while program_running:  # Continue reading serial data while the program is running
        read = serialPort.read_bytes(serialPort.bytes_in_buffer)
        print(read)

        # Check if the "stop" button is pressed
        if read == b'e\x00\x04\x01\xff\xff\xff':
            if loop_jalan:  # If the loop is running
                loop_jalan = False
                if model1 is not None:
                    model1.close()
                # Stop the program
                program_running = False

        time.sleep(1)  # Adjust the sleep duration as needed
def loop_model1():
    for i, result1 in enumerate(results1):
                boxes1 = result1.boxes

                # Initialize a dictionary to count objects of each class
                class_objects = {name: 0 for name in x}

                # Process and count objects for each detected object
                for box in boxes1:
                    dataout = int(box.cls[0])
                    class_name = x[dataout]
                    class_objects[class_name] += 1  # Increment the class counter

                # Filter out classes with a count of 0
                detected_classes = [f"{count} {class_name}" for class_name, count in class_objects.items() if count > 0]

                total_detected_objects = sum(class_objects.values())

                # Construct the class info string
                class_info = " ; ".join(detected_classes)

                # Send the counts for detected classes to the serial display
                
                serialPort.write_raw(f'n0.val={total_detected_objects}'.encode('utf-8'))
                serialPort.write_raw(b'\xff\xff\xff')

                serialPort.write_raw(f't1.txt="{class_info}"'.encode('utf-8'))
                serialPort.write_raw(b'\xff\xff\xff')
                break
            # Initialize serial communication
ports = visa.ResourceManager()
print(ports.list_resources())
serialPort = ports.open_resource('ASRL/dev/ttyUSB0::INSTR')
serialPort.baud_rate = 9600

# Declare class array
x = classNames

# Initialize YOLO models
model1 = None
model2 = None

try:
    # Start a thread for reading and processing serial data
    serial_thread = threading.Thread(target=read_serial)
    serial_thread.start()

    while program_running:  # Infinite loop while the program is running

        # Read data from the serial port
        read = serialPort.read_bytes(serialPort.bytes_in_buffer)
        print(read)

        # Check if the "start" button is pressed
        if read == b'e\x00\x05\x01\xff\xff\xff':
            if not loop_jalan:  # If the loop is not already running
                loop_jalan = True
                # Instantiate the YOLO models
                model1 = YOLO("yolov8s.pt")
                model2 = YOLO("yolov8n.pt")
                serialPort.write_raw(f't0.txt="PROGRAM BERJALAN..."'.encode('utf-8'))
                serialPort.write_raw(b'\xff\xff\xff')

        # Perform actions while the loop is running
        if loop_jalan and model1 is not None and model2 is not None:  # Check if both models are initialized
            # Stream YOLO results

            # Measure the time before starting inference
            

            results1 = model1(source="0", conf=0.25, save=True, stream=True, show=True)
            results2 = model2(source="2", conf=0.25, save=True, stream=True, show=True)
            
            # Measure the time after finishing inference
            

            # Calculate the inference speed in frames per second (FPS)
           

            while True:
                 start_time = time.time()
                 loop_model1()
                 end_time = time.time()
                 inference_speed = (end_time - start_time)*1000
                 rounded = round(inference_speed,2)
                 serialPort.write_raw(f't3.txt="{rounded}"'.encode('utf-8'))
                 serialPort.write_raw(b'\xff\xff\xff')

            # Process and print results for model1
            

            # Process and print results for model2
            

except KeyboardInterrupt:
    print("Ctrl+C detected. Exiting...")

finally:
    serialPort.write_raw(f't0.txt="PROGRAM STOPPED"'.encode('utf-8'))
    serialPort.write_raw(b'\xff\xff\xff')

    serial_thread.join()  # Wait for the serial thread to finish
    serialPort.close()

# Restart the entire program if it was stopped by the "stop" button
if not program_running:
    subprocess.Popen(["python", "last.py"])
        
