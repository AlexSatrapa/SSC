#ifndef __HSCDANN005PGSA5_
#define __HSCDANN005PGSA5_
#include <SSC.h>
#include <SPI.h>

class HSCDANN005PGSA5: public SSC
{
	public:
		HSCDANN005PGSA5(byte slaveSelectPin);
		uint8_t start();
		uint8_t stop();
	protected:
		byte slave_select_pin;
	private:
		SPISettings spi_settings;
		void read(byte buf[]);
};

#endif
// -*- coding: utf-8; tab-width: 3; -*-
