# Mouse movement using computer vision (CV)
# Originally written for tracking a green ball by Adrian Rosebrock
# Modified for mouse movement by Rutuparn Pawar <InputBlackBoxOutput>

# Built-in modules
from collections import deque
import argparse
import time
import math

# External modules: Run install_modules batch file or shell script
from imutils.video import VideoStream
import numpy as np
import cv2
import imutils
import pyautogui


# Setup mouse
POINTER_MOVE_DELAY = 0.2 #sec
resX, resY = pyautogui.size()
print(f"Screen size: {resX}x{resY}")
pyautogui.moveTo(resX/2, resY/2, duration=0.3) 

currentX = resX/2
currentY = resY/2

# Construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
	help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=32,
	help="max buffer size")
args = vars(ap.parse_args())

# Define the lower and upper boundaries of the "green" in the HSV color space
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)

# Initialize the list of tracked points, the frame counter, and the coordinate deltas
pts = deque(maxlen=args["buffer"])
counter = 0
(dX, dY) = (0, 0)
direction = ""

# Setup video source
if not args.get("video", False):
	vs = VideoStream(src=0).start()
else:
	vs = cv2.VideoCapture(args["video"])

time.sleep(1.0)

gotScale = False
prevCenter = (0, 0)

while True: 
	frame = vs.read()
	frame = frame[1] if args.get("video", False) else frame

	if frame is None:
		break
			
	# Resize the frame, blur it, and convert it to the HSV color space
	frame = imutils.resize(frame, width=600)
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

	# Get frame size and scale
	if gotScale == False:
		frameX = frame.shape[1]
		frameY = frame.shape[0]
		print(f"Frame size: {frameX}x{frameY}")

		scaleX = resX//frameX
		scaleY = resY//frameY

		gotScale = True
	
	# Construct a mask for the color 'green'
	mask = cv2.inRange(hsv, greenLower, greenUpper)
	mask = cv2.erode(mask, None, iterations=2)
	mask = cv2.dilate(mask, None, iterations=2)
	
	# Find contours in the mask and initialize the current (x, y) center of the ball
	cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	center = None
	
	# Find center using contours
	if len(cnts) > 0:
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
		
		if radius > 10:
			cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
			cv2.circle(frame, center, 5, (0, 0, 255), -1)
			# print(center)
			
			dX = prevCenter[0] - center[0]
			dY = prevCenter[1] - center[1]
			prevCenter = center
			
			currentX = currentX + (scaleX * dX)
			currentY = currentY + (scaleY * dY)

			threshold = 5
			try:
				if dX >= threshold or dY >= threshold:
					pyautogui.moveRel(scaleX * dX, -1*(scaleY * dY))

				if dX <= (-1 * threshold) or dY <= (-1 * threshold):
					pyautogui.moveRel(scaleX * dX, -1*(scaleY * dY)) 

			except:
				print("Mouse pointer out of control: Calibration required")

	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
	counter += 1
	
	if key == ord("c"):
		pyautogui.moveTo(resX/2, resY/2, duration=0.5)
	if key == ord("q"):
		break

		
if not args.get("video", False):
	vs.stop()
else:
	vs.release()

cv2.destroyAllWindows()