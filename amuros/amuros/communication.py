import pyvisa as visa
import socket
import serial
import time
class communication:

    class serial:
        
        def init():
            rm = visa.ResourceManager()
            print(rm.list_resources())
            # Initialize serial port
            serialA = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)
            return serialA


        def record(serial,mode,x,y,theta,speed,interval): #kirim data ke micro
            global prevT
            #serialA = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)
            dataOut = (str(mode) + ',' + str(x) +  ',' + str(y) + ',' + str(theta) + ',' + str(speed) + '}')
            if dataOut != None:
                print('\n Status: ', serial)
                print(dataOut)
                serial.write(dataOut.encode('utf-8'))

    class udp:
        def get_ip():
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect(("8.8.8.8", 80))
            _ip = s.getsockname()[0]
            s.close()
            return _ip
        
        def get_udp(sock):
            global perintah_seb  # Use global to modify the outer variable
            try:
                data, addr = sock.recvfrom(1024)
                perintah = int(data)

                # Check if `perintah_seb` is different from the current `perintah`
                if perintah_seb != perintah:
                    print("New command received.")

                perintah_seb = perintah  # Update `perintah_seb`

                print(f"Received message from {addr}: {perintah}")
                print("Received command:", perintah)
                return perintah
            except socket.timeout:
                print("Socket timed out. No data received.")
            except Exception as e:
                print("Error:", e)
                

        def init():        
            local_ip = '0.0.0.0'
            local_port = 5005
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.bind((local_ip, local_port))
            ip, port = communication.udp.get_ip(), 9999
            print("IP:", ip, port)
            sock.settimeout(0.1)
            return sock
        



def jalan():
    sockser = communication.udp.init()
    #communication.serial.init()

    while True:
        communication.udp.get_udp(sockser)

        # # Start the timer
        # start_time = time.time()

        # # Prompt for input
        # user_input = None
        # print("Enter record data (or press Enter to skip): ", end="")

        # # Wait for input, check for timeout every 0.1 second
        # while True:
        #     # Check if 10 seconds have passed
        #     if time.time() - start_time > 10:
        #         print("\nNo input received in the given time (10 seconds), continuing the loop.")
        #         user_input = None
        #         break

        #     if user_input is None:
        #         user_input = input()  # Try to get input from the user
        #         if user_input:  # Exit loop if input is received
        #             break

        # if user_input:
        #     # If input was received within the timeout, process it
        #     communication.serial.record(user_input)  # Modify this to use the input
        #     print(f"Recorded data: {user_input}")
        # else:
        #     # No input was received within the timeout, continue loop
        #     print("Continuing without recording.")



if __name__ == "__main__":
    jalan()
        
        