/*
 * distancemeter.hpp
 *
 *  Created on: Sep 10, 2014
 *      Author: peter
 */

#ifndef DISTANCEMETER_HPP_
#define DISTANCEMETER_HPP_

#include <string>

namespace dm {
	const int ERROR_PORT_OPEN = 1;
	const int ERROR_READ = 2;
}

class DistanceMeter
{
public:
	int error;
	DistanceMeter(std::string usbPort);
	~DistanceMeter();
	bool measure(unsigned short pingNumber);
	bool read();
	unsigned int getRawDistance();
	double getDistanceInMMeter();
private:
	unsigned int rawDistance;
	bool done;

	int buf_max;
	int fd;
	int baudrate;  // default
	char eolchar;
	int timeout;
	std::string START_MEASURE;

};

#endif /* DISTANCEMETER_HPP_ */
