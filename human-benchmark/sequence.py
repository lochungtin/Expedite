from time import sleep

import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller

BASE_CONFIG = {"top": 873, "left": 763, "width": 380, "height": 380, "mon": 0}
CLICK_NEXT = (BASE_CONFIG["left"] + 175, BASE_CONFIG["top"] + 297)
mouse = Controller()


def screenshot():
    img = np.array(sct.grab(BASE_CONFIG))
    gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
    gray = ~gray
    _, thres = cv.threshold(gray, 20, 255, cv.THRESH_BINARY_INV)
    return thres


mouse.position = CLICK_NEXT
mouse.click(Button.left)

for i in range(106):
    prev, data = np.zeros((380, 380)), np.zeros((380, 380))
    with mss() as sct:
        while True:
            prev = data
            thres = screenshot()
            data = np.array(thres)

            if data.flatten().any() and np.array_equal(prev, data):
                while screenshot().any():
                    pass
                break

            sleep(0.05)

    sleep(0.1)
    contours, _ = cv.findContours(data, mode=cv.RETR_TREE, method=cv.CHAIN_APPROX_NONE)

    for c in contours:
        x, y = c.reshape((-1, 2)).mean(axis=0).astype(int)
        x += BASE_CONFIG["left"]
        y += BASE_CONFIG["top"]
        sleep(0.1)
        mouse.position = (x, y)
        mouse.click(Button.left)

    sleep(1)
