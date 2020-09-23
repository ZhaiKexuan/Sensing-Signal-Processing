#define trigPin 13
#define echoPin 12

int LED = 4; //connect LED to pin 4
int beep = 5; //connect buzzer to pin 5

//initialize Kalman variables and covariances
float yk = 0.0; //predicted state estimate
float yk1 = 0.0; //predicted state estimate at previous time step
float P = 0.0; //predicted error covariance
float K = 0.0; //Kalman gain
float Q = 0.0; //Input confidence (0 for this task)
float R = 0.01; //Measurement confidence (calculated offline)
float P1 = 100000; //initialized updated error covariance

// Calibration coefficients
float a = 35.486; //determined by the offline calibration
float b = -14.486; //determined by the offline calibration

void setup() {
  Serial.begin (9600); //serial communications at 9600 bps
  pinMode(trigPin, OUTPUT); //set trigPin as output
  pinMode(echoPin, INPUT); //set echoPin as input

  pinMode(LED, OUTPUT); //pin 4 is output
  pinMode(beep, OUTPUT); //pin 5 is output
}

void loop() {
  //Kalman filter prediction stage
  yk = yk1; //predicted state estimate
  P = P1 + Q; //predicted state covariance
  
  //declare measurement variables
  float duration, distance, t;

  //send and receive sound waves from ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); //read the high pulse duration

  //Kalman filter correction stage
  K = P / (P + R); //kalman gain
  yk1 = yk + K * (duration - yk); //updated state estimate
  P1 = (1 - K) * P; //updated state covariance
  
  t = yk1/200; //convert output into total time value in microseconds  
  distance = a*t + b; //use calibration coefficients to calculate distance in mm

  //once the kalman covariance is under .0001, then turn on the LED and sound the buzzer
  if (P1 < 0.0001){
    digitalWrite(LED, HIGH); //turn on LED
    digitalWrite(beep, HIGH); //turn on buzzer
  }

  //print results in the following format: distance, variance
  Serial.print(distance);
  Serial.print(", ");
  Serial.println(P1, 4);
  }
