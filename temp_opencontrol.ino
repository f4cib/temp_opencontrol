
/*
*  This is the openControl for the remoteQth.com
*  If you need help, feel free to contact DM5XX@gmx.de
*  Sketch is developed with IDE Version 1.6.4 and later
*
*  This is free software. You can redistribute it and/or modify it under
*  the terms of Creative Commons Attribution 3.0 United States License if at least the version information says credits to remoteQTH.com :P
*
*  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/
*  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
*
*  Tribute to OL7M!
*  LLAP!
*/


#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
#include "PCF8574.h"

#define I2C_ADDR    0x38        // Define I2C Address for Beverage controller
#define BACKLIGHT_PIN  7
//#define En_pin  4
//#define Rw_pin  5
//#define Rs_pin  6
//#define D4_pin  0
//#define D5_pin  1
//#define D6_pin  2
//#define D7_pin  3

#define  LED_OFF  0
#define  LED_ON  1

#define SKETCHMODE 3         // 0 = multibeaming / 1 = stack2 / 2 = stack3 / 3 = f6knb => this will enable the needed files for each mode... nothing more to do than to change

//LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

PCF8574 I2C_BEV(I2C_ADDR);
LiquidCrystal_I2C lcd(0x27, 20, 4);
byte mac[] = { 0xDE, 0x7D, 0xBE, 0xEF, 0xFE, 0xED };  //**************************************** <-------------------------CHANGE MAC-ADRESS IF YOU HAVE MORE THAN 1 CONTROLLER

////////////////////////////////   CONFIGURE YOUR DEFAULT DETTINGS HERE   /////////////////////////////////////////////

byte ip[] = { 10, 0, 0, 179 };                        //*************************************************** <------------------------CHANGE ARDOINOs IP TO YOUR NEEDs - DONT FORGET TO CHANGE IT EVERYWHERE (see comments WWW Content for PROGMEM) !!!!!!           
byte gateway[] = { 10, 0, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);                            //*************************************************** <------------------------CHANGE PORT, IF YOU DONT LIKE PORT 80           
//F4CIB extended size of these 5 following registers from 4 to 8
boolean registersRx[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };		  // Rx buttons
boolean registersTx[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };		  // Tx buttons
boolean registersRxLed[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };		// Rx LED
boolean registersTxLed[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };		// Tx LED
boolean registersDisplay[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };	// LCD String display here you point to rxDisplayArray or txDisplayArray labels
boolean registersBev[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };     // Beverage buttons
boolean registersBevLed[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };  // Beverage LED

///////////////////////////////////////// Change the labels you want to have... there are about 11 chars left. so dont use longer labels than 11 chars...

#if SKETCHMODE == 0
  String rxDisplayArray[4] = { "Beam USA", "Beam AF", "Beam JA",  "Beam All" };
  String txDisplayArray[7] = { "USA", "AF", "JA",  "USA+AF+JA", "USA+AF",  "USA+JA", "AF+JA" };
#endif

#if SKETCHMODE == 1
  String rxDisplayArray[4] = { "Beam TOP", "BEAM BOTTOM", "InPhase",  "OutOfPhase" };
  String txDisplayArray[4] = { "beam top", "beam bottom", "inphase",  "outofphase" };
#endif

#if SKETCHMODE == 2
  String rxDisplayArray[6] = { "Beam TOP", "Beam Middle", "Beam Bottom",  "STACK ALL", "TOP+Mid", "Mid+Bot" };
  String txDisplayArray[6] = { "Beam tOP", "Beam middle", "Beam bottom",  "STACK all", "TOP+mid", "Mid+bot"};
#endif

#if SKETCHMODE == 3
  String rxDisplayArray[6] = { "Matrice    ", "40M 2 ele  ", "20M Africa ", "80M dipole", "40-6M R8   ", "30-10M Quad"};
  String txDisplayArray[6] = { "Matrice    ", "40M 2 ele  ", "20M Africa ", "80M dipole", "40-6M R8   ", "30-10M Quad" };
  char* bevDisplayArray[] = { "NW", "NE", "W", "E", "SW", "SE"};
#endif

/////////////////////////////////////// WWW Content for PROGMEM ////////////////////////////////////////////////////////////////////////////////////////////

const char  message0[] PROGMEM = { "<html><head>" };
const char  message1[] PROGMEM = { "<script type=\"text/javascript\" src=\"http://code.jquery.com/jquery-1.11.3.js\">" };      //*************************************************** <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
const char  message2[] PROGMEM = { "</script>" };
const char  message3[] PROGMEM = { "<script type='text/javascript' src='http://f4cib.free.fr/dm5xx/c.js'></script>" };         //*************************************************** <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
//const char  message3[] PROGMEM = { "<script type='text/javascript' src='http://h.mmmedia-online.de/c.js'></script>" };        //*************************************************** <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!

#if SKETCHMODE == 0
  const char  message4[] PROGMEM = { "<script type='text/javascript' src='http://h.mmmedia-online.de/cm.js'></script>" };      //******** Multibeam ************<------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
#endif

#if SKETCHMODE == 1
  const char  message4[] PROGMEM  = {"<script type='text/javascript' src='http://h.mmmedia-online.de/cs2.js'></script>"};      //******** Stack 2 Ant************ <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
#endif

#if SKETCHMODE == 2
  const char  message4[] PROGMEM  = {"<script type='text/javascript' src='http://h.mmmedia-online.de/cs3.js'></script>"};      //******** Stack 3 And************ <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
#endif

#if SKETCHMODE == 3
  const char  message4[] PROGMEM = { "<script type='text/javascript' src='http://f4cib.free.fr/dm5xx/cm.js'></script>" };      //******** Multibeam ************<------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
//  const char  message4[] PROGMEM = { "<script type='text/javascript' src='http://h.mmmedia-online.de/cm.js'></script>" };     //******** Multibeam ************<------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
#endif

const char  message5[] PROGMEM = { "<link href=\"http://f4cib.free.fr/dm5xx/c.css\" rel=\"stylesheet\" type=\"text/css\"/>" };    //*************************************************** <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
//const char  message5[] PROGMEM = { "<link href=\"http://h.mmmedia-online.de/c.css\" rel=\"stylesheet\" type=\"text/css\"/>" };    //*************************************************** <------------------------CHANGE to your File-Location URL IF NEEDED !!!!!!
const char  message6[] PROGMEM = { "<link rel = \"shortcut icon\" href=\"http://h.mmmedia-online.de/favicon.ico\">" };
const char  message7[] PROGMEM = { "</head>" };
const char  message8[] PROGMEM = { "<body>" };
const char  message9[] PROGMEM = { "<div id=\"container\">" };
// Beverage Table
const char  message10[] PROGMEM = { "<div class=\"myTab\">" };
const char  message101[] PROGMEM = { "<table class=\"myTable_bev\"> <tr> <td> </td> <td> <a id=\"bev1\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(1)\"> </a> </td> <td> </td> <td> <a id=\"bev2\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(2)\"> </a> </td> <td> </td> </tr> "};
const char  message102[] PROGMEM = { "<tr> <td> <a id=\"bev3\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(3)\"> </a> </td> <td> </td> <td> <a id=\"bev0\" href=\"#\" class=\"myButton\" onClick=\"clkBevButton(0)\"> </a> </td> <td> </td> <td> <a id=\"bev4\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(4)\"> </a> </td> </tr> "};
const char  message103[] PROGMEM = { "<tr> <td> </td> <td> <a id=\"bev5\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(5)\"> </a> </td> <td> </td> <td> <a id=\"bev6\" href=\"#\" class=\"myButton_bev\" onClick=\"clkBevButton(6)\"> </a> </td> <td> </td> </tr> </table> "};
const char  message104[] PROGMEM = { "</div> <div class=\"myTab\">" };
// Tx antenna Table
const char  message11[] PROGMEM = { "<table class=\"myTable\">" };
const char  message12[] PROGMEM = { "<tr><td><a id=\"b0\" href=\"#\" class=\"myButton\" onClick=\"clkButton(0)\"> </a></td>" };
const char  message13[] PROGMEM = { "<td><a id=\"b1\" href=\"#\" class=\"myButton\" onClick=\"clkButton(1)\"> </a></td>" };
const char  message14[] PROGMEM = { "<td><a id=\"b2\" href=\"#\" class=\"myButton\" onClick=\"clkButton(2)\"> </a></td>" };
const char  message15[] PROGMEM = { "<td><a id=\"b3\" href=\"#\" class=\"myButton\" onClick=\"clkButton(3)\"> </a></td>" };
const char  message151[] PROGMEM = { "<td><a id=\"b4\" href=\"#\" class=\"myButton\" onClick=\"clkButton(4)\"> </a></td>" }; 
const char  message152[] PROGMEM = { "<td><a id=\"b5\" href=\"#\" class=\"myButton\" onClick=\"clkButton(5)\"> </a></td>" }; 
//const char  message153[] PROGMEM = { "<td><a id=\"b6\" href=\"#\" class=\"myButton\" onClick=\"clkButton(6)\"> </a></td>" }; 
//const char  message154[] PROGMEM = { "<td><a id=\"b7\" href=\"#\" class=\"myButton\" onClick=\"clkButton(7)\"> </a></td>" }; 
const char  message16[] PROGMEM = { "<td align=\"center\"><a id=\"b9\" href=\"#\" class=\"myButton right\" onClick=\"clkButton(9)\"> </a></td>" };    //Tx button
const char  message17[] PROGMEM = { "</tr>" };
const char  message18[] PROGMEM = { "</table>" };
const char  message19[] PROGMEM = { "</div>" };
const char  message20[] PROGMEM = { "<div class=\"myTab2\" id=\"myTabi\"><div id=\"myRxString\"></div><div id=\"myTxString\"></div></div>" };
const char  message21[] PROGMEM = { "</div>" };
const char  message22[] PROGMEM = { "</body>" };

//// DONT FORGET TO CHANGE THE INTERNAL ARDUINO IP....
#if SKETCHMODE == 0
  const char  message23[] PROGMEM = { "<script>var urlToArduino='http://192.168.1.179';$('#container').css(\"background-image\", \"url(http://h.mmmedia-online.de/multi.png)\"); " }; //********UNCOMMENT/COMMENT NEEDED VERSION: Multibeaming************ <------------------------CHANGE to Arduino AND File-Location URL IF NEEDED !!!!!!
#endif
#if SKETCHMODE == 1
  const char  message23[] PROGMEM  = {"<script>var urlToArduino='http://192.168.1.179';$('#container').css(\"background-image\", \"url(http://h.mmmedia-online.de/stack.png)\"); "}; //********UNCOMMENT/COMMENT NEEDED VERSION: Stack***************** <------------------------CHANGE to Arduino AND File-Location URL IF NEEDED !!!!!!
#endif
#if SKETCHMODE == 2
  const char  message23[] PROGMEM = { "<script>var urlToArduino='http://192.168.1.179';$('#container').css(\"background-image\", \"url(http://h.mmmedia-online.de/stack.png)\"); " }; //********UNCOMMENT/COMMENT NEEDED VERSION: Stack***************** <------------------------CHANGE to Arduino AND File-Location URL IF NEEDED !!!!!!
#endif
#if SKETCHMODE == 3
//  const char  message23[] PROGMEM = { "<script>var urlToArduino='http://192.168.1.179';$('#container').css(\"background-image\", \"url(http://f4cib.free.fr/dm5xx/multi_F6KNB.jpg)\"); " }; //********UNCOMMENT/COMMENT NEEDED VERSION: Multibeaming************ <------------------------CHANGE to Arduino AND File-Location URL IF NEEDED !!!!!!
  const char  message23[] PROGMEM = { "<script>var urlToArduino='http://f4cib.ddns.net:8074';$('#container').css(\"background-image\", \"url(http://f4cib.free.fr/dm5xx/multi_F6KNB.jpg)\"); " }; //********UNCOMMENT/COMMENT NEEDED VERSION: Multibeaming************ <------------------------CHANGE to Arduino AND File-Location URL IF NEEDED !!!!!!
#endif

const char  message24[] PROGMEM = { "getAllContent();window.setTimeout(updateLCD,150);</script>" };
const char  message25[] PROGMEM = { "</html>" };

const byte webArraySize = 35;		// can be larger than necessary, so don't need to adjust size each time you'll add a new line

const char * const messages[webArraySize] PROGMEM =
{
  message0,
  message1,
  message2,
  message3,
  message4,
  message5,
  message6,
  message7,
  message8,
  message9,
  message10,
  message101,
  message102,
  message103,
  message104,
  message11,
  message12,
  message13,
  message14,
  message15,
  message151,
  message152,
//  message153,		// button 7
//  message154,		// button 8
  message16,
  message17,
  message18,
  message19,
  message20,
  message21,
  message22,
  message23,
  message24,
  message25
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String requestString;
const int ptt_InPin = 2;		// PTT Input
const int ptt_OutPin = 9;		// PTT Output
const int txModePin = 6;		// Set Tx hardware pin
const int txModeLedPin = 8;		// Set Tx hardware LED
int DS_pin = 5;					// 74HC595 Shift Register Data pin
int STCP_pin = 3;				// 74HC595 Shift Register Latch (outputs data when HIGH)
int SHCP_pin = 7;				// 74HC595 Shift Register Clock

volatile boolean isTxMode = false;
volatile boolean isTxMode_old = false;
volatile boolean isTxModeSet = false;
volatile boolean isRxModeSet = false;
volatile boolean isRxModeSet_old = false;

long debouncing_time = 10;
volatile unsigned long last_millis;
unsigned long pushDog = 0;

int buttonPin = 0;				// Tx Antenna voltage divider goes to A0
byte oldButton = 0;

int bev_buttonPin = 1;			// Beverage voltage divider goes to A1
byte oldBevButton = 0;

boolean inTxEditMode = false;
byte oldTxEditMode = 0;

int leadIn = 10;
int leadOut = 20;

byte currentButton;
byte currentBevButton;
byte readTxEditMode;

boolean isInterruptDetached = false;
boolean isInRelayInvertMode = false;    // set this to true if you want to use hardware with inverted relay out. Normal (false) mode: relays are switched on (0=>1). Inverted mode: relays are switched out (1 => 0);

//////////////////////////////////////////////// Main Setup //////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  setupPinMode();
  setupDigitalWrites();
  setupLCD();
  setupRegisters();
  Ethernet.begin(mac, ip); // Client starten
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  attachInterrupt(0, setInterruptTxMode, FALLING);		//Interruption for Tx
//  attachInterrupt(1, setInterruptBevMode, FALLING);		//Interruption for Beverage buttons A1
  I2C_BEV.begin();
}

void setupDigitalWrites()
{
  digitalWrite(ptt_InPin, HIGH);
  digitalWrite(ptt_OutPin, LOW);
  digitalWrite(txModePin, HIGH);
  digitalWrite(txModeLedPin, HIGH);
}

void setupPinMode()
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(bev_buttonPin, INPUT_PULLUP);		//Pullup mandatory to minimize rebounds (a capacitor between pushbutton pins is a good idea) 
  pinMode(DS_pin, OUTPUT);
  pinMode(STCP_pin, OUTPUT);
  pinMode(SHCP_pin, OUTPUT);
  pinMode(ptt_InPin, INPUT);
  pinMode(ptt_OutPin, OUTPUT);
  pinMode(txModePin, INPUT);
  pinMode(txModeLedPin, OUTPUT);
}

void setupRegisters()
{
  writeDisplayRegister(registersRxLed);
  writeRelayRegister(registersRx);
  setLabels(registersTxLed, true);
  writeDisplayRegister(registersTxLed);
  writeRelayRegister(registersTx);
}

void setupLCD()
{
  lcd.begin(16, 2); // initialize the lcd
//  lcd.setBacklightPin(BACKLIGHT_PIN, NEGATIVE);
  lcd.setBacklight(LED_ON);
  displayWelcomeText();
  delay(2000);
  displayVersion();
  delay(2000);
  displayGreetings();
  delay(3000);
  displayMain();
  setLabels(registersRxLed, false);
}

//////////////////////////////////////////////// Main Loop //////////////////////////////////////////////////////////////
void loop()
{
  ///////////////////// PTT CONTROL SECTION /////////////////////////
  debouncePtt(); // txptt is handled by interrupt, but from time to time it fails. this is handled inside debouncePtt();

  if (isTxMode)
  {
    if (!isTxModeSet)
    {
      //Serial.println("ptt fire");
      triggerPttWorkflow();
    }
  }
  else
  {
    if (!isRxModeSet)
    {
      receiving();      // set outupt pins
      setRxSetup();     // handle rx relays
      setBevSetup();    //handle Beverage relays
      isRxModeSet = true;
      isTxModeSet = false;
    }
    currentBevButton = getPressedBevButton();
    currentButton = getPressedButton();
    readTxEditMode = digitalRead(txModePin);
  }

  ///////////////////// BUTTON SECTION /////////////////////////    
  // here comes the TX-Mode-Button logic
  if (!isTxMode && readTxEditMode == 0 && readTxEditMode != oldTxEditMode)
  {
    if (inTxEditMode)
    {
      inTxEditMode = false;
      switchArrow(false);                               // true for Tx (row 1);  false for Rx (row 0)
      digitalWrite(txModeLedPin, HIGH);
      writeDisplayRegister(registersRxLed);
      writeRelayRegister(registersRx);
    }
    else                                               // else go to the tx-edit
    {
      inTxEditMode = true;
      switchArrow(true);                               // true for Tx (row 1);  false for Rx (row 0)
      digitalWrite(txModeLedPin, LOW);
      writeDisplayRegister(registersTxLed);
      writeRelayRegister(registersTx);
    }
  }

  // here comes the button logic
  if (!isTxMode && currentButton > 0 && currentButton != oldButton && oldButton == 0 && (long)(millis() - pushDog) >= 200)
  {
    if (inTxEditMode)
    {
      setRegisterArray(currentButton, registersTx);
      setDisplayAndRelays(true);
    }
    else
    {
      setRegisterArray(currentButton, registersRx);
      setDisplayAndRelays(false);
//      setRegisterArray(currentBevButton, registersBev);
//      setBevDisplayAndRelays(false);
    }
    pushDog = millis();
  }
  
  if (!isTxMode && currentBevButton > 0 && currentBevButton != oldBevButton && oldBevButton == 0 && (long)(millis() - pushDog) >= 200)
  {
    if (inTxEditMode)
    {
      setRegisterArray(currentButton, registersTx);
      setDisplayAndRelays(true);
    }
    else
    {
//      setRegisterArray(currentButton, registersRx);
//      setDisplayAndRelays(false);
      setRegisterArray(currentBevButton, registersBev);
      setBevDisplayAndRelays(false);
    }
    pushDog = millis();
  }
  
  if (!isTxMode)
  {
    oldButton = currentButton;
    oldBevButton = currentBevButton;
    oldTxEditMode = readTxEditMode;
    webServer();
    if (isInterruptDetached)
      interrupts();
  }
}

/*------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------HELPER METHODS ------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------- LCD Display ---------------------------------------------------------*/

// the version info. Dont change anything here! CC
void displayVersion()
{
  resetDisplay();
  lcd.print("1.6 011016 IP");
  lcd.setCursor(0, 1);
  lcd.print(" OK2ZAW & DM5XX");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Adapted by F4CIB");
}

void displayGreetings()
{
  resetDisplay();
  lcd.print("Hello my Master!");
  lcd.setCursor(0, 1);
  lcd.print("Have fun!");
}

// reset the display and set the cursor to default
void resetDisplay()
{
  lcd.clear();
  lcd.home();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

// switch the -> to the needed position
void switchArrow(boolean mode)                               // true for Tx (row 1);  false for Rx (row 0)
{

  byte a = 0;
  byte b = 1;
  if (mode)
  {
    a = 1;
    b = 0;
  }
  lcd.setCursor(0, a);
  lcd.print("->");
  lcd.setCursor(0, b);
  lcd.print("  ");
}

// remove the stars - this is a brute force method.. you do not have to think about which one changed tho' :P
void clearLabels(boolean mode)
{
  byte row = 0;       // default we are on Rx row

  if (mode)         // if mode is true, that means we are on Tx row
    row = 1;

  for (int i = 5; i < 16; i++)  // erase everything after column 5
  {
    lcd.setCursor(i, row);
    lcd.print(" ");
  }
}

// calculate the position and display antenna label on HW LCD  needed to represent the registerTx/Rx Array
void setLabels(boolean regArry[], boolean mode)
{
  byte row = 0;               //default mode false Rx

  if (mode)                   // if mode is true Tx, second row of the display
    row = 1;

  setDisplay(regArry, row);   // function is in f6knb.ino it will update hardware LCD with antenna label
}

/*-------------------------------------------------- Register handling --------------------------------------------------------------*/
// my little Register method. Chains the displayregister with the rx/tx register and writes them into the chip.
void writeRelayRegister(boolean registers[])
{
  boolean tempRegisters[16];    // F4CIB extended from 8 to 16 to handle 8 relays will use 2 74HC595 to handle this size
  byte inverter = 0;
  byte displayinverter = 0;

//  for (int x = 0; x < 4; x++)
//  {
//    tempRegisters[x] = registers[x];
//    tempRegisters[x + 4] = registersDisplay[x];
//  }
  for (int x = 0; x < 8; x++)     //F4CIB 8 values for Tx & 8 for Rx
  {
    tempRegisters[x] = registers[x];					    // fill antenna register (either Tx or Rx, this a temp register)
    tempRegisters[x + 8] = registersDisplay[x];		// fill DisplayLED register
  }

  digitalWrite(STCP_pin, LOW);

  for (int i = 15; i >= 0; i--) // F4CIB from 7 to 15
  {
    if (i < 8 && isInRelayInvertMode) // from 4 to 8 not sure what it does here...
    {
      if (tempRegisters[i] == 0)
        inverter = 1;
      else
        inverter = 0;
    }
    else
    {
      inverter = tempRegisters[i];
    }
    digitalWrite(SHCP_pin, LOW);
    digitalWrite(DS_pin, inverter);
    digitalWrite(SHCP_pin, HIGH);
  }
  digitalWrite(STCP_pin, HIGH); // Once register is loaded, outputs data
}

/*-------------------------------------------------- Beverage Register handling --------------------------------------------------------------*/
// A mix of DM5XX method adapted to the PCF8574
void writeBevRelayRegister(boolean registers[])
{
  boolean tempRegisters[16];                      // up to 8 relays with PCF8574 but 8 LED as well so size is 16 //need to check if i need the LED register

  for (int x = 0; x < 8; x++)                     // F4CIB 8 values for Tx & 8 for Rx
  {
    tempRegisters[x] = registers[x];              // fill antenna register (either Tx or Rx, this a temp register)
//    tempRegisters[x + 8] = registersDisplay[x];   // fill DisplayLED register
  }
// I need to build the string in binary based on the register values
  for (int i = 0; i< 7; i++)
  {
        I2C_BEV.write(i, registers[i]);           // read beverage array and set PCF8574 outputs 
  }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
// write the values of a given register into the display register, only needed for showing the button status
void writeDisplayRegister(boolean regA[])
{
  for (int a = 0; a < 8; a++)   //from 4 to 8
    registersDisplay[a] = regA[a];
}

// set the complete Register array to given value
void toggleRegisterArray(boolean regA[], byte v1)
{
  for (int a = 0; a < 8; a++)   //from 4 to 8
    regA[a] = v1;
}

// In some Setups, at least 1 output should be connected
byte verifyButtons(boolean arri[], byte nrOfButtons)
{
  int sum = 0;

  for (int u = 0; u < nrOfButtons; u++)
    sum += arri[u];

  return sum;
}

void setDisplayAndRelays(boolean isTx)
{
  setRegisterLed(isTx);
  //Serial.println("called");
  if (isTx)
  {
    clearLabels(true);
    setLabels(registersTxLed, true);
    writeDisplayRegister(registersTxLed);
    writeRelayRegister(registersTx);
  }
  else
  {
    clearLabels(false);
    setLabels(registersRxLed, false);
    writeDisplayRegister(registersRxLed);
    writeRelayRegister(registersRx);
  }
}

void setBevDisplayAndRelays(boolean isTx) //always send false for Beverage
{
  setRegisterLed(isTx);
  //Serial.println("called");
  if (isTx)                               //Always false, so this part should never run
  {
    clearLabels(true);
    setLabels(registersTxLed, true);
    writeDisplayRegister(registersTxLed);
    writeRelayRegister(registersTx);
  }
  else                                    //But this one !
  {
    clearLabels(false);
    setLabels(registersBevLed, false);
    writeDisplayRegister(registersBevLed);
    writeBevRelayRegister(registersBev);
  }
}

/*-------------------------------------------------- Fancy Helpers --------------------------------------------------------------*/
// my little string splitting method
String getStringPartByNr(String data, char separator, int index)
{
  int stringData = 0;
  String dataPart = "";
  for (int i = 0; i<data.length(); i++)
  {
    if (data[i] == separator)
      stringData++;
    else if (stringData == index)
      dataPart.concat(data[i]);
    else if (stringData>index)
    {
      return dataPart;
      break;
    }
  }
  Serial.print("datapart ");
  Serial.println(dataPart);
  return dataPart;
}

// never call delay micros direct in interruptfunction..
void myDelay(int x)   {
  for (int i = 0; i <= x; i++)
  {
    delayMicroseconds(1000);
  }
}

/*------------------------------------------------- PTT --------------------------------------------------------------------------*/
void setInterruptTxMode()
{
  noInterrupts();
  isInterruptDetached = true;
  pttTriggerTX();
  triggerPttWorkflow();
}

void triggerPttWorkflow()
{
  myDelay(leadIn);
  digitalWrite(ptt_OutPin, HIGH);

  isRxModeSet = false;
  isTxModeSet = true;
  if (currentButton > 0 && currentButton != oldButton && oldButton == 0)
  {
    clearLabels(true);
    setLabels(registersTxLed, true);
  }
  isRxModeSet_old = false;
}

void debouncePtt() {
  if ((long)(millis() - last_millis) >= debouncing_time) { // debounce the standard digitalWrites...
    isTxMode = !digitalRead(ptt_InPin);
    if (isTxMode)
    {
      if (!isInterruptDetached) // if interrupt is not detached = interrupt function was not called.
        pttTriggerTX();
    }
    else
      if (isRxModeSet_old != true)
        pttTriggerRX();
    last_millis = millis();
  }
}

void pttTriggerTX()
{
  isTxMode = true;
  if (isTxMode != isTxMode_old)
  {
    if (!inTxEditMode)
      writeDisplayRegister(registersTxLed);   //////////////////
    writeRelayRegister(registersTx);
  }
  isTxMode_old = isTxMode;
}

void pttTriggerRX()
{
  isTxMode = false;
  isTxMode_old = false;
  isRxModeSet = false;
  isTxModeSet = false;
  isRxModeSet_old = true; // prevent next debouncePtt call from executing pttTriggerRX, because it was called before..
}

// set pins for receiving
void receiving()
{
  digitalWrite(ptt_InPin, HIGH); // set input to high just to be sure.. :P
  digitalWrite(ptt_OutPin, LOW);
}

/*------------------------------------------------- RX Setup --------------------------------------------------------------------*/

// set the needed relays and display messages for RX
void setRxSetup()
{
  if (currentButton > 0 && currentButton != oldButton && oldButton == 0)    //if we detect that a valid button has been pressed and is different as the one active
  {
    clearLabels(false);                     // Remove everything after column 5 on hardware LCD
    setLabels(registersRxLed, false);       // Set Antenna label on Rx Row according to registerRxLed array
  }
  if (!inTxEditMode)
    writeDisplayRegister(registersRxLed);     // Copy registersRxLed to RegistersDisplaySet
    myDelay(leadOut);                         // leadout time - wait to release the relays from tx position to rx position
    writeRelayRegister(registersRx);          // Set relays and LED (74HC595) according to registersRx
}
/*------------------------------------------------- Beverage Setup --------------------------------------------------------------------*/

// set the needed relays and display messages for Beverage
void setBevSetup()
{
  if (currentBevButton > 0 && currentBevButton != oldBevButton && oldBevButton == 0)
  {
    clearLabels(false);
    setLabels(registersBevLed, false);
  }
  if (!inTxEditMode)
    writeDisplayRegister(registersBevLed);    // copy registersBevLed to RegistersDisplay
    myDelay(leadOut);                         // leadout time - wait to release the relays from tx position to rx position
    writeBevRelayRegister(registersBev);      // Set relays (and LED) (PCF8574) according to registersBev
}


/*------------------------------------------------- Button handling ---------------------------------------------------------------*/
// return the pressed button depending on the resistor array
// accept a difference of about +/- 20 to the centervalue
// F4CIB updated these value for my own voltage divider (and enable a larger window capture)
// 6 buttons but provision for 8
// added the Beverage button handling in this chapter
byte getPressedButton() 
{
  int c = getMyAverageValue();
  if (c < 10 && c >= 0)
    return 1;
  else if (c > 160 && c < 200)    //179
    return 2;
  else if (c > 285 && c < 325)    //305
    return 3;
  else if (c > 415 && c < 455)    //436
    return 4;
  else if (c > 540 && c < 580)    //560 F4CIB added some butons as well
    return 5;
  else if (c > 650 && c < 690)  //672 F4CIB added some butons as well //F4CIB system will be capable of handling 8 relay let's prepare the code
    return 6;
//  else if (c > 755 && c < 795)  //773 F4CIB added some butons as well
//    return 7;
//  else if (c > 850 && c < 890)  //869 F4CIB added some butons as well
//    return 8;
  else
    return 0;
}

//calculate the average on 4 readings
int getMyAverageValue()
{
  int sum = 0;

  for (int i = 0; i < 4; i++)
  {
    int currentV = analogRead(buttonPin);
    sum += currentV;
    delay(5);
  }
//  Serial.print("button averaged value ");Serial.println( sum / 4);
  return sum / 4;
}
// The same for Beverage (could have been smarted to not duplicate code and use boolean test... ToDoList)
// return the pressed bev_button depending on the resistor array
// accept a difference of about +/- 20 to the centervalue
// Provision for 8 directions, but only 6 used.
byte getPressedBevButton()            //measured on A1 @+12V = 0 ; 210 ; 395 ; 595 ; 749 ; 857
{
  int c = getMyAverageValueBev();
  if (c < 10 && c >= 0)
    return 1;
  else if (c > 190 && c < 230)   	//179 theory
    return 2;
  else if (c > 375 && c < 415)    //305
    return 3;
  else if (c > 575 && c < 615)   	//436
    return 4;
  else if (c > 730 && c < 770)    //560 
    return 5;
  else if (c > 835 && c < 875)  	//672 
    return 6;
//  else if (c > 755 && c < 795)    //773 
//    return 7;
//  else if (c > 850 && c < 890)  	//869 
//    return 8;
  else
    return 0;
}
//calculate the average on 4 readings
int getMyAverageValueBev()
{
  int sum = 0;

  for (int i = 0; i < 4; i++)
  {
    int currentV = analogRead(bev_buttonPin);
    sum += currentV;
    delay(5);
  }
//  Serial.print(" Bev button averaged value ");Serial.println( sum / 4);
  return sum / 4;
}


/*------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------- WEB Server Part --------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*---- Example: http://192.168.1.180 for a simple get. -------------------------------------------------------------------*/
/*---- Example: http://192.168.1.180/Set/1/0101 to Set 1 (TX) to 0101 (off-on-off-on), use 0 to set RX -------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
void webServer()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (requestString.length() < 100) {
          requestString += c;
        }

        //if HTTP request has ended
        if (c == '\n') {
          int cmdSet = requestString.indexOf("Set/"); // see if its a set request
          int cmdGet = requestString.indexOf("Get/"); // see if its a set request

          if (cmdSet >= 0)
          {
            byte currentBank = getStringPartByNr(requestString, '/', 2).toInt(); // the 2nd part is the bank-number
            Serial.print("currentBank puis currentPinString et enfin myString ");
            Serial.println(currentBank);
            String currentPinString = getStringPartByNr(requestString, '/', 3); // the 3nd part is the decimal-value to react on
            Serial.println(currentPinString);

            String myString = currentPinString.substring(0, currentPinString.indexOf(" HTT")); // remove the _HTTP... and convert to int
            Serial.println(myString);

            if (!isTxMode)
            {
              if (currentBank == 1)                         // CurrentBank= 0 Rx ; CurrentBank= 1 Tx ; CurrentBank= 3 Beverage  
              {
                inTxEditMode = true;
                switchArrow(true);
                writeToTheRegister(registersTx, myString);
                setDisplayAndRelays(true);
              }
              else if (currentBank == 0)                    // CurrentBank= 0 Rx ; CurrentBank= 1 Tx ; CurrentBank= 3 Beverage
              {
                inTxEditMode = false;
                switchArrow(false);
                writeToTheRegister(registersRx, myString);
                setDisplayAndRelays(false);
              }
              else                                        // if we're not on Tx or Rx, that means we sent a Beverage set command
              {
                inTxEditMode = false;
                switchArrow(false);
                writeToTheRegister(registersBev, myString);
                setBevDisplayAndRelays(false);            // Update LED on www and set PCF8574 outputs
              }
            }
            getStatus(client);
          }
          else if (cmdGet >= 0)
            getStatus(client);
          else
            getPage(client);

          requestString = "";

          delay(1);
          client.stop();
        }
      }
    }
  }
}

// A simple cors safe webserver response
void getStatus(EthernetClient client)
{
  String arrTx = convertArrayToString(registersTx);
  String arrRx = convertArrayToString(registersRx);
  String arrBev = convertArrayToString(registersBev);       //F4CIB add Beverage
  client.println("HTTP/1.1 200 OK"); //send new page
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: POST, GET, OPTIONS");
  client.println("Access-Control-Allow-Headers: Authorization");
  client.println();
  client.print("xx({\"v\": \"");
  client.print(arrRx);
  Serial.println("arrRx puis arrTx et ArrBev");                      //F4CIB debug
  Serial.println(arrRx);                                  //F4CIB debug
  client.print("|");
  client.print(arrTx);
  Serial.println(arrTx);                                  //F4CIB debug
  client.print("|");                                        //F4CIB add Beverage
  client.print(arrBev);
  Serial.println(arrBev);                                  //F4CIB debug
  client.print("\"})");
}

void getPage(EthernetClient client)
{
  client.println("HTTP/1.1 200 OK"); //send new page
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: POST, GET, OPTIONS");
  client.println("Access-Control-Allow-Headers: Authorization");
  client.println();

  for (int i = 0; i < webArraySize; i++)
  {
    printProgStr((const char *)pgm_read_word(&messages[i]), client);
    client.println();
  }
}

void printProgStr(const char * str, EthernetClient client)
{
  char c;
  if (!str)
    return;
  while ((c = pgm_read_byte(str++)))
    client.print(c);
}

// Writes to both of the registers directly - Remember: If you are calling the webserver you have to take care about urself about button-switching logic and restrictions
void writeToTheRegister(boolean regiA[], String theString) // here we transform the string (made of 0 and 1) command sent through the www in a true boolean array
{
  for (int d = 0; d < 8; d++)   //F4CIB from 4 to 8
  {
    boolean val = true;
    if (theString[d] == '0')
      val = false;

    regiA[d] = val;             // input array (Rx, Tx or Bev array) as the string converted in boolean
    registersDisplay[d] = val;  // registersDisplay at the same time
  }
}

// convert an array into the string for the response method
String convertArrayToString(boolean ri[])
{
  String mytemp;
  for (int f = 0; f < 8; f++)   //from 4 to 8
  {
    if (ri[f] == true)
      mytemp += "1";
    else
      mytemp += "0";
  }

  return mytemp;
}

//Beverage Gpios using PCF8574 I2C
void Gpio(){
  //=====[ GPIOs]=================
  //multi dimension array adressing: array [row] [column]
//  int Ports = 2;
//  //Columns: 0: i2c adress ; 1:# ; 2:PTT ; 3:Error ; 4:Manual ; 5:part
//  for (i = 0; i < Ports; i++) {                   // 6 beverages
//    if (menu1state == 1 && i == enc0Pos) {
//      rx(port[i][0], i, 1, port[i][5]);
//    } else if (port[i][4] == 1) {
//      rx(port[i][0], i, 1, port[i][5]);
//    } else if (port[i][4] == 0) {
//      rx(port[i][0], i, 0, port[i][5]);
//    }
//
//    c = 0;
//    for (j = 0; j < Ports; j++) {
//      if (i != j && port[i][1] == port[j + 4][1]) {
//        c++;
//      }
//    }
//    if (c > 0) {
//      port[i][3] = 1;
//      if (port[i][2] == 0) {
//        port[i + 4][1] = 0;
//      }
//    } else {
//      port[i][3] = 0;
//      if (port[i][2] == 0) {
//        port[i + 4][1] = port[i][1];
//      }
//    }
//    if (port[i + 4][5] == 2) {
//      tx(port[i + 4][0], i);
//    }
//  }
}
//=====[ RX ]===================================================
void rx(byte addr, int portNR, int PTTonly, int Bank) {
//  Wire.beginTransmission(addr);
//  Wire.write(0x12);
//  Wire.endTransmission();
//  Wire.requestFrom(addr, 1);
//#if defined(inputHigh)
//  a = Wire.read();
//#else
//  a = ~Wire.read();
//#endif
//  Wire.beginTransmission(addr);
//  Wire.write(0x13);
//  Wire.endTransmission();
//  Wire.requestFrom(addr, 1);
//  b = ~Wire.read();
//  if (Bank == 1) {
//    if (b & (1 << 1)) {
//      port[portNR][2] = 1;
//    } else {
//      port[portNR][2] = 0;
//    }
//    if (PTTonly == 0) {
//      if (a & (1 << 0)) {
//        a = a | (1 << 7);
//      } else {
//        a = a & ~(1 << 7);
//      };
//      if (a & (1 << 1)) {
//        a = a | (1 << 6);
//      } else {
//        a = a & ~(1 << 6);
//      };
//      if (a & (1 << 2)) {
//        a = a | (1 << 5);
//      } else {
//        a = a & ~(1 << 5);
//      };
//      if (a & (1 << 3)) {
//        a = a | (1 << 4);
//      } else {
//        a = a & ~(1 << 4);
//      };
//      a = a >> 4;
//
//      port[portNR][1] = BCDmatrixINOUT[0][a];
//    }
//  } else if (Bank == 2) {
//    if (b & (1 << 0)) {
//      port[portNR][2] = 1;
//    } else {
//      port[portNR][2] = 0;
//    }
//    if (PTTonly == 0) {
//      a = a >> 4;
//      port[portNR][1] = BCDmatrixINOUT[1][a];
//
//    }
//  }
}


//=====[ TX ]===================================================
void tx(byte addr, int portNR) {
//  switch (portNR]) {
//    case 0: a = B00000000; break;
//    case 1: a = B00000001; break;
//    case 2: a = B00000010; break;
//    case 3: a = B00000100; break;
//    case 4: a = B00001000; break;
//    case 5: a = B00010000; break;
//    case 6: a = B00100000; break;
//    case 7: a = B00000000; break;
//  }
//  Wire.beginTransmission(port[portNR + 4][0]);
//  Wire.write(0x12);
//  Wire.write((byte)a);
//  Wire.endTransmission();
//  Wire.beginTransmission(port[portNR + 4][0]);
//  Wire.write(0x13);
//  Wire.write((byte)b);
//  Wire.endTransmission();
}
