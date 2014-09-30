/*
 * imagereader.hpp
 *
 *  Created on: Sep 14, 2014
 *      Author: peter
 */

#ifndef IMAGEREADER_HPP_
#define IMAGEREADER_HPP_

#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace ir
{

class ImageReader
{
public:
	ImageReader(int cameraNumber = 0,
			cv::Size origSize = cv::Size(640, 480),
			cv::Size newSize = cv::Size(160,120),
			int fps = 15);
	~ImageReader();
	bool readCamera();
	bool readFile(std::string fileName);
	cv::Mat getImage();
	bool saveImage(std::string fileName);
	cv::Size getOrigSize();
	cv::Size getNewSize();
	int getCameraNumber();
private:
	cv::Mat image;
	cv::Size ORIG_SIZE;
	cv::Size NEW_SIZE;
	int FPS;
	int cameraNumber;
	cv::VideoCapture cap;
	void openCamera();
	void closeCamera();
};

} // end of namespace

#endif /* IMAGEREADER_HPP_ */
