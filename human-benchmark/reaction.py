from time import sleep

import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller

PIXEL = (938, 1170)
BASE_CONFIG = {"top": PIXEL[1], "left": PIXEL[0], "width": 1, "height": 1, "mon": 0}
mouse = Controller()

mouse.position = PIXEL
mouse.click(Button.left)

sct = mss()
for i in range(5):
    sleep(0.5)
    while np.array(sct.grab(BASE_CONFIG))[0][0][1] < 200:
        pass
    mouse.click(Button.left)
    sleep(0.2)
    mouse.click(Button.left)

sct.close()
