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

int PREV_STATE = -1;
int STATE = -1;
int STATE_COUNTER = 0;
std::string stateStrings[] = {"start", "hold", "scan", "error"};

bool do_dm = false, do_ns = false, do_ir = false;

void initState(int state) {
	if (state != STATE) {
		STATE_COUNTER = 1;
		PREV_STATE = STATE;
		std::cout << "new state:" << stateStrings[state] << std::endl;
	}
	else {
		STATE_COUNTER++;
		std::cout << "in state:" << stateStrings[state] << ", "
			<< STATE_COUNTER <<std::endl;
	}
}

void stateStart() {
	initState(0);
}

void stateHold() {
	initState(1);
}

void stateScan() {
	initState(2);
}

void stateError() {
	initState(3);
}

void showPictures(cv::Mat const dspImg, cv::Mat camImg) {
	if (do_ns) cv::imshow("display", dspImg);
	if (do_ir) cv::imshow("camera", camImg);

	cv::waitKey(3);
}

int main(int argc, const char** argv) {

	CLI cli;
	int ir_cam = -10, ns_cam = -10, fps = 15;
	std::string dm_dev("");
	if (!cli.parse(argc, argv, dm_dev, ir_cam, ns_cam, fps)) {
		return 0;
	}

	do_dm = (dm_dev.compare("") != 0);
	do_ir = (ir_cam >= 0);
	do_ns = (ns_cam >= 0);

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
	double distance = -1;
	double storedDistance = -1;
	std::cout << "dm init - error:" << dm.error << std::endl;
	if (do_dm) measure_OK = dm.measure(0);

	while (true) {

		if (do_ns) sv = ns.scanCamera();
		if (do_ir) ir.readCamera();

		if (do_dm && measure_OK) {
			bool success = dm.read();
			if (success) {
				distance = dm.getDistanceInMMeter();
			}
			else {
				std::cout << "dm readDistance - error:" << dm.error << std::endl;
			}
		}

		if (do_dm) measure_OK = dm.measure(3);

		printf("Distance: %03.1f mm, Temperature: %03.1f, Scan:%d, Hold:%d\n",
				distance, sv.value, sv.scan, sv.hold);

		// Thermometer states
		// name     | scan | hold | state
		//----------+------+------+--------
		// start    | 0    |    0 | Showing the last measured value and laser is on.
		//                          camera can gets the word in the time of scanning.
		//                          SHOW PICTURE
		// hold     | 0    |    1 | Laser is off
		//                          STORE PICTURE right after state (1,0) with stored distance
		//                          SHOW PICTURE
		// scan     | 1    |    0 | Measuring. Showed temperature is false. Laser is ON
		//                          STORE DISTANCE
		//                          STORE PICTURE with laser
		// error    | 1    |    1 | Impossible

		if ((sv.scan == 0) && (sv.hold == 0)) {
			stateStart();
		}
		else if ((sv.scan == 0) && (sv.hold == 1)){
			stateHold();
		}
		else if ((sv.scan == 1) && (sv.hold == 0)){
			stateScan();
		}
		else {
			stateError();
		}


		showPictures(ns.getBinaryImage(), ir.getImage());
	}

	std::cout << "END" << std::endl;
}

