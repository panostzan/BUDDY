import cv2 
import serial
import numpy as np


# --------------------
#CONFIGURATION
#---------------------
SERIAL_PORT = "COMT5"
BAUD_RATE = 9600

lower_red = np.array([0,0,150])
upper_red = np.array([100,100,255])

kp = 0.03
STARTING_SERVO_ANGLE = 90



#---------------
# HARDWARE SETUP
#---------------

camera = cv2.VideoCapture(0)
arduino = serial.Serial("COM5",9600)
servoAngle = STARTING_SERVO_ANGLE

#------------------
#MAIN LOOP
#------------------

while True:

    #read one frame from the camera
    success, frame = camera.read()

    if(success == False):
        break

        #mirror the image so movement feels natural 
    mirrorframe = cv2.flip(frame , 1) # flip the frame accross the vertical axis 

    #get the dimensions
    height, width , channels = mirrorframe.shape
    frameCenterX= width /2 


    #----------------------
    #PERCEPTION
    #----------------------
    # [B, G, R]
   
    #create a binary mask containing pixels that look red 
    mask = cv2.inRange(mirrorframe, lower_red,upper_red)


    # cv2.findContours(the array we want, what contours it RETRieves, the chain (the links))
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    #--------------------
    #TARGET TRACKING
    #--------------------

    if len(contours) > 0:
        biggest = max(contours, key =cv2.contourArea)
        cv2.drawContours(mirrorframe, [biggest], -1, (0, 255, 0), 2)

        #find bounding box around target 
        x,y,w,h = cv2.boundingRect(biggest)

        #find the horizontal center 
        centerx = x + (w/2)

        #calcualte distance from the real center
        error = centerx -frameCenterX

        #---------------
        #CONTROLLER
        #---------------

        correction = kp * error 
        servoAngle = servoAngle + correction
        servoAngle = int(max(0, min(180, servoAngle)))

        #-----------------
        #SERIAL SENDING
        #----------------

        arduino.write((str(servoAngle) + "\n").encode("utf-8"))

        #----------------
        #DEBUG DISPLAY
        #----------------


        cv2.putText(mirrorframe, str(centerx), (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.putText(mirrorframe, str(error), (20, 80),cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

#-----------------------
#DISPLAY
#-----------------------
    cv2.imshow("this is the window", mirrorframe)
    cv2.imshow("Mask", mask)

    



    



    if cv2.waitKey(1) == ord("q"):
        break 

cv2.destroyAllWindows()
camera.release()
arduino.close()