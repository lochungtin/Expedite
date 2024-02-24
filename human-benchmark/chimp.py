from time import sleep

import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller

BASE_CONFIG = {"top": 810, "left": 598, "width": 710, "height": 440, "mon": 0}
CELL_BRD = 7
CELL_DIM = 66
CELL_SHIFT = 90
CLICK_XPAD = BASE_CONFIG["left"] + 40
CLICK_YPAD = BASE_CONFIG["top"] + 40
CLICK_NEXT = (BASE_CONFIG["left"] + 340, BASE_CONFIG["top"] + 360)
mouse = Controller()


def checkDigit(arr, shift):
    if arr[26, 33 + shift] == 255:
        return 1
    if arr[49, 21 + shift] == 255:
        return 2
    if arr[40, 36 + shift] == 255:
        return 4
    if arr[37, 44 + shift] == 255:
        return 5
    if arr[28, 32 + shift] == 255:
        return 6
    if arr[41, 32 + shift] == 255:
        return 7
    if arr[29, 27 + shift] == 255:
        return 8
    if arr[36, 31 + shift] == 255:
        return 9
    if arr[31, 32 + shift] == 255:
        return 3
    return 0


def arr2num(arr):
    if arr[:, 50].any():
        ten = checkDigit(arr, -14)
        digit = checkDigit(arr, 14)
        return ten * 10 + digit
    return checkDigit(arr, 0)


def next():
    global CLICK_NEXT, mouse
    mouse.position = CLICK_NEXT
    mouse.click(Button.left)


next()

with mss() as sct:
    for i in range(4, 41):
        sleep(0.1)

        img = np.array(sct.grab(BASE_CONFIG))
        gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
        gray = ~gray
        _, thres = cv.threshold(gray, 100, 255, cv.THRESH_BINARY_INV)
        data = np.array(thres)

        cells = {}
        for y in range(5):
            for x in range(8):
                xPad = CELL_BRD + (CELL_SHIFT * x)
                yPad = CELL_BRD + (CELL_SHIFT * y)
                crop = data[yPad : yPad + CELL_DIM, xPad : xPad + CELL_DIM]
                if crop.astype(bool).any():
                    cells[arr2num(crop)] = (x, y)

        for i in range(1, len(cells) + 1):
            x, y = cells[i]
            x = CLICK_XPAD + (CELL_SHIFT * x)
            y = CLICK_YPAD + (CELL_SHIFT * y)
            mouse.position = (x, y)
            mouse.click(Button.left)

        next()
