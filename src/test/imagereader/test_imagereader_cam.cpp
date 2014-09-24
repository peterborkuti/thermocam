/*
 * test_imagereader_file.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: peter
 */

#include "../../imagereader/imagereader.hpp"

using namespace ir;

int main() {
	ImageReader ir(0);
	cv::namedWindow("win", cv::WINDOW_AUTOSIZE);
	while (true) {
		ir.readCamera();
		cv::imshow("win", ir.getImage());
		cv::waitKey(30);
	}
}


