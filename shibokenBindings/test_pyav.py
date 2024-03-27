import av
import cv2
from PySide6.QtGui import QImage
import numpy as np

options = {
    'rtsp_transport': 'tcp',
}
current_stream_link = 'rtsp://admin:abcd1234@192.168.1.250'
container = av.open(current_stream_link, timeout=20, options=options)
video_stream = next((s for s in container.streams if s.type == 'video'), None)
i = 0

if video_stream is None:
    raise ValueError("No video stream found in the container.")

for packet in container.demux(video_stream):
    for frame in packet.decode():
        mat_frame_default = frame.reformat(format='rgb24')
        mat_frame_default = mat_frame_default.to_ndarray()
        print(mat_frame_default.shape)
        if i < 5 :
            # Assuming framedata is a NumPy array containing image data
            # and width, height, and bytes_per_line are integer values
            # Here's a dummy initialization for demonstration purposes
            # framedata = np.random.randint(0, 256, size=(frame.height, frame.width, 3), dtype=np.uint8)
            width = mat_frame_default.shape[1]
            height = mat_frame_default.shape[0]

            # Convert framedata to QImage
            image = QImage(mat_frame_default.data, width, height, QImage.Format_RGB888)

            # Save QImage to a file (e.g., PNG)
            image.save('frame_'+str(i)+'.png')
            print('frame_'+str(i)+'.png')
            i += 1
            
        # use OpenCV to show the frame
        cv2.imshow('frame', mat_frame_default)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
