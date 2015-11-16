#pragma once
typedef enum {
  temperature, conditions, ping_msg, trigger_msg, temperature_us
} PropertyName;

typedef void (*ModelChanged)(PropertyName propertyName, const char* const value);

void controller_on_modelchanged(ModelChanged callback);

void controller_getWeather();

void controller_ping();

void controller_garageDoor();

void controller_init();

void controller_deinit();
