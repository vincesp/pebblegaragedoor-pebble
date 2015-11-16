//Get one at http://openweathermap.org/appid
var myAPIKey = '';
//Obtain the weather for this location:
var myGarageLocation = 'Santa Barbara,us';
//Configure your Heroku instance and secret
var herokuUrl = 'https://your-instance.herokuapp.com/keepout!';

function xhrRequest(url, type, callback, fail) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  
  if (fail) {
    xhr.addEventListener("error", fail);
    xhr.addEventListener("abort", fail);
  }

  xhr.open(type, url);
  xhr.send();
}

function sendAppMessage(msg) {
  Pebble.sendAppMessage(
    msg,
    function(e) {
      console.log("Message sent to Pebble successfully!");
    },
    function(e) {
      console.log("Error sending message to Pebble!", e);
    }
  );
}

function getWeather() {
  console.log("getWeather");
  ping();
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?q=' + myGarageLocation + '&appid=' + myAPIKey;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      var msg = {};
      
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      msg.KEY_TEMPERATURE = String(Math.round((json.main.temp - 273.15) * 10) / 10);
      console.log("Temperature is " + msg.KEY_TEMPERATURE);
      
      // Special treatment for local US unit
      msg.KEY_TEMPERATURE_US = 
        json.sys.country.toLowerCase() === 'us' ?
        Math.round((json.main.temp - 273.15) * 1.8 + 32) + ' °F' :
        '';

      // Conditions
      msg.KEY_CONDITIONS = json.weather[0].description;      
      console.log("Conditions are " + msg.KEY_CONDITIONS);
      
      sendAppMessage(msg);
    }      
  );
}

function checkResponseText(responseText, errorText) {
  return responseText.length >= 32 ? errorText : responseText;
}

function triggerGarageDoor() {
  xhrRequest(
    herokuUrl + '/trigger',
    'GET',
    function (responseText) {
      console.log(responseText);
      sendAppMessage({
        KEY_TRIGGER_MSG: checkResponseText(responseText, 'Error')
      });
    },
    function (e) {
      console.log(e);
      sendAppMessage({
        KEY_TRIGGER_MSG: 'Error'
      });
    }
  );
}

function ping() {
  xhrRequest(
    herokuUrl + '/check',
    'GET',
    function (responseText) {
      console.log(responseText);
      sendAppMessage({
        KEY_PING_MGS: checkResponseText(responseText, '!'),
        KEY_TRIGGER_MSG: ''
      });
    },
    function (e) {
      console.log(e);
      sendAppMessage({
        KEY_PING_MGS: '!',
        KEY_TRIGGER_MSG: ''
      });
    }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

var commandMap = {
  1: getWeather,
  2: triggerGarageDoor,
  3: ping
};

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('Received message: ' + JSON.stringify(e.payload));
    if (e.payload.KEY_COMMAND) commandMap[e.payload.KEY_COMMAND]();
  }                     
);
