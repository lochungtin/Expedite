import cv2
import numpy as np
from mss import mss

step = 1
config = {"top": 0, "left": 0, "width": 1920, "height": 1080, "mon": 0}
sct = mss()
while True:
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break
    elif key == ord("w"):
        config["height"] -= step
    elif key == ord("a"):
        config["width"] -= step
    elif key == ord("s"):
        config["top"] += step
    elif key == ord("d"):
        config["left"] += step
    elif key == ord("e"):
        step -= 1
    elif key == ord("r"):
        step += 1

    print(config, step)

    img = np.array(sct.grab(config))
    cv2.imshow("Screenshot", img)
sct.close()
cv2.destroyAllWindows()
