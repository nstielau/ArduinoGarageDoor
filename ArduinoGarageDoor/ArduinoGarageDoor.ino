/*
ArdunioGarageDoor 

Adds a delayed-trigger for garage door.

Press and hold the button to delay door
action by three times the duration held.

MIT License

Nick Stielau
*/

// Constants (pin numbers, etc)
const int buttonPin = 6;        // pin for button reading
const int relayPin =  7;        // pin for relay control
const int internalLedPin =  13; // pin for timing LED
const int debugLedPin =  12;    // pin for 'debug' LED.  If the relay is triggerd this will stay HIGH.

const int debounceThreshold = 50;             // Minimum detectable press duration 
const int relayOnDuration = 500;              // duration to connect relay
const unsigned long pressHoldThreshold = 500; // minimum time to be considered a press-and-hold

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

unsigned long lastDepressTime = 0;

void setup() {
  pinMode(internalLedPin, OUTPUT); // initialize the internalLED pin as an output:
  pinMode(debugLedPin, OUTPUT); // initialize the debugLedPing pin as an output:
  pinMode(relayPin, OUTPUT); // initialize the relay control pin as an output
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input
}

void loop() {
  // Read the state of the pushbutton value
  int reading = digitalRead(buttonPin);
  
  // If the switch changed, due to noise or pressing:
  if (reading != buttonState) {
    // The buttonState has changed, long live the buttonState!
    buttonState = reading;

    if (buttonState == HIGH) {
      // If we're pressed, then trigger relay and start counting
      lastDepressTime = millis();
    } else {
      // If we're released, then delay and trigger relay
      int remainingDuration = millis() - lastDepressTime;
      
      // If button was held down...
      if (lastDepressTime != 0 && remainingDuration  > pressHoldThreshold) {
        // Wait 3x as long a button was pushed
        remainingDuration = remainingDuration * 3;

        // Flash light faster and faster...
        while (remainingDuration > 0) {
          // Flash LED for 1/10th of remaining delay
          digitalWrite(internalLedPin, HIGH);
          delay(remainingDuration * 0.1);
          digitalWrite(internalLedPin, LOW);
          delay(remainingDuration * 0.1);

          // reducing the remaining delay
          remainingDuration = remainingDuration - (remainingDuration * 0.1) - 10;
        }
      }

      if (millis() - lastDepressTime > debounceThreshold) {
        triggerRelay();
        lastDepressTime = 0;
      }
    }
  }
}

void triggerRelay(){
  // Write HIGH level to the debug pin, and don't turn off
  digitalWrite(debugLedPin, HIGH);

  // Write HIGH level to the relay pin, 
  // turning the relay to the NO position,
  // where circuit is intact between NO and COM.
  digitalWrite(relayPin, HIGH);
  digitalWrite(internalLedPin, HIGH);
  
  // Waitabit, long enough to trigger opener
  delay(relayOnDuration);

  // Write LOW to the relay pin, turning the relay to the NC position, 
  // so the circuit is intact between NC and COM.   
  digitalWrite(relayPin,LOW);  
  digitalWrite(internalLedPin, LOW);
}
