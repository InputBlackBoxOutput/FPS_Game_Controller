@ECHO OFF
REM Batch file to install modules required by mouse.py and calibrator.py

ECHO Installing modules required by mouse.py and calibrator.py
ECHO.

pip install pyautogui
pip install opencv-python
pip install imutils

ECHO.
ECHO Done
PAUSE