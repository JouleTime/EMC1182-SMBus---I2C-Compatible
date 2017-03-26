//This sketch was written by Frank Fox for JouleTime.
//Find us at http://www.jouletime.com
//Shop our listings on Ebay at http://www.ebay.com/usr/jouletime

/* This sketch will demonstrate how to use the Dual Channel Temperature Sensor, EMC1182, from Microchip
 
 The sensor is accurate to +/-1 degree Celsius with Beta compensation to correct readings. 
 Include both an internal and external temperature sensor. The external temperature sensor uses a diode/transitor with an optional capacitor filter. 
 This device includes numerous options, high/low limits, Beta configuration, Diode Ideality Factor, etc. 
 
 The device is compatible with 1.8V SMBus and I2C communications. We will use a fixed slave address version, EMC1182-1. 
 For this device, the slave address is 1001100b, (0x4C). Otherwise the device address is selected by the value of resistor connected.
 
 This device has multiple operation registers, 0x00 to 0xFF.
 
 Write Mode: 
 
 Write mode must include three bytes of information:
 
 Write Slave Address, Control/Operating Register, Data for Control/Operating Register
 
 
 Some of the important control/operating register addresses are:
 
 0x00 Internal Diode Data High Byte. 
 0x01 External Diode Data High Byte.
 0x03 Configuration, controls general operation. mirrored 0x09.
 0x04 Conversion Rate, how often the reading is updated, mirrored 0x0A.
 0x10 External Diode Data Low Byte, only 3 MSB.
 0x25 External Diode Beta Configuration. (default 0x08)
 0x27 External Diode Ideality Factor. (default 0x12)
 0x29 Internal Diode Data Low Byte, only 3 MSB.
 
 
 Read Mode: 
 
 Send Slave, Send Control Register, Read Slave Address, and how many data bytes. 
 
 There is no auto increment after reading a register. You must send the register address everytime you want to look at a new register.
 Otherwise it will repeat reading the same over and over. 
 Before read, you can select an operating register by writing the register address first. 
 If the address is already set then read can happen immediately.
 */

#include <Wire.h>
//Include the I2C/TWI (two wire interface)library with your sketch.

#define TempSensor 0x4C

byte i=0;

void setup()
/* Perform these actions:
 1. Prepare and start Serial Monitor. 2. Write register address and print out to serial monitor the values in the registers.
 */

{
  Serial.begin(9600); // 1. set up Serial Monitor to send data to screen.
  Wire.begin();
  while (!Serial) { // Wait for Serial Monitor to start. Delay to allow Leonardo type boards to connect to serial port. 
    ;  
  } 

  // 2. Write registers address, then read register data.

  delay(3000); // wait 3 seconds
  i=0;
  Serial.print("This is Internal Diode Data High Byte, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // line return
  i++;
  Serial.print("This is External Diode Data High Byte, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // line return
  i++;
  Serial.print("This is Status, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // line return
  i++;
  Serial.print("This is Configuration, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // this creates a line return
  i++;
  Serial.print("This is Conversion Rate, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x06.");
  Serial.println(); // this creates a line return
  i++;
  Serial.print("This is Internal Diode High Limit, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x55.");
  Serial.println(); // this creates a line return
  i++;
  Serial.print("This is Internal Diode Low Limit, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // this creates a line return
  i++;
  Serial.print("This is External Diode High Limit, High Byte, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x55.");
  Serial.println(); // this creates a line return
  i++;
  Serial.print("This is External Diode Low Limit, High Byte, ");
  Serial.print(GetTempRegister(i), HEX); // the value is displayed
  Serial.print(", default 0x00.");
  Serial.println(); // this creates a line return

    Serial.print("This is External Diode Beta Configuration, ");
  Serial.print(GetTempRegister(0x25), HEX); // 
  Serial.print(", default 0x08.");
  Serial.println(); // this creates a line return

    Serial.print("This is External Diode Ideality Factor, ");
  Serial.print(GetTempRegister(0x27), HEX); // 
  Serial.print(", default 0x12.");
  Serial.println(); // this creates a line return

    Serial.print("This is Internal Diode Data Low Byte, only 3 MSB, ");
  Serial.print(GetTempRegister(0x29), HEX); // 
  Serial.print(", default 0x00.");
  Serial.println(); // this creates a line return

  /*  i++;
   Serial.print("This is Internal Diode Low Limit (Mirror) ");
   Serial.print(GetTempRegister(i), HEX); // the value is displayed
   Serial.print(", default 0x00.");
   Serial.println(); // this creates a line return
   */
  delay(2000); // 2 second delay until repeat
}

void loop(){
  // Read the internal and external temperature, and Send the data to serial monitor.

  i=0;
  int temp=0;

  Serial.println(); // this creates a line return
  Serial.println(); // this creates a line return

  Serial.print("This is Internal Diode, High Byte ");
  Serial.print(GetTempRegister(i), DEC); // the decimal value is displayed
  Serial.print("C, or ");
  temp=GetTempRegister(i)*9/5+32; //Convert to Fahrenheit
  Serial.print(temp, DEC);
  Serial.print("F. ");
  Serial.println(); // this creates a line return

    i++;
  Serial.print("This is External Diode, High Byte ");
  Serial.print(GetTempRegister(i), DEC); // the decimal value is displayed
  Serial.print("C, or ");
  temp=GetTempRegister(i)*9/5+32; //Convert to Fahrenheit
  Serial.print(temp, DEC);
  Serial.print("F. ");
  Serial.println(); // this creates a line return
  delay(4000);// 4 second delay
}


// this function moves the register pointer then reads the data
byte GetTempRegister(byte Register)
{
  Wire.beginTransmission(TempSensor); // send slave device address
  // NOTE: All write() statements send data to a buffer, actual writing is initiated by endTransmission
  Wire.write(Register); // send the register address. 
  Wire.endTransmission(); // end the setup.
  byte data = 0 ; // variable to return the data
  Wire.requestFrom(TempSensor, 1); // send the slave address, and specify how may bytes of data to read
  while (Wire.available()) {
    data=Wire.read(); //keep reading data while available, we asked for 13 bytes(0-12).
  }
  return data;
}


