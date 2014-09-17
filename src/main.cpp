/*
 * main.cpp
 *
 *  Created on: Sep 9, 2014
 *      Author: peter
 */
#include <iostream>
#include "numberscanner/numberscanner.hpp"
#include "distancemeter/distancemeter.hpp"
#include "imagereader/imagereader.hpp"
#include <cstdio>


int main() {

	std::cout << "START" << std::endl;
	ns::NumberScanner ns(-1);
	dm::DistanceMeter dm("/dev/ttyUSB0");
	ir::ImageReader ir(1);

	for(int i = 6; i < 98; i++) {
		dm.measure(3);

		char buf[100];
		std::sprintf(buf, "%08d.png", i);
		std::string fileName(buf);

		ns::ScannedValue sv = ns.scanFile(fileName);
		std::cout << sv.scan << sv.hold << sv.value << std::endl;

		if ((sv.error == 0) && sv.scan && dm.read() && ir.readCamera()) {
			int distance = dm.getRawDistance();
			sprintf(buf, "tcam_%f_%d.png", sv.value, distance);
			std::string fn(buf);
			ir.saveImage(fn);
		}
	}

	std::cout << "END" << std::endl;
}

