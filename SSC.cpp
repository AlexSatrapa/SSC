#include "SSC.h"
// From https://code.google.com/p/arduino-ssc/

SSC::SSC()
{
  q = 0;
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
      p = (((int) (x & 0x3f)) << 8) | y;
      t = ((((int) v) << 8) | w) >> 5;
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
