#include <pebble.h>

static Window *s_window;

static int hours = 0;
static int hour_symbol_0 = 0;
static int hour_symbol_1 = 0;

static int minutes = 0;
static int minute_symbol_0 = 0;
static int minute_symbol_1 = 0;

static char digit_width = 69;
static char digit_height = 81;

static GBitmap *s_digit_0_image;
static GBitmap *s_digit_1_image;
static GBitmap *s_digit_2_image;
static GBitmap *s_digit_3_image;
static GBitmap *s_digit_4_image;
static GBitmap *s_digit_5_image;
static GBitmap *s_digit_6_image;
static GBitmap *s_digit_7_image;
static GBitmap *s_digit_8_image;
static GBitmap *s_digit_9_image;

static GBitmap *ara[10];

static BitmapLayer *s_layer0;
static BitmapLayer *s_layer1;
static BitmapLayer *s_layer2;
static BitmapLayer *s_layer3;

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  ara[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_0);
  ara[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_1);
  ara[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_2);
  ara[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_3);
  ara[4] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_4);
  ara[5] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_5);
  ara[6] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_6);
  ara[7] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_7);
  ara[8] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_8);
  ara[9] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_9);

  s_layer0 = bitmap_layer_create(GRect(0, 0, digit_width, digit_height));
  s_layer1 = bitmap_layer_create(GRect(digit_width + 6,0, digit_width, digit_height));

  s_layer2 = bitmap_layer_create(GRect(0, digit_height + 6, digit_width, digit_height));
  s_layer3 = bitmap_layer_create(GRect(digit_width + 6, digit_height + 6, digit_width, digit_height));

  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer0));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer1));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer2));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer3));
}

static void prv_window_unload(Window *window) {
  // destroy the image layers
  gbitmap_destroy(s_digit_0_image);
  gbitmap_destroy(s_digit_1_image);
  gbitmap_destroy(s_digit_2_image);
  gbitmap_destroy(s_digit_3_image);
  gbitmap_destroy(s_digit_4_image);
  gbitmap_destroy(s_digit_5_image);
  gbitmap_destroy(s_digit_6_image);
  gbitmap_destroy(s_digit_7_image);
  gbitmap_destroy(s_digit_8_image);
  gbitmap_destroy(s_digit_9_image);

  layer_remove_from_parent(bitmap_layer_get_layer(s_layer0));
  layer_remove_from_parent(bitmap_layer_get_layer(s_layer1));
  layer_remove_from_parent(bitmap_layer_get_layer(s_layer2));
  layer_remove_from_parent(bitmap_layer_get_layer(s_layer3));

  bitmap_layer_destroy(s_layer0);
  bitmap_layer_destroy(s_layer1);
  bitmap_layer_destroy(s_layer2);
  bitmap_layer_destroy(s_layer3);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);


  hours = tick_time->tm_hour;
  minutes = tick_time->tm_min;


  hour_symbol_0 = hours / 10;
  hour_symbol_1 = hours % 10;

  minute_symbol_0 = minutes / 10;
  minute_symbol_1 = minutes % 10;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Hours = %d (symbol_0 = %d , symbol_1 = %d", hours, hour_symbol_0, hour_symbol_1);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Minutes = %d (symbol_0 = %d , symbol_1 = %d", minutes, minute_symbol_0, minute_symbol_1);

  bitmap_layer_set_bitmap(s_layer0, ara[hour_symbol_0]);
  bitmap_layer_set_bitmap(s_layer1, ara[hour_symbol_1]);
  bitmap_layer_set_bitmap(s_layer2, ara[minute_symbol_0]);
  bitmap_layer_set_bitmap(s_layer3, ara[minute_symbol_1]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void prv_init(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_window_handlers(s_window, (WindowHandlers) {
      .load = prv_window_load,
      .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
