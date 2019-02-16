#include "Curing_Chamber_Variables.h"

void setup(void) {
  pinMode(rot_A, INPUT);
  pinMode(rot_B, INPUT);
  pinMode(button, INPUT);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  startup_screen();
  intialize_menu();
  display_menu(menu, option);
}

void loop() {
  update_display();
}
