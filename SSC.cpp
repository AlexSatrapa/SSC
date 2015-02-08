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

uint8_t SSC::commandRequest(Stream& stream)
{
  if(stream.available()) {
    switch(stream.read()) {
      //  get pressure
      case 'p':
        return commandReply(stream, 0, pressure());

      //  get temperature
      case 't':
        return commandReply(stream, 0, temperature());

        //  get i2c address
      case 'a':
        return commandReply(stream, 0, address());

        //  get power pin
      case 'q':
        return commandReply(stream, 0, powerPin());

        //  update
      case 'u':
        return commandReply(stream, update());

        //  start
      case '1':
        return commandReply(stream, start());

        //  stop
      case '0':
        return commandReply(stream, stop());

        //  get min pressure
      case 'b':
        return commandReply(stream, 0, minPressure());

        //  get max pressure
      case 'c':
        return commandReply(stream, 0, maxPressure());

        //  get min raw
      case 'd':
        return commandReply(stream, 0, minRaw());

        //  get max raw
      case 'e':
        return commandReply(stream, 0, maxRaw());

        //  set min pressure
      case 'B':
        if(stream.available()) {
          setMinPressure(stream.parseFloat());
          return commandReply(stream, 0);
        }
        return commandReply(stream, 1);

        //  set max pressure
      case 'C':
        if(stream.available()) {
          setMaxPressure(stream.parseFloat());
          return commandReply(stream, 0);
        }
        return commandReply(stream, 1);

        //  set min raw
      case 'D':
        if(stream.available()) {
          setMinRaw(stream.parseInt());
          return commandReply(stream, 0);
        }
        return commandReply(stream, 1);

        //  set max raw
      case 'E':
        if(stream.available()) {
          setMaxRaw(stream.parseInt());
          return commandReply(stream, 0);
        }
        return commandReply(stream, 1);
    }

    return commandReply(stream, 254);
  }
  return commandReply(stream, 255);
}
