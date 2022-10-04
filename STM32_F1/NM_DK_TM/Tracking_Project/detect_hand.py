import cv2
import mediapipe as mp
import time
import math
import serial

wCam, hCam = 1280, 720

cap = cv2.VideoCapture(0)
cap.set(3, wCam)
cap.set(4, hCam)
#cap.set(10, 100)

mpHands = mp.solutions.hands
hands = mpHands.Hands()
myDraw = mp.solutions.drawing_utils

def initConnection(portNo, baudRate):
    try:
        ser = serial.Serial(portNo, baudRate)
        print("Device Connected")
        return ser
    except:
        print("Not Connected")

cur_time,pre_time,cx_4,cy_4 = 0,0,0,0

if not (cap.isOpened()):
    print('Could not open video device')

if __name__ == "__main__":
    ser = initConnection("COM8", 9600)
    count = 0
    while True:
        success, img = cap.read()
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        results = hands.process(imgRGB)
        #print(results.multi_hand_landmarks)

        if results.multi_hand_landmarks:
            for handLms in results.multi_hand_landmarks:
                for id, lm in enumerate(handLms.landmark):              
                    h, w, c = img.shape
                    cx, cy = int(lm.x*w), int(lm.y*h)

                    if id==4:
                        cx_4,cy_4 = cx,cy
                    if id==8:
                        distance = round(((float)((((cx-cx_4)**2+(cy-cy_4)**2)**0.5)*(28.5/1280))),2)          
                    
                        datasend = str(distance)+"\r\n"
                        count = count + 1
                        if(count==3):
                            count = 0
                            ser.write(datasend.encode())

                myDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)

        cur_time = time.time()
        fps = 1/(cur_time - pre_time)
        pre_time = cur_time

        cv2.putText(img, str(int(fps)),(10,70),cv2.FONT_HERSHEY_PLAIN,3,(255,0,255),3)
        cv2.imshow("image", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
