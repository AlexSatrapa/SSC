#include <HSCDANN005PGSA5.h>

HSCDANN005PGSA5::HSCDANN005PGSA5(byte slaveSelectPin)
{
	setMinRaw(1638);
	setMaxRaw(14746);
	setTemperatureCompensated(1);
	digitalWrite(slaveSelectPin, HIGH);
	pinMode(slaveSelectPin, OUTPUT);
	slave_select_pin = slaveSelectPin;
	spi_settings = SPISettings(800000, MSBFIRST, SPI_MODE0);
	SPI.begin();
}

void HSCDANN005PGSA5::read(byte buf[])
{
	byte s = 0xAA;

	SPI.beginTransaction(spi_settings);
	digitalWrite(slave_select_pin, LOW);
	buf[0] = SPI.transfer(s);
	buf[1] = SPI.transfer(s);
	buf[2] = SPI.transfer(s);
	buf[3] = SPI.transfer(s);
	delay(1);
	digitalWrite(slave_select_pin, HIGH);
	SPI.endTransaction();
}

uint8_t HSCDANN005PGSA5::start()
{
	digitalWrite(powerPin(), HIGH);
	delay(5);
	return 1;
}

uint8_t HSCDANN005PGSA5::stop()
{
	digitalWrite(powerPin(), LOW);
	return 1;
}

// -*- coding: utf-8; tab-width: 3; -*-
