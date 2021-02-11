// one button added for beverages Tx Set is now button 9
// function setButton_bev added to deal with css when hovering beverages direction buttons
var isTxEditMode = false;
var txArray = [];
var rxArray = [];
var urlToArduino;

var myResponseHandler = function(data) {

    var completeContent = [];

	completeContent = data.split('|');
	fillArrays(completeContent[0], completeContent[1]);
	
	if(!isTxEditMode) 
		setButtons(rxArray);
	else
	    setButtons(txArray);
}

function fillArrays(stringRx, stringTx)
{
	for(var i=0; i<8; i++)
	{
		rxArray[i] = stringRx[i].charCodeAt()-48;
		txArray[i] = stringTx[i].charCodeAt()-48;
	}
}


function setButtons(arr)	//cette fonction maj le statut des boutons du web
{
	for(var i=0; i<6; i++) //tant que 5 boutons on va pas plus loin que 6
	{		
		setButton(i, arr[i]);
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
function setButton_bev(btnNr, expo)
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
