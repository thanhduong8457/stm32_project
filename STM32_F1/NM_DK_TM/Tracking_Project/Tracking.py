import cv2
import ObjectDetectionModule as odm
import SerialModule as sm
import numpy as np
import time

cap = cv2.VideoCapture('Resources/test.mkv')
#cap = cv2.VideoCapture(0)

ser = sm.initConnection('COM7', 115200) # on jetson replace COM8 with /dev/ttyTHS1 coresponding 

faceCascade = cv2.CascadeClassifier("Resources/haarcascade_frontalface_default.xml")

while True:
    success, img = cap.read()
    img = cv2.resize(img, (0,0), None, 0.3, 0.3) # reduce amount process of program

    imgObjects, objects = odm.findObjects(img, faceCascade, 1.08,8)

    cd, cr, cx, cy, imgObjects = odm.findCenter(imgObjects, objects)

    h, w, c = imgObjects.shape
    cv2.line(imgObjects,(w//2,0),(w//2,h),(255,0,255),1)    #when run this program we can delete this command
    cv2.line(imgObjects,(0,h//2),(w,h//2),(255,0,255),1)    #when run this program we can delete this command

    sm.sendData(ser, [int(cx-w//2), cd], 5)

    img = cv2.resize(imgObjects, (0,0), None, 2, 2) #when run this program we can delete this command
    cv2.imshow("Result", img)   #when run this program we can delete this command

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break