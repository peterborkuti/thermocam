/*
 * imagereader.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: peter
 */

#include "imagereader.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

namespace ir
{

ImageReader::ImageReader(int cameraNumber, cv::Size origSize, cv::Size newSize,
		int fps)
{
	ORIG_SIZE = cv::Size(origSize);
	NEW_SIZE = cv::Size(newSize);
	FPS = fps;
	this->cameraNumber = cameraNumber;

	if (cameraNumber >= 0)
	{
		openCamera();
	}
}

ImageReader::~ImageReader()
{
	closeCamera();
}

cv::Size ImageReader::getNewSize() {
	return NEW_SIZE;
}

cv::Size ImageReader::getOrigSize() {
	return ORIG_SIZE;
}

void ImageReader::closeCamera()
{
	if (cap.isOpened())
	{
		cap.release();
	}
}

void ImageReader::openCamera()
{

	if (cameraNumber < 0)
	{
		//probably will use scanning from files
		return;
	}

	cap.open(cameraNumber);

	if (!cap.isOpened())
	{
		//help();
		std::cout << "***Could not initialize capturing...***\n";
		std::cout << "Current parameter's value: \n";
		exit(-1);
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, NEW_SIZE.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, NEW_SIZE.height);
	if (FPS > 0)
	{
		cap.set(CV_CAP_PROP_FPS, FPS);
	}
}

bool ImageReader::readCamera()
{

	cap >> image;
	if (image.empty())
	{
		return false;
	}

	return (!image.empty());
}

cv::Mat ImageReader::getImage()
{
	return image;
}

bool ImageReader::saveImage(std::string fileName)
{
	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try
	{
		cv::imwrite(fileName, image, compression_params);
	} catch (std::runtime_error& ex)
	{
		std::cerr << "Exception converting image to PNG format: %s\n"
				<< ex.what() << std::endl;
		return false;
	}

	return true;
}

bool ImageReader::readFile(std::string fileName)
{
	cv::Mat frame;

	frame = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);

	if (!frame.empty())
	{
		cv::resize(frame, image, NEW_SIZE);
	}

	return (!frame.empty());
}

} // end of namespace
