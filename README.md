# Bioreactor Project

## Project Overview

The project involved designing and programming the circuits and control systems necessary to regulate the internal stirring speed, pH and temperature of a liquid in a closed system. This is a "Bioreactor" in the context of regulating the conditions of a yeast solution to make a Tuberculosis vaccine. Moreover, in the context of producing and distributing TB vaccines, I considered the non-technical factors surrounding the project and informing the design of our system in a follow-up individual report assignment (e.g. distribution to rural areas, shipping, risks from local extreme weather conditions or political events, etc).

In this Bioreactor project, we did not have access to physical components due to the COVID-19 pandemic. The project involved three subsystems: the stirring subsystem, pH subsystem and heating subsystem. I was the Stirring Subsystem leader and thus designed the full stirring subsystem. I succeeded in producing the desired Stirring Subsystem that can regulate a stirring motor to spin at a setpoint within 500-1500RPM to an accuracy of +-20RPM. The Stirring Subsystem uses an Arduino connected to a circuit with a motor. The Arduino controls a transistor with PWM to drive a high, controlled current through a motor attached to the stirrer. The Arduino then senses the applied current and uses closed-loop feedback PID control to alter the circuit current (through PWM) and hence converge at the stirrer setpoint RPM.

I simulated the circuitry in Multisim, wrote the Arduino code in the Arduino IDE and compiled the Arduino code in SimAVR to simulate and troubleshoot it.

## Group

I acted with initiative to coordinate the team and organise the project deadlines, both enforced submission deadlines and our team deadlines to create the subsystems, test them individually, integrate them and test the integration. This included systems engineering principles to identify the system requirements and KPIs, divide the project in subsystems, and coordinate the subsystem and integrated system construction.

This project was in a team of 4.

## Team
- Iason Chaimalas (EEE)
- Arnas Vysniauskas (EEE)
- Suraj Kothari (CS)
- Arslan Aftab (CS)
