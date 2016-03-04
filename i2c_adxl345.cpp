#include "i2c_adxl345.h"
#include "math.h"

LowPassFilter::LowPassFilter(double alpha) : mAlpha(alpha) {
  mPrevXYZ.x = mPrevXYZ.y = mPrevXYZ.z = 0.;
}

GenXYZ LowPassFilter::filter(GenXYZ data){
  filter(mPrevXYZ.x, data.x);
  filter(mPrevXYZ.y, data.y);
  filter(mPrevXYZ.z, data.z);
  return mPrevXYZ;
}

void LowPassFilter::filter(double& prev, double& cur){
  prev = cur * mAlpha + (prev * (1.0 - mAlpha));
}

ADXL345::ADXL345(TwoWire* twoWire, double alphaLowPass) : mTwoWire(twoWire), mFilter(alphaLowPass) {
}

void ADXL345::init(){
  // FYI: 0x00 = 2G, 0x01 = 4G, 0x02 = 8G, 0x03 = 16G
  write(DATA_FORMAT, 0x00);
  // Measurement Mode = 0x08
  write(POWER_CTL, 0x08);
}

void ADXL345::write(byte addr, byte value){
  mTwoWire->beginTransmission(ADXL345_ADDR);
  mTwoWire->write(addr); 
  mTwoWire->write(value);
  mTwoWire->endTransmission();
}

void ADXL345::read(byte addr, int toRead, byte* buf){
  mTwoWire->beginTransmission(ADXL345_ADDR); // start transmission to device
  mTwoWire->write(addr); 
  mTwoWire->endTransmission(); 
  mTwoWire->beginTransmission(ADXL345_ADDR); 
  mTwoWire->requestFrom(ADXL345_ADDR, toRead);

  for(int i = 0; mTwoWire->available(); ++i){
    buf[i] = mTwoWire->read();
  }

  mTwoWire->endTransmission();
}

double ADXL345::convertRawToGen(short val){
  return val * 2. / 512.; //where 2. = Range; 512. = 2 ^ (Resolution - 1)
}

RawXYZ ADXL345::getRaw(){
  byte buf[6];
  read(DATAX0, 6, buf);
  
  // each axis in 10 bit resolution, ie 2 bytes.
  // esp8266 is 32bit, use short not int
  RawXYZ raw;
  raw.x = (((short)buf[1]) << 8) | buf[0];
  raw.y = (((short)buf[3]) << 8) | buf[2];
  raw.z = (((short)buf[5]) << 8) | buf[4];

  return raw;
}

GenXYZ ADXL345::getGeneralized(){
  RawXYZ raw = getRaw();
  GenXYZ gen;
  gen.x = convertRawToGen(raw.x);
  gen.y = convertRawToGen(raw.y);
  gen.z = convertRawToGen(raw.z);
  return gen;
}

GenXYZ ADXL345::getFiltered(){
  return mFilter.filter(getGeneralized());
}

Angels ADXL345::getAngels(){
  GenXYZ cur = getFiltered();
  Angels angels;
  angels.roll  = atan2(-cur.y, cur.z);
  angels.pitch = atan2(cur.x, sqrt(cur.y*cur.y + cur.z*cur.z));
  return angels;
}