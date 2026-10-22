import serial 
import time

print(dir(time))
print()
print(dir(serial))


#arduino = serial.Serial("COM5" , 9600)
time.sleep(2)

while True:
    
    userInput =input("Enter a number between 0-180 or q to quit")
    if(userInput=="q"):
        break
    userInput = int(userInput)
    if (0<= userInput <=180):
        arduino.write((str(userInput) + "\n").encode())
    else:
        print("please try again that value is not applicable ")
    



arduino.write(userInput)



