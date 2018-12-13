#include <Arduino.h>
#include <driver/touch_pad.h>

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_FILTER_MODE_EN  (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (1)

uint16_t val = 0;
uint16_t filterVal = 0;

void c_setup()
{
  Serial.begin(9600);
  touch_pad_init();
    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_config(TOUCH_PAD_GPIO4_CHANNEL, TOUCH_THRESH_NO_USE);
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    touch_pad_set_meas_time(0xffff, 0xffff);
}

void c_loop()
{
  uint16_t touch_value;
  uint16_t touch_filter_value;
  touch_pad_read_raw_data(TOUCH_PAD_GPIO4_CHANNEL, &val);
  touch_pad_read_filtered(TOUCH_PAD_GPIO4_CHANNEL, &filterVal);

  Serial.println(val);
}