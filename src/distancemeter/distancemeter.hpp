/*
 * distancemeter.hpp
 *
 *  Created on: Sep 10, 2014
 *      Author: peter
 */

#ifndef DISTANCEMETER_HPP_
#define DISTANCEMETER_HPP_

namespace dm {
	const int ERROR_PORT_OPEN = 1;
	const int ERROR_READ = 2;
}
const char* START_MEASURE = 'S';

class DistanceMeter
{
public:
	int error = 0;
	DistanceMeter(std::string usbPort);
	void measure();
	bool read();
	int getRawDistance();
	double getDistanceInMeter();
private:
	int rawDistance = 0;
	std::string usbPort;
	bool done = false;

	const int buf_max = 256;
	int fd = -1;
	char * serialport = new char[buf_max];
	int baudrate = 9600;  // default
	char eolchar = '\n';
	int timeout = 5000;

};

#endif /* DISTANCEMETER_HPP_ */
