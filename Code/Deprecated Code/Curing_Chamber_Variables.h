// Adafruit TFT Libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// ESP32 TFT I2C initializer
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

///////////////////////////////////////////////////////////////////////////////////////

// GLOBALS
bool rotary_A = true, rotary_B = true, button_value = false, update_screen = false, button_press = false;
// time_value in minutes, temp_value in fahrenheit,
int menu = 1, option = 1, time_value = 0, temp_value = 0, dial_value = 0, time_min = 1, temp = 1;

/*===================================================================================
 * startup_screen:
 *
 *   Executes the startup screen routine. Displays the team's name, device name, and
 *   an "Initialization . . ." delay with a scrolling screen refresh.
 *
 * @param   - none
 * @returns - none
 *===================================================================================*/

void startup_screen() {
  // Sets Up Screen and Text
  tft.fillScreen(0xC01F);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(3);

  // Displays Team Name
  tft.setCursor(15, 12);
  tft.println("UV");
  tft.setCursor(34, 37);
  tft.println("VIKINGS");

  // Displays Device Name
  tft.setTextSize(2);
  tft.setCursor(30, 70);
  tft.println("Curing");
  tft.setCursor(50, 90);
  tft.println("Chamber");

  // "Initializing" Text
  tft.setTextSize(1);
  tft.setCursor(5, 115);
  tft.println("Initializing");
  delay(750);
  tft.setCursor(75, 115);
  tft.println(" .");
  delay(750);
  tft.setCursor(85, 115);
  tft.println(" .");
  delay(750);
  tft.setCursor(95, 115);
  tft.println(" .");
  delay(1250);

  // Screen Refresh for Menu
  for(int i = 0; i <= 260; i++)
    for(int j = 0; j <= 160; j++) {
      tft.drawPixel(j, i, 0xC000 + (50 * i));
      tft.drawPixel(j, i-5, 0xC01F);
    }
}

///////////////////////////////////////////////////////////////////////////////////////

void intialize_menu() {
  tft.setTextColor(0xFFFF);

  tft.setTextSize(2);
  tft.setCursor(55, 5);
  tft.println("MENU");
  tft.drawLine(55, 21, 100, 21, 0xFFFF);
  tft.drawLine(55, 22, 100, 22, 0xFFFF);
  tft.drawLine(55, 23, 100, 23, 0xFFFF);
  tft.fillTriangle(140, 45, 145, 35, 150, 45, 0xFFFF);
  tft.fillCircle(145, 77, 5, 0xFFFF);
  tft.fillTriangle(140, 110, 145, 120, 150, 110, 0xFFFF);
}

bool poll_button () {
  button_press = digitalRead(BUTT_PRESS);
  if(!button_press) {
    while(!button_press)
      button_press = digitalRead(BUTT_PRESS);
    return true;
  }
  return false;
}

int poll_rotary_dial() {
  rotary_A = digitalRead(ROT_A);
  rotary_B = digitalRead(ROT_B);
  if(!rotary_A) {
    if(!rotary_B){
      while (!digitalRead(ROT_B) || !digitalRead(ROT_A))
        delay(1);
      return 1;
    }
    else if(rotary_B) {
      while(!digitalRead(ROT_A) || !digitalRead(ROT_B))
        delay(1);
      return -1;
    }
    return 0;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////


void menu_options(int menu_type, int region, String heading, int time_min, int temp) {
  switch (menu_type) { // Allows the user to view various menus with menu-specific text regions

    case 1: // Curing Settings Menu
      switch (region) {
        case 1:
          tft.fillRect(10, 30, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 30);
          tft.println(heading);
          break;
        case 2:
          tft.fillRect(10, 50, 125, 55, 0x8812);
          tft.setTextColor(0xFFFF);
          tft.setCursor(15, 55);
          tft.println(heading);
          tft.setTextSize(1);
          tft.setCursor(50, 75);
          tft.println("Time:");
          tft.setCursor(85, 75);
          tft.println(time_min);
          tft.setCursor(105, 75);
          tft.println("min");
          tft.setCursor(50, 90);
          tft.println("Temp:");
          tft.setCursor(85, 90);
          tft.println(temp);
          tft.setCursor(105, 90);
          tft.println("*F");
          tft.setTextSize(2);
          break;
        case 3:
          tft.fillRect(10, 110, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 110);
          tft.println(heading);
          break;
      break;
    }
    break;

    case 2: // Custom/Preset Time Menu
      switch (region) {
        case 1:
          tft.fillRect(10, 30, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 30);
          tft.println("Set Time:");
          break;
        case 2:
          tft.fillRect(10, 50, 125, 55, 0x8812);
          tft.setTextColor(0xFFFF);
          tft.setCursor(15, 70);
          tft.println(time_min);
          break;
        case 3:
          tft.fillRect(10, 110, 125, 17, 0xC01F);
          break;
        }
        break;

    case 3: // Custom/Preset Temp Menu
      switch (region) {
        case 1:
          tft.fillRect(10, 30, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 30);
          tft.println("Set Temp:");
          break;
        case 2:
          tft.fillRect(10, 50, 125, 55, 0x8812);
          tft.setTextColor(0xFFFF);
          tft.setCursor(15, 70);
          tft.println(temp);
          break;
        case 3:
          tft.fillRect(10, 110, 125, 17, 0xC01F);
          break;
        }
        break;

    case 4: // Confirm Settings + Pause Menu
      switch (region) {
        case 1:
          tft.fillRect(10, 30, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 30);
          tft.println(heading);
          break;
        case 2:
          tft.fillRect(10, 50, 125, 55, 0x8812);
          tft.setTextColor(0xFFFF);
          tft.setCursor(15, 70);
          tft.println(heading);
          break;
        case 3:
          tft.fillRect(10, 110, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 110);
          tft.println(heading);
          break;
        }
        break;

        default: // ROM Test Screen
      switch (region) {
        case 1:
          tft.fillRect(10, 30, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 30);
          tft.println("ROM Test");
          break;
        case 2:
          tft.fillRect(10, 50, 125, 55, 0x8812);
          tft.setTextColor(0xFFFF);
          tft.setCursor(15, 70);
          tft.println(time_min);
          break;
        case 3:
          tft.fillRect(10, 110, 125, 17, 0xC01F);
          tft.setTextColor(0xFFFF);
          tft.setCursor(10, 110);
          tft.println(" ");
          break;
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////


void menu_1(int option) { // Curing Settings Menu
  switch (option) {
    case 1: menu_options(1, 1, " ", 0, 0);
            menu_options(1, 2, "Regular", regResin[0], regResin[1]);
            menu_options(1, 3, "Tough", 0, 0);
            break;
    case 2: menu_options(1, 1, "Regular", 0, 0);
            menu_options(1, 2, "Tough", toughResin[0], toughResin[1]);
            menu_options(1, 3, "High Temp", 0, 0);
            break;
    case 3: menu_options(1, 1, "Tough", 0, 0);
            menu_options(1, 2, "High Temp", highResin[0], highResin[1]);
            menu_options(1, 3, "Tough", 0, 0);
            break;
    case 4: menu_options(1, 1, "High Temp", 0, 0);
            menu_options(1, 2, "Preset 1", 93, 93);
            menu_options(1, 3, "Preset 2", 0, 0);
            break;
    case 5: menu_options(1, 1, "Preset 1", 0, 0);
            menu_options(1, 2, "Preset 2", 94, 94);
            menu_options(1, 3, "Custom Set", 0, 0);
            break;
    default:menu_options(1, 1, "Preset 2", 0, 0);
            menu_options(1, 2, "Custom Set", 95, 95);
            menu_options(1, 3, " ", 0, 0);
            break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////

void menu_2(int time_min) { // Custom/Preset Time Menu
  menu_options(2, 1, " ", 0, 0);
  menu_options(2, 2, " ", time_min, 0);
  menu_options(2, 3, " ", 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////

void menu_3(int temp) { // Custom/Preset Temp Menu
  menu_options(3, 1, " ", 0, 0);
  menu_options(3, 2, " ", 0, temp);
  menu_options(3, 3, " ", 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////

void menu_4(int option) { // Confirm Settings Menu

  switch (option) {
    case 1: menu_options(4, 1, " ", 0, 0);
            menu_options(4, 2, "Confirm", 0, 0);
            menu_options(4, 3, "Return", 0, 0);
            break;
    default:menu_options(4, 1, "Confirm", 0, 0);
            menu_options(4, 2, "Return", 0, 0);
            menu_options(4, 3, " ", 0, 0);
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////

void menu_pause(int option) { // Pause Menu Settings
  switch (option) {
    case 1: menu_options(4, 1, " ", 0, 0);
            menu_options(4, 2, "Resume", 0, 0);
            menu_options(4, 3, "Menu", 0, 0);
            break;
    default:menu_options(4, 1, "Resume", 0, 0);
            menu_options(4, 2, "Menu", 0, 0);
            menu_options(4, 3, " ", 0, 0);
            break;
  }
}



void rom_test(int rom_data) {
  menu_options(4, 1, " ", 0, 0);
  menu_options(5, 2, " ", rom_data, 0);
  menu_options(4, 3, " ", 0, 0);
}


///////////////////////////////////////////////////////////////////////////////////////



void display_menu(int menu_num, int option_num) {
  switch (menu_num) {
    case 0: menu_pause(option_num);
      break;
    case 1: menu_1(option_num);
      break;
    case 2: menu_2(option_num);
      break;
    case 3: menu_3(option_num);
      break;
    case 4: menu_4(option_num);
      break;
    default:rom_test(option_num);
      break;
  }

}

bool update_menu_options(int dial_value, bool button_value) {
  // Menu 1 = Options 1 - 6
  if(menu == 1) {
    if((option >= 1 && option < 6 && dial_value == 1) || (option > 1 && option <= 6 && dial_value == -1))
      option += dial_value;
    if(button_value) {
      if(option == 6)
        menu = 2;
      else
        menu = 4;
      option = 1;
    }
    button_value = false;
    dial_value = 0;
    return true;
  }

  // Menu 2 = Time 1 - 100
  if(menu == 2) {
    if((option >= 1 && option < 100 && dial_value == 1) || (option > 1 && option <= 100 && dial_value == -1))
      option += dial_value;
    if(button_value) {
      time_min = option;
      menu = 3;
      option = 1;
    }
    button_value = false;
    dial_value = 0;
    return true;
  }

  // Menu 3 = Temp 1 - 100
  if(menu == 3) {
    if((option >= 1 && option < 100 && dial_value == 1) || (option > 1 && option <= 100 && dial_value == -1))
      option += dial_value;
    if(button_value) {
      temp = option;
      menu = 4;
      option = 1;
    }
    button_value = false;
    dial_value = 0;
    return true;
  }

  // Menu 4 = Confirm + Return [Menu 1]
  if(menu == 4) {
    if((option == 1 && dial_value == 1) || (option == 2 && dial_value == -1))
      option += dial_value;
    if(option == 1 && button_value == true) {
      menu = 5;
      option = 1;
    }
    if(option == 2 && button_value == true) {
      menu = 1;
      option = 1;
    }
    button_value = false;
    dial_value = 0;
    return true;
  }

if (menu == 5) {
  cureTrigger = 1;
}

  // Pause Menu = Resume + Menu [Menu 1]
  if(menu == 6) {
    if((option == 1 && dial_value == 1) || (option == 2 && dial_value == -1))
      option -= dial_value;
    if(option == 1 && button_value == true) {
      menu = 0;
      option = 1;
    }
    if(option == 2 && button_value == true) {
      menu = 1;
      option = 1;
    }
    button_value = false;
    dial_value = 0;
    return true;
  }
  return false;
}

void update_display() {
  dial_value = poll_rotary_dial();
  button_value = poll_button();
  if((dial_value != 0) || button_value)
    if(update_menu_options(dial_value, button_value))
      display_menu(menu, option);
}
