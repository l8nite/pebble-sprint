#ifndef TEST
#include <pebble.h>

Window *window;
TextLayer *text_layer;
#else
#include <stdio.h>
#include <time.h>
#endif

#define BUFFER_SIZE 5

char current_sprint_buffer[BUFFER_SIZE];

static void update_sprint(struct tm* t) {
  int month = t->tm_mon + 1;
  int yday_now = t->tm_yday;
  int year_now = t->tm_year;
  
  int quarter = month <= 3 ? 1 : month <= 6 ? 2 : month <= 9 ? 3 : 4;
  
  // get the current quarter's start day so we can do math
  struct tm t_quarter = { 0 };
  t_quarter.tm_year = year_now;
  t_quarter.tm_mon = (quarter - 1) * 3;
  t_quarter.tm_mday = 1;

  mktime(&t_quarter);
  
  // find first friday after the quarter started, this is day 1 of sprint 1
  while (t_quarter.tm_wday != 5) {
    t_quarter.tm_mday++;
    mktime(&t_quarter);
  }

  int yday_day1_s1 = t_quarter.tm_yday;
  int days_elapsed = yday_now - yday_day1_s1;
  
  int sprint = days_elapsed < 0 ? 0 : days_elapsed / 14;
  if (sprint > 6) sprint = 6; // leave buffer days

  snprintf(current_sprint_buffer, BUFFER_SIZE, "Q%iS%i", quarter, sprint);
  current_sprint_buffer[BUFFER_SIZE-1] = 0;
}

#ifndef TEST
static void handle_day_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_sprint(tick_time);
  text_layer_set_text(text_layer, current_sprint_buffer);
}

void handle_init(void) {
	window = window_create();
  
  const bool animated = true;
  window_stack_push(window, animated);
  
  window_set_background_color(window, GColorBlack);
  
  GFont font = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);

  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);
  
  text_layer = text_layer_create(GRect(0, 20, frame.size.w, frame.size.h - 20));
  text_layer_set_background_color(text_layer, GColorBlack); 
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, font);
  layer_add_child(root_layer, text_layer_get_layer(text_layer));
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_sprint(t);
  
  tick_timer_service_subscribe(DAY_UNIT, &handle_day_tick);
}


void handle_deinit(void) {
	  text_layer_destroy(text_layer);
	  window_destroy(window);
}
#else
void test(void)
{
  struct tm day = { 0 };
  day.tm_year = 114;
  day.tm_mon = 0;
  day.tm_mday = 1;
  mktime(&day);

  char buffer[80];
  int i = 0;
  for (i = 0; i < 365; i++)
  {
    strftime(buffer, 80, "%F", &day);
    update_sprint(&day);
    printf("%s: %s\n", buffer, current_sprint_buffer);
  	//printf("%s\n", buffer);
    day.tm_mday++;
    mktime(&day);
  }
}
#endif

int main(void) {
#ifdef TEST
  test();
  return 0;
#else
  handle_init();
	app_event_loop();
	handle_deinit();
#endif
}

