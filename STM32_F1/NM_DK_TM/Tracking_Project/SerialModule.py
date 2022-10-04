import serial   #sudo apt-get install python3-serial
import time

def initConnection(portNo, baudRate):
    try:
        ser = serial.Serial(portNo, baudRate)
        print("Device Connected")
        return ser
    except:
        print("Not Connected")

def sendData(se, data, digits):
    myString = "$"
    for d in data:
        myString += str(d).zfill(digits) + "|"
    myString += "\r\n"
    try:
        se.write(myString.encode())
        #print(myString)
    except:
        print("Data Transmition Failed")

if __name__ == "__main__":
    ser = initConnection("COM8", 9600)
    while True:
        sendData(ser, [50,255], 3)
        time.sleep(1)