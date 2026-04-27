
from communication import communication
from yolo import yolo
from opencv2 import opencv2
from main import main

variables = opencv2.var_init
#serial=communication.serial.init()
#sock = communication.udp.init()
cap, font = opencv2.init()
model = yolo.init()
#try:
while True:
    #perintah=communication.udp.get_udp()
    saturated_frame=opencv2.read_cam(cap)
    #main.program()
    result=yolo.read_yolo(model, saturated_frame)


    boxes_info=yolo.result(result)
    for box in boxes_info:
        x1, y1, x2, y2, conf0, cls0 = box
        angle_degrees0=opencv2.calculate(x1,y1,x2,y2)
        opencv2.draw(saturated_frame, font, cls0, conf0, x1, y1, x2, y2)
    opencv2.show(saturated_frame)
    #main.program(communication,perintah,angle_degrees0,cls0,variables)
    