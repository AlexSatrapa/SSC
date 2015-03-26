#include "SSC.h"
// From https://code.google.com/p/arduino-ssc/

SSC::SSC(uint8_t address, uint8_t powerPin)
  : a(address), q(powerPin), f(0), p(0), t(0), rmin(0), rmax(16383), pmin(0.0), pmax(1.6), data_mode(SPI_MODE0), bit_order(MSBFIRST), clock_div(SPI_CLOCK_DIV64)
{
  if(q < 255) {
    if(a) {
      digitalWrite(q, LOW);
    }
    else {
      digitalWrite(q, HIGH);
    }
    pinMode(q, OUTPUT);
  }
}

uint8_t SSC::start()
{
  if(!isRunning()) {
    if(q < 255) {
      if(a) {
        digitalWrite(q, HIGH);
      }
      else {
        SPI.begin();
        SPI.setDataMode(data_mode);
        SPI.setBitOrder(bit_order);
        SPI.setClockDivider(clock_div);
      }
    }
    f |= RunningFlag;
  }
  return setError(NoError);
}

uint8_t SSC::stop()
{
  if(isRunning()) {
    if(q < 255) {
      if(a) {
        digitalWrite(q, LOW);
      }
      else {
        SPI.end();
      }
    }
    f &= ~RunningFlag;
  }
  return setError(NoError);
}

uint8_t SSC::update()
{
  uint8_t x, y, v, w, s;

  if(a) { // If it has an address, it's an I2C device
    while(1) {
     Wire.requestFrom(a, (uint8_t) 4);
     if(Wire.available()) {
       x = Wire.read();
       y = Wire.read();
       v = Wire.read();
       w = Wire.read();
       Wire.endTransmission();
       s = x >> 6;

       switch(s) {
        case 0:
         p = (((uint16_t) (x & 0x3f)) << 8) | y;
         t = ((((uint16_t) v) << 8) | w) >> 5;
         return setError(NoError);

        case 2:
         Wire.endTransmission();
         break;

        case 1:
         Wire.endTransmission();
         return setError(CommandModeError);

        case 3:
         Wire.endTransmission();
         return setError(DiagnosticError);
      }
     }
    }
    return setError(DiagnosticError);
  }
  else { // Address of 0 means it's SPI
    digitalWrite(q, LOW);
    delay(1);
    s = 0xAA; // Useful bit sequence for debugging
    x = SPI.transfer(s);
    y = SPI.transfer(s);
    v = SPI.transfer(s);
    w = SPI.transfer(s);
    delay(1);
    digitalWrite(q, HIGH);
    s = x >> 6;
    switch(s) {
      case 0:
        p = (((uint16_t) (x & 0x3f)) << 8) | y;
        t = ((((uint16_t) v) << 8) | w) >> 5;
        return setError(NoError);

      case 2:
        break;

      case 1:
        return setError(CommandModeError);

      case 3:
        return setError(DiagnosticError);
    }
  }
  return setError(DiagnosticError);
}
