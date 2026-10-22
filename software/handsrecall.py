import cv2
import numpy 
import mediapipe 


cap = cv2.VideoCapture(0)

while True: 
    success, frame = cap.read()
    if not success:
        break 

    cv2.imshow("recal practice simple framing", frame)

    if cv2.waitKey(1) == ord("q"):   
    
        break



cap.release
cv2.destroyAllWindows


    



