function setRegisterArray(button, regArr)
{
    var checkHowManyOn = verifyButtons(regArr, 8);
    console.log(checkHowManyOn);

    var isSameKeyPressed = false;

    if (regArr[button - 1] == 1)
        isSameKeyPressed = true;

    if (checkHowManyOn == 1 && button < 8) // I want only one button at once
    {
        if (isSameKeyPressed == false)
        {
			regArr[0] = 0;					// Set all array to 0
			regArr[1] = 0;
			regArr[2] = 0;
			regArr[3] = 0;
			regArr[4] = 0;
			regArr[5] = 0;
			regArr[6] = 0;
			regArr[7] = 0;
			regArr[button - 1] = 1;			// then set to 1 only pressed button
            return regArr;
        }
    }
/*    if (checkHowManyOn == 2 && button < 4)
    {
        regArr[0] = 0;
        regArr[1] = 0;
        regArr[2] = 0;
        regArr[button - 1] = 1;
        return regArr;
    }
*/
/*    if (checkHowManyOn == 1 && button == 4)
    {
        toggleRegisterArray(regArr, 1);
        return regArr;
    }

    if (checkHowManyOn == 4 && button < 4 || checkHowManyOn == 3 && button < 4)
    {
        toggleRegisterArray(regArr, 0);
        regArr[button - 1] = 1;
        return regArr;
    }

    if (checkHowManyOn == 4 && button == 4)
    {
        regArr[0] = 1;
        regArr[1] = 1;
        regArr[2] = 0;
        return regArr;
    }

    if (checkHowManyOn == 3 && button == 4 && regArr[0] == 1)
    {
        regArr[0] = 0;
        regArr[1] = 1;
        regArr[2] = 1;
        return regArr;
    }

    if (checkHowManyOn == 3 && button == 4 && regArr[2] == 1)
    {
        regArr[0] = 1;
        regArr[1] = 1;
        regArr[2] = 1;
        return regArr;
    }

    if (checkHowManyOn == 2 && button == 4)
    {
        regArr[0] = 1;
        regArr[1] = 1;
        regArr[2] = 1;
        regArr[3] = 1;
        return regArr;
    }
*/
}

function createLcdString(arrei, isTx)
{
   var tempString = "";

   if(!isTx)
   {
      if(!isTxEditMode)
         tempString += "-> ";
      else
         tempString += "&nbsp;&nbsp;&nbsp;";
      tempString += "RX&nbsp;";     
   }
   else
   {
      if(isTxEditMode)
         tempString += "-> "; 
      else
         tempString += "&nbsp;&nbsp;&nbsp;";
      tempString += "TX&nbsp;";
   }

   if (arrei[0]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "1&nbsp;";

   if (arrei[1]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "2&nbsp;";

   if (arrei[2]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "3&nbsp;";

   if (arrei[3]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "4&nbsp;";

   if (arrei[4]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "5&nbsp;";

   if (arrei[5]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "6&nbsp;";

/*   if (arrei[6]==1)						// uncomment if you need more buttons
      tempString += "*";					// but you will need to update setButton(arr) function below and in c.js
   else										// buttons have to be added as well in Arduino code
      tempString += "&nbsp;";
   tempString += "7&nbsp;";

   if (arrei[7]==1)
      tempString += "*";
   else
      tempString += "&nbsp;";
   tempString += "8&nbsp;";
*/
   if(!isTx)
      $('#myRxString').html(tempString);	// build Rx string for the web pseudo LCD
   else
      $('#myTxString').html(tempString);	// build Tx string for the web pseudo LCD
}

function setButtons(arr) { 					// override setbuttons here we set the buttons for html server                                                      

    var checkOne = verifyButtons(arr, 6); 	// I use 6 buttons

    for (var i = 0; i < 6 ; i++) {			// Stop the loop before 6 for 6 buttons

        if (i == 3 && checkOne == 3)
            setButton(i, 0);
        else
            setButton(i, arr[i]);
    }
}
