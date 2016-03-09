#ifndef DICTIONARY_EN_
#define DICTIONARY_EN_

//------------------------------------
//------------------------------------"________________"
const char strWakeUp[]      PROGMEM = "Wake up...";
const char strMatrixHas[]   PROGMEM = "Matrix has you..";
const char strFollow[]      PROGMEM = "Follow the";
const char strWhiteRabbit[] PROGMEM = "     WhiteRabbit";

const char* const DICTIONARY[] PROGMEM = {
  strWakeUp,               //0
  strMatrixHas,            //1
  strFollow,               //2
  strWhiteRabbit           //3
};

#define KEY_WAKE_UP              0
#define KEY_MATRIX_HAS           1
#define KEY_FOLLOW               2
#define KEY_WHITE_RABBIT         3

#define SERVICE_STRING           254
#define SERVICE_ERROR            255

#endif //DICTIONARY_EN_