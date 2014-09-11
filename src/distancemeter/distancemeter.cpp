/*
 * distancemeter.cpp
 *
 *  Created on: Sep 10, 2014
 *      Author: peter
 */

#ifndef DISTANCEMETER_CPP_
#define DISTANCEMETER_CPP_

#include "../third-party/arduino-serial-lib.h"

DistanceMeter::DistanceMeter(std::string usbPort) {
	this->usbPort = usbPort;
	done = false;
	rawDistance = 0;
	strcpy(serialport, usbPort.c_str());
	fd = serialport_init(serialport, baudrate);
	if (fd == -1)
	{
		std::cerr << "can not open port:" << "" << std::endl;
		error = dm::ERROR_PORT_OPEN;
	}
}

void DistanceMeter::measure() {
	serialport_flush(fd);
	serialport_write(fd, START_MEASURE);
}

bool DistanceMeter::read() {
	char * buf = new char[buf_max]; //
	buf[0] = 0;
	bool success = false;

	serialport_read_until(fd, buf, eolchar, buf_max, timeout);

	if (buf[0] == 0) {
		error = dm::ERROR_READ;
	}
	else {
		rawDistance = atoi(buf);
		success = true;
	}

	return success;
}

int DistanceMeter::getRawDistance() {
	return rawDistance;
}

double DistanceMeter::getDistanceInMeter() {
	return (rawDistance * 100.0) / 54.0;
}


#endif /* DISTANCEMETER_CPP_ */
