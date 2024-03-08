import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QWidget, QPushButton


class CustomWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.label = QLabel("Custom Widget", self)
        self.button = QPushButton("Click Me", self)
        self.button.clicked.connect(self.on_button_clicked)

        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(self.button)

        self.setLayout(layout)

    def on_button_clicked(self):
        self.label.setText("Button Clicked!")


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
