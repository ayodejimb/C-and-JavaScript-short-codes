//The blocks of code written below was used to configure 4-Channel RF IC2272 module with ATMEGA328P microcontroller 

// declare variable bulbs pin
int pin1 = 5;
int pin2 = 6;
int pin3 = 7;
int pin4 = 8;

// declare relay pins
int relay1= 2; 
int relay2= 3;
int relay3= 4;
int relay4= 9;

// initialize values variable  as zero
int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;

void setup() {

   pinMode (pin1, INPUT);   // bulb variable is an input to the pin declared
   pinMode (pin2, INPUT);
   pinMode (pin3, INPUT);
   pinMode (pin4, INPUT) ;
   pinMode (relay1, OUTPUT); // relay is an output 
   pinMode (relay2, OUTPUT); 
   pinMode (relay3, OUTPUT); 
   pinMode (relay4, OUTPUT); 
   Serial.begin (9600);      // initialize serial monitor

  }


 void loop() { 

    int val1 = digitalRead (pin1);  // read the input bulb pin and store in a variable value
    int val2 = digitalRead (pin2);
    int val3 = digitalRead (pin3);
    int val4 = digitalRead (pin4);   
    int delayed = 200;
 
    if (val1 == HIGH) value1 ++; // increment value1 if condition is true
      boolean oddOrEven1 = value1 % 2; // find the remainder of value1

        if (oddOrEven1) {
           digitalWrite (relay1, HIGH); // if a remainder , turn on bulb
        } 
        
        else {
          digitalWrite (relay1, LOW); // otherwise, turn off bulb
        } 

    Serial.println (val1); // print the value read on serial monitor

    delay(delayed); 

    if (val2 == HIGH) value2 ++; 
      boolean oddOrEven2 = value2 % 2; 

        if (oddOrEven2) {
           digitalWrite (relay2, HIGH); 
        } 
        
        else {
          digitalWrite (relay2, LOW); 
        }

    Serial.println (val2);  

    delay(delayed);

    if (val3 == HIGH) value3 ++; 
      boolean oddOrEven3 = value3 % 2; 

        if (oddOrEven3) {
           digitalWrite (relay3, HIGH); 
        } 
        
        else {
          digitalWrite (relay3, LOW); 
        }

    Serial.println (val3);  

    delay(delayed);

    if (val4 == HIGH) value4 ++; 
      boolean oddOrEven4 = value4 % 2; 

        if (oddOrEven4) {
           digitalWrite (relay4, HIGH); 
        } 
        
        else {
          digitalWrite (relay4, LOW); 
        }

    Serial.println (val4); 

    delay(delayed);

 }
