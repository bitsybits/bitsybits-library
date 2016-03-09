#include <Wire.h>
#include <Arduino.h>
#include <BitsyBits.h>

SSD1306 mDisplay(&Wire);
ConsoleView mConsoleView(&mDisplay);
ConsoleController mConsole(&mConsoleView);

bt::TaskScheduler mScheduler;
bt::SchedulerTask taskConsole(&mConsole);
bt::SchedulerTask taskAddDot([] { mConsole.print("."); });

void setup() {
  Serial.begin(9600);
  Wire.begin(5, 4);
  Wire.setClock(400000); 
  mDisplay.init();
  mDisplay.clear();
  mDisplay.display();
  mDisplay.flipScreenVertically();
  mDisplay.setFontScale2x2(false);

  mScheduler.push(&taskConsole)->push(&taskAddDot);
  taskConsole.attach(100, true);
  taskAddDot.attach(1000, true);

  mConsole.print(KEY_WAKE_UP);
  mConsole.print(KEY_MATRIX_HAS);
  mConsole.print(KEY_FOLLOW);
  mConsole.print(KEY_WHITE_RABBIT);
}

void loop() {
  mScheduler.execute();
}
