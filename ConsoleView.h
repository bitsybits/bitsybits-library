#ifndef CONSOLE_VIEW_H
#define CONSOLE_VIEW_H

#include "i2c_ssd1306.h"

#define ROW_MAX 16
#define ROW_FIT_COUNT 8 // HEIGHT/FONT_SIZE
#define CHARS_IN_ROW 16 // WIDTH/FONT_SIZE

class ConsoleView {
public:
  ConsoleView(SSD1306* display);
  void draw(void);
  void redraw(void);
  void incRow(void);
  void decRow(void);
  void println(const char* str);
  void printlnPGM(const char* PSTR_str);
  void clear(void);

private:
  void clearScreen(void);
  void addLineToScreen(void);
  void execLine(void);
  void charToLine(uint8_t c);

  SSD1306* mpDisplay;
  uint8_t mCurRowBuf[CHARS_IN_ROW + 1]; // + \0
  uint8_t mCurRowCharPos = 0;
  uint8_t mScreen[ROW_MAX][CHARS_IN_ROW + 1]; // + \0
  uint8_t mRealRowCount, mCurRowPos;
};

#endif //CONSOLE_VIEW_H