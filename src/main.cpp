#include <Arduino.h>

#include <NHB_AD7124.h>



#define csPin 10
#define channels 8

Ad7124 adc(csPin, 4000000);


void calibrate() {
  adc.setAdcControl(AD7124_OpMode_Idle, AD7124_FullPower, false);
  delay(100);
  for(int i =0; i< channels; i++) {
    adc.enableChannel(i, true);
    adc.setAdcControl(AD7124_OpMode_SystemOffsetCalibration, AD7124_FullPower, false);
    delay(100);
    adc.enableChannel(i, false);
  }
  adc.setAdcControl (AD7124_OpMode_Continuous, AD7124_FullPower, false);
}
void setup() {

  //Initialize serial and wait for port to open:
  pinMode(2, INPUT_PULLUP);
  Serial.begin (500000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println ("AD7124");

  // Initializes the AD7124 device
  adc.begin();
  



 
// The filter select bits determine the filtering and ouput data rate
//     1 = Minimum filter, Maximum sample rate
//  2047 = Maximum filter, Minumum sample rate
//  
  for(int i =0; i< channels; i++) {
    adc.setup[i].setConfig(AD7124_Ref_Avdd, AD7124_Gain_128, true);    
    adc.setup[i].setFilter(AD7124_Filter_SINC4, 1);
  }
  
  for(int i =0; i< channels; i++) {
     adc.setChannel(i, i, static_cast<AD7124_InputSel>(i*2), static_cast<AD7124_InputSel>(i*2+1), true);
  }
  
  
  adc.setAdcControl (AD7124_OpMode_Continuous, AD7124_FullPower, false);

}

int printorder[7] = {0, 1, 7, 3, 4, 5, 6};

void loop() {

  int readings[channels];
  for(int i =0; i < channels; i++) {
    readings[i] = adc.readRaw(i);
  }  

  Serial.print(digitalRead(2), DEC); 
  Serial.print('\t');
  //dt = micros() - dt;
  
  for(uint8_t i = 0; i < 7; i++) {
    Serial.print((readings[printorder[i]]-0x800000));
    if(i +1 < channels) Serial.print('\t');
  }
 
  Serial.println();
  if (Serial.available()) {
    int inByte = Serial.read();
    if(inByte == 'c'){
      calibrate();
    }

  }
}