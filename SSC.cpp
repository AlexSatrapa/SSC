#include "SSC.h"
// From https://code.google.com/p/arduino-ssc/

// FIXME: Not sure why this is needed, but if it's not here I get:
//
// Arduino: 1.6.5 (Mac OS X), Board: "ATtinyx41, ATtiny841, 16 MHz (external), B.O.D. Disabled"
//
// SSC/SSC.cpp.o:(.rodata._ZTV3SSC[vtable for SSC]+0x4): undefined reference to `__cxa_pure_virtual'
// SSC/SSC.cpp.o:(.rodata._ZTV3SSC[vtable for SSC]+0x6): undefined reference to `__cxa_pure_virtual'
// SSC/SSC.cpp.o:(.rodata._ZTV3SSC[vtable for SSC]+0x8): undefined reference to `__cxa_pure_virtual'
// collect2: error: ld returned 1 exit status
// Error compiling.
//
// Workaround: http://stackoverflow.com/questions/920500/what-is-the-purpose-of-cxa-pure-virtual

extern "C" void __cxa_pure_virtual() { while (1); }

SSC::SSC()
{
  power_pin = 0;
}

byte SSC::update()
{
  byte x, y, v, w, s;
  byte buf[4];

  read(buf);
  x = buf[0];
  y = buf[1];
  v = buf[2];
  w = buf[3];
  s = x >> 6;

  switch(s) {
    case 0:
      pressure_count = (((int) (x & 0x3f)) << 8) | y;
      temperature_count = ((((int) v) << 8) | w) >> 5;
      return setError(NoError);

    case 2:
      break;

    case 1:
      return setError(CommandModeError);

    case 3:
      return setError(DiagnosticError);
  }
  return setError(DiagnosticError);
}

void SSC::setPowerPin( byte pin )
{
  power_pin = pin;
  powerControlled = true;
}

void SSC::setFlag( Flag flag )
{
  conditions |= flag;
}

void SSC::clearFlag( Flag flag )
{
  conditions &= ~flag;
}
