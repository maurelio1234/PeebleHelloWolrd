#include <pebble.h>

Window    *window;
TextLayer *local_text_layer;
TextLayer *recife_text_layer;
TextLayer *taipei_text_layer;

TextLayer *recife_label_text_layer;
TextLayer *taipei_label_text_layer;

TextLayer *battery_label_text_layer;

// using different strings so that the compiler wont optimize them to a single value!
char*     local_buffer  = "00:00";
char*     recife_buffer = "00:01";
char*     taipei_buffer = "00:02";
char*     battery_buffer = "100%";

void update_buffer(char* buffer, struct tm *tick_time) {
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
    
}
void update_time() {
  time_t temp = time(NULL);
  
  update_buffer(local_buffer, localtime(&temp));
  
  temp = temp - 5*60*60;
  update_buffer(recife_buffer, localtime(&temp));
  
  temp = temp + 5*60*60 + 6*60*60;
  update_buffer(taipei_buffer, localtime(&temp));
    
  // Display this time on the TextLayer
  text_layer_set_text(local_text_layer, local_buffer);  
  text_layer_set_text(recife_text_layer, recife_buffer);  
  text_layer_set_text(taipei_text_layer, taipei_buffer);  
}

void update_battery() {
  BatteryChargeState charge_state = battery_state_service_peek();
  snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
  text_layer_set_text(battery_label_text_layer, battery_buffer);  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  update_battery();
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  // display size: 144×168
	local_text_layer = text_layer_create(GRect(0, 0, 144, 50));
	recife_text_layer = text_layer_create(GRect(0, 70, 144, 100));
	taipei_text_layer = text_layer_create(GRect(0, 100, 144, 130));

  recife_label_text_layer = text_layer_create(GRect(0, 70, 72, 100));
	taipei_label_text_layer = text_layer_create(GRect(0, 100, 72, 130));
  
	battery_label_text_layer = text_layer_create(GRect(0, 150, 144, 168));
    
	// Set the text, font, and text alignment
	text_layer_set_font(local_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	text_layer_set_text_alignment(local_text_layer, GTextAlignmentCenter);

 	text_layer_set_font(recife_text_layer, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(recife_text_layer, GTextAlignmentRight);

  text_layer_set_font(taipei_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(taipei_text_layer, GTextAlignmentRight);
	
 	text_layer_set_font(recife_label_text_layer, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(recife_label_text_layer, GTextAlignmentLeft);

  text_layer_set_font(taipei_label_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(taipei_label_text_layer, GTextAlignmentLeft);
	
  text_layer_set_font(battery_label_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(battery_label_text_layer, GTextAlignmentRight);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(local_text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(recife_text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(taipei_text_layer));

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(recife_label_text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(taipei_label_text_layer));

 	layer_add_child(window_get_root_layer(window), text_layer_get_layer(battery_label_text_layer));
  
  text_layer_set_text(recife_label_text_layer, "Recife: ");  
  text_layer_set_text(taipei_label_text_layer, "Taipei: ");  
  
	// Push the window
	window_stack_push(window, true);
	
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // initialize window
  update_time();
  update_battery();
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	text_layer_destroy(local_text_layer);
	text_layer_destroy(recife_text_layer);
	text_layer_destroy(taipei_text_layer);
  
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
