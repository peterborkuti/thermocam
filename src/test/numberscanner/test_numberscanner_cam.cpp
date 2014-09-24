/*
 * test_numberscanner.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: peter
 */

#include "../../numberscanner/numberscanner.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>


using namespace ns;

int main(int argc, const char** argv)
{
	NumberScanner ns(0);

	cv::namedWindow("win", cv::WINDOW_AUTOSIZE);

	while (true)
	{
		ns.scanCamera();
		cv::imshow("win", ns.getProcessedImage());
		ScannedStringValue sv = ns.getStringData();
		std::string value = sv.scan + sv.hold + sv.number;
		std::cout << value << std::endl;
		cv::waitKey(30);
	}
}
