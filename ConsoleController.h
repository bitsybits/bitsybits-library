#ifndef CONSOLE_CONTROLLER_H
#define CONSOLE_CONTROLLER_H

#include "queue.h"
#include "ConsoleDictionary.h"
#include "ConsoleView.h"

class ConsoleController : public IExecutor
{
public:
  ConsoleController(ConsoleView* display);
  void print(unsigned char dictKey);
  void print(String str);
  virtual uint8_t execute();

private:
  ConsoleView* mpConsole;
  Queue<unsigned char> mKeyQueue;
  Queue<String> mStrQueue;
};

ConsoleController::ConsoleController(ConsoleView* console): mpConsole(console){
}

void ConsoleController::print(unsigned char dictKey){
  mKeyQueue.push(dictKey);
}

void ConsoleController::print(String str){
  mKeyQueue.push(SERVICE_STRING);
  mStrQueue.push(str);
}

uint8_t ConsoleController::execute(){
  if(!mKeyQueue.isEmpty()) {
    unsigned char key = SERVICE_ERROR;
    while((key = mKeyQueue.pop(SERVICE_ERROR)) != SERVICE_ERROR) {
      if(key == SERVICE_STRING) {
        if(!mStrQueue.isEmpty()) {
          mpConsole->println(mStrQueue.pop("").c_str());
        }
      } else {
        mpConsole->printlnPGM(DICTIONARY[key]);
      }
    }
    mpConsole->redraw();
  }
  return 0;
}

#endif //CONSOLE_CONTROLLER_H