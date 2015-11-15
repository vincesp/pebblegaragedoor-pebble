#include <pebble.h>
#include "communicationcontroller.h"

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_PING_MGS 2
#define KEY_TRIGGER_MSG 3
#define KEY_COMMAND 4

#define COMMAND_WEATHER 1
#define COMMAND_GARAGEDOOR 2
#define COMMAND_PING 3

const PropertyName property_map[] = {
  temperature, conditions, ping_msg, trigger_msg
};

static ModelChanged modelChangedCallback = NULL;

static AppSync s_sync;
static uint8_t s_sync_buffer[255];

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  if (!modelChangedCallback) return;
  modelChangedCallback(property_map[key], new_tuple->value->cstring);
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Sync Error %d", app_message_error);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void send_command(const uint8_t command) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Sending command: %d", command);
  
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, KEY_COMMAND, command);

  // Send the message!
  app_message_outbox_send();  
  
  APP_LOG(APP_LOG_LEVEL_INFO, "Command sent: %d", command);
}

void controller_init() {
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Setup initial values
  Tuplet initial_values[] = {
    TupletCString(KEY_TEMPERATURE, ""),
    TupletCString(KEY_CONDITIONS, "Weather …"),
    TupletCString(KEY_PING_MGS, "?"),
    TupletCString(KEY_TRIGGER_MSG, "")
  };

  // Begin using AppSync
  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);
}

void controller_on_modelchanged(ModelChanged callback) {
  modelChangedCallback = callback;
}

void controller_getWeather() {
  send_command(COMMAND_WEATHER);
}

void controller_ping() {
  send_command(COMMAND_PING);
}

void controller_garageDoor() {
  send_command(COMMAND_GARAGEDOOR);
}

void controller_deinit() {
  app_sync_deinit(&s_sync);
}
