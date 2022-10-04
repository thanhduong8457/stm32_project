import cv2

def findObjects(img, objectCascade, scaleF = 1.1, minN = 4):   
    imgObjects = img.copy()
    imgGray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    objects = objectCascade.detectMultiScale(imgGray,scaleF,minN)
    objectsUot = []
    for (x,y,w,h) in objects:
        cv2.rectangle(imgObjects,(x,y),(x+w,y+h),(255,0,255),2)
        objectsUot.append([[x,y,w,h],w*h])
    
    objectsUot = sorted(objectsUot, key = lambda x:x[1], reverse = True)

    return imgObjects, objectsUot

def findCenter(imgObjects, objects):
    ih, iw, ic = imgObjects.shape
    cx, cy = iw//2, ih//2
    x,y,w,h = 0,0,0,0
    if len(objects)!=0:
        
        x,y,w,h =objects[0][0]
        cx = x + w//2
        cy = y + h//2

        cv2.circle(imgObjects,(cx,cy),2,(0,255,0),cv2.FILLED)
        cv2.line(imgObjects,(iw//2,cy),(cx,cy),(0,255,0),1)
        cv2.line(imgObjects,(cx,ih//2),(cx,cy),(0,255,0),1)

    return w, h, cx, cy, imgObjects

def main():
    cap = cv2.VideoCapture(0)
    faceCascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

    while True:
        success, img = cap.read()
        imgObjects, objects = findObjects(img, faceCascade)
        cv2.imshow("output", imgObjects)
        cv2.waitKey(1)

if __name__ == "__main__":
    main()