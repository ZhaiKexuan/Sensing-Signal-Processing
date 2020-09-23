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

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture3.png"/></div>  

- Calibration  
*The technical approach for calibration is shown below:*
  - For all four ultrasonic sensors, recorded data of filtered measurements for obstacles between 20 and 150 cm away with 10 cm increments
  - The data was averaged at each measurement distance.
  - Graphed these averages to their corresponding actual distance in mm.
  - A and b were Used to convert Kalman filter output from microseconds to distance in mm.  
*Sensor data was collected separately for calibration. The data collected from sensor 1 is shown in table 1.*  

*(Table)*

*The signal processing result for ultrasonic sensor 1 is shown in figure 4:*  

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture4.png"/></div>  

---

## 1.3	Hardware and Software Implementation

- Hardware Implementation:
  - LED was connected to Arduino pin 4 through a 1k resistor.
  - The buzzer was connected to Arduino pin 5.
  - LED and Buzzer shared the same ground.
  - Ultrasonic sensor:
    - Trig was connected to Arduino pin 13.
    - The echo was connected to Arduino pin 12.
    - The power of the ultrasonic sensor was 5V.
    - Utilizing different ground was other than sharing the same ground with LED and Buzzer.

- Software Implementation:
  - The hardware connections were defined
  - Kalman variables and covariances were initialized
  - Calibration coefficients were declared
  - Measurement variables were declared
  - Sound waves from the ultrasonic sensor were sent and received
  - High pulse duration was Read
  - Kalman filter prediction stage
  - Kalman filter correction stage
  - Convert Kalman filter output into total time value in microseconds
  - Use calibration coefficients to calculate the distance in mm
  - Once Kalman covariance under 0.0001, turn on LED and sound buzzer
  - Print results in the following format: distance (mm), variance

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture5.png"/></div>  

---

## 1.4 Experimental Results

The experimental result for signal processing for using one ultrasonic sensor to measure an obstacle 200 mm away is shown in figure 6. The error in measurement is 2.54%, the variance in data is 0.295864 and time for Kalman Variance to decrease bellow 0.0001 is 3.31 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture6.png"/></div>  

The experimental result for signal processing for using one ultrasonic sensor to measure an obstacle 850 mm away is shown in figure 7. The error in measurement is 0.83%, the variance in data is 0.321486 and time for Kalman Variance to decrease bellow 0.0001 is 2.77 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture7.png"/></div>  

The experimental result for signal processing for using one ultrasonic sensor to measure an obstacle 1500 mm away is shown in figure 8. The error in measurement is 0.45%, the variance in data is 0.902735 and time for Kalman Variance to decrease bellow 0.0001 is 2.93 s.  

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture8.png"/></div>  

The experimental results are organized in table 2:

*table*
---

## Sensor Fusion of Two Ultrasonic Sensors

---

## 2.1 Problem Statement

The second task is to use two ultrasonic sensors to accurately measure a range of distances between 20 and 150 cm

---

## 2.2	Technical Approach

- Fusion with Kalman filter  
The technical approach for sensor fusion is shown below:
  - The technical approach for sensor fusion is shown below: in less error and variance than using only one sensor
  - Set the same initialized updated error covariance to 100000.
  - Prediction stage using the first sensor output as the prediction
  - Correction stage 1 updated state estimate using the first sensor output
  - Correction stage 2 updated state estimate using the second sensor output and state estimate from correction stage 1


- Calibration  
The same calibration coefficients were obtained as discovered in part 1. However, Calibration was performed before Kalman filter implementation rather than after. The technical approach for calibration is shown below:
  - Used calibration coefficients determined in part 1.
  - Converted sensor output to the distance in mm using calibration coefficients before Kalman filter implementation, rather than after as in part 1.

---

## 2.3	Hardware and Software Implementation

- Hardware Implementation:  
The hardware connection follows these steps:
  - Ultrasonic sensor 1:
    - Trig connected to Arduino pin 13
    - Echo connected to Arduino pin 12
    - The power is 5V 
    - Different ground than LED and buzzer
  - Ultrasonic sensor 2:
    - Trig connected to Arduino pin 11
    - Echo connected to Arduino pin 10
    - Shared same power as sensor 1
    - Shared same ground as sensor 1

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture9.jpg"/></div>  

- Software Implementation:  
The programming process follows these steps (Same process as part 1, except for the following changes):
  - Needed to define hardware connections for sensor 2
  - Needed to initialize Kalman variables and covariances for two Kalman correction stages rather than one and PID throttle values for 300 microseconds, then set vehicle to neutral steering and throttle for 100 microseconds, then set vehicle to neutral steering and maximum reverse for 75 microseconds, and then set vehicle to neutral steering and throttle for 50 microseconds
  - Needed to declare the calibration coefficients for sensor 2
  - Needed to initialize measurement variables for two sensors rather than one
  - Needed to send and receive sound waves from two sensors rather than one
  - Needed to use two Kalman filter correction stages rather than one
  - Conducted calibration before Kalman filter implementation, rather than afterward as in part 1

---

## 2.4 Experimental Results

The experimental result for Sensor Fusion of Two Ultrasonic Sensors to measure an obstacle 200 mm away is shown in figure 11. The error in measurement is 0.97%, the variance in data is 1.878630 and time for Kalman Variance to decrease bellow 0.0001 is 3.35 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture10.png"/></div>  

The experimental result for Sensor Fusion of Two Ultrasonic Sensors to measure an obstacle 850 mm away is shown in figure 12. The error in measurement is 0.31%, the variance in data is 0.215971 and time for Kalman Variance to decrease bellow 0.0001 is 3.47 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture11.png"/></div>  

The experimental result for Sensor Fusion of Two Ultrasonic Sensors to measure an obstacle 1500 mm away is shown in figure 13. The error in measurement is 0.83%, the variance in data is 0.091745 and time for Kalman Variance to decrease bellow 0.0001 is 3.79 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture12.png"/></div>  

The experimental results are organized in table 3.

---

## Ultrasound-based Localization

---

## 3.1 Problem Statement

The problem statement for task 3 is to use two ultrasonic sensors to accurately measure the x and y coordinates (localization) of a cylindrical object for a range of 3 cm to 15 cm in the x-direction and 40 cm to 80 cm in the y-direction.

---

## 3.2	Technical Approach

- Sampling
The technical approach for sampling is shown below:
  - Send and receive sound waves from the first ultrasonic sensor
  - Read high pulse durations of the first sensor
  - Divide high pulse duration of sensor 1 by 200 to convert into the total time value of microseconds
  - Send a delay of 100 microseconds to avoid interference between the two sensor signals
  - Send and receive sound waves from the second ultrasonic sensor
  - Read high pulse durations of the second sensor
  - Divide high pulse duration of sensor 2 by 200 to convert into the total time value of microseconds
  - No delay at end of loop resulted in the fastest possible sampling rate

- Four separate Kalman filters (two before localization, two after localization)  
The technical approach for Kalman filter includes the following steps:
  - Filter 1 (before localization)
    - Prediction phase for sensor 1 distance
    - Correction phase for sensor 1 distance
  - Filter 2 (before localization)
    - Prediction phase for sensor 2 distance
    - Correction phase for sensor 2 distance
  - Filter 3 (before localization)
    - Prediction phase for sensor 3 distance
    - Correction phase for sensor 3 distance
  - Filter 4 (before localization)
    - Prediction phase for sensor 4 distance
    - Correction phase for sensor 4 distance

- Calibration
The technical approach for calibration includes the following steps:
  - Used calibration coefficients determined in part 1
  - Converted sensor output to distance in mm using calibration coefficients before Kalman filter implementation, rather than after as in part 1
  - When converting sensor output to distance in mm, added the radius of the cylindrical object in order to calculate the distance to the center of the object

- Localization  
The equations of localization is shown below:

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/equation1.png"/></div>  

Where:  
dxk2 is Kalman filter output of sensor 2 distance,  
L is the distance between the two sensors,  
xx is the initial calculation of x,  
y: calculation of y,  
x: recalculation of x that results in less error.

---

## 3.3	Hardware and Software Implementation

- Hardware Implementation:  
The steps of hardware connection are shown below:
  - Hardware connections
    - LED and Buzzer same as parts 1 and 2
    - Ultrasonic sensors same as part 2

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture13.jpg"/></div>  

- Software Implementation:  
The steps of coding are shown below:
  - Initialize Kalman variables and covariances for filters before and after localization
  - Declare L (distance between the two sensors on the breadboard)
  - Initialize localization variables
  - Send and receive sound waves from ultrasonic sensor 1, read high pulse duration
  - Convert high pulse duration of sensor 1 into total time value in microseconds
  - Add radius of object to calibration equation to calculate distance in mm
  - Send delay of 100 microseconds to avoid interferences between the two sensor outputs
  - Same process for sensor 2 as sensor 1
  - Kalman filters before localization for sensors 1 and 2 distances
  - Once Kalman covariances of both x and y go under 0.0001, LED turned on and buzzer sounded
  - Print results as: x (mm), y(mm), variance of x, variance of y

---

## 3.4 Experimental Results

The experimental result for ultrasonic-based localization to measure an obstacle at x = 200 mm and y=400 mm away is shown in figure 18. The error in measurement of x is 3.70%, y is 3.36%. The variance in x data is 12.31286, in y data is 0.03528 and time for Kalman Variance to decrease bellow 0.0001 is 10.75 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture14.png"/></div>  

The experimental result for ultrasonic-based localization to measure an obstacle at x = 90 mm and y = 600 mm away is shown in figure 19. The error in measurement of x is 3.32%, y is 0.48%. The variance in x data is 25.15365, in y data is 0.343186 and time for Kalman Variance to decrease bellow 0.0001 is 10.87 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture15.png"/></div>  

The experimental result for ultrasonic-based localization to measure an obstacle at x = 15 mm and y = 80 mm away is shown in figure 20. The error in measurement of x is 5.37%, y is 0.97%. The variance in x data is 35.63241, in y data is 0.758432 and time for Kalman Variance to decrease bellow 0.0001 is 11.38 s.

<div align=center><img src="https://github.com/ZhaiKexuan/Sensing-Signal-Processing/blob/master/images/Picture16.png"/></div>  

The experimental results are organized in table 4.

---

## Conclusions and Discussions

## 4.1 Conclusions (a summary of the results of different approaches)

- Using a Kalman filter for sensor fusion with two sensors in part 2 resulted in overall less error and variance in the measurement data, and shorter time for the LED and buzzer to turn on than only using one sensor in part 1
- It was difficult to accurately measure the x-coordinate of the cylindrical object in part 3, with the error and variance of the x-coordinate data being relatively much higher than the y-coordinate

## 4.2 Discussions (a comparison of different approaches, and potential future work to further improve each approach)

- •	There were two differences in process between parts 1 and 2: when calibration occurred, and how many sensors were used. We could not exactly determine which of these differences contributed to part 2 having less error and variance. To be able to better understand the effect of calibration occurring either before or after filtering, the team would run tests with calibration occurring before and after filtering using only one sensor.
- •	In order to more accurately measure the x-coordinate in part 3, the team would use a larger breadboard to increase the distance between the two sensors, in order to reduce the possibility of interference between the two sensors’ signals.











