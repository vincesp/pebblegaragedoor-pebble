//Get one at http://openweathermap.org/appid
var myAPIKey = '';
//Obtain the weather for this location:
var myGarageLocation = 'Santa Barbara,us';
//Configure your Heroku instance and secret
var herokuUrl = 'https://your-instance.herokuapp.com/keepout!';

function xhrRequest(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
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
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?q=' + myGarageLocation + '&appid=' + myAPIKey;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = String(Math.round((json.main.temp - 273.15) * 10) / 10);
      console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      sendAppMessage({
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions
      });
    }      
  );
}

function triggerGarageDoor() {
  console.log("trigger");
  xhrRequest(
    herokuUrl + '/trigger',
    'GET',
    function (responseText) {
      console.log(responseText);
      sendAppMessage({
        "KEY_TRIGGER_MSG": responseText
      });
    });
}

function ping() {
  console.log("ping");
  xhrRequest(
    herokuUrl + '/check',
    'GET',
    function (responseText) {
      console.log(responseText);
      sendAppMessage({
        "KEY_PING_MGS": responseText,
        "KEY_TRIGGER_MSG": ""
      });
    });
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
    ping();
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
    commandMap[e.payload.KEY_COMMAND]();
  }                     
);
