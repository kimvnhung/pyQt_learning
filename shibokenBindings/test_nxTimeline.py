import sys
import random
from PySide6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QPushButton, QLabel
from PySide6.QtQuickWidgets import QQuickWidget
from nxTimeLine import TimeLineController

class CustomWidget(QWidget):
    def __init__(self):
        super().__init__()

        layout = QVBoxLayout()
        self.label = QLabel("Random Number Will Appear Here")
        layout.addWidget(self.label)
        self.setLayout(layout)

    def update_label(self, formatted_time):
        self.label.setText(f"Formatted Time: {formatted_time}")

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Random Number Generator")
        self.setGeometry(100, 100, 1200, 600)

        layout = QVBoxLayout()

        self.custom_widget = CustomWidget()
        layout.addWidget(self.custom_widget)

        self.generate_button = QPushButton("Generate Random Number")
        self.generate_button.clicked.connect(self.generate_random_number)
        layout.addWidget(self.generate_button)

        self.timeline_controller = TimeLineController()
        self.timeline_controller.positionChanged.connect(self.position_changed)
        self.timeline_controller.setMinimumSize(300,200)
        layout.addWidget(self.timeline_controller.widget())

        central_widget = QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

    def generate_random_number(self):
        random_number = random.randint(100000, 999999999)
        formatted_time = self.milliseconds_to_time(random_number)
        self.custom_widget.update_label(formatted_time)
        self.timeline_controller.initData(random_number)

    def milliseconds_to_time(self, milliseconds):
        # Calculate days, hours, minutes, seconds, and milliseconds
        seconds, milliseconds = divmod(milliseconds, 1000)
        minutes, seconds = divmod(seconds, 60)
        hours, minutes = divmod(minutes, 60)
        days, hours = divmod(hours, 24)

        # Format the time string
        formatted_time = f"{days:02d}:{hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}"
        return formatted_time

    def position_changed(self, position):
        print(position)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
