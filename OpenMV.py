# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math, ustruct
from pyb import UART
threshold_index = 0 # 0 for red, 1 for green, 2 for blue

pink_threshold =(45, 65, 40, 81, 15, 70)

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [pink_threshold] # generic_blue_thresholds

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)
def send_data_packet(x, y):
    temp = ustruct.pack("<bbii",                #格式为俩个字符俩个整型
                   65,                       #帧头1
                   66,                       #帧头2
                   int(x),                       #数据1
                   int(y))
    return temp
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.
x,y=0,0
while(True):
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True)
    if len(blobs) != 0:
        for blob in blobs:
            # These values depend on the blob not being circular - otherwise they will be shaky.
            s1=0
            if(blob.area()>s1):
                s1=blob.area()
                aimblob=blob
            # These values are stable all the time.
        img.draw_rectangle(aimblob.rect())
        x,y = aimblob.cx(), aimblob.cy()
        img.draw_cross(x,y)
        # Note - the blob rotation is unique to 0-180 only.
        uart.write(send_data_packet(x,y))
    print(clock.fps())

