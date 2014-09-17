/*
 * test-dm.cpp

 *
 *  Created on: Sep 11, 2014
 *      Author: peter
 */

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../../distancemeter/distancemeter.hpp"
#include <iostream>
using namespace dm;

int main() {
	DistanceMeter dm("/dev/ttyUSB0");

	int key = 0;
	while (key != 27) {
		if (!dm.measure(0)) {
			std::cout << "write error" << std::endl;
		}
		key = cv::waitKey(1);
		if (dm.read()) {
			std::cout << dm.getRawDistance() << ", " <<
				dm.getDistanceInMMeter() << std::endl;
		}
		else {
			std::cout << "read error" << std::endl;
		}
	}

}
