#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

/* MOTOR Configs*/
// Give the motor control pins names:
#define pwmA 3
#define pwmB 11
#define brakeA 9
#define brakeB 8
#define dirA 12
#define dirB 13
const int stepsPerRevolution = 200; // Define number of steps per revolution:
#define MotorInterfaceType 2  // Define the AccelStepper interface type:
static int revCount1 = 0; //open count
static int revCount2 = 0; //close count
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(MotorInterfaceType, dirA, dirB);
/* MOTOR Configs*/

/* LCD Config*/
LiquidCrystal_I2C lcd1(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd2(0x26, 16, 2); // I2C address 0x27, 16 column and 2 rows
int hours = 5;
int minutes = 33;
int seconds = 33;
String string1 = "Open : ";
String string2 = "Close: ";
String result1;
String result2;
String result3; //holds time data
/* LCD Config*/

/* Button Config*/
const int buttonPin = 2;
/* Button Config*/

int runapp = 0;

void lcd_setup()
{
  lcd1.init(); // initialize the lcd
  lcd2.init(); // initialize the lcd
  lcd1.backlight();
  lcd2.backlight();
  lcd1.clear();            
  lcd2.clear();      // clear display
  lcd1.setCursor(0, 0); 
  lcd2.setCursor(0, 0); 
}

void button_setup()
{
 pinMode(buttonPin, INPUT_PULLUP); 
}

void motor_setup()
{
   // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);

  // Set the maximum steps per second:
  stepper.setMaxSpeed(400);
}

void lcd_startup_display()
{
  lcd1.setCursor(0, 0);
  lcd1.print("Zum Starten");
  lcd1.setCursor(0, 1);
  lcd1.print("Knopf drucken");
  lcd2.setCursor(0, 0);
  lcd2.print("Abgelaufene Zeit");
}

void timer_setup()
{
  /* Setup Timer1 for 1sec*/
    TCCR1A = 0;
    TCCR1B = 0; 
    OCR1A = 0xF424;
    TCCR1B = (1<<WGM12) | (1<<CS12); 
    TIMSK1 = (1<<OCIE1A); 
}


void setup() 
{
  lcd_setup();
  button_setup();
  motor_setup();
  Serial.begin(9600);
  lcd_startup_display();
  sei(); /*enable interrupts*/
}

void run_motorapp();
void run_stopmotorapp();
void run_idle_task();


void loop() {
 
  if(digitalRead(buttonPin) == LOW)
  {
    lcd1.clear();
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("Abgelaufene Zeit:"); //timeLapsed displayed
    
    /*Display Open: 0 / Count: 0 when button pressed */
    lcd1.setCursor(0, 0);         // move cursor to   (2, 1)
    result1 = string1 + (String)revCount1; // concate both string
    lcd1.print(result1);
    lcd1.setCursor(0, 1);         // move cursor to   (2, 1)
    result2 = string2 + (String)revCount1; //concate both string
    lcd1.print(result2); // print message at (2, 1)
    
    Serial.print("ButtonPressed!");
    timer_setup();
    runapp = 1;
  }
  
  if (runapp == 1) 
  {
    run_motorapp();
  }
  else if(runapp == 2)
  {
    run_stopmotorapp();
  }
  else if(runapp == 3)
  {
    run_idle_task();
  }

}

void run_stopmotorapp()
{
  stepper.stop();
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Finished");
  runapp = 3;
}

void run_idle_task() {
  delay(1000);
}

void run_motorapp()
{
   // Set the current position to 0:
    stepper.setCurrentPosition(0);
    // Run the motor forward at 120 steps/second until the motor reaches 200 steps (1 revolution):
    while (stepper.currentPosition() != 200) 
    {
    stepper.setSpeed(133.333333333333333333);
    stepper.runSpeed();
    }
    revCount1++;

  lcd1.setCursor(0, 0);         // move cursor to   (2, 1)
  result1 = string1 + (String)revCount1; // concate both string
  lcd1.print(result1);

  delay(500);

  // Reset the position to 0:
  stepper.setCurrentPosition(0);
  
  // Run the motor backwards at 120 steps/second until the motor reaches -200 steps (1 revolution):
  while (stepper.currentPosition() != -200) 
  {
    stepper.setSpeed(-133.333333333333333333);
    stepper.runSpeed();
  }

  revCount2++;
  lcd1.setCursor(0, 1);         // move cursor to   (2, 1)
  result2 = string2 + (String)revCount2; //concate both string
  lcd1.print(result2); // print message at (2, 1)
}

ISR(TIMER1_COMPA_vect)
{
    if (seconds == 0) {
      if (minutes == 0) {
        hours--;
        minutes = 59;
        seconds = 59;
      } else {
        minutes--;
        seconds = 59;
      }
    } else {
      seconds--;
    }

    if(seconds == 0 && minutes == 0 && hours == 0)
    {
      runapp = 2; // run_stopmotorapp();
      TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    }
    else{
    result3= (String)hours + "h " + (String)minutes + "m " + (String)seconds + "s";
    // Display the elapsed time
    sei();
    lcd2.setCursor(0,1);
    lcd2.print(result3);
    cli();
}