// 6 buttons for our needs (can be increased up to 8 with some described mods) 
// Tx Set is now button 9
// function setButton_bev added to deal with css when hovering beverages direction buttons
var isTxEditMode = false;
var txArray = [];
var rxArray = [];
var bevArray = [];
var urlToArduino;

var myResponseHandler = function(data) {

    var completeContent = [];

	completeContent = data.split('|');
	fillArrays(completeContent[0], completeContent[1], completeContent[2]); // 0:Rx ; 1: Tx ; 2: Beverage
	
	if(!isTxEditMode)
	{	
		setButtons(rxArray);
		setBevButtons(bevArray);
	}
	else
	    setButtons(txArray);
}

function fillArrays(stringRx, stringTx, stringBev)
{
	for(var i=0; i<8; i++)
	{
		rxArray[i] = stringRx[i].charCodeAt()-48;
		txArray[i] = stringTx[i].charCodeAt()-48;
		bevArray[i] = stringBev[i].charCodeAt()-48;
	}
}


function setButtons(arr)	//cette fonction maj le statut des boutons du web
{
	for(var i=0; i<7; i++) //tant que 6 boutons on va pas plus loin que 7
	{		
		setButton(i, arr[i]);
	}	
}

function setBevButtons(arr)	//cette fonction maj le statut des boutons du web pour les Beverage
{
	for(var i=0; i<7; i++) //tant que 6 boutons on va pas plus loin que 7
	{		
		setBevButton(i, arr[i]);
	}	
}

	function setButton(btnNr, expo)
{
	var buttonname = 'b'+btnNr;

	if(expo == 1)
	{
		var onbut = document.getElementById(buttonname);
		onbut.className = "myButton right bon";
	};
			
	if(expo == 0)
	{
	var offbut = document.getElementById(buttonname);
		offbut.className = "myButton right boff";
	};
}

function setBevButton(btnNr, expo)
{
	var buttonname = 'bev'+btnNr;

	if(expo == 1)
	{
		var onbut = document.getElementById(buttonname);
		onbut.className = "myButton_bev right bon";
	};
			
	if(expo == 0)
	{
		var offbut = document.getElementById(buttonname);
		offbut.className = "myButton_bev right boff";
	};
}

function getAllContent() {
		$.ajax({
			crossDomain: true,
			async : true,
			type: "GET",
			headers: {
				'Access-Control-Allow-Origin': '*',
				'Access-Control-Allow-Headers': '*'
			},
			url: urlToArduino+"/Get/?callback=?",
			dataType: 'jsonp',
			jsonpCallback:'xx'})
			.done(function(data){ 
			    myResponseHandler(data.v);
			});
}

function setContent(currentBank, toBankValue) {
		jQuery.support.cors = true;

		return $.ajax({
			crossDomain: true,
			async : true,
			type: "GET",
			headers: {
				'Access-Control-Allow-Origin': '*',
				'Access-Control-Allow-Headers': '*'	
			},
			url: urlToArduino+"/Set/"+currentBank+"/"+toBankValue,
			contentType: "text/html",
			dataType: "html",
			success: function (data) { }
		});
}

function clkButton(butNr)
{
	if(butNr == 9)
	{
		if(isTxEditMode== false)
		{
			setButton(butNr, 1);
			isTxEditMode = true;
		}
		else
		{
			setButton(butNr, 0);
			isTxEditMode = false;			
		}
		fetchContent();
	}
	else
	{
		if(!isTxEditMode)
		{
			rxArray = setRegisterArray(butNr+1, rxArray);
			setButtons(rxArray);
			setContent(0, rxArray.join(''));
		}
		else
		{
			txArray = setRegisterArray(butNr+1, txArray);
			setButtons(txArray);			
			window.setTimeout(500);
			setContent(1, txArray.join(''));
		}
	}

	window.setTimeout(updateLCD,100);
}

function clkBevButton(butNr)
{
	if(butNr == 9)					// Reste du Set Tx one va jamais ici, les boutons bev vont de 0 à 6
	{
		if(isTxEditMode== false)
		{
			setButton(butNr, 1);
			isTxEditMode = true;
		}
		else
		{
			setButton(butNr, 0);
			isTxEditMode = false;			
		}
		fetchContent();
	}
	else
	{
		if(!isTxEditMode)
		{
			bevArray = setRegisterArray(butNr+1, bevArray);
			setBevButtons(bevArray);
			setContent(3, bevArray.join(''));				// setContent(0, for Rx ; 1, for Tx ; 3 for Beverage
		}
/* 		else												// on n'est pas sensé faire quoi que ce soit sur les Beverage en mode Set Tx !
		{
			txArray = setRegisterArray(butNr+1, txArray);
			setButtons(txAsetButtonsrray);			
			window.setTimeout(500);
			setContent(1, txArray.join(''));
		}
 */	}

//	window.setTimeout(updateLCD,100);			//Pour le moment on ne s'occupe pas du LCD
}

function verifyButtons(arri, nrOfButtons)
{
  var sum = 0;
  
  for(var u = 0;u < nrOfButtons; u++)
    sum += arri[u];   
    
  return sum;
}

function updateLCD()
{
	createLcdString(rxArray, false);
	createLcdString(txArray, true);
	console.log("updateLCD");
}

// set the complete Register array to given value
function toggleRegisterArray(regA, v1)
{
  for (var a = 0; a < 9; a++)
    regA[a] = v1;
}

var myVar = setInterval(function () { fetchContent() }, 5000);

function init()
{
    fetchContent();
}

function fetchContent() {
        getAllContent();
        window.setTimeout(updateLCD, 150);
}
