//The blocks of code written below was used to configure Ultrasonic sensors with ATMEGA328P microcontroller as a bidirectional visitor counter 

#include<LiquidCrystal.h>     //Import LCD Library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// define ultrasonic pins and variables
const int trigPin = 9;
const int echoPin = 10;
const int trigPin1 = 7;
const int echoPin1 = 8;
int led  = 13;
int count = 0;
int count1 = 0;
long duration;
int distance;
long duration1;
int distance1;

void setup() {
// Set the LCD pins and display
lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print ("A Biderectional");  
lcd.setCursor(0,1);
lcd.print ("Visitor Counter"); 
delay(5000);
lcd.clear();  


pinMode(led, OUTPUT);    // Set the led pin
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(trigPin1, OUTPUT); 
pinMode(echoPin1, INPUT);

}

void loop() {

  if (count > 0) {
    digitalWrite(led, HIGH);
  }
  else {digitalWrite(led, LOW); 
  };

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;

//Repeats the same process for the second ultrasonic sensor
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distance1= duration1*0.034/2;

// Conditions for Increment and decrement 
if (distance >= 0 && distance <= 20)   {
    count ++ ; 
  }
else if (distance1 >= 0 && distance1 <= 20){
    count -- ; 
  }
else if ((distance >= 0 && distance <= 50) && (distance1 >= 0 && distance1 <= 50)) {
    count = count;
  }


if (count < 0) { 
  count = 0;
  }
else {

}; 

// Printing the count on the LCD
lcd.setCursor(0,0);
lcd.print ("Visitors = ");
lcd.print (count);

// Wait for 0.5 second
delay(500);
}
