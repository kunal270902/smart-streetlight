import cv2

thres = 0.5
import serial
import time
# import drivers
from time import sleep

a = 0
# display = drivers.Lcd()
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
ser.flush()


def esp():
    '''while True:
       # if ser.in_waiting>0:
            #print("hello")
           while ser.in_waiting<=0:
               time.sleep(0.01)
           line=ser.readline().decode('utf-8').rstrip()
           print(line)
           #print(len(line))
           if (line=="0"):
               print("empty")
               ser.write("1".encode('utf-8'))'''


def cv():
    # cap= cv2.VideoCapture(0)
    cap = cv2.VideoCapture("2.mp4")

    cap.set(3, 640)
    cap.set(4, 480)
    classNames = []
    classFile = 'coco.names'
    with open(classFile, 'rt') as f:
        classNames = f.read().rstrip('\n').split('\n')

    configPath = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
    weightsPath = 'frozen_inference_graph.pb'

    net = cv2.dnn_DetectionModel(weightsPath, configPath)
    net.setInputSize(320, 320)
    net.setInputScale(1.0 / 127.5)
    net.setInputMean((127.5, 127.5, 127.5))
    net.setInputSwapRB(True)

    while True:
        ser.write("esp init".encode('utf-8'))
        while ser.in_waiting <= 0:
            time.sleep(0.01)
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        # display.lcd_display_string("Hello World")
        if (line != "inf"):
            print("esp working")
            # print("Writing to display")
            # display.lcd_display_string("Hello World")  # Write line of text to first line of display

            # print(len(line))
        if (line == "inf"):
            print("empty")
            success, img = cap.read()
            classIds, confs, bbox = net.detect(img, confThreshold=thres)
            # print(classIds,bbox)
            if len(classIds) != 0:
                for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
                    cv2.rectangle(img, box, color=(0, 255, 0), thickness=3)
                    cv2.putText(img, classNames[classId - 1].upper(), (box[0] + 10, box[1] + 30),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

                    cv2.putText(img, str(round(confidence * 100, 2)), (box[0] + 300, box[1] + 30),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

                    if (classNames[classId - 1].upper() == "CAR"):
                        print("car identified")
                        # display.lcd_display_string("lights on")
                        ser.write("1".encode('utf-8'))
                        print("lights on")
                    cv2.imshow("output", img)
                    cv2.waitKey(1)
        time.sleep(1)

        # print(classIds,bbox)


if _name_ == '_main_':
    cv()