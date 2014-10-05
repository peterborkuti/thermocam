/*
 * test_imagereader_file.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: peter
 */

#include "../../imagereader/imagereader.hpp"
#include <iostream>

using namespace ir;

int main(int argc, const char** argv) {
	int device = 0;
	if (argc > 1) {
		device = atoi(argv[1]);
	}
	ImageReader ir(device);
	cv::namedWindow("win", cv::WINDOW_AUTOSIZE);
	while (true) {
		bool success = ir.readCamera();
		if (success) {
			cv::imshow("win", ir.getImage());
		}
		else {
			std::cout << "can not read from camera" << std::endl;
		}
		cv::waitKey(30);
	}
}


