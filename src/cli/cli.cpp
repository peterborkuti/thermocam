/*
 * cli.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: Peter Borkuti
 */

#include "opencv2/core/core.hpp"
#include "cli.hpp"
#include <iostream>

CLI::CLI() {
	keys =
		"{ d |              |             | device of distancemeter , ex: /dev/ttyUSB0 }"
		"{ c |              | -1          | number of camera device for seeing the world }"
		"{ s |              | -1          | number of camera device for seeing the screen }"
		"{ f |              | 15          | frames per second }"
		"{ h |              | false       | help              }"
		"{ v |              | false       | version information }"
	;
}

bool CLI::parse(int argc, const char** argv, std::string &dm_device,
		int &world_camera_number, int &screen_camera_number, int &fps) {
	cv::CommandLineParser parser(argc, argv, keys);

	if (parser.get<bool>("h")) {
		parser.printParams();

		std::cout << std::endl;
		std::cout << "more info:https://github.com/peterborkuti/thermometer" <<
				std::endl;
		std::cout << "author: Péter Borkuti, 2014" << std::endl;

		return false;
	}

	if (parser.get<bool>("version")) {
		std::cout << cv::getBuildInformation() << std::endl;
		return false;
	}

	dm_device = parser.get<std::string>("d");
	world_camera_number = parser.get<int>("c");
	screen_camera_number = parser.get<int>("s");
	fps = parser.get<int>("f");

	std::cout << "dm_device:" << dm_device << ":" << std::endl;
	std::cout << "world_camera:" << world_camera_number  << ":" << std::endl;
	std::cout << "screen_camera:" << screen_camera_number << ":" << std::endl;
	std::cout << "fps:" << fps << ":" << std::endl;

	return true;

}

