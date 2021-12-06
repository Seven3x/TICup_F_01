import cv2 as cv
import numpy as np
import time
import serial

def sendMessage(msg):
    ser.write(msg.encode("ascii"))


ser = serial.Serial(port="/dev/ttyAMA1", baudrate=9600, timeout = 0)

state = b'B'
temp = ser.read(1)
print(temp)
cap  = cv.VideoCapture(0)
kernel = cv.getStructuringElement(cv.MORPH_RECT,(3,3))
if not cap.isOpened():
    print("Cannot open video flow!")
    exit()

#读取所有的模板储存在templas里
templas = []
for i in range(1,9):
    templ = cv.imread(str(i) + ".jpg",0)
    retVal, templa = cv.threshold(templ, 127, 255, cv.THRESH_BINARY)
    templas.append(templa)
for i in range(1,9):
    templ = cv.imread(str(i) + "_1.jpg", 0)
    retVal, templa = cv.threshold(templ, 127, 255, cv.THRESH_BINARY)
    templas.append(templa)
print(len(templas))

#获取模板的长宽
w = templas[1].shape[1]
h = templas[1].shape[0]

#读取第一个图象用于获取gray的长宽
ret, frame = cap.read()
frame = cv.pyrDown(frame)
frame = frame[0:180,:]
gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
img = frame.copy()

x = frame.shape[1]

while True:
    if state == b'B':#如果串口收到B，返回B，树莓派不处理图像
        sendMessage('B')
        ret, frame = cap.read()
        temp = ser.read(1)
        ser.flushInput()
        print(temp)
        #sendMessage(str(3))
        if temp == b'A' or temp == b'B' or temp == b'Z' or temp == b'Y':
            state = temp
        continue
    
    elif state == b'A':如果串口收到A，返回A，树莓派处理图像
        #读取图像并获得ROI
        sendMessage('A')
        ret, frame = cap.read()
        frame = cv.pyrDown(frame)
        frame = frame[0:180,:]
        black = np.zeros(gray.shape, gray.dtype)

        #转换为二值图像并进行形态学操作滤波
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        retVal, thresh = cv.threshold(gray,100,255,cv.THRESH_BINARY)
        mor = cv.morphologyEx(thresh, cv.MORPH_OPEN, kernel, iterations=2)
        mor = cv.morphologyEx(mor, cv.MORPH_CLOSE, kernel, iterations=1)

        #Canny并获取轮廓，注意此处提取的是外部轮廓
        edges = cv.Canny(mor, 120, 480)
        cnts, hier = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        # cv.drawContours(black, cnts, -1, (0, 255, 0), 1)
        flag = False
        img = frame.copy()
        #存储dst
        dsts = []
        #存储dst的位置
        coords = []

        for cnt in cnts:
            if cv.contourArea(cnt) >= 120 :

                #获取最小面积矩形并进行透视变换
                rect = cv.minAreaRect(cnt)
                # calculate coordinates of the minimum area rectangle
                box = cv.boxPoints(rect)
                #储存位置
                # normalize coordinates to integers
                box = np.int0(box)
                coords.append(box[0])
                pts_o = np.float32(box)
                pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                """下面这段用不用无所谓，可以替换上一句，但是增加挺多运算量"""
                # if box[1][0] - box[0][0] < box[2][1] - box[1][1]:
                #     pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                # else:
                #     pts_d = np.float32([ [150, 0], [150, 200], [0, 200], [0, 0]])

                M = cv.getPerspectiveTransform(pts_o, pts_d)

                cv.drawContours(black, [cnt], -1, (255),2)
                dst = cv.warpPerspective(black, M, (150, 200))
                dsts.append(dst)
                cv.imshow("dst",dst)
                flag = True

        if flag:
            flag = False
            if len(dsts) == 0 :
                pass
            else:
                for i in range(0, len(templas)):
                    for j in range(0, len(dsts)):
                        # print(dsts[j])
                        # print(type(templas[i]),type(dsts[j]))
                        if not dsts[j].data:
                            pass
                        else:

                            result = cv.matchTemplate(dsts[j], templas[i], cv.TM_CCOEFF_NORMED)
                            (minVal, maxVal, minLoc, maxLoc) = cv.minMaxLoc(result)
                            if maxVal < 0.5:
                                pass
                            else:
                                # print(coords[j])
                                #该图像即为找到的数字！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                                cv.putText(img, str(i), (coords[j][0],coords[j][1]), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 5)
                                if i >= 8:
                                    i = i - 8
                                    sendMessage(str(i))
                                else:
                                    sendMessage(str(i))
        temp = ser.read(1)
        ser.flushInput()
        print(temp)
        if temp == b'A' or temp == b'B' or temp == b'Z' or temp == b'Y':
            state = temp
    
                            
    elif state == b'Y':如果串口收到Y，返回Y，树莓派处理左半边图像
        #读取图像并获得ROI
        sendMessage('Y')
        ret, frame = cap.read()
        frame = cv.pyrDown(frame)
        frame = frame[0:180,0:x//2]
        black = np.zeros(gray.shape, gray.dtype)

        #转换为二值图像并进行形态学操作滤波
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        retVal, thresh = cv.threshold(gray,100,255,cv.THRESH_BINARY)
        mor = cv.morphologyEx(thresh, cv.MORPH_OPEN, kernel, iterations=2)
        mor = cv.morphologyEx(mor, cv.MORPH_CLOSE, kernel, iterations=1)

        #Canny并获取轮廓，注意此处提取的是外部轮廓
        edges = cv.Canny(mor, 120, 480)
        cnts, hier = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        # cv.drawContours(black, cnts, -1, (0, 255, 0), 1)
        flag = False
        img = frame.copy()
        #存储dst
        dsts = []
        #存储dst的位置
        coords = []

        for cnt in cnts:
            if cv.contourArea(cnt) >= 120 :

                #获取最小面积矩形并进行透视变换
                rect = cv.minAreaRect(cnt)
                # calculate coordinates of the minimum area rectangle
                box = cv.boxPoints(rect)
                #储存位置
                # normalize coordinates to integers
                box = np.int0(box)
                coords.append(box[0])
                pts_o = np.float32(box)
                pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                """下面这段用不用无所谓，可以替换上一句，但是增加挺多运算量"""
                # if box[1][0] - box[0][0] < box[2][1] - box[1][1]:
                #     pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                # else:
                #     pts_d = np.float32([ [150, 0], [150, 200], [0, 200], [0, 0]])

                M = cv.getPerspectiveTransform(pts_o, pts_d)

                cv.drawContours(black, [cnt], -1, (255),2)
                dst = cv.warpPerspective(black, M, (150, 200))
                dsts.append(dst)
                flag = True

        if flag:
            flag = False
            if len(dsts) == 0 :
                pass
            else:
                for i in range(0, len(templas)):
                    for j in range(0, len(dsts)):
                        # print(dsts[j])
                        # print(type(templas[i]),type(dsts[j]))
                        if not dsts[j].data:
                            pass
                        else:

                            result = cv.matchTemplate(dsts[j], templas[i], cv.TM_CCOEFF_NORMED)
                            (minVal, maxVal, minLoc, maxLoc) = cv.minMaxLoc(result)
                            if maxVal < 0.5:
                                pass
                            else:
                                # print(coords[j])
                                #该图像即为找到的数字！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                                cv.putText(img, str(i), (coords[j][0],coords[j][1]), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 5)
                                if i >= 8:
                                    i = i - 8
                                    sendMessage(str(i))
                                else:
                                    sendMessage(str(i))
        temp = ser.read(1)
        ser.flushInput()
        print(temp)
        if temp == b'A' or temp == b'B' or temp == b'Z' or temp == b'Y':
            state = temp
                          
    
                          
    elif state == b'Z':如果串口收到Z，返回Z，树莓派处理右半边图像
        #读取图像并获得ROI
        sendMessage('Z')  
        ret, frame = cap.read()
        frame = cv.pyrDown(frame)
        frame = frame[0:180,x//2:x]
        black = np.zeros(gray.shape, gray.dtype)

        #转换为二值图像并进行形态学操作滤波
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        retVal, thresh = cv.threshold(gray,100,255,cv.THRESH_BINARY)
        mor = cv.morphologyEx(thresh, cv.MORPH_OPEN, kernel, iterations=2)
        mor = cv.morphologyEx(mor, cv.MORPH_CLOSE, kernel, iterations=1)

        #Canny并获取轮廓，注意此处提取的是外部轮廓
        edges = cv.Canny(mor, 120, 480)
        cnts, hier = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        # cv.drawContours(black, cnts, -1, (0, 255, 0), 1)
        flag = False
        img = frame.copy()
        #存储dst
        dsts = []
        #存储dst的位置
        coords = []

        for cnt in cnts:
            if cv.contourArea(cnt) >= 120 :

                #获取最小面积矩形并进行透视变换
                rect = cv.minAreaRect(cnt)
                # calculate coordinates of the minimum area rectangle
                box = cv.boxPoints(rect)
                #储存位置
                # normalize coordinates to integers
                box = np.int0(box)
                coords.append(box[0])
                pts_o = np.float32(box)
                pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                """下面这段用不用无所谓，可以替换上一句，但是增加挺多运算量"""
                # if box[1][0] - box[0][0] < box[2][1] - box[1][1]:
                #     pts_d = np.float32([[0, 0], [150, 0], [150, 200], [0, 200]])
                # else:
                #     pts_d = np.float32([ [150, 0], [150, 200], [0, 200], [0, 0]])

                M = cv.getPerspectiveTransform(pts_o, pts_d)

                cv.drawContours(black, [cnt], -1, (255),2)
                dst = cv.warpPerspective(black, M, (150, 200))
                dsts.append(dst)
                flag = True

        if flag:
            flag = False
            if len(dsts) == 0 :
                pass
            else:
                for i in range(0, len(templas)):
                    for j in range(0, len(dsts)):
                        # print(dsts[j])
                        # print(type(templas[i]),type(dsts[j]))
                        if not dsts[j].data:
                            pass
                        else:
                            result = cv.matchTemplate(dsts[j], templas[i], cv.TM_CCOEFF_NORMED)
                            (minVal, maxVal, minLoc, maxLoc) = cv.minMaxLoc(result)
                            if maxVal < 0.5:
                                pass
                            else:
                                # print(coords[j])
                                #该图像即为找到的数字！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
                                cv.putText(img, str(i), (coords[j][0],coords[j][1]), cv.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 5)
                                if i >= 8:
                                    i = i - 8
                                    sendMessage(str(i))
                                else:
                                    sendMessage(str(i))
    
        temp = ser.read(1)
        ser.flushInput()
        print(temp)
        if temp == b'A' or temp == b'B' or temp == b'Z' or temp == b'Y':
            state = temp
    cv.imshow("1",img)
    if cv.waitKey(1) & 0xFF == ord('q'):
        cv.imwrite("dst.jpg",dst)
        exit()

cv.destroyAllWindows() 