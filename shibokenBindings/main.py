import sys
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtMultimedia import QMediaPlayer
from PySide6.QtMultimediaWidgets import QVideoWidget
from qtWindowSample import CustomWidget

class CustomWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Custom Window")

        self.custom_widget = CustomWidget()
        # self.custom_widget = QVideoWidget()
        self.setCentralWidget(self.custom_widget)
        self.custom_widget.print("Hello from python")
        self.custom_widget.play("rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CustomWindow()
    window.setGeometry(100, 100, 400, 300)
    window.show()
    sys.exit(app.exec())
