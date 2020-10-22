#include <Servo.h>
#include "TimerOne.h"
#include <HX711.h>

#define DOUT  4
#define CLK  3

HX711 scale;

Servo myservo;  // create servo object to control a servo

float setWeight = 1000.0;
int iteration_period = 5000;
int timelast = 0;
int servoangle = 100;
float Kp = 0.05;
float c = 90.0;
//float servosetting = 0;

float calibration_factor = 230;

int pos = 0;    // variable to store the servo position
int buttonPin = 7;
int buttonState = 0;
int LED = 13;


unsigned int counter=0; //Sets Counter to 0

unsigned int rotation = 0;


void docount()  //counts from the speed sensor
{
  counter++;  // increase +1 the counter value
} 

void timerIsr()
{
  Timer1.detachInterrupt();  //stop the timer
  //Serial.print("Motor Speed: "); 
  rotation = (counter * 5.78125);  
  //Serial.print(rotation, DEC);  
  //Serial.print(" Rotation per Minute"); 
  //Serial.print("\t");
  //Serial.println();
  counter=0;  //  reset counter to zero
  Timer1.attachInterrupt( timerIsr );  //enable the timer
}

void setup() {
  Serial.begin(115200);

  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(buttonPin, INPUT);

  pinMode(LED, OUTPUT);

  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt((0), docount, RISING);  // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr ); // enable the timer  
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  //Serial.print("Weight (g): ");
  //Serial.print(scale.get_units(), 1);
  //Serial.print("\t");
  //Serial.println();

//delay(1000);



int  now = micros();

  if ((now-timelast)>=iteration_period){
    //Serial.println(now-timelast);
    timelast = now;

 float  servosetting = (setWeight-scale.get_units())*Kp+c; //Feedback equation
   servosetting = constrain(servosetting, 90, 100); //constrain the servo 
   myservo.write(servosetting); //apply the braking

    //Serial.print(servosetting);
    //Serial.print(" ");
    Serial.print(millis());
    Serial.print(",");
    Serial.println(scale.get_units());

  }




}
