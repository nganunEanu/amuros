import tkinter as tk
from tkinter import Listbox, Scrollbar
from PIL import Image, ImageTk  # Ensure ImageTk is imported from PIL
import cv2

class CameraApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Camera Feed with Data Log")

        # Initialize OpenCV VideoCapture
        self.video_source = 0  # Use default camera
        self.cap = None
        self.is_running = False

        # Frame for Camera and Data Log
        self.main_frame = tk.Frame(root, bg="gray")
        self.main_frame.pack(fill=tk.BOTH, expand=True)

        # Camera Display Area (Labeled "kamera")
        self.camera_frame = tk.Frame(self.main_frame, bg="gray", width=640, height=480)
        self.camera_frame.grid(row=0, column=0, padx=10, pady=10)
        self.camera_label = tk.Label(self.camera_frame, text="kamera", bg="gray", fg="black")
        self.camera_label.pack(fill=tk.BOTH, expand=True)

        # Data Log Area (Labeled "data log")
        self.log_frame = tk.Frame(self.main_frame, bg="gray", width=300, height=480)
        self.log_frame.grid(row=0, column=1, padx=10, pady=10)
        self.log_label = tk.Label(self.log_frame, text="data log", bg="gray", fg="black")
        self.log_label.pack(fill=tk.BOTH, expand=True)

        # Listbox for Data Log
        self.data_log = Listbox(self.log_frame, height=20, width=40)
        self.data_log.pack(side=tk.LEFT, fill=tk.Y)
        self.scrollbar = Scrollbar(self.log_frame, orient="vertical", command=self.data_log.yview)
        self.scrollbar.pack(side="right", fill="y")
        self.data_log.config(yscrollcommand=self.scrollbar.set)

        # Buttons (Start, Record, Stop)
        self.button_frame = tk.Frame(root, bg="gray")
        self.button_frame.pack(fill=tk.X)
        self.start_button = tk.Button(self.button_frame, text="Start", command=self.start_camera)
        self.start_button.pack(side=tk.LEFT, padx=5, pady=5)
        self.record_button = tk.Button(self.button_frame, text="Record", state=tk.DISABLED)  # Record function not implemented
        self.record_button.pack(side=tk.LEFT, padx=5, pady=5)
        self.stop_button = tk.Button(self.button_frame, text="Stop", state=tk.DISABLED, command=self.stop_camera)
        self.stop_button.pack(side=tk.LEFT, padx=5, pady=5)

    def start_camera(self):
        """Start the camera and detection loop."""
        self.is_running = True
        self.cap = cv2.VideoCapture(self.video_source)
        if not self.cap.isOpened():
            print("Error: Could not open video source.")
            return
        
        self.start_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)
        self.record_button.config(state=tk.NORMAL)
        
        self.update_frame()

    def stop_camera(self):
        """Stop the camera and reset the UI."""
        self.is_running = False
        if self.cap:
            self.cap.release()
        self.start_button.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)
        self.record_button.config(state=tk.DISABLED)
        self.data_log.delete(0, tk.END)  # Clear the data log

    def update_frame(self):
        """Capture frames and display them in the tkinter window."""
        if self.is_running:
            ret, frame = self.cap.read()
            if not ret:
                print("Failed to grab frame.")
                return

            # Convert the frame to RGB (from BGR)
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            # Convert the frame to a PIL Image and then to a format Tkinter understands
            img = Image.fromarray(frame_rgb)
            img_tk = ImageTk.PhotoImage(image=img)

            # Create a label for the frame and update it
            if hasattr(self, 'camera_label_instance'):
                self.camera_label_instance.configure(image=img_tk)
                self.camera_label_instance.image = img_tk  # Keep a reference to the image
            else:
                self.camera_label_instance = tk.Label(self.camera_frame, image=img_tk, bg="gray")
                self.camera_label_instance.image = img_tk  # Keep a reference
                self.camera_label_instance.pack(fill=tk.BOTH, expand=True)

            # Update the frame every 10 milliseconds
            self.root.after(10, self.update_frame)

if __name__ == "__main__":
    root = tk.Tk()
    app = CameraApp(root)
    root.mainloop()
