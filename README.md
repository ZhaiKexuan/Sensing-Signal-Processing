# Ultrasonic Sensing Signal Processing

*Author: Kexuan Zhai*

## Signal Processing for One Ultrasonic Sensor
---

## 1.1 Problem Statement

The problem for this task is to use one ultrasonic sensor to accurately measure a range of distances between 20 and 150 cm.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture1.png"/></div>

---

## 1.2	Technical Approach

- Sensor Placement

- Sampling
  - The first step is sending and receiving sound waves from the ultrasonic sensor. To implement that, the Arduino is utilized for programming
  - The second step is reading high pulse duration
  - The third step is dividing Kalman filter output by 200 to convert into total time value in microseconds after Kalman filter implementation
  - By changing delay multiple times, the experimental results show that no delay at the end of the loop resulted in the fastest possible sampling rate

- Kalman filter  
*The R-value (measurement confidence) is determined offline utilizing the average method. The sequence of the method is shown below:*
  - The sensor reading at 20 cm and 150 cm were taken
  - The variance in data was calculated
  - The variances were averaged to get the final R-value  
*The initialized updated error covariance was set to 100000. Because the first prediction is untrustable. The value of 100000 resulted in the best output from the Kalman filter by operating several tests.*

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture2.png"/></div>  

- Calibration  
*The technical approach for calibration is shown below:*
  - For all four ultrasonic sensors, recorded data of filtered measurements for obstacles between 20 and 150 cm away with 10 cm increments
  - The data was averaged at each measurement distance.
  - Graphed these averages to their corresponding actual distance in mm.
  - A and b were Used to convert Kalman filter output from microseconds to distance in mm.  
*Sensor data was collected separately for calibration. The data collected from sensor 1 is shown in table 1.*

---

## 1.3	Hardware and Software Implementation

- Hardware Implementation:
  - The vehicle battery pack used to power servo motor for throttle
  - Portable power bank used to power Arduino
  - Steering control powered by Arduino Vin pin
  - Ultrasonic sensor 1:
    - Trig to Arduino pin 5
    - Echo to Arduino pin 4
    - Ground shared with all other ultrasonic sensors
    - 5V power shared between all other ultrasonic sensors
  - Ultrasonic sensor 2:
    - Trig to Arduino pin 3
    - Echo to Arduino pin 2

<div align=center><img src="https://github.com/ZhaiKexuan/Adaptive-Cruise-Control-and-Autonomous-Lane-Keeping/blob/master/imgaes/Picture2.jpg"/></div>


- Software Implementation
  - Constrain throttle value to a maximum reverse of 1440 and a maximum forward throttle of 1535
  - If statement to tell the car to shift to neutral when changing between forward and reverse and vice versa
  - If the car is moving forward, is moving faster than 50 mm per loop iteration, and an obstacle is less than 1500 mm away from the car - set vehicle to neutral steering and throttle for 50 microseconds, then set vehicle to neutral steering and maximum reverse for 1 second, and then set vehicle to neutral steering and throttle for 50 microseconds
  - Else statement for a close condition: set vehicle to PID steering value and PID throttle value for 100 microseconds, and then set vehicle to neutral steering and throttle for 50 microseconds

---

## 1.4 Experimental Results

The experimental result at the start line is shown in the figure below:

<div align=center><img src="https://github.com/ZhaiKexuan/Adaptive-Cruise-Control-and-Autonomous-Lane-Keeping/blob/master/imgaes/Picture3.png"/></div>


The vehicle can stop properly. This is the picture at the finish line: 

<div align=center><img src="https://github.com/ZhaiKexuan/Adaptive-Cruise-Control-and-Autonomous-Lane-Keeping/blob/master/imgaes/Picture4.png"/></div>


---

## Autonomous Lane Keeping

---

## 2.1 Problem Statement

To not hit any boundaries on a predefined path and pass five-lane checkpoints

<div align=center><img src="https://github.com/ZhaiKexuan/Adaptive-Cruise-Control-and-Autonomous-Lane-Keeping/blob/master/imgaes/Picture5.png"/></div>

---


## 2.2	Technical Approach

- Sensor Placement
  - The team decided to use two sensors with Kalman filter sensor fusion because that set-up resulted in less error and variance than using only one sensor
  - The team decided to place the two sensors on the left side of the vehicle because the team felt that the wall on the left side of the path would provide more accurate measurements than the boxes on the right side of the path

- Sampling
  - Send and receive sound waves from two ultrasonic sensors
  - Read high pulse durations of both sensors
  - Before Kalman filter fusion implementation, divide sensor outputs by 200 to convert into total time value in microseconds

- Filtteing
  - Used same R-value for both sensors as calculated in part 2 of project 1
  - Set same initialized updated error covariance to 100000
  - rediction stage using the first sensor output as the prediction
  - Correction stage 3 updated state estimate using the third sensor output
  - Correction stage 4 updated state estimate using fourth sensor output and state estimate from correction stage 3

- PID Controller
  - Set reference distance of 305 so that the vehicle would stay between the five-lane checkpoints along the path
  - Used PID error (the difference between reference distance and sensor output) along with PID gains to calculate PID value (throttle)
  - PID value was used to tell the vehicle whether to reverse, stay neutral, or go forward
  - Tuned PID gains to optimize performance of the vehicle (kp = 1, ki = 0, kd = 0.7)

---

## 2.3	Hardware and Software Implementation

- Hardware Implementation:
  - The vehicle battery pack used to power servo motor for throttle
  - Portable power bank used to power Arduino
  - Steering control powered by Arduino Vin pin
  - Ultrasonic sensor 3:
    - Trig to Arduino pin 9
    - Echo to Arduino pin 8
  - Ultrasonic sensor 4:
    - Trig to Arduino pin 7
    - Echo to Arduino pin 6

- Programming:
  - Constrain steering values to a maximum of 80 for left and 100 for right
  - If the car is moving forward and the obstacle is greater than 1500 mm away, set vehicle to PID steering and PID throttle values for 300 microseconds, then set vehicle to neutral steering and throttle for 100 microseconds, then set vehicle to neutral steering and maximum reverse for 75 microseconds, and then set vehicle to neutral steering and throttle for 50 microseconds
  - Same procedure as above if the car is stationary and the obstacle is greater than 1500 mm away

---

## 2.4 Experimental Results

The figure shows that the vehicle was running in the lane:

<div align=center><img src="https://github.com/ZhaiKexuan/Adaptive-Cruise-Control-and-Autonomous-Lane-Keeping/blob/master/imgaes/Picture6.png"/></div>

---

## Conclusions and Discussions

---
## 3.1 Conclusions (a summary of the results of different approaches)

- The ACC of the vehicle was implemented successfully, with the front of the vehicle consistently stopping at exactly 30 cm from obstacles in front
- The LKA was implemented with slightly less success. The team occasionally had problems with tires on one side of the vehicle driving on top of the lanes rather than keeping completely inside the lanes
- The vehicle seemed to work the best under cooler weather conditions while being more unpredictable as the temperature warmed up

## 3.2 3.2	Discussions (a comparison of different approaches, and potential future work to further improve each approach)

- For future work, the team would construct a path themselves on the same ramp that the formal test was to be conducted on so that the team would have more opportunities to test their vehicle
- Also, the team would keep one sensor on each side of the vehicle and use the difference of distance between these two sensors to implement LKA to identify differences between approaches
- Also, the team would keep two sensors on the same side of the vehicle and use the first sensor to measure the distance and use the other sensor measurement as a comparison to make sure the vehicle and lane are parallel.
- Finally, the team would spend more time testing their vehicle in weather conditions similar to the predicted weather for the formal test











