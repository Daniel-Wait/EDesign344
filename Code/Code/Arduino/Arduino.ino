// Serial communication setup
constexpr long serial_baud_rate = 19200;
constexpr auto serial_config = SERIAL_8E1;

// Leonardo board information
constexpr int digital_pins[] = {9, 10, 11};
constexpr int analogue_pins[] = {A0, A1, A2};

const String STUDENT_NUMBER = "20887507";

unsigned long time1 = millis(); // LED receive indicator timer
unsigned long time2 = millis(); // Debug mode timer
unsigned long time3 = millis(); // Reset latch timer
unsigned long MillisecondsUpdtime = 0; // Uptime Counter

const byte numChars = 3; // two read bytes + end-of-line
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
boolean DebugMode = false;
boolean resetHigh =false;
String BinaryString = "";
String Outputstring = "";
int Aread = 0;

float volt_trans = 0;
float curr_trans = 0;
float phase_trans = 0;


void setup() {
  Serial.begin(19200,serial_config);
  pinMode(digital_pins[0], OUTPUT);
  pinMode(digital_pins[1], OUTPUT);
  pinMode(digital_pins[2], INPUT);
  
  TCCR1B = _BV(CS00);
  TCCR1A = _BV(CS01); 

  analogWrite(9, 250);

  digitalWrite(10, HIGH);
  delay(10000);
  digitalWrite(10, LOW);
   
}

void loop() 
{
 analogWrite(9, 250); //NB!!
 //Transducers();
 ReceiveData();
 TransmitData();
 DebugCheck();

  if((!resetHigh) && (receivedChars[0] == 'Q'))
  {
    Serial.println("Give that switch a reset. Switches love resets!");

    resetHigh = true;
    digitalWrite(10, HIGH);
    time3 = millis();
  }

  if((resetHigh) && (millis()-time3 > 1000))
  {
    resetHigh = false;
    digitalWrite(10, LOW);
    memset(receivedChars, 0, sizeof receivedChars);
  }
 
 // LED receive notifier timeout
 if(millis()-time1 > 200) {
    digitalWrite(LED_BUILTIN, LOW);  
    time1 = millis(); }
 
}

void ReceiveData() {
  
 char Read_end = '\n';
 static byte rdx = 0;
 char ReadCharacter;
 

 while (Serial.available() > 0 && newData == false) {
  
  ReadCharacter = Serial.read();
 
  digitalWrite(LED_BUILTIN, HIGH); 
 
  if (ReadCharacter != Read_end) {
    receivedChars[rdx] = ReadCharacter;
    rdx++;
    if (rdx >= numChars) {
      rdx = numChars - 1;
    }
  }
  else {
    receivedChars[rdx] = '\0'; // terminate the string
    rdx = 0;
    newData = true;
    }
  }
}

void TransmitData(){
  if (newData == true){
    if (receivedChars[0] == '0' && DebugMode == false){
        Serial.println(receivedChars[0]);
        Serial.println(STUDENT_NUMBER);
    }
    if (receivedChars[0] == '1' && DebugMode == false){
     Serial.println(receivedChars[0]);
     Serial.println(receivedChars[1]);
		 //Here you can apply calibration as necessary. 
		 switch (receivedChars[1]) {
			 case '0' : Aread = analogRead(analogue_pins[0]); break;
       case '1' : Aread = analogRead(analogue_pins[1]); break;
			 case '2' : Aread = analogRead(analogue_pins[2]); break;
     }
    
    
  	Serial.println(Aread);
    }
    if (receivedChars[0] == '2' && DebugMode == false){
      Serial.println(receivedChars[0]);
      Serial.println(receivedChars[1]);
		switch (receivedChars[1]) {
			case '0' : Serial.println(digitalRead(digital_pins[0])); break;
      case '1' : Serial.println(digitalRead(digital_pins[1])); break;
			case '2' : Serial.println(digitalRead(digital_pins[2])); break;
    }
  }
 	if (receivedChars[0] == 'x' || receivedChars[0] == 'X'){
         if (receivedChars[1] == '0'){
            DebugMode = false;
         }
         else if (receivedChars[1] == '1'){
            DebugMode = true;
         }
    }

   if ((receivedChars[0] == 'U') && DebugMode == false) // Return uptime if '?' is received
     {
       MillisecondsUpdtime=millis(); 
       uptime(); 
     }    
 }
  newData = false;
}

void DebugCheck(){
  if (DebugMode == true){
          String DebugOutput = "";
          /*
          float Aread0 = 0.078*analogRead(analogue_pins[0]) - 11.652;
          if( (int(Aread0) >= 46) || (int(Aread0) <= 0))
          {
            Aread0 = 0.0;
          }*/
          float Aread0 = analogRead(analogue_pins[0]);
          delay(10);
          
          float Aread1 = 61.879*5*analogRead(analogue_pins[1])/1023;
          delay(10);
          
          float Aread2 = ((3.4837*5*analogRead(analogue_pins[2])/1023) + 16.75)/sqrt(2);
          delay(10);
          
          DebugOutput = STUDENT_NUMBER + ',' + "A0:" + Aread0 + "(deg)," + "A1:" + Aread1 + "(mA)," + "A2:" + Aread2 + 
          "(Vrms)," + "D0:" + ReturnDigitalRead(digitalRead(digital_pins[0])) + ',' + "D1:" + ReturnDigitalRead(digitalRead(digital_pins[1])) + 
          ',' + "D2:" + ReturnDigitalRead(digitalRead(digital_pins[2]));
          if(millis()-time2 > 2000) { // LED receive notifier timeout 
            Serial.println(DebugOutput);

          if(digitalRead(digital_pins[2]) == LOW)
            Serial.println("Switch on");
          else
            Serial.println("Switches be tripping");
            
            time2 = millis(); }
       }
}

String ReturnDigitalRead(int Input){
  if (Input == 0){
    return "LOW";
  }
  else {
    return "HIGH";
  }
}

void uptime()
{
 long days=0;
 long hours=0;
 long mins=0;
 long secs=0;
 secs = MillisecondsUpdtime/1000; //convect milliseconds to seconds
 mins=secs/60; //convert seconds to minutes
 hours=mins/60; //convert minutes to hours
 days=hours/24; //convert hours to days
 secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
 mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
 hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
 //Display results
 Serial.print("Uptime : ");
 Serial.print(hours);
 Serial.print(":");
 Serial.print(mins);
 Serial.print(":");
 Serial.println(secs);
}

void ResetLatch()
{
  int len = sizeof(receivedChars)/sizeof(char);
  
  if(!resetHigh)
  {
    for(int k = 0; k < len; k++)
    {
      if(receivedChars[k] == 'r')
      {
        time3 = millis();
        resetHigh = true;
        digitalWrite(10, HIGH);     
      }
    }
  }
  else if(millis()-time3 >= 100)
  {
    resetHigh = false;
    digitalWrite(10, LOW);
  }
}
