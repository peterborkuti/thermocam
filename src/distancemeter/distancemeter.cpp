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
	const char * serialport = new char[buf_max];
	baudrate = 115200;  // default
	eolchar = '\n';
	timeout = 15000;

	serialport = usbPort.c_str();
	fd = serialport_init(serialport, baudrate);
	if (fd == -1)
	{
		std::cerr << "can not open port:" << usbPort << std::endl;
		error = dm::ERROR_PORT_OPEN;
	}
	else
	{
		serialport_flush(fd);
	}
}

DistanceMeter::~DistanceMeter()
{
	if (fd != -1)
	{
		serialport_close(fd);
	}
}

bool DistanceMeter::measure(unsigned short pingNumber)
{
	int rc = 0;
	rc = serialport_writebyte(fd, (uint8_t) pingNumber);
	return (rc != -1);
}

bool DistanceMeter::read()
{
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
