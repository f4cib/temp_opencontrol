
#if SKETCHMODE == 3
// the welcome info. you are not allowed to change anything here! CC
void displayWelcomeText()
{
  lcd.init();
  lcd.clear();
  lcd.home();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" remoteQTH.com ");
  lcd.setCursor(0, 1);
  lcd.print(" openMultiCon");
}

// the default start screen template
void displayMain()
{
  resetDisplay();
  lcd.print("->RX");
  lcd.setCursor(0, 1);
  lcd.print("  TX");
}

// set value of the pressed button into the relay register. Here you can define the mappings for the relays
void setRegisterArray(byte button, boolean regArr[])
{
  if (!inTxEditMode)                    // in rx mode
  {
    if (button < 8)                     // only one button could be active, except button 4 // F4CIB from 4 to 8 i want only one button at one time
    {
      toggleRegisterArray(regArr, 0); // F4CIB one button added ie. 8 buttons, and i want only one active at once so always in this case

      if (regArr[button - 1] == 1)
        regArr[button - 1] = 0;
      else
        regArr[button - 1] = 1;
    }
    else                                // button 4: all combinations possible // F4CIB not anymore, we should never go here now
    {
      if (regArr[button - 1] == 0)
        toggleRegisterArray(regArr, 1);
    }
  }
  else // in tx mode
  {
    byte checkOne = verifyButtons(regArr,8);   // F4CIB 4 replaced by 5, we now have 5 buttons... but to 8
  if (checkOne == 1) // only one is on
  {
    if (regArr[button - 1] == 0) // was the button off before? For sure, because its only one on...
    {
      if (button < 8) // if its 2, turn it on.. // F4CIB again 4 replaced by 5 because now 5 buttons but button 1 is for beverage so never Tx on it... 
        regArr[button-1] = 1;                   // F4CIB and buttons 3, 4, 5 are band dependant, let's hope the remote operator is not an idiot !
      else
      {
        regArr[0] = 0; //regArr is for relay in my use & button = one relay
        regArr[1] = 0;
        regArr[2] = 0;
        regArr[3] = 0;
        regArr[4] = 0;    //F4CIB register enlarges to 16 to be able to switch 8 relays by using 2 74HC595N
        regArr[5] = 0;
        regArr[6] = 0;
        regArr[7] = 0;
      }
 //     regArr[3] = 1; // if more than one is on, switch on 4, because of phasing...// F4CIB no phasing anymore last relay is an antenna so one condition up
    }
  }
  
  if (checkOne == 2)    // F4CIB we should not come here so i will not change anything  
  {
    if (regArr[button - 1] == 0) // was the button off before? For sure, because its only one on...
    {
      if (button < 4) // if its 2, turn it on..  
        regArr[button-1] = 1;                   
      else
      {
        regArr[0] = 1;
        regArr[1] = 1;
        regArr[2] = 1;
        regArr[3] = 1;
      }
    }
  }
 
  if (checkOne == 3) // if 3 are on (hint: 2 can never be on, because phasing is missing.. //F4CIB hint is not valid anymore... add case checkOne == 2 // F4CIB we should not come here so i will not change anything
  {
    if (regArr[button - 1] == 0) // turn last missing one on...
      regArr[button - 1] = 1;
    else // so now a button is switched off...
    {
      if (button==5) // if its 4, go back to default 1 on // F4CIB again 4 replaced by 5 because now 5 buttons but button 1 is for beverage so never Tx on it...
      {
        regArr[0] = 1;
        regArr[1] = 0;
        regArr[2] = 0;
      }
      else // turn the button off
        regArr[button-1] = 0;
      regArr[3] = 0; // phasing has to be off
    }
  }
  if (checkOne == 4) // if all are on...  //F4CIB we should never come here
  {
    if (button < 4) // if button < 4 is pressed, just turn it off
      regArr[button-1] = 0;
    else // if button 4 is pressed, go to default 1 on
    {
      regArr[0] = 1;
      regArr[1] = 0;
      regArr[2] = 0;
      regArr[3] = 0;
    }
  }
  }
}

// Here you can define possible exceptions buttons/leds vs. relays. Default is 1:1
void setRegisterLed(boolean isTx)
{
  if (isTx)
  {
    byte checkOne = verifyButtons(registersTx, 8);  //from 4 to 5 but will be 8
    Serial.println(checkOne);
    for (int i = 0; i<8; i++)     //from 4 to 8 remember arrays have been enlarged to 8
    {
      registersTxLed[i] = registersTx[i];
    }

    if (checkOne == 3) //we should never go here
      registersTxLed[3] = 0;
  }
  else
  {
    for (int i = 0; i<8; i++)   //from 4 to 8 remember arrays have been enlarged to 8
    {
      registersRxLed[i] = registersRx[i];
    }
  }
}


void setDisplay(boolean regArry[], byte row)
{
  clearLabels(row);
  lcd.setCursor(5, row);

  if (row == 0)
  {
    for (byte i = 7; i >= 0; i--)       //F4CIB from 3 to 7
    {
      if (regArry[i] == 1)              // We look for value set to 1 in registersRxLed
      {
        lcd.print(rxDisplayArray[i]);   // and then we display its label on HW LCD
        return;
      }   
    }
  }
  else
  {
    int sum = 0;
    sum = verifyButtons(regArry, 8);      // from 4 to 5 buttons can be up to 8 // returns numbers of buttons active. must return always 1 due to code change. (only one button at one time)
    Serial.print("sum - nb of buttons selected ");
    Serial.println(sum);

    if (sum == 1 || sum > 2)
    {
      for (byte i = 7; i >= 0; i--)       // from 3 to 7 but in Tx never go to 1 which are bev from 0 to 1 (maybenot switched back to 0
      {
        if (regArry[i] == 1)              // we look for value set to 1 in registerTxLed
        {
          lcd.print(txDisplayArray[i]);   // and we display its label on HW LCD
          return;
        }
      }
    }
//    else  //F4CIB only one button we should not go here
//    {
//      if (regArry[0] == 1 && regArry[1] == 1)
//      {
//        lcd.print(txDisplayArray[4]);
//        return;
//      }
//      if (regArry[0] == 1 && regArry[2] == 1)
//      {
//        lcd.print(txDisplayArray[5]);
//        return;
//      }
//      if (regArry[1] == 1 && regArry[2] == 1)
//      {
//        lcd.print(txDisplayArray[6]);
//        return;
//      }
//    }
//
  }
}
#endif
