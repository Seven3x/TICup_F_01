# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2 as cv
import numpy as np

def printTime(t1):
    t2 = cv.getTickCount()
    print((t2-t1)/cv.getCPUTickCount())
    return t2



# initialize the camera and grab a reference to the raw camera capture
kernel = cv.getStructuringElement(cv.MORPH_RECT, (3,3))
camera = PiCamera()
camera.resolution = (256, 192)
camera.framerate = 30
camera.shutter_speed = 4000
rawCapture = PiRGBArray(camera, size=(256, 192))
# allow the camera to warmup
time.sleep(0.1)



# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    # grab the raw NumPy array representing the image, then initialize the timestamp
    # and occupied/unoccupied text
    img = frame.array[0:112,:,:]#get ROI
    
    t1 = cv.getTickCount()
    # change color to gray
    img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    thresh = cv.inRange(img, 3, 100)
    mor = cv.morphologyEx(thresh, cv.MORPH_CLOSE, kernel, iterations= 1)
    mor = cv.morphologyEx(thresh, cv.MORPH_OPEN, kernel, iterations= 1)
    
    cnts, hier = cv.findContours(mor, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    
    for cnt in cnts:
        if cv.contourArea(cnt)>40:
            
            #找到最小区域坐标
            rect = cv.minAreaRect(cnt)
            box = cv.boxPoints(rect)
            box = np.int0(box)

            
            if(rect[0][1] >= rect[1][0]):
                pts_o = np.float32(box)
                pts_d = np.float32([[0, 64], [0, 0], [48, 0], [48, 64]])
            if(rect[0][1] < rect[1][0]):
                pts_o = np.float32(box)
                pts_d = np.float32([[0, 0], [48, 0], [48, 64], [0, 64]])
            
            M = cv.getPerspectiveTransform(pts_o, pts_d)
            #执行透视变换
            dst = cv.warpPerspective(mor, M, (48, 64))
            cv.imshow("Frame", dst)
                
    #显示输出图像
    
    cv.imshow("b", img)

    rawCapture.truncate(0)
    
    if cv.waitKey(1) & 0xFF == ord('q'):
        cv.imwrite("dst.jpg",dst)
        exit()
        
