#include <Wire.h>
#include <Arduino.h>
#include <BitsyBits.h>

SSD1306 mDisplay(&Wire);
ConsoleView mConsoleView(&mDisplay);
ConsoleController mConsole(&mConsoleView);

Scheduler mScheduler;
Task taskConsole(300, -1, &taskConsoleCallback);
Task taskAddDot(2000, -1, &taskAddDotCallback);

void setup() {
  Serial.begin(9600);
  Wire.begin(5, 4);
  Wire.setClock(400000); 
  mDisplay.init();
  mDisplay.clear();
  mDisplay.display();
  mDisplay.flipScreenVertically();
  mDisplay.setFontScale2x2(false);

  mScheduler.init();
  mScheduler.addTask(taskConsole);
  mScheduler.addTask(taskAddDot);
  taskConsole.enable(); 
  taskAddDot.enable();

  mConsole.print(KEY_WAKE_UP);
  mConsole.print(KEY_MATRIX_HAS);
  mConsole.print(KEY_FOLLOW);
  mConsole.print(KEY_WHITE_RABBIT);
}

void loop() {
  mScheduler.execute();
}

void taskConsoleCallback() {
  mConsole.execute();
}

void taskAddDotCallback() {
  mConsole.print(".");
}
