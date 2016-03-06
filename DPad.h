#ifndef DPAD_H
#define DPAD_H

class DPad {
public:
  DPad(byte up, byte center, byte down, byte actState) 
  : mUp(up), mCenter(center), mDown(down), mActState(actState) {}
  
  void init(void) {
    pinMode(mUp, INPUT);
    pinMode(mCenter, INPUT);
    pinMode(mDown, INPUT);
  }

  byte isUp(void){
    return isAct(mUp);
  }

  byte isCenter(void){
    return isAct(mCenter);
  }

  byte isDown(void){
    return isAct(mDown);
  }

private:
  byte isAct(byte btn) {
    return digitalRead(btn) == mActState;
  }
  
  byte mUp, mCenter, mDown, mActState;
};

#endif //DPAD_H