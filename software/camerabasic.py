import cv2




#HAPPENS ONCE CONNECT TO CAMERA 
camera = cv2.VideoCapture(0)



while True:
  
    success , frame = camera.read()

    if not success:
        break


    BGR = frame[240,320,:] 
    blue = frame[240, 320, 0]
    green = frame[240,320,1]
    red = frame[240,320,2]

    brightness = (int(blue) +int(green) + int(red)) / 3
    print(brightness)

    if brightness >= 60:
        print("BRIGHT")
    elif brightness <=10:
        print("DARK")
    else:
        print("neutral")
    
        






    cv2.imshow("yesbababe" , frame)

    if cv2.waitKey(1) == ord('q'):
        break 


cv2.destroyAllWindows()
camera.release()

