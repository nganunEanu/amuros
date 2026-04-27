import pyvisa as visa
from ultralytics import YOLO
import cv2
import math
import numpy as np
from classess import classNames
import imutils
import time
import socket
import serial

# IP address and port to bind to
local_ip = '0.0.0.0'
local_port = 1234
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((local_ip, local_port))

z = classNames
rm = visa.ResourceManager()
print(rm.list_resources())

kiri_lambat = 230
kiri_berhenti = 260
kanan_lambat = 315
kanan_berhenti = 340

berhenti_maju_x1 = 250
berhenti_maju_x2 = 350
berhenti_maju_y1 = 170
berhenti_maju_y2 = 310
berhenti_putar_x1 = 310
berhenti_putar_x2 = 290

center_frame = (290, 274)
top_center = (290, 0)

stage = 0
perintah = 0
prevT = 0
cam = 0

# Initialize serial port
serialA = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)

start_time = int(time.time())
delay_duration = 20

model = YOLO('/home/jetson/Downloads/besttrain3.pt')
font = cv2.FONT_HERSHEY_SIMPLEX
cap = cv2.VideoCapture(0)

def handle_timed_loop(record_params, start_time, delay_duration):
    current_timedelay = int(time.time())
    print("Current time:", current_timedelay)
    record(*record_params)
    elapsed_time = current_timedelay - start_time
    print("Elapsed time:", elapsed_time)

    if elapsed_time >= delay_duration:
        print(f"{delay_duration} seconds have passed.")
        return True, int(time.time())  # Return updated start_time
    return False, start_time

def check_first_loop_condition():
    return kiri_lambat < center_x_box0 < kiri_berhenti or kanan_berhenti < center_x_box0 < kanan_lambat or center_x_box0 < kiri_lambat or kanan_lambat < center_x_box0

def record(mode,x,y,theta,speed,interval): #kirim data ke micro
    global prevT
    serialA = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)
    currT = round(time.time(), 2)
    while (currT - prevT > interval):
        
        prevT = currT
        dataOut = (str(mode) + ',' + str(x) +  ',' + str(y) + ',' + str(theta) + ',' + str(speed) + '}')
        if dataOut != None:
                #print('\n Status: ', serialA)
            print(dataOut)
            serialA.write(dataOut.encode('utf-8'))

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    _ip = s.getsockname()[0]
    s.close()
    return _ip

def increase_saturation(image, saturation_scale=10):
    hsv_img = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    hsv_img = np.float32(hsv_img)
    h, s, v = cv2.split(hsv_img)
    s = np.clip(s * saturation_scale, 0, 255)
    hsv_img = cv2.merge([h, s, v])
    hsv_img = np.uint8(hsv_img)
    return cv2.cvtColor(hsv_img, cv2.COLOR_HSV2BGR)

ip, port = get_ip(), 9999
serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSock.settimeout(0.001)
serverSock.bind((ip, port))
print("IP:", ip, port)
sock.settimeout(0.01)

try:
    while True:
        
        try:
            data, addr = sock.recvfrom(1024)
            perintah = int(data)
            if perintah_seb != perintah:
                start_time=int(time.time())

            perintah_seb=perintah
            last_udp_execution_time = time.time()
            print(f"Received message from {addr}: {perintah}")
            print("Received command:", perintah)
        except socket.timeout:
            print("Socket timed out. No data received.")
        except Exception as e:
            print("Error:", e)
            
        ret, frame0 = cap.read()
        if not ret:
            break

        saturated_frame = increase_saturation(frame0)
        saturated_frame = imutils.resize(saturated_frame, width=568, height=568)

        cls0 = 5
        result0 = model(saturated_frame, stream=True)
        overlay1 = saturated_frame.copy()
       
        overlay2 = saturated_frame.copy()
        cv2.rectangle(overlay2, (kiri_lambat, 0), (kanan_lambat, 1800), (255, 0, 255,), -1)
        cv2.rectangle(overlay2, (kiri_berhenti, 0), (kanan_berhenti, 1800), (255, 125, 255,), -1)

        for r in result0:
            boxes = r.boxes
            for box in boxes:
                x1_1, y1_1, x1_2, y1_2 = map(int, box.xyxy[0])
                cv2.rectangle(saturated_frame, (x1_1, y1_1), (x1_2, y1_2), (0, 0, 255), 3)
                conf0 = math.ceil(box.conf[0] * 100) / 100
                cls0 = int(box.cls[0])
                center_x_box0 = (x1_1 + x1_2) / 2
                center_y_box0 = (y1_1 + y1_2) / 2
                center_objek0 = (int(center_x_box0), int(center_y_box0))

                vector0_1 = np.array(center_objek0) - np.array(center_frame)
                vector0_2 = np.array(top_center) - np.array(center_frame)
                angle_radians0 = np.arctan2(vector0_1[1], vector0_1[0]) - np.arctan2(vector0_2[1], vector0_2[0])
                angle_degrees0 = (math.degrees(angle_radians0) + 360) % 360

                cv2.line(saturated_frame, center_frame, center_objek0, (0, 255, 0), 2)
                cv2.line(saturated_frame, center_frame, top_center, (0, 255, 0), 2)
                angle_text = f'Sudut: {angle_degrees0:.2f} derajat'
                text_size = cv2.getTextSize(angle_text, font, 1, 2)[0]
                text_x = (saturated_frame.shape[1] - text_size[0]) // 2
                text_y = (saturated_frame.shape[0] + text_size[1]) // 2
                cv2.putText(saturated_frame, angle_text, (text_x, text_y), font, 1, (0, 255, 0), 2)
                cv2.putText(saturated_frame, f'Kelas: {z[cls0]}, Kepercayaan: {conf0}', (x1_1, y1_1), font, 1, (0, 255, 0))

                if cls0 == 0:
                    ball = True
                if perintah == 1:
                    
                    if stage == 0:
                        current_timedelay = int(time.time() )
                        print("Current time:", current_timedelay)
                        record(1, 0, 0, 0, 0, 0.1)
                        elapsed_time = current_timedelay - start_time
                        print("Elapsed time:", elapsed_time)

                        if elapsed_time >= delay_duration:
                            stage = 1
                            elapsed_time = 0
                            print("2 seconds have passed.")
                            start_time = current_timedelay


                    if stage == 1:
                        record(0, 0, 0, 3, 1, 0.1)
                        if cls0 == 2:
                            
                            cv2.rectangle(saturated_frame, (x1_1, y1_1), (x1_2, y1_2), (255, 0, 0), 6)
                            cv2.putText(saturated_frame, angle_text, (text_x, text_y), font, 1, (255, 0, 0), 2)

                            if kiri_lambat < center_x_box0 < kiri_berhenti:
                                record(0, 0, 0, 1, 1, 0.1)
                            elif kanan_berhenti < center_x_box0 < kanan_lambat:
                                record(0, 0, 0, -1, 1, 0.1)
                            elif center_x_box0 < kiri_lambat:
                                record(0, 0, 0, 5, 1, 0.1)
                            elif kanan_lambat < center_x_box0:
                                record(0, 0, 0, 5, 1, 0.1)
                            else:
                                if not ball == True:
                                    record(2, 0, 0, 0, 0, 0.1)
                                    stage = 3
                                    while True:
                                        print("In second loop")
                                        if handle_timed_loop():
                                            second_loop_done = True
                                            break
                                    break
                     # Recheck the first condition before going to the next loop
                    if second_loop_done and not check_first_loop_condition():
                        print("Proceeding to third loop")
                        condition_met, start_time = handle_timed_loop(start_time, delay_duration, (3, 0, 0, 0, 0, 0.1))
                        if condition_met:
                            third_loop_done = True


                    if second_loop_done and third_loop_done and not check_first_loop_condition():
                        print("Proceeding to fourth loop")
                        condition_met, start_time = handle_timed_loop(start_time, delay_duration, (3, 0, 0, 0, 0, 0.1))
                        if condition_met:
                            third_loop_done = True


                    if second_loop_done and third_loop_done and fourth_loop_done and not check_first_loop_condition():
                        print("Proceeding to fifth loop")
                        condition_met, start_time = handle_timed_loop(start_time, delay_duration, (3, 0, 0, 0, 0, 0.1))
                        if condition_met:
                            third_loop_done = True

                else:
                    record(0, 0, 0, 0, 0, 0.1)
                    print("a")
                    stage = 0
                    cap.release()
                    cap = cv2.VideoCapture(1)
                    stage = 0
                    cam = 1
                    elapsed_time=0

        print(stage)
        if perintah == 1:
                            
            if stage == 0:
                current_timedelay = int(time.time())
                print("Current time:", current_timedelay)
                record(1, 0, 0, 0, 0, 0.1)
                elapsed_time = current_timedelay - start_time
                print("Elapsed time:", elapsed_time)

                if elapsed_time >= delay_duration:
                    stage = 1
                    print("2 seconds have passed.")
                    start_time = current_timedelay
                                    
        else:
            record(9, 0, 0, 0, 0, 0.1)
            print("a")
            stage = 0
            cap.release()
            cap = cv2.VideoCapture(1)
            stage = 0
            cam = 1
            
            
     
        cv2.addWeighted(overlay2, 0.5, saturated_frame, 0.5, 0, saturated_frame)
        cv2.imshow('me1', saturated_frame)

        if cv2.waitKey(1) & 0xFF == 27:
            break
finally:
    sock.close()
    cap.release()
    cv2.destroyAllWindows()
    serialA.close()
