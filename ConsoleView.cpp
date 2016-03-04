#include "ConsoleView.h"

ConsoleView::ConsoleView(SSD1306* display): mpDisplay(display){
  mCurRowPos = mRealRowCount = ROW_MAX - ROW_FIT_COUNT;
}

void ConsoleView::draw(void){
  for (uint8_t i = 0; i < ROW_FIT_COUNT; ++i){
    mpDisplay->drawString(0, i*FONT_SIZE, (char *)(mScreen[i + mCurRowPos]));
  }
}

void ConsoleView::redraw(void){
  mpDisplay->clear();
  draw();
  mpDisplay->display();
}

void ConsoleView::incRow(void){
  if (mCurRowPos < mRealRowCount){
    mCurRowPos++;
    redraw();
  }
}

void ConsoleView::decRow(void){
  if (mCurRowPos > 0){
    mCurRowPos--;
    redraw();
  }
}

void ConsoleView::println(const char* str){
  size_t len = strlen(str);
  for (uint8_t i = 0; i < len; ++i){
    if (!(mCurRowCharPos < CHARS_IN_ROW)){
      execLine();
    }
    charToLine(str[i]);
  }
  execLine();
}

void ConsoleView::printlnPGM(const char* PSTR_str){
  size_t len = strlen_P(PSTR_str);
  for (uint8_t i = 0; i < len; ++i){
    if (!(mCurRowCharPos < CHARS_IN_ROW)){
      execLine();
    }
    charToLine(pgm_read_byte_near(PSTR_str + i));
  }
  execLine();
}

void ConsoleView::clear(void){
  clearScreen();
  mpDisplay->clear();
  mpDisplay->display();
}

void ConsoleView::clearScreen(void){
  uint8_t i, j;
  for (i = 0; i < ROW_MAX; i++){
    for (j = 0; j < CHARS_IN_ROW; j++){
      mScreen[i][j] = 0;
    }
  }
}

void ConsoleView::addLineToScreen(void){
  uint8_t i, j;
  for (j = 0; j < CHARS_IN_ROW; j++){
    for (i = 0; i < ROW_MAX - 1; i++){
      mScreen[i][j] = mScreen[i + 1][j];
    }
  }
  for (j = 0; j < CHARS_IN_ROW; j++){
    mScreen[ROW_MAX - 1][j] = mCurRowBuf[j];
  }
}

void ConsoleView::execLine(void){
  addLineToScreen();
  mCurRowPos = mRealRowCount; // sroll to the last row
  mCurRowBuf[mCurRowCharPos = 0] = '\0';
  redraw();
}

void ConsoleView::charToLine(uint8_t c){
  mCurRowBuf[mCurRowCharPos] = c;
  mCurRowBuf[++mCurRowCharPos] = '\0';
}