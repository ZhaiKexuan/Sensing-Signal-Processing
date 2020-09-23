#define trigPin1 13
#define echoPin1 12
#define trigPin2 11
#define echoPin2 10

int LED = 4; //connect LED to pin 4
int beep = 5; //connect buzzer to pin 5

//calibration coefficients
float a1 = 35.486; //determined by offline calibration
float b1 = -14.486; //determined by offline calibration
float a2 = 35.871; //determined by offline calibration
float b2 = -10.939; //determined by offline calibration

//initialize kalman variables and covariances
float dxk=0; //predicted state estimate of distance 1 before localization
float dxk1=0; //initialized updated state estimate of distance 1 before localization
float dyk=0; //predicted state estimate of distance 2 before localization
float dyk1=0; //initialized updated state estimate of distance 2 before localization
float dP_1 = 0; //initialized updated error covariance distance 1 before localization
float dP1_1 = 100000; //predicted error variance of distance 1 before localization
float dQ_1 = 0.00; //initialized input confidence of distance 1 before localization
float dR_1 = 0.01; //measurement confidence of distance 1 before localization (calculated offline)
float dP_2 = 0; //initialized updated state estimate of distance 2 before localization
float dP1_2 = 100000; //predicted error covariance of distance 2 before localization
float dQ_2 = 0.00; //initialized input confidence of distance 2 before localization
float dR_2 = 0.01; //measurement confidence of distance 2 before localization (calculated offline)
float dK_1 = 0; //kalman gain for distance 1 before localization
float dK_2 = 0; //kalman gain for distance 2 before localization

float xk=0; //predicted state estimate of x after localization
float xk1=0; //initialized updated state estimate of x after localization
float yk=0; //predicted state estimate of y after localization
float yk1=0; //initialized updated state estimate of y after localization
float P_1 = 0; //initialized updated error covariance x after localization
float P1_1 = 100000; //predicted error variance of x after localization
float Q_1 = 0.00; //initialized input confidence of x after localization
float R_1 = 0.01; //measurement confidence of x after localization (calculated offline)
float P_2 = 0; //initialized updated state estimate of y after localization
float P1_2 = 100000; //predicted error covariance of y after localization
float Q_2 = 0.00; //initialized input confidence of y after localization
float R_2 = 0.01; //measurement confidence of y after localization (calculated offline)
float K_1 = 0; //kalman gain for x after localization
float K_2 = 0; //kalman gain for y after localization


float L = 100; //length between the two ultrasonic sensors

//initialize localization variables
float x = 0;
float y = 0;
float xx = 0;

void setup() {
  
  Serial.begin (9600); //serial communications at 9600 bps
  pinMode(trigPin1, OUTPUT); //set trigPin1 as output
  pinMode(echoPin1, INPUT); //set echoPin1 as input 
  pinMode(trigPin2, OUTPUT); //set trigPin2 as output
  pinMode(echoPin2, INPUT); //set echoPin2 as input
  pinMode(LED, OUTPUT); //pin 4 is output
  pinMode(beep, OUTPUT); //pin 5 is output

}

void loop() {

  float duration1, distance1, duration2, distance2; //initialize measurement variables
  float t1, t2; //initialize measured time variables

  //send and receive sound waves from ultrasonic sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH); //read high pulse duration of sensor 1
  
  t1 = duration1 / 200; //convert duration of sensor 1 into total time value in microseconds
  distance1 = ((a1 * t1) + b1 + 36); //use calibration coefficients of sensor 1 and add radius of object to calculate distance in mm

  delay(100); //delay to avoid interference between the two sensor signals

  //send and receive sound waves from ultrasonic sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH); //read high pulse duration of sensor 2
  
  t2 = duration2 / 200; //convert duration of sensor 2 into total time value in microseconds
  distance2 = ((a2 * t2) + b2 + 36); //use calibration coefficients of sensor 2 and add radius of object to calculate distance in mm

  //Kalman filter implemented before localization
  //Prediction phase for distance 1
  
    dxk = dxk1; //predicted state estimate of distance 1
    dP_1 = dP1_1 + dQ_1; //predicted state covariance of distance 1
    
  //Correction phase for distance 1
  
    dK_1 = dP_1 / (dP_1 + dR_1); //kalman gain for distance 1
    dxk1 = dxk + dK_1 * (distance1 - dxk); //updated state estimate of distance 1 (use for localization)
    dP1_1 = (1 - dK_1) * dP_1; //updated state covariance of distance 1

  //Prediction phase for distance 2
  
    dyk = dyk1; //predicted state estimate of distance 2
    dP_2 = dP1_2 + dQ_2; //predicted state covariance of distance 2
    
  //Correction phase for distance 2
  
    dK_2 = dP_2 / (dP_2 + dR_2); //kalman gain for distance 2
    dyk1 = dyk + dK_2 * (distance2 - dyk); //updated state estimate of distance 2 (use for localization)
    dP1_2 = (1 - dK_2) * dP_2; //updated state covariance of distance 2
 
 
  //localization
  
  distance1=(abs(dxk1)); //use kalman filter output of distance 1
  distance2=(abs(dyk1)); //use kalman filter output of distnace 2
  xx = (sq(distance1) - sq(distance2) + sq(L)) / (2*L); //calculation of x
  y = sqrt((sq(distance1) - sq(xx))); //calculation of y
  x = sqrt((sq(distance1) - sq(y))); //rewrite x so that it results in less error

  //Kalman filter implemented after localization
  //Prediction phase for x
  
    xk = xk1; //predicted state estimate of x
    P_1 = P1_1 + Q_1; //predicted state covariance of x
  
  //Correction phase for x
  
    K_1 = P_1 / (P_1 + R_1); //kalman gain for x
    xk1 = xk + K_1 * (x - xk); //updated state estimate of x (use for output)
    P1_1 = (1 - K_1) * P_1; //updated state covariance of x (use for output)

  //Prediction phase for y
    
    yk = yk1; //predicted state estimate of y
    P_2 = P1_2 + Q_2; //predicted state covariance of y
  
  //Correction phase for y
  
    K_2 = P_2 / (P_2 + R_2); //kalman gain for y
    yk1 = yk + K_2 * (y - yk); //updated state estimate of y (use for output)
    P1_2 = (1 - K_2) * P_2; //updated state covariance of y (use for output)


  //once the kalman covariances of both x and y are under .0001, then turn on the LED and sound the buzzer
  if (P1_1 < 0.0001 && P1_2 < 0.0001) {
    digitalWrite(LED, HIGH); //turn on LED
    digitalWrite(beep, HIGH); //turn on buzzer    
  }
    //print results in the following format: x, y, variance of x, variance of y
    Serial.print(xk1);
    Serial.print(", ");
    Serial.print(yk1);
    Serial.print(", ");
    Serial.print(P1_1,4);
    Serial.print(", ");
    Serial.println(P1_2,4);

}
