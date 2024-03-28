import threading
import av
import cv2
import sys
from PySide6.QtGui import QImage
import numpy as np
from PySide6.QtWidgets import QApplication, QMainWindow,QGridLayout,QWidget
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtOpenGL import QOpenGLFunctions_3_3_Core,QOpenGLShaderProgram
import time
from PySide6.QtCore import Signal
from glwidget import GLWidget


class CustomWindow(QMainWindow):
    frame_signal = Signal(tuple)

    loop_count = 0

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Custom Window")
        options = {
            'rtsp_transport': 'tcp',
        }
        current_stream_link = 'rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif'
        container = av.open(current_stream_link, timeout=20, options=options)
        video_stream = next((s for s in container.streams if s.type == 'video'), None)

        if video_stream is None:
            raise ValueError("No video stream found in the container.")

        self.frame_signal.connect(self.updateFrame)
        self.custom_widget = QWidget()

        layout = QGridLayout()
        # Add some buttons to the grid layout
        for i in range(3):
            for j in range(3):
                widget = GLWidget()
                layout.addWidget(widget, i, j)

        self.custom_widget.setLayout(layout)
        
        self.setCentralWidget(self.custom_widget)
        threading.Thread(target=self.openCamera,args=(container,video_stream)).start()

        
    def updateFrame(self,data):
        frame,width,height = data

        # Iterate through all widgets in the grid layout
        for i in range(3):
            for j in range(3):
                item = self.centralWidget().layout().itemAtPosition(i, j)
                if item:
                    widget = item.widget()
                    widget.setRGBParameters(width,height)
                    widget.setImage(frame)
        


    def openCamera(self,container,video_stream):
        for packet in container.demux(video_stream):
            for frame in packet.decode():
                start1 = time.time_ns()
                mat_frame_default = frame.reformat(format='rgb24')
                mat_frame_default = mat_frame_default.to_ndarray()
                print(mat_frame_default.shape)
                width = mat_frame_default.shape[1]
                height = mat_frame_default.shape[0]
                # Get the current time before starting the process
                start_time = time.time_ns()
                # Convert framedata to QImage
                image = QImage(mat_frame_default.data, width, height, QImage.Format_RGB888)
                data = (image,width,height)
                self.frame_signal.emit(data)


                # Output the time taken by the process
                print("Time taken by the process:", (time.time_ns() - start1))
                # cv2.imshow("name",mat_frame_default)
                # break
                # use OpenCV to show the frame
                self.loop_count += 1
                if self.loop_count > 300:
                    return
            # break
       
if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = CustomWindow()
    window.setGeometry(100, 100, 400, 300)
    window.show()
    sys.exit(app.exec())
