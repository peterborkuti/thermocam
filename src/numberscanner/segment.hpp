/*
 * segment.hpp
 *
 *  Created on: Feb 4, 2014
 *      Author: peter
 */

#ifndef SEGMENT_HPP_
#define SEGMENT_HPP_

#include "opencv2/core/core.hpp"

class Segment {
public:
	Segment();
	Segment(cv::Rect r);
	Segment(cv::Rect r, std::string stringValueOn, std::string stringValueOff);

	std::string read(const cv::Mat grayImage);
	void print(cv::Mat image);
	void draw(cv::Mat img);
	short getValue();
	std::string getStringValue();
	void setThreshold(float threshold);

private:
	cv::Rect rect;
	int areaMinimum;
	static const float areaThreshold = 0.3;
	short value;
	bool isDigitized;
	std::string strValue;
	std::string strValues[2];
};

#endif /* SEGMENT_HPP_ */
