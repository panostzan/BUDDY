import cv2
import mediapipe as mp
import numpy as np
from mediapipe.tasks import python as mp_python
from mediapipe.tasks.python import vision


def calc_distance(p1, p2):
    return int(np.linalg.norm(np.array(p1) - np.array(p2)))



options = vision.HandLandmarkerOptions(
    base_options=mp_python.BaseOptions(model_asset_path="hand_landmarker.task"),
    num_hands=1)
detector = vision.HandLandmarker.create_from_options(options)

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

while True:
    success, frame = cap.read()
    if not success:
        break

    frame = cv2.flip(frame, 1)
    h, w, _ = frame.shape
    center_point = (w // 2, h // 2)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    mp_img = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb_frame)
    result = detector.detect(mp_img)

    index_finger_pos = None

    if result.hand_landmarks:
        hand = result.hand_landmarks[0]
        tip = hand[8]
        thumb =hand[4]
        middle = hand[12]
        ring = hand[16]
        pinky= hand[20]
        thumb_pos = (int(thumb.x*w), int(thumb.y*h))
        index_finger_pos = (int(tip.x * w), int(tip.y * h))
        middle_finger_pos = (int(middle.x * w), int(middle.y * h))
        ring_finger_pos = (int(ring.x * w), int(ring.y * h))
        pinky_finger_pos = (int(pinky.x * w), int(pinky.y * h))

    #cv2.circle(frame, center_point, 10, (255, 0, 0), -1)

    if index_finger_pos:
        cv2.circle(frame, index_finger_pos, 10, (0, 255, 0), -1)
        cv2.circle(frame, thumb_pos, 10, (0, 255, 0), -1)
        cv2.circle(frame, middle_finger_pos, 10, (0, 255, 0), -1)
        cv2.circle(frame, ring_finger_pos, 10, (0, 255, 0), -1)
        cv2.circle(frame, pinky_finger_pos, 10, (0, 255, 0), -1)

        #distance = calc_distance(center_point, index_finger_pos)

        thumb_pinch_distance =calc_distance(index_finger_pos, thumb_pos)
        pinky_pinch_distance =calc_distance(pinky_finger_pos, thumb_pos)


        cv2.line(frame, thumb_pos, index_finger_pos, (0, 255, 0), 2)
        cv2.line(frame, index_finger_pos,middle_finger_pos, (0, 255, 0), 2)
        cv2.line(frame, middle_finger_pos,ring_finger_pos, (0, 255, 0), 2)
        cv2.line(frame,ring_finger_pos,pinky_finger_pos,(0,255,0),2)

        #cv2.line(frame,thumb_pos,index_finger_pos, (0,0,255),2)
        #cv2.putText(frame, f"Distance: {distance}", (50, 50),
                    #cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)


        cv2.putText(frame, f"Thumb Pinch Distance: {thumb_pinch_distance}", (50, 50),
        cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0, 0), 2)
        
        cv2.putText(frame, f"Pinky Pinch Distance: {pinky_pinch_distance}", (50, 100),
        cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0, 0), 2)


        if thumb_pinch_distance <= 50:
            cv2.putText(frame, f"Thumb Pinch Distance: {thumb_pinch_distance}", (50, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0,0,200), 2)
        
        if pinky_pinch_distance <= 100:
            cv2.putText(frame, f"Pinky Pinch Distance: {pinky_pinch_distance}", (50, 100),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0,0, 200), 2)



    cv2.imshow("Hand Tracking", frame)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
