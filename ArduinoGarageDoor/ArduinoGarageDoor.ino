/*
  Button

This report would have more information with
"Show verbose output during compilation"
option enabled in File -> Preferences.

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int relayPin =  7;      // the number of the LED pin
const int internalLedPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int pressCount = 0;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 2000;    // the debounce time; increase if the output flickers

void setup() {
  // initialize the LED pin as an output:
  pinMode(internalLedPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != buttonState) {
    // The buttonState has changed, long live the buttonState!
    buttonState = reading;
    
    // reset the debouncing timer
    lastDebounceTime = millis();
    
    // If we're pressed, than increment counter
    if (buttonState == HIGH) {
      pressCount = pressCount + 1;
      Serial.print(pressCount);
      Serial.println(" is the press count");
      if (pressCount == 1) {
        // First time it's pressed, we connect the relay to open the garage door
        Serial.println("Triggering Relay based on 1st press");
        triggerRelay();
      } 
    }
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // it's been more than `debounceDelay`, so we'll check presses

    // if the button has been pressed 3 times,
    // we want to wait a bit, and then close the door.
    // Wait 1 second for each press over 3.
    if (pressCount > 3) {
       for (int i=0; i <= pressCount-1; i++){
          // turn internal LED On
          Serial.print("Waiting ");
          Serial.print(i);
          Serial.print(" of ");
          Serial.print(pressCount);
          Serial.println(" seconds.");
          digitalWrite(internalLedPin, HIGH);
          delay(500);          
          digitalWrite(internalLedPin, LOW);
          delay(500);                    
       }

        // Connect relay to close door.  
        Serial.println("Triggering Relay based on presses");
        triggerRelay();
    }
    // reset pressCount
    pressCount = 0;    
  }  
}

void triggerRelay(){
  // Write HIGH level to the relay pin, 
  // turning the relay to the NO position,
  // where circuit is intact between NO and COM.
  digitalWrite(relayPin,HIGH);
  digitalWrite(internalLedPin, HIGH);
  
  Serial.println("Triggering Relay");
  
  // Waitabit
  delay(500);

  // Write LOW to the relay pin, turning the relay to the NC position, 
  // so the circuit is intact between NC and COM.   
  digitalWrite(relayPin,LOW);  
  digitalWrite(internalLedPin, LOW);
}
