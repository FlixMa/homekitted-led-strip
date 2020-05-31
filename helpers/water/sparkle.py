import cv2 as cv
import numpy as np

NUM_LEDS = 150
WATER_HUE = 90

WINDOW_NAME = 'image'

hsv = np.full((1, NUM_LEDS, 3), fill_value=255, dtype=np.float)
hsv[:, :, 0] = WATER_HUE + np.sin(np.linspace(-1, 1, NUM_LEDS)).reshape(hsv.shape[:2]) * 10

SPARKLE_LENGTH = 20
sparkle = np.abs(np.sin(np.linspace(0.3, np.pi * 0.8, SPARKLE_LENGTH)))

running = True
while running:

    hsv[:, :, 1] += 1
    sparkle_indices = np.random.uniform(0, NUM_LEDS - SPARKLE_LENGTH + 1, 1).astype(np.uint8)
    for i in sparkle_indices:
        width = np.random.normal(5, 50)
        hsv[0, i:i+SPARKLE_LENGTH, 1] -= sparkle * width
    hsv[:, :, 1] = np.minimum(np.maximum(0, hsv[:, :, 1]), 100)


    bgr = cv.cvtColor(hsv.astype(np.uint8), cv.COLOR_HSV2BGR)
    output = cv.resize(bgr, (0, 0), fx=10, fy=10, interpolation=cv.INTER_LINEAR)
    cv.namedWindow(WINDOW_NAME)
    cv.moveWindow(WINDOW_NAME, 200, 200)
    cv.imshow(WINDOW_NAME, output)
    if cv.waitKey(50) != -1:
        running = False
        break

cv.destroyAllWindows()