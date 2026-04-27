import cv2
import math
import numpy as np
import imutils
import cv2
import math
import numpy as np
import imutils

class opencv2:
   
    def var_init():
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
        classNames = ("person", "bicycle", "car", "motorbike", "aeroplane", 
                      "bus", "train", "truck", "boat", "traffic light", "fire hydrant", 
                      "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse",
                      "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", 
                      "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", 
                      "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                      "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl",
                      "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog",
                      "pizza", "donut", "cake", "chain", "sofa", "pottedplant", "bed",
                      "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote",
                      "keyboard", "cell phone", "microwave", "oven", "toaster", "sink",
                      "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", 
                      "hair drier", "toothbrush")

        center_frame = (290, 274)
        top_center = (290, 0)
        
        return {
            "kiri_lambat": kiri_lambat,
            "kiri_berhenti": kiri_berhenti,
            "kanan_lambat": kanan_lambat,
            "kanan_berhenti": kanan_berhenti,
            "berhenti_maju_x1": berhenti_maju_x1,
            "berhenti_maju_x2": berhenti_maju_x2,
            "berhenti_maju_y1": berhenti_maju_y1,
            "berhenti_maju_y2": berhenti_maju_y2,
            "berhenti_putar_x1": berhenti_putar_x1,
            "berhenti_putar_x2": berhenti_putar_x2,
            "center_frame": center_frame,
            "top_center": top_center,
            "z": classNames
        }

   
    def init():
        font = cv2.FONT_HERSHEY_SIMPLEX
        cap = cv2.VideoCapture(0)
        return cap, font
    
    def read_cam(cap):
        ret, frame0 = cap.read()
        if not ret:
            raise Exception("Failed to capture frame from camera.")
        saturated_frame = opencv2.increase_saturation(frame0)
        saturated_frame = imutils.resize(saturated_frame, width=568, height=568)
        return saturated_frame


   
    def increase_saturation(image, saturation_scale=10):
        hsv_img = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        hsv_img = np.float32(hsv_img)
        h, s, v = cv2.split(hsv_img)
        s = np.clip(s * saturation_scale, 0, 255)
        hsv_img = cv2.merge([h, s, v])
        hsv_img = np.uint8(hsv_img)
        return cv2.cvtColor(hsv_img, cv2.COLOR_HSV2BGR)

   
    def overlay(saturated_frame):
        # Get the variables from the other function
        variables = opencv2.var_init()

        # Overlay 1 with rectangles
        overlay1 = saturated_frame.copy()
        cv2.rectangle(overlay1, (variables["berhenti_maju_x1"], variables["berhenti_maju_y1"]),
                    (variables["berhenti_maju_x2"], variables["berhenti_maju_y2"]), (255, 0, 0), -1)
        cv2.rectangle(overlay1, (variables["berhenti_putar_x1"], 0),
                    (variables["berhenti_putar_x2"], 375), (255, 125, 0), -1)

        # Overlay 2 with rectangles
        overlay2 = saturated_frame.copy()
        cv2.rectangle(overlay2, (variables["kiri_lambat"], 0),
                    (variables["kanan_lambat"], 1800), (255, 0, 255), -1)
        cv2.rectangle(overlay2, (variables["kiri_berhenti"], 0),
                    (variables["kanan_berhenti"], 1800), (255, 125, 255), -1)

        return overlay1, overlay2

   
    def calculate(x1_1, x1_2, y1_1, y1_2):
        variables = opencv2.var_init()
        center_x_box0 = (x1_1 + x1_2) / 2
        center_y_box0 = (y1_1 + y1_2) / 2
        center_objek0 = (int(center_x_box0), int(center_y_box0))

        vector0_1 = np.array(center_objek0) - np.array(variables["center_frame"])
        vector0_2 = np.array(variables["top_center"]) - np.array(variables["center_frame"])
        angle_radians0 = np.arctan2(vector0_1[1], vector0_1[0]) - np.arctan2(vector0_2[1], vector0_2[0])
        angle_degrees0 = (math.degrees(angle_radians0) + 360) % 360

        return angle_degrees0, center_objek0

   
    def draw(saturated_frame, font, cls0, conf0, x1, y1, x2, y2):
        variables = opencv2.var_init()
        angle_degrees0, center_objek0 = opencv2.calculate(x1, x2, y1, y2)
        cv2.rectangle(saturated_frame, (x1, y1), (x2, y2), (0, 0, 255), 3)
        cv2.line(saturated_frame, variables["center_frame"], center_objek0, (0, 255, 0), 2)
        cv2.line(saturated_frame, variables["center_frame"], variables["top_center"], (0, 255, 0), 2)
        angle_text = f'Sudut: {angle_degrees0:.2f} derajat'
        text_size = cv2.getTextSize(angle_text, font, 1, 2)[0]
        text_x = (saturated_frame.shape[1] - text_size[0]) // 2
        text_y = (saturated_frame.shape[0] + text_size[1]) // 2
        cv2.putText(saturated_frame, angle_text, (text_x, text_y), font, 1, (0, 255, 0), 2)
        cv2.putText(saturated_frame, f'Kelas: {variables["z"][cls0]}, Kepercayaan: {conf0}', (x1, y1), font, 1, (0, 255, 0))

   
    def show(saturated_frame, cam=0):  # Added `cam` as a parameter
        overlay1, overlay2 = opencv2.overlay(saturated_frame)
        if cam == 1:
            cv2.addWeighted(overlay1, 0.5, saturated_frame, 0.5, 0, saturated_frame)
            cv2.imshow('me1', saturated_frame)
            if cv2.waitKey(1) & 0xFF == 27:
                raise Exception("Failed to capture frame from camera.")
        elif cam == 0:
            cv2.addWeighted(overlay2, 0.5, saturated_frame, 0.5, 0, saturated_frame)
            cv2.imshow('me0', saturated_frame)
            if cv2.waitKey(1) & 0xFF == 27:
                raise Exception("Failed to capture frame from camera.")


    def jalan():
        # Initialize the video capture and font
        saturated_frame, font = opencv2.init()
        
        # Create a VideoCapture object to capture video from the webca       
        # Start a loop to continuously process and display the frame
        while True:            
            # Draw and overlay on the new frame
            opencv2.draw(saturated_frame, font, 0, 0, 0, 0, 0, 0)
            opencv2.show(saturated_frame)  # Show the updated frame

            # Add a break condition or a key event to exit the loop (e.g., press 'q' to quit)
            if cv2.waitKey(1) & 0xFF == ord('q'):  # Exit on 'q' key
                break

        # Release the capture and close all windows
        cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    opencv2.jalan()