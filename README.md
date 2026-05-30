# Autonomous Line-Following Robot (Pololu 3pi / ATmega328P)

🏆 **1st Place Winner** at the **Robo Intelligence 2026** competition (2D-trace category).  
⏱️ **Official Lap Time:** **13.88 seconds**  

![Robot Run Demo](assets/run_demo.gif)

---

## 🏁 Track Specifications & Performance

The robot was optimized for the official competition track layout shown below:

<p align="center">
  <img src="assets/track_layout.png" alt="Track Layout" width="500"/>
</p>


The track features a mix of high-speed straight lines, where the robot actively boosted speed up to **235 PWM**, and consecutive tight S-curves, where the dynamic braking algorithm successfully maintained stability by dropping speed down to **95 PWM**.

---

## 🔴 The Hardware Challenge: Brown-out Reset (BOR)
During testing, the Pololu 3pi robot suffered from electrical instability:
**Static Stability:** High. The robot could run at 100% motor power (255 PWM units) under linear acceleration.
**Dynamic Stability:** Low. Rapid changes in motor direction (sudden reverse) or instant acceleration caused the battery power rail voltage to drop below **2.7V**. This voltage drop triggered a **Brown-out Reset (BOR)**, causing the ATmega328P microcontroller to reboot mid-race.

---

## 🛠️ Software-Level Solutions

To keep the robot fast and prevent reboots, the code implements four simple but effective rules:

### 1. Smooth Speed Changes (Slew Rate Limit)
Instead of changing speed instantly, the robot accelerates and brakes gradually.
Motor speed can only increase by 5 units or decrease by 9–11 units per loop cycle (1-2 ms).  Prevents sudden battery drain that causes the processor to restart.

### 2. Limited Reverse Speed
Spinning wheels backward helps with sharp corners, but it consumes too much power.
We limited the reverse speed to `-40` (instead of the physical maximum of `-255`).  Reduces the heaviest electrical load on the batteries.

### 3. Slowing Down on Turns (Dynamic Braking)
The robot is under the most stress when cornering.
The robot automatically slows down when it detects a turn using a simple rule:  
  `New Speed = Base Speed - (Tracking Error / 9)`
Reduces motor strain and keeps the robot stable on sharp S-curves.

### 4. Smooth PID & Fast Calculations

We tuned the navigation system to predict turns early and react smoothly.
The algorithm anticipates corners to avoid sharp jerks. All calculations use fast bit-shifting (`>> 8`) instead of slow division, making the code run instantly on the simple 8-bit processor.
Eliminates sudden movements and keeps power consumption stable.

---

## Achievements & Official Credentials
* **1st Place Diploma:** [View Diploma](assets/diploma_robo_intel.jpg) — Official confirmation of securing the 1st place in the 2D-trace category.
* **ECTS Certificate:** [View Certificate](assets/certificate_robo_intel.jpg) — Confirms the academic workload of **0.5 ECTS credits** awarded for participation and victory.
