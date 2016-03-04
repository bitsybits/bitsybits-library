#ifndef SCANNER_I2C_H
#define SCANNER_I2C_H

#include <Arduino.h>
#include <Wire.h>

class I2CScanner{
public:
  static void scan(TwoWire* twoWire, HardwareSerial* serial){
    serial->println("I2CScanner: begin");
    for(byte address = 1; address < 127; ++address) 
    {
      twoWire->beginTransmission(address);
      switch(twoWire->endTransmission()){
        case 0:
        serial->print("I2CScanner: found on 0x");
        serial->print(address < 16 ? "0" : "");
        serial->println(address, HEX);
        break;
        case 4:
        serial->print("I2CScanner: error on 0x");
        serial->print(address < 16 ? "0" : "");
        serial->println(address, HEX);
        break;
        default:
        break;
      }
    }
    serial->println("I2CScanner: end");
  }
};

#endif //SCANNER_I2C_H