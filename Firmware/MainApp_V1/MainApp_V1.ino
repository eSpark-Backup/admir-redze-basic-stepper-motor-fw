#include "my_library.h"
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;
static void Timer1_setup(void);
//float userInput; // Variable to store the user input
//float timePerRev; //time Per Revolution
//float value_of_ocr1a; //value of OCR1A Register stored in this variable
//int intValue;

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

// void calculation ()
// {
//   timePerRev = 1/((userInput/60)*200);
//   value_of_ocr1a = ((timePerRev*16000000)/256);
//   intValue = int(value_of_ocr1a);
// }

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
  // put your main code here, to run repeatedly:
}

ISR(TIMER1_COMPA_vect)
{
		digitalWrite(stepPin, HIGH);
		digitalWrite(stepPin, LOW);
}