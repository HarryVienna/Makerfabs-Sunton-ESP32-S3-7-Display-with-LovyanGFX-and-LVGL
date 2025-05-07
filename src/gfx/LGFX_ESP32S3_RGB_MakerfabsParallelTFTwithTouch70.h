#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB     _bus_instance;
  lgfx::Panel_RGB   _panel_instance;
  lgfx::Light_PWM   _light_instance;
  lgfx::Touch_GT911 _touch_instance;

  LGFX(void)
  {
    // --- Panel Configuration ---
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width  = 480;  // New panel resolution width
      cfg.memory_height = 272;  // New panel resolution height
      cfg.panel_width   = 480;
      cfg.panel_height  = 272;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    
    // --- Panel Detailed Config ---
    {
      auto cfg = _panel_instance.config_detail();
      cfg.use_psram = 1;
      _panel_instance.config_detail(cfg);
    }
    
    // --- RGB Bus & Pin Mappings ---
    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;
      
      // Update the data pins based on the documentation for the 4.3" board.
      cfg.pin_d0  = GPIO_NUM_15;
      cfg.pin_d1  = GPIO_NUM_7;
      cfg.pin_d2  = GPIO_NUM_6;
      cfg.pin_d3  = GPIO_NUM_5;
      cfg.pin_d4  = GPIO_NUM_4;
      cfg.pin_d5  = GPIO_NUM_9;
      cfg.pin_d6  = GPIO_NUM_46;
      cfg.pin_d7  = GPIO_NUM_3;
      cfg.pin_d8  = GPIO_NUM_8;
      cfg.pin_d9  = GPIO_NUM_16;
      cfg.pin_d10 = GPIO_NUM_1;
      cfg.pin_d11 = GPIO_NUM_14;
      cfg.pin_d12 = GPIO_NUM_21;
      cfg.pin_d13 = GPIO_NUM_47;
      cfg.pin_d14 = GPIO_NUM_48;
      cfg.pin_d15 = GPIO_NUM_45;

      // Control signal pins; update if the board wiring differs.
      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync   = GPIO_NUM_40;
      cfg.pin_hsync   = GPIO_NUM_39;
      cfg.pin_pclk    = GPIO_NUM_42;
      
      // The write frequency and signal timings may need to be adjusted
      // according to the 4.3" display datasheet or Makerfabs recommendations.
      cfg.freq_write  = 12000000; // (example value, verify if needed)
      cfg.hsync_polarity    = 0;
      cfg.hsync_front_porch = 8;   // example value; consult datasheet
      cfg.hsync_pulse_width = 2;   // example value; consult datasheet
      cfg.hsync_back_porch  = 43;  // example value; consult datasheet
      cfg.vsync_polarity    = 0;
      cfg.vsync_front_porch = 8;   // example value; consult datasheet
      cfg.vsync_pulse_width = 2;   // example value; consult datasheet
      cfg.vsync_back_porch  = 12;  // example value; consult datasheet
      cfg.pclk_idle_high    = 1;
      
      _bus_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);

    // --- Backlight Configuration ---
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = GPIO_NUM_2;  // Configure as needed—if the hardware uses a fixed voltage, this may be bypassed.
      _light_instance.config(cfg);
    }
    _panel_instance.light(&_light_instance);

    // --- Touchscreen Configuration (GT911) ---
    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.y_min = 0;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      // I2C connection: verify these pins based on the 4.3" board documentation.
      cfg.i2c_port = I2C_NUM_0;
      cfg.pin_sda  = GPIO_NUM_19;
      cfg.pin_scl  = GPIO_NUM_20;
      cfg.pin_int  = GPIO_NUM_NC;  // Use proper interrupt pin if available
      cfg.pin_rst  = GPIO_NUM_38;
      // Set touch coordinate max values to match the new resolution.
      cfg.x_max = 480;
      cfg.y_max = 272;
      cfg.freq  = 100000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    
    setPanel(&_panel_instance);
  }
};
