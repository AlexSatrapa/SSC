#ifndef HONEYWELL_SSC_H
#define HONEYWELL_SSC_H

// Based on code originally by Tomas Van Verrewegen <tomasvanverrewegen@telenet.be>
// From https://code.google.com/p/arduino-ssc/

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

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

  SSC();

  byte address() const { return i2c_address; }
  byte powerPin() const { return power_pin; }

  byte error() const { return conditions & ErrorMask; }
  byte flags() const { return conditions & FlagsMask; }
  void setFlag( Flag flag );
  void clearFlag( Flag flag );

  bool powerControlled = false;
  bool isRunning() const { return conditions & RunningFlag; }
  void setPowerPin( byte pin );

  // HSC devices are temperature compensated
  void setTemperatureCompensated(boolean is_compensated) { temperature_compensated = is_compensated; }
  boolean isCompensated() const {
    return temperature_compensated;
  }

  //  minimum pressure of the sensor (in bar, Pascal, ...)
  float minPressure() const { return pmin; }
  void setMinPressure(float p) { pmin = p; }

  //  maximum pressure of the sensor (in bar, Pascal, ...)
  float maxPressure() const { return pmax; }
  void setMaxPressure(float p) { pmax = p; }

  //  minimum reading of the sensor (see datasheet)
  int minRaw() const { return rmin; }
  void setMinRaw(int raw) { rmin = raw; }

  //  maximum reading of the sensor (see datasheet)
  int maxRaw() const { return rmax; }
  void setMaxRaw(int raw) { rmax = raw; }

  //  return pressure and temperature
  int rawPressure() const { return pressure_count; }
  int rawTemperature() const { return temperature_count; }
  float pressure() const { return rawToPressure(rawPressure()); }
  float temperature() const {
    if (isCompensated()) {
      return rawToTemperature(rawTemperature());
    }
    else {
      return 0;
    }
  }

  //  start / stop the device
  virtual byte start() = 0;
  virtual byte stop() = 0;

  //  update pressure and temperature
  byte update();

  //  convert pressure and temperature
  float rawToPressure(int raw) const { return rawToPressure(raw, rmin, rmax, pmin, pmax); }
  static float rawToPressure(int raw, int rawMin, int rawMax, float pMin, float pMax) {
    return (float(raw - rawMin) * (pMax - pMin)) / (rawMax - rawMin) + pMin;
    }
  static float rawToTemperature(int raw) {
    return float(raw) * 200.0 / 2047 - 50.0;
    }

  private:

  virtual void read(byte buf[]) = 0;
  byte setError(byte error) { conditions = (conditions & FlagsMask) | (error & ErrorMask); return error; }

  byte i2c_address;
  byte power_pin;
  byte conditions;
  int pressure_count;
  int temperature_count;
  int rmin;
  int rmax;
  float pmin;
  float pmax;
  byte data_mode;
  byte bit_order;
  // HSC clock speed is 50-800kHz
  // For a 16MHz Arduino:
  //   SPI_CLOCK_DIV128  125kHz
  //   SPI_CLOCK_DIV64   250kHz
  byte clock_div;
  boolean temperature_compensated = false; // Remember, only the HSC modules are temperature compensated

};

#include "HSCDANN005PGSA5.h"

#endif
// -*- coding: utf-8; tab-width: 3; indent-tabs-mode: nil -*-
