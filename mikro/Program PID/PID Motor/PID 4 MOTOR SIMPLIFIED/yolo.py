from ultralytics import YOLO
import numpy

# load a pretrained YOLOv8n model
model = YOLO("yolov5n.pt", "v5")  

# predict on an image
detection_output = model.predict(source="0", conf=0.25, save=True, stream=True, show=True) 
#detection_output.show()

for r in detection_output:
 #print(r.probes)
 boxes=r.boxes
 print(r.boxes)
 for box in boxes:
     
    print(box.xyxy)
    print(box.xywh)
    print(box.xyxyn)
    print(box.xywhn)

# Display tensor array
#print(detection_output)

# Display numpy array
#print(detection_output[0].numpy())