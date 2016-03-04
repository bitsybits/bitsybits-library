#ifndef I2C_ADXL345_H
#define I2C_ADXL345_H

#include <Arduino.h>
#include <Wire.h>

#define ADXL345_ADDR  (0x53)

#define POWER_CTL     0x2D
#define DATA_FORMAT   0x31
#define DATAX0        0x32
#define DATAX1        0x33
#define DATAY0        0x34
#define DATAY1        0x35
#define DATAZ0        0x36
#define DATAZ1        0x37

struct RawXYZ
{
  short x, y, z;
};

struct GenXYZ
{
  double x, y, z;
};

struct Angels
{
  double pitch, roll;
};

class LowPassFilter
{
public:
  LowPassFilter(double alpha);
  GenXYZ filter(GenXYZ data);
private:
  void filter(double& prev, double& cur);
  double mAlpha;
  GenXYZ mPrevXYZ;
};

class ADXL345
{
public:
  ADXL345(TwoWire* twoWire, double alphaLowPass = 0.2);
  void init();
  RawXYZ getRaw();
  GenXYZ getGeneralized();
  GenXYZ getFiltered();
  Angels getAngels();
private:
  TwoWire* mTwoWire;
  LowPassFilter mFilter;

  void write(byte addr, byte value);
  void read(byte addr, int toRead, byte* buf);
  double convertRawToGen(short val);
};

#endif //I2C_ADXL345_H