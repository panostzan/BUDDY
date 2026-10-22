# BUDDY

**A personal robotics project built from scratch.**

At the start of the school year, I sat down and thought about which of my stats I wanted to upgrade, almost like a character in a video game.

I felt pretty confident in my soft skills. I'd worked as a project manager and a salesman, and had learned a lot about motivating people, communicating ideas, and pitching. But when I compared myself to some of the people who inspired me, I realized my technical skills were lacking.

So I decided to build Buddy.

I've always believed that projects are one of the best ways to learn. You're free to experiment, make mistakes, and follow your curiosity without being slowed down by the bureaucracy of school and traditional testing. Plus, I just love building things.

Currently, Buddy is a two-wheeled, obstacle-avoiding rover. But that's just the beginning. My long-term goal is to develop him into a capable autonomous home robot that can navigate rooms, interact with people, and perform useful tasks.

This repository documents that process: the code, hardware, experiments, mistakes, and everything I learn along the way.

## Current Capabilities

- **Differential-drive locomotion:** Independently controlled DC motors.
- **Encoder feedback:** Measuring wheel rotation and tracking movement.
- **Motor control:** PWM speed control and proportional feedback.
- **Movement sequences:** Driving specified distances and executing turns using encoder feedback.
- **Obstacle avoidance:** Detecting obstacles with a time-of-flight distance sensor and responding autonomously.
- **Untethered operation:** Running from an onboard power source.

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno R3 | Main microcontroller |
| 12V DC gearmotors with encoders | Locomotion and rotational feedback |
| L298N H bridge | Motor speed and direction control |
| VL53L1X time-of-flight sensor | Obstacle detection |
| Rechargeable battery (CITYORK 12V) | Onboard power |
| Custom prototype chassis (my cutting board) | Mechanical structure |

The hardware will continue to change and be upgraded as Buddy evolves.

## Development

Buddy is an ongoing project. I'm documenting both successful experiments and the problems I encounter along the way.

## Engineering Notes

A major reason for building Buddy is to become a more independent engineer.

Some of the subjects I've explored so far include:

- Embedded C++ and Arduino programming
- Finite-state machines and non-blocking control
- Pulse-width modulation (PWM) and motor drivers
- Interrupt service routines and rotary encoders
- Proportional–integral–derivative (PID) control
- Differential-drive kinematics and odometry
- Hardware debugging and system integration

## What's Next?

My original vision was for Buddy to become a self-balancing robot using PID control, which I spent some time learning about and found fascinating. Unfortunately, the motors Buddy currently runs on have a tendency to stall, making self-balancing difficult with the current hardware.

But Buddy continues to evolve, and that's one of the most fun parts of this project. I'm really only limited by my creativity (and my student budget, ffs).

Some things I'd love to add are more personality and emotion, potentially through an LCD screen or a speaker so Buddy can talk. I'd also love to give him a moving head and neck so he can bop along to music.

Eventually, I want to explore simultaneous localization and mapping (SLAM), allowing Buddy to understand his surroundings and navigate more independently.

---

*Built out of curiosity, one experiment at a time.*

**[panostzan.com](https://panostzan.com)**
