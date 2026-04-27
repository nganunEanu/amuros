from ultralytics import YOLO
import cv2
import math
import numpy as np
from classess import classNames  # Import class names (kelas objek) dari file classess.py
import imutils  # Import library imutils untuk pengolahan gambar

x = classNames
# Deklarasi setpoint (aturan atau parameter)

# Webcam
kiri_lambat = 475
kiri_berhenti = 490
kanan_lambat = 525
kanan_berhenti = 510

# Omni
berhenti_maju_x1 = 450
berhenti_maju_x2 = 550
berhenti_maju_y1 = 325
berhenti_maju_y2 = 425
berhenti_putar_x1 = 490
berhenti_putar_x2 = 510

# Inisialisasi model YOLO dengan file checkpoint tertentu (yolov8n, v5)
model = YOLO('yolov8n.pt')

# Definisikan font untuk menampilkan teks pada gambar
font = cv2.FONT_HERSHEY_SIMPLEX

# Inisialisasi objek VideoCapture untuk webcam USB (mungkin perlu mengubah indeks)
cap = cv2.VideoCapture(0)  # Gunakan indeks 0 untuk kamera default
#cap1 = cv2.VideoCapture(0)  # Gunakan indeks 2 untuk kamera kedua

while True:
    # Tangkap frame dari webcam USB
    ret, frame0 = cap.read()
    ret, frame1 = cap.read()

    if not ret:
        break

    # Ubah ukuran frame untuk diproses
    frame0 = imutils.resize(frame0, width=1000, height=750)
    frame1 = imutils.resize(frame1, width=1000, height=750)

    # Lakukan deteksi objek menggunakan YOLO pada frame
    result0 = model(frame0, stream=True, )
    result1 = model(frame1, stream=True, )

    # Buat sebuah persegi transparan setengah
    overlay1 = frame0.copy()
    cv2.rectangle(overlay1, (kiri_lambat, 0), (kanan_lambat, 1800), (255, 0, 255,), -1)  # Warna RGBA dengan alpha=128
    cv2.rectangle(overlay1, (kiri_berhenti, 0), (kanan_berhenti, 1800), (255, 125, 255,), -1)

    # Buat persegi lainnya
    overlay2 = frame1.copy()
    cv2.rectangle(overlay2, (berhenti_maju_x1, berhenti_maju_y1), (berhenti_maju_x2, berhenti_maju_y2), (255, 0, 0,), -1)
    cv2.rectangle(overlay2, (berhenti_putar_x1, 0), (berhenti_putar_x2, 375), (255, 125, 0,), -1)

    # Iterasi melalui objek yang terdeteksi pada frame pertama (frame0)
    for r in result0:
        boxes = r.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
            cv2.rectangle(frame0, (x1, y1), (x2, y2), (0, 0, 255), 3)
            conf0 = math.ceil((box.conf[0] * 100)) / 100
            cls0 = int(box.cls[0])
            center_x_box0 = (x1 + x2) / 2
            center_y_box0 = (y1 + y2) / 2

            # Koordinat tengah objek yang terdeteksi
            center_objek0 = (int(center_x_box0), int(center_y_box0))

            # Koordinat tengah frame kamera
            frame_width0 = frame1.shape[1]
            frame_height0 = frame1.shape[0]
            center_frame0 = (frame_width0 // 2, frame_height0 // 2)

            # Koordinat tengah atas frame kamera
            top_center = (frame_width0 // 2, 0)
            # Hitung sudut antara tiga titik
            vector0_1 = np.array(center_objek0) - np.array(center_frame0)
            vector0_2 = np.array(top_center) - np.array(center_frame0)

            # Hitung sudut dengan tanda dalam derajat (-180 hingga 180)
            angle_radians0 = np.arctan2(vector0_1[1], vector0_1[0]) - np.arctan2(vector0_2[1], vector0_2[0])
            angle_degrees0 = math.degrees(angle_radians0)

            # Pastikan sudut berada dalam rentang 0 hingga 360 derajat
            angle_degrees0 = (angle_degrees0 + 360) % 360

            # Gambar garis menghubungkan tiga titik
            cv2.line(frame0, center_frame0, center_objek0, (0, 255, 0), 2)
            cv2.line(frame0, center_frame0, top_center, (0, 255, 0), 2)
            # Tampilkan sudut di tengah frame
            angle_text = f'Sudut: {angle_degrees0:.2f} derajat'
            text_size = cv2.getTextSize(angle_text, font, 1, 2)[0]
            text_x = (frame0.shape[1] - text_size[0]) // 2
            text_y = (frame0.shape[0] + text_size[1]) // 2
            cv2.putText(frame0, angle_text, (text_x, text_y), font, 1, (0, 255, 0), 2)

            # Tampilkan kelas dan kepercayaan
            cv2.putText(frame0, f'Kelas: {x[cls0]}, Kepercayaan: {conf0}', (x1, y1), font, 1, (0, 255, 0))

    # Iterasi melalui objek yang terdeteksi pada frame kedua (frame1)
    for r in result1:
        boxes = r.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
            cv2.rectangle(frame1, (x1, y1), (x2, y2), (0, 0, 255), 3)
            conf1 = math.ceil((box.conf[0] * 100)) / 100
            cls1 = int(box.cls[0])
            center_x_box1 = (x1 + x2) / 2
            center_y_box1 = (y1 + y2) / 2

            # Koordinat tengah objek yang terdeteksi
            center_objek1 = (int(center_x_box1), int(center_y_box1))

            # Koordinat tengah frame kamera
            frame_width1 = frame1.shape[1]
            frame_height1 = frame1.shape[0]
            center_frame1 = (frame_width1 // 2, frame_height1 // 2)

            # Koordinat tengah atas frame kamera
            top_center = (frame_width1 // 2, 0)
            # Hitung sudut antara tiga titik
            vector1_1 = np.array(center_objek1) - np.array(center_frame1)
            vector1_2 = np.array(top_center) - np.array(center_frame1)

            # Hitung sudut dengan tanda dalam derajat (-180 hingga 180)
            angle_radians1 = np.arctan2(vector1_1[1], vector1_1[0]) - np.arctan2(vector1_2[1], vector1_2[0])
            angle_degrees1 = math.degrees(angle_radians1)

            # Pastikan sudut berada dalam rentang 0 hingga 360 derajat
            angle_degrees1 = (angle_degrees1 + 360) % 360

            # Gambar garis menghubungkan tiga titik
            cv2.line(frame1, center_frame1, center_objek1, (0, 255, 0), 2)
            cv2.line(frame1, center_frame1, top_center, (0, 255, 0), 2)
            # Tampilkan sudut di tengah frame
            angle_text = f'Sudut: {angle_degrees1:.2f} derajat'
            text_size = cv2.getTextSize(angle_text, font, 1, 2)[0]
            text_x = (frame0.shape[1] - text_size[0]) // 2
            text_y = (frame0.shape[0] + text_size[1]) // 2
            cv2.putText(frame0, angle_text, (text_x, text_y), font, 1, (0, 255, 0), 2)

            # Tampilkan kelas dan kepercayaan
            cv2.putText(frame1, f'Kelas: {x[cls1]}, Kepercayaan: {conf1}', (x1, y1), font, 1, (0, 255, 0))

            if (angle_degrees0 < 180):
                print("Putar kiri")
            elif (angle_degrees0 > 180):
                print("Putar kanan")

            if (berhenti_maju_x1 < center_x_box1 < berhenti_maju_x2 and berhenti_maju_y1 < center_y_box1 < berhenti_maju_y2):
                print("Berhenti maju")
            else:
                print("Maju")

    # Gabungkan overlay pertama ke dalam frame dan tampilkan di jendela pertama
    cv2.addWeighted(overlay1, 0.5, frame0, 0.5, 0, frame0)
    cv2.imshow('me1', frame0)

    # Tampilkan frame yang telah diproses dengan overlay kedua di jendela kedua
    cv2.addWeighted(overlay2, 0.5, frame1, 0.5, 0, frame1)
    cv2.imshow('me2', frame1)

    # Periksa apakah tombol 'Esc' ditekan untuk keluar dari loop
    if cv2.waitKey(1) & 0xFF == 27:
        break

# Lepaskan objek VideoCapture dan tutup jendela OpenCV
cap.release()
cv2.destroyAllWindows()
