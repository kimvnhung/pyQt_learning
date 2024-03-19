import sys
from PySide6.QtWidgets import QApplication, QMainWindow,QWidget
from PySide6.QtMultimedia import QMediaPlayer
from PySide6.QtMultimediaWidgets import QVideoWidget
from PySide6.QtCore import QMetaType
from qtWindowSample import MyPlayer

class CustomWindow(QMainWindow):
    myplayer = None

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Custom Window")
        self.myplayer = MyPlayer()

        self.custom_widget = self.myplayer.widget()
        # self.custom_widget = QVideoWidget()
        self.setCentralWidget(self.custom_widget)
        self.custom_widget.print("Hello from python")

        # Example string
        input_string = "rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif"

                # Desired array length
        array_length = 9

        # Create an array with the same string repeated
        input_array = [input_string] * array_length

        for i in range(array_length):
            self.myplayer.play("rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif")

        # rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif 
        # self.custom_widget.play("gst-pipeline:rtspsrc location=rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif ! decodebin ! autovideosink")
        # self.custom_widget.play("rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif")

if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = CustomWindow()
    window.setGeometry(100, 100, 400, 300)
    window.show()
    sys.exit(app.exec())
