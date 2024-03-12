import sys
from PySide6.QtWidgets import QApplication, QMainWindow
from qtWindowSample import CustomWidget


class CustomWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Custom Window")

        self.custom_widget = CustomWidget()
        self.setCentralWidget(self.custom_widget)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CustomWindow()
    window.setGeometry(100, 100, 400, 300)
    window.show()
    sys.exit(app.exec())
