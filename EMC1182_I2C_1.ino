//This sketch was written by Frank Fox for JouleTime.
//Find us at http://www.jouletime.com
//Shop our listings on Ebay at http://www.ebay.com/usr/jouletime
//Find the I2C library at DSS Circuits http://dsscircuits.com/articles/86-articles/66-arduino-i2c-master-library
//Or you can go to Github here: https://github.com/rambo/I2C

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

#include <I2C.h>   //Include the I2C library with your sketch.
#define TempSensor 0x4C
#define Expander_Adr 0x20 // this is the PCA9535 slave address

//------------------ Initialize Variables ----------------------

byte i=0; //create a couple of variables for incrementing
int a=0;
byte SetupPorts_Output[2]={0x00, 0x00};
// Read is bit = 1, and Write is bit = 0. Every other bit is set as output.

/*Array where each element corresponds to the byte for lighting that number. E.g. cell[1] has the byte data to light up a "1" on the seven-segment display.*/

byte AllDigits0[17]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09, 0x11, 0xC1, 0x63, 0x85, 0x61, 0x71, 0xFE}; // digits 0-9 and "."
byte AllDigits1[17]={0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09, 0x11, 0xC1, 0x63, 0x85, 0x61, 0x71, 0xFE}; // digits 0-9 and "."
byte AllDigits_DP[17]={0x02, 0x9E, 0x24, 0x0C, 0x98, 0x48, 0x40, 0x1E, 0x00, 0x08, 0x10, 0xC0, 0x62, 0x84, 0x60, 0x70, 0xFE}; // digits 0-9 and "."



//---------------------- Setup Section ------------------------

void setup()
{
  I2c.begin();
  // Command Byte and Configure Direction of I/O Pins

  //-------------------- Setup I/O Expander ---------------------

  I2c.write(Expander_Adr, B00000110, SetupPorts_Output, 2); 
  /* Send slave device address, instruction byte, and data array. The SetupPorts_Output was initialized to be 0,0 so all the outputs are write. */
  delay(20);
  
  Serial.begin(9600); // 1. set up Serial Monitor to send data to screen.
  while (!Serial) { // Wait for Serial Monitor to start. Delay to allow Leonardo type boards to connect to serial port. 
    ;  
  } 

  delay(30); // wait 30 milliseconds
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

//------------------ Main Program Loop ----------------------

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
  Hex2bcd (temp);
  Serial.print(temp, DEC);
  Serial.print("F. ");
  Serial.println(); // this creates a line return
  delay(2000);// 2 second delay
  
    i++;
  Serial.print("This is External Diode, High Byte ");
  Serial.print(GetTempRegister(i), DEC); // the decimal value is displayed
  Serial.print("C, or ");
  temp=GetTempRegister(i)*9/5+32; //Convert to Fahrenheit
  Hex2bcdDecimal (temp);
  Serial.print(temp, DEC);
  Serial.print("F. ");
  Serial.println(); // this creates a line return
  delay(2000); // 2 second delay until repeat
}



//---------------------------------------------------------------------------
//--------------------Set Up Functions --------------------------------------
//---------------------------------------------------------------------------

//-------------- Move Register Pointer, Read Temperature Data ---------------

int GetTempRegister(int Register)
{
  I2c.write(TempSensor, Register);
  int data = 0 ; // variable to return the data
  I2c.read(TempSensor,1); // send the slave address, and specify how may bytes of data to read
  data=I2c.receive(); //keep reading data while available.
  return data;
}
//------------------ Write Digit to Display0 ----------------

void Display7Segment0(int DIGIT1) // 
{
  I2c.write(Expander_Adr, B00000010, DIGIT1); 
  // Send slave, control register 0x02=Port 0, SEND Digit1
}

//------------------ Write Digit to Display1 ----------------

void Display7Segment1(int DIGIT1) // 
{
  I2c.write(Expander_Adr, B00000011, DIGIT1);
  // Send slave, control register 0x03=Port 1, SEND Digit1 

}

//------- Break Number into High/Low Byte & Display ---------

void Hex2digit (byte x) // convert hexadecimal to high bits (7-4) and low bits (3-0)for two digit display.
{
  byte Highbits;
  byte Lowbits;
  Highbits=x>>4;
  Display7Segment1(AllDigits1[Highbits]);
  Lowbits=x<<4;
  Lowbits=Lowbits>>4;
  Display7Segment0(AllDigits0[Lowbits]);
}

//------- Convert hexidimal to Binary Coded Decimal (BCD) & Display ---------

void Hex2bcd (byte x) // convert hexadecimal to BCD binary coded decimal good for 0x00 - 0x63 (0-99 decimal)
{
  byte y; //byte to hold BCD value
  byte Highbits;
  byte Lowbits;
  y = (x / 10) << 4; //find the highbits of x then shift to store in y
  y = y | (x % 10); //find the remained which are the lowbits and add to y

  Highbits=y>>4;
  Display7Segment1(AllDigits0[Highbits]);
  Lowbits=y<<4;
  Lowbits=Lowbits>>4;
  Display7Segment0(AllDigits0[Lowbits]);
}

//------- Convert hexidimal to BCD add a decimal at end & Display ---------

void Hex2bcdDecimal (byte x) // convert hexadecimal to BCD binary coded decimal good for 0x00 - 0x63 (0-99 decimal)
{
  byte y; //byte to hold BCD value
  byte Highbits;
  byte Lowbits;
  y = (x / 10) << 4; //find the highbits of x then shift to store in y
  y = y | (x % 10); //find the remained which are the lowbits and add to y

  Highbits=y>>4;
  Display7Segment1(AllDigits0[Highbits]);
  Lowbits=y<<4;
  Lowbits=Lowbits>>4;
  Display7Segment0(AllDigits_DP[Lowbits]);
}
