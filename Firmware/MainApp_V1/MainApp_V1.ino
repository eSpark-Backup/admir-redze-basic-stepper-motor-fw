#include "my_library.h"
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;
static void Timer1_setup(void);

static void Timer1_setup(void)
{
   TCCR1A = 0; //CTC mode 
   TCCR1B = B00001100; //prescaler=256,CTC mode
   TIMSK1 |= B00000010; //Interrupt enable for OCR1A compare match
   OCR1A = intValue; //OC match value for 100ms time base generation
   // Declare pins as Outputs
	  pinMode(stepPin, OUTPUT);
	  pinMode(dirPin, OUTPUT);
    digitalWrite(dirPin, HIGH);
}

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  Serial.println("Please enter RPM: "); // Prompt the user
  while (!Serial.available()) {
    // Wait for user input
  }
  userInput = Serial.parseInt(); // Read the integer value from Serial Monitor
  calculation();
  Timer1_setup();
}

void loop() {

}

ISR(TIMER1_COMPA_vect)
{
		digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin, LOW);
}