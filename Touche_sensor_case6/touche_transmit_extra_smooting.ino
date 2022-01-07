#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>




//touché
#define steps 128
float values[steps];
float alpha;
int maxPos, maxVal;

//transmittor
RF24 radio(5,6);
const byte address[6]= "00001";
int test = 5;

//pot
int Cvalue=0;
int Pvalue=0;
int Threshold = 4;
int Verschil=0;
bool Aanraking = true;
unsigned long Ptime = 0;
unsigned long timer =0;

//smooting
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                


void setup ()
{
  //touché
  pinMode (9, OUTPUT); 
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1A |= (1 << COM1A0);        // Toggle OC1A on Compare Match.
  TCCR1B |= (1 << WGM12);         // CTC mode
  //Serial.begin(9600);

  //transmittor
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  //smooting
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
    readings[thisReading] = 0;
  }
}

void loop () {

  //touché
  if (Serial.available()) {
    alpha = (float)Serial.read() / 255.0f;
  }
  maxPos = 0;
  maxVal = 0;
  for (int i = 0; i < steps; i++) {
    TCCR1B &= 0xFE;                       // turns off timer
    TCNT1 = 0;                            // resets timer counter register
    OCR1A = i;                            // sets new frequency step
    TCCR1B |= 0x01;                       // turns on timer
    float curVal = analogRead(0);
    values[i] = values[i] * alpha + curVal * (1 - alpha);  // exponential moving avg
    if (values[i] > maxVal) {                              // finds the signal peak
      maxVal = values[i];
      maxPos = i;
    }
  }
  //maxPos = maxPos - 50;
  //maxPos = maxPos * 6;

  if (maxPos < 0) {
    maxPos = 0;
  }

  if (maxPos > 255) {
    maxPos = 255;
  }
  
  //Serial.println(maxPos);
  
  //Serial.print(maxPos, DEC);
  //Serial.print(" ");
  //Serial.println(maxVal, DEC);

  maxPos = map (maxPos, 0, 255, -50, 600);
  if (maxPos>127)
    {maxPos=127;}

    maxPos = smooting (maxPos);
    
    Cvalue = maxPos;
    Verschil = abs(Cvalue-Pvalue);
    if (Verschil>Threshold)
    {
      int data[2] ={1,Cvalue};
      radio.stopListening();
      radio.write(&data, sizeof(data));
      Pvalue = Cvalue;
      delay(100);
    }
    
  
    
//transmittor
  
  //Serial.println(maxPos);
 
  
 
}

int smooting (int value)
{ // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) 
  {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  return average;
  delay(100);        // delay in between reads for stability
}
