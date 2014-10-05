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
#include "cli/cli.hpp"
#include <cstdio>

/*
int omain() {

	std::cout << "START" << std::endl;
	ns::NumberScanner ns(0);
	dm::DistanceMeter dm("/dev/ttyUSB0");
	ir::ImageReader ir(1);

	for(int i = 6; i < 98; i++) {
		dm.measure(3);

		char buf[100];
		std::sprintf(buf, "%08d.png", i);
		std::string fileName(buf);

		ns::ScannedValue sv = ns.scanFile(fileName);

		if ((sv.error == 0) && sv.scan && dm.read() && ir.readCamera()) {
			int distance = dm.getRawDistance();

			int j = 1;
			while ((distance == 0) && (j < 10)) {
				dm.measure(0);
				distance = dm.getRawDistance();
				j++;
			}
			if (j >= 10) {
				std::cerr << "Distancemeter got 0, I gave it up" << std::endl;
				continue;
			}

			sprintf(buf, "tcam_%03.1f_%d.png", sv.value, distance);
			std::string fn(buf);
			ir.saveImage(fn);
		}
	}

	std::cout << "END" << std::endl;
}
*/

int readDistance(dm::DistanceMeter &dm, int times = 0, bool measure_OK = false) {
	if (dm::ERROR_TEST_MODE == dm.error) {
		return -1;
	}

	bool success = dm.read();
	std::cout << "dm.read - success:" << success << std::endl;

	int distance = -1;

	if (success) {
		distance = dm.getRawDistance();
		std::cout << "0 distance " << distance << std::endl;


		int j = 1;
		while ((distance == 0) && (j < 10)) {
			success = false;
			bool measure_OK = dm.measure(0);
			std::cout << j << " - dm.measure_OK" << measure_OK << std::endl;

			if (measure_OK) success = dm.read();
			std::cout << j << " - dm.read success: " << success << std::endl;
			if (measure_OK && success) distance = dm.getRawDistance();
			std::cout << j << " - distance:" << distance << std::endl;
			j++;
		}
		if (j >= 10) {
			std::cerr << "Distancemeter got 0, I gave it up" << std::endl;
			return -1;
		}
	}

	return distance;
}

int main(int argc, const char** argv) {

	CLI cli;
	int ir_cam = -10, ns_cam = -10, fps = 15;
	std::string dm_dev("");
	if (!cli.parse(argc, argv, dm_dev, ir_cam, ns_cam, fps)) {
		return 0;
	}

	bool do_dm = (dm_dev.compare("") != 0);
	bool do_ir = (ir_cam >= 0);
	bool do_ns = (ns_cam >= 0);

	printf("ns_cam: %d, ir_cam:%d\n", ns_cam, ir_cam);

	ir::ImageReader ir(ir_cam);
	ir::ImageReader ir_ns(ns_cam);
	std::cout << "ir_cam" << ir_cam << std::endl;

	ns::NumberScanner ns(ir_ns);
	dm::DistanceMeter dm(dm_dev);

	cv::namedWindow("camera", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("display", cv::WINDOW_AUTOSIZE);

	ns::ScannedValue sv;
	bool measure_OK = false;
	int distance = -1;
	std::cout << "dm init - error:" << dm.error << std::endl;
	if (do_dm) measure_OK = dm.measure(0);

	while (true) {

		if (do_ns) sv = ns.scanCamera();
		if (do_ir) ir.readCamera();

		if (!do_ns) cv::waitKey(10);
		if (!do_ir) cv::waitKey(10);

		if (do_dm && measure_OK) {
			bool success = dm.read();
			if (success) {
				distance = dm.getRawDistance();
			}
			else {
				std::cout << "dm readDistance - error:" << dm.error << std::endl;
			}
		}

		if (do_dm) measure_OK = dm.measure(3);

		printf("Distance: %d, Temperature: %03.1f\n", distance, sv.value);

		if (do_ns) cv::imshow("display", ns.getBinaryImage());
		if (do_ir) cv::imshow("camera", ir.getImage());
		cv::waitKey(30);

	}

	std::cout << "END" << std::endl;
}

