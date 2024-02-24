from time import sleep

import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller

BASE_CONFIG = {"top": 820, "left": 598, "width": 710, "height": 440, "mon": 0}
CLICK_START = (BASE_CONFIG["left"] + 320, BASE_CONFIG["top"] + 220)
mouse = Controller()

mouse.position = CLICK_START
mouse.click(Button.left)
sleep(0.005)

with mss() as sct:
    for i in range(30):
        sleep(0.055)
        img = np.array(sct.grab(BASE_CONFIG))
        gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
        gray = ~gray
        _, thres = cv.threshold(gray, 100, 255, cv.THRESH_BINARY_INV)
        arr = np.array(thres)

        mx = arr.argmax() % thres.shape[1]
        my = arr.argmax() // thres.shape[1] + 30
        mouse.position = (BASE_CONFIG["left"] + mx, BASE_CONFIG["top"] + my)
        mouse.click(Button.left)
