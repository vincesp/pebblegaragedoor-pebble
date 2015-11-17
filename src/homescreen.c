#include <pebble.h>
#include "homescreen.h"
#include "communicationcontroller.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_icon_reload;
static GBitmap *s_res_icon_key;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_gothic_24_bold;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_28;
static GFont s_res_bitham_42_bold;
static ActionBarLayer *s_actionbar_layer;
static TextLayer *s_time_layer;
static TextLayer *s_conditions_layer;
static TextLayer *s_trigger_msg_layer;
static TextLayer *s_temperature_us_layer;
static TextLayer *s_temperature_layer;
static TextLayer *s_ping_msg_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 0);
  #endif
  
  s_res_icon_reload = gbitmap_create_with_resource(RESOURCE_ID_ICON_RELOAD);
  s_res_icon_key = gbitmap_create_with_resource(RESOURCE_ID_ICON_KEY);
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_res_gothic_28 = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  s_res_bitham_42_bold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  // s_actionbar_layer
  s_actionbar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbar_layer, s_window);
  action_bar_layer_set_background_color(s_actionbar_layer, GColorBlack);
  action_bar_layer_set_icon(s_actionbar_layer, BUTTON_ID_UP, s_res_icon_reload);
  action_bar_layer_set_icon(s_actionbar_layer, BUTTON_ID_DOWN, s_res_icon_key);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbar_layer);
  
  // s_time_layer
  s_time_layer = text_layer_create(GRect(3, 0, 121, 42));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "12:34");
  text_layer_set_font(s_time_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_time_layer);
  
  // s_conditions_layer
  s_conditions_layer = text_layer_create(GRect(3, 69, 121, 56));
  text_layer_set_background_color(s_conditions_layer, GColorClear);
  text_layer_set_text(s_conditions_layer, "Weather …");
  text_layer_set_font(s_conditions_layer, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_conditions_layer);
  
  // s_trigger_msg_layer
  s_trigger_msg_layer = text_layer_create(GRect(3, 106, 93, 32));
  text_layer_set_background_color(s_trigger_msg_layer, GColorClear);
  text_layer_set_text(s_trigger_msg_layer, " ");
  text_layer_set_font(s_trigger_msg_layer, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_trigger_msg_layer);
  
  // s_temperature_us_layer
  s_temperature_us_layer = text_layer_create(GRect(62, 47, 60, 28));
  text_layer_set_background_color(s_temperature_us_layer, GColorClear);
  text_layer_set_text(s_temperature_us_layer, " ");
  text_layer_set_text_alignment(s_temperature_us_layer, GTextAlignmentRight);
  text_layer_set_font(s_temperature_us_layer, s_res_gothic_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_temperature_us_layer);
  
  // s_temperature_layer
  s_temperature_layer = text_layer_create(GRect(3, 47, 69, 28));
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  text_layer_set_text(s_temperature_layer, " ");
  text_layer_set_font(s_temperature_layer, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_temperature_layer);
  
  // s_ping_msg_layer
  s_ping_msg_layer = text_layer_create(GRect(93, 98, 30, 44));
  text_layer_set_background_color(s_ping_msg_layer, GColorClear);
  text_layer_set_text(s_ping_msg_layer, "?");
  text_layer_set_text_alignment(s_ping_msg_layer, GTextAlignmentCenter);
  text_layer_set_font(s_ping_msg_layer, s_res_bitham_42_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_ping_msg_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbar_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_conditions_layer);
  text_layer_destroy(s_trigger_msg_layer);
  text_layer_destroy(s_temperature_us_layer);
  text_layer_destroy(s_temperature_layer);
  text_layer_destroy(s_ping_msg_layer);
  gbitmap_destroy(s_res_icon_reload);
  gbitmap_destroy(s_res_icon_key);
}
// END AUTO-GENERATED UI CODE

typedef struct DataBindingConfig {
  TextLayer** const text_layer;
  const char* const format;
  char buffer[32];
} DataBindingConfig;

static int8_t msg_hide_countdown = 0; //minutes

static DataBindingConfig modelMap[] = {
  {.text_layer = &s_temperature_layer, .format = "%s °C"},
  {.text_layer = &s_conditions_layer, .format = "%s"},
  {.text_layer = &s_ping_msg_layer, .format = "%s"},
  {.text_layer = &s_trigger_msg_layer, .format = "%s"},
  {.text_layer = &s_temperature_us_layer, .format = "%s"}
};

static void modelChanged(PropertyName propertyName, const char* const value) {
  APP_LOG(APP_LOG_LEVEL_INFO, "modelChanged: %d = %s", propertyName, value);
  DataBindingConfig* pConfig = &modelMap[propertyName];
  snprintf(pConfig->buffer, sizeof(pConfig->buffer), pConfig->format, value);
  text_layer_set_text(*pConfig->text_layer, pConfig->buffer);
  if (propertyName == trigger_msg) msg_hide_countdown = 2;
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_ping_msg_layer, "?");
  controller_getWeather();
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_trigger_msg_layer, "Sending …");
  controller_garageDoor();
}

static void update_time() {
  // Get a tm structure
  time_t current_time = time(NULL); 
  struct tm *tick_time = localtime(&current_time);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
           "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  
  // Get weather update every 30 minutes and garage door every 2 minutes
  if(tick_time->tm_min % 30 == 0) {
    controller_getWeather();
  } else if(tick_time->tm_min % 2 == 0) {
    controller_ping();
  }
  
  if (msg_hide_countdown) {
    if (!--msg_hide_countdown) {
      controller_setmodel(trigger_msg, "");
    }
  }
}

static void actionbar_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  controller_deinit();
}

void show_homescreen(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  action_bar_layer_set_click_config_provider(s_actionbar_layer, actionbar_click_config_provider);
  window_stack_push(s_window, true);
  controller_init();
  controller_on_modelchanged(modelChanged);
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void hide_homescreen(void) {
  window_stack_remove(s_window, true);
}
