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
#include "../imagereader/imagereader.hpp"

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
	NumberScanner(ir::ImageReader imageReader);
	~NumberScanner();
	ScannedValue scanCamera();
	ScannedValue scanImage();
	ScannedValue scanFile(std::string fileName);
	ScannedStringValue getStringData();

	cv::Mat getBinaryImage();
	cv::Mat getProcessedImage();

private:
	int NUM_OF_DIGITS;

	std::vector<Digit> d;
	Segment scan;
	Segment hold;
	cv::Mat binaryImage;

	ir::ImageReader imageReader;

	void readData();
};

} // end of namespace

#endif /* NUMBERSCANNER_HPP_ */
