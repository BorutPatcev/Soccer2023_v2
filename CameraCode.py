# Untitled - By: Borut - Sun Feb 19 2023

import sensor, image, time, math
from pyb import UART
from pyb import LED

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

uart = UART(3, 115200)

r = LED(1)
g = LED(2)
b = LED(3)

clock = time.clock()

thresholds = (30, 100, 15, 127, 15, 127)

r.off()
g.off()
b.off()

while(True):

    clock.tick()
    img = sensor.snapshot()
    found = False
    xM = 0
    yM = 0
    rM = 0

    for blob in img.find_blobs([thresholds], pixels_threshold=50, area_threshold=50, merge=True):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        found = True
        if blob.r() > rM:
            xM = blob.cx()
            yM = blob.cy()

    if found == True:
        r.on()
    else:
        r.off()

    uart.write(str(xM) + ' ' + str(yM) + '#')
