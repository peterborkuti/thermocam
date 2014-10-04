/*
 * distancemeter.cpp
 *
 *  Created on: Sep 10, 2014
 *      Author: peter
 */

#ifndef DISTANCEMETER_CPP_
#define DISTANCEMETER_CPP_

#include "../third-party/arduino-serial-lib.h"
#include "distancemeter.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace dm
{
DistanceMeter::DistanceMeter(std::string usbPort)
{

	done = false;
	rawDistance = 0;

	error = 0;

	buf_max = 256;
	fd = -1;
	baudrate = 115200;  // default
	eolchar = '\n';
	timeout = 15000;

	if (usbPort.compare("") == 0) {
		error = ERROR_TEST_MODE;
		return;
	}

	serialPort = usbPort;

	openPort();

}

DistanceMeter::~DistanceMeter()
{
	if (fd != -1)
	{
		serialport_close(fd);
	}
}

void DistanceMeter::openPort() {
	const char * serialport = new char[buf_max];

	serialport = serialPort.c_str();

	fd = serialport_init(serialport, baudrate);
	if (fd == -1)
	{
		std::cerr << "can not open port:" << serialPort << std::endl;
		error = dm::ERROR_PORT_OPEN;
		rawDistance = -1;
	}
	else
	{
		serialport_flush(fd);
	}
}

bool DistanceMeter::measure(uint8_t pingNumber)
{
	if (error == ERROR_TEST_MODE) {
		return true;
	}

	if (error == ERROR_PORT_OPEN) {
		openPort();
	}

	if (error != 0) {
		return false;
	}

	int rc = 0;

	error = 0;

	rc = serialport_writebyte(fd, pingNumber);

	if (rc == -1) {
		error = ERROR_WRITE;
		rawDistance = -1;
	}

	return (rc != -1);
}

bool DistanceMeter::read()
{
	if (error == ERROR_TEST_MODE) {
		return true;
	}

	rawDistance = -1;

	if (error != 0) {
		return false;
	}

	char buf[buf_max]; //
	bool success = false;

	memset(buf, 0, buf_max);
	serialport_read_until(fd, buf, eolchar, buf_max, timeout);

	if (buf[0] == 0)
	{
		error = dm::ERROR_READ;
	}
	else
	{
		rawDistance = std::atol(buf);
		success = true;
	}

	return success;
}

unsigned int DistanceMeter::getRawDistance()
{
	return rawDistance;
}

double DistanceMeter::getDistanceInMMeter()
{
	return (rawDistance * 10.0) / 54.0;
}

} //end of namespace

#endif /* DISTANCEMETER_CPP_ */
