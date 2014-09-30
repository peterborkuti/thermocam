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

namespace ns
{

NumberScanner::NumberScanner(ir::ImageReader & imageReader)
{
	std::cout << "NumberScanner.constructor camnum:"<< imageReader.getCameraNumber() << std::endl;
	ptrImageReader = &imageReader;
	NUM_OF_DIGITS = 4;

	RectResizer resizer(imageReader.getOrigSize(), imageReader.getNewSize());
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

}

NumberScanner::~NumberScanner()
{
	(*ptrImageReader).~ImageReader();
}

void NumberScanner::readData()
{
	cv::Mat channels[3];
	cv::split((*ptrImageReader).getImage(), channels);
	cv::Mat blue = channels[0];

	//cv::threshold( blue, binary, threshold_value, max_BINARY_value,threshold_type );
	cv::threshold(blue, binaryImage, 145, 255, cv::THRESH_BINARY);

	for (int i = 0; i < NUM_OF_DIGITS; i++)
	{
		d[i].read(binaryImage);
	}

	scan.read(binaryImage);
	hold.read(binaryImage);

}

ScannedStringValue NumberScanner::getStringData()
{
	ScannedStringValue sv;
	sv.scan = scan.getStringValue();
	sv.hold = hold.getStringValue();
	sv.number = "";
	sv.number.push_back(d[0].decode());
	sv.number.push_back(d[1].decode());
	sv.number.push_back(d[2].decode());
	sv.number.push_back('.');
	sv.number.push_back(d[3].decode());

	return sv;
}

cv::Mat NumberScanner::getProcessedImage()
{
	cv::Mat processedImage((*ptrImageReader).getImage());

	hold.draw(processedImage);
	scan.draw(processedImage);

	for (int i = 0; i < NUM_OF_DIGITS; i++)
	{
		d[i].draw(processedImage);
	}

	return processedImage;
}

cv::Mat NumberScanner::getBinaryImage()
{

	return binaryImage;
}

ScannedValue NumberScanner::scanImage()
{
	ScannedValue scannedValue;

	readData();

	scannedValue.error = 0;
	scannedValue.value = 0;

	scannedValue.scan = scan.getValue();
	scannedValue.hold = hold.getValue();
	std::string valueStr("");

	valueStr += d[0].decode();
	valueStr += d[1].decode();
	valueStr += d[2].decode();
	valueStr += '.';
	valueStr += d[3].decode();

	if (std::string::npos != valueStr.find('X'))
	{
		scannedValue.error = ERROR_BAD_NUMBER;
	}
	else
	{
		scannedValue.value = atof(valueStr.c_str());
	}

	return scannedValue;
}

ScannedValue NumberScanner::scanCamera()
{
	ScannedValue sv;
	sv.error = ERROR_EMPTY_FRAME;

	bool success = (*ptrImageReader).readCamera();

	if (success) {
		return scanImage();
	}

	return sv;
}

ScannedValue NumberScanner::scanFile(std::string fileName)
{
	ScannedValue sv;
	sv.error = ERROR_COULD_NOT_OPEN;

	bool success = (*ptrImageReader).readFile(fileName);

	if (success) {
		return scanImage();
	}

	return sv;
}

} // end of namespace
