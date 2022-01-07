#include <MIDI.h>


#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//transmittor
RF24 radio(5,6);
const byte address[6]= "00001";
int data[2];
int aantalsensoren = 2;
int aanrakingP = 0;
int aanrakingC = 0;
//midi
MIDI_CREATE_DEFAULT_INSTANCE();
byte cc = 1;
byte midiCh = 1;

void setup() 
{
  
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  

}

void loop() 
{

        
        radio.startListening();
        while (!radio.available());
        radio.read(&data ,sizeof(data));

     
        if (data[1]>0)
        {
          MIDI.sendControlChange(data[0], data[1],data[0]);
        }
          
        //Serial.println(data[1]);
         

          
          /*if (data[0] == 1)
          {
            if (data[1]> 90)
            {aanrakingC =1;}
            else 
            {
              aanrakingC = 0;
              }
            if (aanrakingC - aanrakingP == 1)
            {
              MIDI.sendNoteOn(36,127,2);
              delay (100);
              MIDI.sendNoteOn(36,0,2);
              aanrakingP=aanrakingC;
              }
            if (aanrakingC - aanrakingP == -1)
            {
             MIDI.sendNoteOn(36,127,2);
              delay (100);
              MIDI.sendNoteOn(36,0,2);
              aanrakingP=aanrakingC;

            
            }
     
        }*/
}
