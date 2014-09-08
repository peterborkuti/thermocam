/*
 * main.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: peter
 */

#define UTIL_HPP_DEBUG true

#include "numberscanner.hpp"
#include "rectresizer.hpp"
#include <iostream>


NumberScanner::NumberScanner(int cameraNumber) {
	NUM_OF_DIGITS = 4;
	ORIG_SIZE = cv::Size(640,480);
	NEW_SIZE = cv::Size(160, 120);

	RectResizer resizer(ORIG_SIZE, NEW_SIZE);
	scan = Segment(resizer.resize(cv::Rect(63, 112, 107, 48)), "SCAN", "    ");
	hold = Segment(resizer.resize(cv::Rect(170, 108, 111, 51)), "HOLD", "    ");
	scan.setThreshold(0.01);
	hold.setThreshold(0.01);

	Digit digit(resizer.resize(cv::Rect(45, 182, 112, 214)));
	d.push_back(digit);
	digit = Digit(resizer.resize(cv::Rect(156, 182, 112, 214)));
	d.push_back(digit);
	digit = Digit(resizer.resize(cv::Rect(272, 182, 103, 214)));
	d.push_back(digit);
	digit = Digit(resizer.resize(cv::Rect(419, 179, 105, 214)));
	d.push_back(digit);

	if (cameraNumber >= 0) {
		openCamera(cameraNumber);
	}
}

void NumberScanner::readData(const cv::Mat image, std::vector<Digit> d, Segment& scan, Segment& hold) {
	cv::Mat channels[3];
	cv::split(image, channels);
	cv::Mat blue = channels[0];
	cv::Mat binary;
	//cv::threshold( blue, binary, threshold_value, max_BINARY_value,threshold_type );
	cv::threshold(blue, binary, 145, 255, cv::THRESH_BINARY);

	for (int i = 0; i < NUM_OF_DIGITS; i++) {
		d[i].read(binary);
	}

	scan.read(binary);
	hold.read(binary);

}

ScannedStringValue NumberScanner::getStringData() {
	ScannedStringValue sv;
	sv.scan = scan.getStringValue();
	sv.hold = hold.getStringValue();
	sv.number =
		d[0].decode() + d[1].decode() + d[2].decode() + '.' + d[3].decode();

	return sv;
}

cv::Mat NumberScanner::getProcessedImage() {
	cv::Mat processedImage(image);

	hold.draw(processedImage);
	scan.draw(processedImage);

	for (int i = 0; i < NUM_OF_DIGITS; i++) {
		d[i].draw(processedImage);
	}

	return processedImage;
}


void NumberScanner::openCamera(int cameraNumber) {

	if (cameraNumber < 0) {
		//probably will use scanning from files
		return;
	}

	cap.open(cameraNumber);

	if (!cap.isOpened()) {
		//help();
		std::cout << "***Could not initialize capturing...***\n";
		std::cout << "Current parameter's value: \n";
		exit (-1);
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 160);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 120);
}

ScannedValue NumberScanner::scanImage(cv::Mat srcImage) {

	srcImage.copyTo(image);

	ScannedValue scannedValue;

	readData(image, d, scan, hold);

	scannedValue.error = 0;
	scannedValue.value = 0;

	scannedValue.scan = scan.getValue();
	scannedValue.hold = hold.getValue();
	std::string floatStr =
		d[0].decode() + d[1].decode() + d[2].decode() + "." + d[3].decode();

	if (!floatStr.find('X')) {
		scannedValue.error = ERROR_BAD_NUMBER;
	}
	else {
		double value = atof(floatStr.c_str());
		scannedValue.value = value;
	}

	return scannedValue;
}

ScannedValue NumberScanner::scanCamera() {

	cv::Mat frame;

	cap >> frame;
	if (frame.empty()) {
		ScannedValue sv;
		sv.error = ERROR_EMPTY_FRAME;
		return sv;
	}

	return scanImage(frame);
}

ScannedValue NumberScanner::scanFile(std::string fileName) {
	cv::Mat frame;

	frame = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);

	if(!frame.data) {
		ScannedValue sv;
		sv.error = ERROR_COULD_NOT_OPEN;

		return sv;
	}

	cv::resize(frame, frame, NEW_SIZE);

	return scanImage(frame);
}
