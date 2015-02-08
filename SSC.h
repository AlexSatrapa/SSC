#ifndef HONEYWELL_SSC_H
#define HONEYWELL_SSC_H

//  Author: Tomas Van Verrewegen <tomasvanverrewegen@telenet.be>
//  Version: 0.2

// From https://code.google.com/p/arduino-ssc/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
class SSC
{
  public:

  enum Error
  {
    NoError                 = 0,
    ConnectionError         = 1,
    CommunicationError      = 2,
    NotRunningError         = 3,
    DiagnosticError         = 4,
    CommandModeError        = 5,
    ErrorMask               = 15
  };

  enum Flag
  {
    RunningFlag             = 128,
    FlagsMask               = ~ErrorMask
  };

  SSC(uint8_t address, uint8_t powerPin = 255);

  uint8_t address() const { return a; }
  uint8_t powerPin() const { return q; }

  uint8_t error() const { return f & ErrorMask; }
  uint8_t flags() const { return f & FlagsMask; }

  bool isRunning() const { return f & RunningFlag; }

  // SPI specifics
  void setSPImode(uint8_t mode) { data_mode = mode; }
  void setBitMode(uint8_t order) { bit_order = order; }
  void setClockDiv(uint8_t div) { clock_div = div; }
  void setTemperatureCompensated(boolean comp) { temp_comp = comp; }
  boolean is_compensated() const {
    return temp_comp;
  }

  //  minimum pressure of the sensor (in bar, Pascal, ...)
  float minPressure() const { return pmin; }
  void setMinPressure(float p) { pmin = p; }

  //  maximum pressure of the sensor (in bar, Pascal, ...)
  float maxPressure() const { return pmax; }
  void setMaxPressure(float p) { pmax = p; }

  //  minimum reading of the sensor (see datasheet)
  uint16_t minRaw() const { return rmin; }
  void setMinRaw(uint16_t raw) { rmin = raw; }

  //  maximum reading of the sensor (see datasheet)
  uint16_t maxRaw() const { return rmax; }
  void setMaxRaw(uint16_t raw) { rmax = raw; }

  //  return pressure and temperature
  uint16_t pressure_Raw() const { return p; }
  uint16_t temperature_Raw() const { return t; }
  float pressure() const { return rawToPressure(pressure_Raw()); }
  float temperature() const {
    if (is_compensated()) {
      return rawToTemperature(temperature_Raw());
    }
    else {
      return 0;
    }
  }

  //  start / stop the device
  uint8_t start();
  uint8_t stop();

  //  update pressure and temperature
  uint8_t update();

  //  convert pressure and temperature
  float rawToPressure(uint16_t raw) const { return rawToPressure(raw, rmin, rmax, pmin, pmax); }
  static float rawToPressure(uint16_t raw, uint16_t rawMin, uint16_t rawMax, float pMin, float pMax) {
    return (float(raw - rawMin) * (pMax - pMin)) / (rawMax - rawMin) + pMin;
    }
  static float rawToTemperature(uint16_t raw) {
    return float(raw) * 200.0 / 2047 - 50.0;
    }

  //  control the sensor from a stream
  uint8_t commandRequest(Stream& stream);

  private:

  uint8_t setError(uint8_t error) { f = (f & FlagsMask) | error; return error; }
  uint8_t commandReply(Stream& stream, uint8_t result) { stream.println(result); return result; }
  template<typename T> uint8_t commandReply(Stream& stream, uint8_t result, const T& data) { stream.println(data); return result; }

  uint8_t a;
  uint8_t q;
  uint8_t f;
  uint16_t p;
  uint16_t t;
  uint16_t rmin;
  uint16_t rmax;
  float pmin;
  float pmax;
  uint8_t data_mode;
  uint8_t bit_order;
  // HSC clock speed is 50-800kHz
  // For a 16MHz Arduino:
  //   SPI_CLOCK_DIV128  125kHz
  //   SPI_CLOCK_DIV64   250kHz
  uint8_t clock_div;
  boolean temp_comp; // Remember, only the HSC modules are temperature compensated

};

#endif
