import threading
import av
import cv2
import sys
from PySide6.QtGui import QImage, QSurfaceFormat
import numpy as np
from PySide6.QtCore import QCoreApplication,Qt
from PySide6.QtWidgets import QApplication, QMainWindow,QGridLayout,QWidget,QPushButton, QVBoxLayout, QLabel
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
        # threading.Thread(target=self.openCamera,args=(container,video_stream)).start()

        
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
                if self.loop_count > 100000:
                    return
            # break
       

class MainWindow(QMainWindow):
    frame_signal = Signal(tuple)
    loop_count = 0

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Custom Widget Example")

        # open stream 
        options = {
            'rtsp_transport': 'tcp',
        }
        current_stream_link = 'rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif'
        container = av.open(current_stream_link, timeout=20, options=options)
        video_stream = next((s for s in container.streams if s.type == 'video'), None)

        if video_stream is None:
            raise ValueError("No video stream found in the container.")

        self.frame_signal.connect(self.updateFrame)
        #


        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.grid_layout = QGridLayout()
        self.vbox_layout = QVBoxLayout()
        self.central_widget.setLayout(self.vbox_layout)

        self.custom_widget = QWidget()
        self.custom_widget.setLayout(self.grid_layout)
        self.vbox_layout.addWidget(self.custom_widget)

        self.addButton = QPushButton("Add Custom Widget")
        self.addButton.clicked.connect(self.add_custom_widget)
        self.vbox_layout.addWidget(self.addButton)
        self.row_idex = 0
        self.column_idex = 0
        self.gl_widgets = []
        # threading.Thread(target=self.openCamera,args=(container,video_stream)).start()

    def add_custom_widget(self):
        custom_widget = GLWidget()
        # custom_widget = QLabel("new")
        self.grid_layout.addWidget(custom_widget, self.row_idex, self.column_idex)
        self.gl_widgets.append(custom_widget)
        if(self.column_idex < 3):
            self.column_idex += 1
        else:
            self.column_idex = 0
            self.row_idex += 1
        
    def updateFrame(self,data):
        frame,width,height = data

        # Iterate through all widgets in the grid layout
        # for i in range(3):
        #     for j in range(3):
        #         item = self.centralWidget().layout().itemAtPosition(i, j)
        #         if item:
        #             widget = item.widget()
        #             widget.setRGBParameters(width,height)
        #             widget.setImage(frame)
        
        for widget in self.gl_widgets:
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
                if self.loop_count > 100000:
                    return
            # break

if __name__ == "__main__":

    QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts)

    app = QApplication(sys.argv)

    window = MainWindow()
    window.setGeometry(100, 100, 800, 600)
    window.show()
    sys.exit(app.exec())
