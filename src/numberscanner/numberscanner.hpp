/*
 * numberscanner.hpp
 *
 *  Created on: Sep 5, 2014
 *      Author: peter
 */

#ifndef NUMBERSCANNER_HPP_
#define NUMBERSCANNER_HPP_

const int ERROR_EMPTY_FRAME = 1;
const int ERROR_BAD_NUMBER = 2;
const int ERROR_COULD_NOT_OPEN = 3;

#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "digit.hpp"

namespace ns {


struct ScannedStringValue {
	std::string number;
	std::string scan;
	std::string hold;
};

struct ScannedValue {
	int error;
	int hold;
	int scan;
	double value;
};

class NumberScanner {
public:
	/**
	 * constructor
	 * if cameraNumber < 0, it will not initialize the
	 * camera.
	 * See scanFile
	 */
	NumberScanner(int cameraNumber);
	~NumberScanner();
	ScannedValue scanCamera();
	ScannedValue scanImage(cv::Mat image);
	ScannedValue scanFile(std::string fileName);
	ScannedStringValue getStringData();

	cv::Mat getBinaryImage();
	cv::Mat getProcessedImage();

private:
	int NUM_OF_DIGITS;
	//The original picture was 640x480, so the digit's places was adopt to
	//this size
	cv::Size ORIG_SIZE;
	cv::Size NEW_SIZE;
	cv::VideoCapture cap;
	std::vector<Digit> d;
	Segment scan;
	Segment hold;
	cv::Mat image;
	cv::Mat binaryImage;

	cv::Mat readImage(int argc, const char** argv, int i);
	void readData();
	void openCamera(int cameraNumber);
	void closeCamera();
};

} // end of namespace

#endif /* NUMBERSCANNER_HPP_ */
