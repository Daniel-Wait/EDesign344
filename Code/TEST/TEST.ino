#include <avr/io.h>
#include <time.h>


// Serial communication setup
constexpr long serial_baud_rate = 19200;
constexpr auto serial_config = SERIAL_8E1;

// Leonardo board information
constexpr int digital_pins[] = {9, 10, 11};
constexpr int analogue_pins[] = {A0, A1, A2};

const String STUDENT_NUMBER = "20887507";

unsigned long time1 = millis(); // LED receive indicator timer
unsigned long time2 = millis(); // Debug mode timer
unsigned long MillisecondsUpdtime = 0; // Uptime Counter

const byte numChars = 3; // two read bytes + end-of-line
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
boolean DebugMode = false;

String BinaryString = "";
String Outputstring = "";
int Aread = 0;

int on =0;

void setup() {
  Serial.begin(19200,serial_config);
  
  pinMode(3, OUTPUT);
  pinMode(digital_pins[0], OUTPUT);
  pinMode(digital_pins[1], OUTPUT);
  pinMode(digital_pins[2], OUTPUT);
  
  pinMode(analogue_pins[0], OUTPUT);
  pinMode(analogue_pins[1], OUTPUT);
  pinMode(analogue_pins[2], OUTPUT);
  TCCR1B = _BV(CS00);
  TCCR1A = _BV(CS01);

  analogWrite(9, 250);
  //digitalWrite(10, HIGH);
  //delay(10000);
 // digitalWrite(10, LOW);
   
/*
  TCCR1A = bit(COM1A1) | bit(COM1B1) | bit(WGM11) | bit(WGM10);
  TCCR1B = bit(WGM12) | bit(CS10);
  OCR1A = 63;
  OCR1B = 31;


  // Set up the 250KHz output
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  OCR1AH = 0;
  OCR1AL = 15;
  OCR1BH = 0;
  OCR1BL = 0;

  analogWrite(9, 250);
  */

  
}

void loop() 
{
 // LED receive notifier timeout
 if(millis()-time1 > 200) {
    digitalWrite(LED_BUILTIN, LOW);  
    time1 = millis(); }
}
