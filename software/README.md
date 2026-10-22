# Software

This directory contains Python programs I wrote while learning robotics, computer vision, and communication between software and embedded hardware.

Unlike the firmware running directly on Buddy's Arduino, these programs run on my computer. They are primarily early experiments rather than software currently integrated into Buddy.

## Contents

- **`camerabasic.py`** — Basic OpenCV camera capture and pixel brightness detection.
- **`scratchpaper.py`** — Red-object tracking using OpenCV, with proportional control and serial communication to move an Arduino-controlled servo.
- **`hands.py`** — Hand tracking using MediaPipe, including finger landmark detection and distance calculations.
- **`handsrecall.py`** — A simple camera experiment for practising OpenCV and MediaPipe.
- **`servocontrol.py`** — An early experiment with sending servo angle commands from Python to Arduino.
- **`hand_landmarker.task`** — Pretrained MediaPipe hand landmark detection model.

These files are preserved as part of my learning process. Some are unfinished experiments, and I plan to continue building on this work as Buddy develops.
