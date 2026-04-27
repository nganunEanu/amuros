import cv2
import math
from ultralytics import YOLO
class yolo:
    def init():
        model = YOLO('/home/aditya/best.pt')
        return model

    def read_yolo(model,saturated_frame):
        result0 = model(saturated_frame)
        return result0

    def result(result0):
        box_info = []
        x1, y1, x2, y2, conf0, cls0 = 0, 0, 0, 0, 0, 0
        for r in result0:
            boxes = r.boxes
            if len(boxes) > 0:  # Check if there are any boxes detected
                for box in boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    conf0 = math.ceil(box.conf[0] * 100) / 100
                    cls0 = int(box.cls[0])
                    box_info.append((x1, y1, x2, y2, conf0, cls0)) 

        return box_info
    def jalan ():
        font = cv2.FONT_HERSHEY_SIMPLEX
        cap = cv2.VideoCapture(0)
        while True:
            ret, frame0 = cap.read()
            if not ret:
                raise Exception("Failed to capture frame from camera.")
            model=yolo.init()
            result=yolo.read_yolo(model,frame0)
     
            boxes_info=yolo.result(result)
            for box in boxes_info:
                x1, y1, x2, y2, conf0, cls0 = box
                cv2.rectangle(frame0, (x1, y1), (x2, y2), (0, 0, 255), 3)
                cv2.putText(frame0, f'Kelas: {[cls0]}, Kepercayaan: {conf0}', (x1, y1), font, 1, (0, 255, 0))
            cv2.imshow('me1', frame0)
            if cv2.waitKey(1) & 0xFF == 27:
                break
            
        
if __name__ == "__main__":
    yolo.jalan()