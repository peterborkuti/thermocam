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
#include <cstdio>
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"


namespace ir
{

ImageReader::ImageReader(int cameraNumber,
		cv::Size origSize,
		cv::Size newSize,
		int fps)
{
	ORIG_SIZE = cv::Size(origSize);
	NEW_SIZE = cv::Size(newSize);
	FPS = fps;
	printf("ImageReader - cam_num:%d, orig_size_w:%d, orig_size_h:%d, fps:%d\n",cameraNumber, origSize.width, origSize.height, fps);
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

int ImageReader::getCameraNumber() {
	return cameraNumber;
}

cv::Size ImageReader::getNewSize() {
	return NEW_SIZE;
}

cv::Size ImageReader::getOrigSize() {
	return ORIG_SIZE;
}

void ImageReader::closeCamera()
{
	if ((cameraNumber >=0) && cap.isOpened())
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

	/*
	cap.set(cv::CAP_PROP_FRAME_WIDTH, NEW_SIZE.width);
	std::cout << "camera with set " << NEW_SIZE.width << std::endl;
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, NEW_SIZE.height);
	std::cout << "camera height set" << NEW_SIZE.height << std::endl;


	if (FPS > 0)
	{
		cap.set(cv::CAP_PROP_FPS, FPS);
		std::cout << "camera fps set" << FPS << std::endl;;
	}
	*/
}

bool ImageReader::readCamera()
{

	if ((cameraNumber < 0) || !cap.isOpened()) {
		return false;
	}

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

	frame = cv::imread(fileName, cv::IMREAD_COLOR);

	if (!frame.empty())
	{
		cv::resize(frame, image, NEW_SIZE);
	}

	return (!frame.empty());
}

} // end of namespace
