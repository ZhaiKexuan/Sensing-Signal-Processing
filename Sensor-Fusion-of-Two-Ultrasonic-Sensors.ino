#define trigPin1 13
#define echoPin1 12
#define trigPin2 11
#define echoPin2 10

int LED = 4; //connect LED to pin 4
int beep = 5; //connect buzzer to pin 5

//initialize measured time variables
float t1;
float t2;

//initialize kalman variables and covariances
float yk = 0.0; //predicted state estimate
float yk1 = 0.0; //initialized updated state estimate for correction 1
float yk2 = 0.0; //initialized updated state estimate for correction 2
float R1 = 0.01; //measurement confidence for correction 1 (calculated offline)
float R2 = 0.01; //measurement confidence for correction 2 (calculated offline)
float P = 100000; //predicted error covariance
float P1 = 0.0; //initialized updated error covariance for correction 1
float P2 = 0.0; //initialized updated error covariance for correction 2
float K = 0.0; //kalman gain

//calibration coefficients
float a1 = 35.486; //determined by offline calibration
float b1 = -14.486; //determined by offline calibration
float a2 = 35.871; //determined by offline calibration
float b2 = -10.939; //determined by offline calibration


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

  //send and receive sound waves from ultrasonic sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH); //read high pulse duration of sensor 1
  
  t1 = duration1 / 200; //convert duration of sensor 1 into total time value in microseconds
  distance1 = a1 * t1 + b1; //use calibration coefficients of sensor 1 to calculate distance in mm

  //sensor fusion
  //Kalman filter prediction stage
  yk = yk2; //use the first distance as prediction
  P1 = P; //predicted state covariance

  //send and receive sound waves from ultrasonic sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH); //read high pulse duration of sensor 2

  t2 = duration2 / 200; //convert duration of sensor 2 into total time value in microseconds
  distance2 = a2 * t2 + b2; //use calibration coefficients of sensor 2 to calculate distance in mm
  
  //Kalman filter correction 1
  K = P1 / (P1 + R1); //kalman gain
  yk1 = yk + K * (distance1 - yk); //updated state estimate using first sensor
  P = (1 - K) * P1; //updated state covariance of first sensor

  //Kalman filter correction 2
  K = P / (P + R2); //kalman gain
  yk2 = yk1 + K * (distance2 - yk1); //updated state estimate using second sensor (distance to output)
  P2 = (1 - K) * P; //updated state covariance using second sensor (variance to output)

  //once the kalman covariance is under .0001, then turn on the LED and sound the buzzer
  if (P2 < 0.0001) {
    digitalWrite(LED, HIGH); //turn on LED
    digitalWrite(beep, HIGH); //turn on buzzer    
  }
  //print results in the following format: distance, variance
  Serial.print(yk2);
  Serial.print(", ");
  Serial.println(P2,4);
}
