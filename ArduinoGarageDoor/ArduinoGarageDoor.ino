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
const unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

unsigned long lastDepressTime = 0;


void setup() {
  // initialize the LED pin as an output:
  pinMode(internalLedPin, OUTPUT); // initialize the internalLED pin as an output:
  pinMode(relayPin, OUTPUT); // initialize the relay control pin as an output
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input
}

void loop() {
  // read the state of the pushbutton value:
  int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != buttonState) {
    // The buttonState has changed, long live the buttonState!
    buttonState = reading;

    // If we're pressed, then trigger relay and start counting
    if (buttonState == HIGH) {
      lastDepressTime = millis();
    } else {
      int duration = millis() - lastDepressTime;
      
      // If button was held down...
      if (lastDepressTime != 0 && duration > debounceDelay) { 
        // Turn on internal LED and wait 3x as long as the button was pressed
        digitalWrite(internalLedPin, HIGH);
        delay(duration * 3);
        digitalWrite(internalLedPin, LOW);
      }
      triggerRelay();
      lastDepressTime = 0;
    }
  }
}

void triggerRelay(){
  // Write HIGH level to the relay pin, 
  // turning the relay to the NO position,
  // where circuit is intact between NO and COM.
  digitalWrite(relayPin,HIGH);
  digitalWrite(internalLedPin, HIGH);
  
  // Waitabit
  delay(500);

  // Write LOW to the relay pin, turning the relay to the NC position, 
  // so the circuit is intact between NC and COM.   
  digitalWrite(relayPin,LOW);  
  digitalWrite(internalLedPin, LOW);
}
