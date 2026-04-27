
import time
class main:
    def program(communication,perintah,angle_degrees0,cls0,variables):
        kiri_lambat = variables["kiri_lambat"]
        kiri_berhenti = variables["kiri_berhenti"]
        kanan_lambat = variables["kanan_lambat"]
        kanan_berhenti = variables["kanan_berhenti"]
        berhenti_maju_x1 = variables["berhenti_maju_x1"]
        berhenti_maju_x2 = variables["berhenti_maju_x2"]
        berhenti_maju_y1 = variables["berhenti_maju_y1"]
        berhenti_maju_y2 = variables["berhenti_maju_y2"]
        delay_duration=2
        if perintah == 1:
            current_timedelay = int(time.time())
            
            # Stage 0: Initial delay handling
            if stage == 0:
                communication.serial.record(1, 0, 0, 0, 0, 0.1)
                if current_timedelay - start_time >= delay_duration:
                    stage = 1
                    start_time = current_timedelay
                    print("2 seconds have passed.")

            # Stage 1: Object tracking and handling angles
            elif stage == 1:
                if cls0 == 0:
                    # Efficiently handle angle ranges
                    angle_actions = [
                        (8, 30, (0, -1)),
                        (30, 180, (0, -5)),
                        (180, 330, (0, 5)),
                        (330, 355, (0, 1))
                    ]

                    for lower, upper, action in angle_actions:
                        if lower < angle_degrees0 < upper:
                            communication.serial.record(0, 0, 0, action[1], 1, 0)
                            break
                    else:
                        # Check if the object is within the stopping area
                        if berhenti_maju_x1 < center_objek0[0] < berhenti_maju_x2 and \
                        berhenti_maju_y1 < center_objek0[1] < berhenti_maju_y2:
                            communication.serial.record(0, 0, 0, 0, 1, 0.1)
                            stage = 2
                else:
                    communication.serial.record(0, 0, 10, 0, 2, 0.1)

            # Stage 2: Further object handling based on classification
            elif stage == 2:
                communication.serial.record(0, 0, 0, 3, 1, 0.1)
                if cls0 == 2:
                    if kiri_lambat < center_objek0[0] < kiri_berhenti:
                        communication.serial.record(0, 0, 0, 1, 1, 0.1)
                    elif kanan_berhenti < center_objek0[0] < kanan_lambat:
                        communication.serial.record(0, 0, 0, -1, 1, 0.1)
                    else:
                        communication.serial.record(2, 0, 0, 0, 0, 0.1)
                        stage = 3

        else:
            # Reset conditions if no command received
            communication.serial.record(0, 0, 0, 0, 0, 0.1)
            stage = 0
            #cap.release()
            #cap = cv2.VideoCapture(0)
            start_time = int(time.time())  
    