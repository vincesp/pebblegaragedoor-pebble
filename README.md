# Garage door opener for your Pebble watch

This is the application for your [Pebble watch](https://www.pebble.com/).

It displays how many sockets are currently connected to your Heroku instance, and allows you to open/close the garage door by pressing to bottom button on your watch.

Before installing, you need to make the following configuration in [src/js/pebble-js-app.js](src/js/pebble-js-app.js):
* The API key aka appid for openweathermap.org
* The location of your garage, so that we look up the correct weather
* Your Heroku instance with your secret

```javascript
//Get one at http://openweathermap.org/appid
var myAPIKey = '';
//Obtain the weather for this location:
var myGarageLocation = 'Santa Barbara,us';
//Configure your Heroku instance and secret
var herokuUrl = 'https://your-instance.herokuapp.com/keepout!';
```

## Todo

* Nice menu icon
