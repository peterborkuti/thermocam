/*
 * rectresizer.hpp
 *
 *  Created on: Sep 6, 2014
 *      Author: peter
 */

#ifndef RECTRESIZER_HPP_
#define RECTRESIZER_HPP_

#include "opencv2/core/core.hpp"

class RectResizer {
private:
	float dx, dy;
public:
	RectResizer(cv::Size origSize, cv::Size newSize);
	cv::Rect resize(cv::Rect r);
};


#endif /* RECTRESIZER_HPP_ */
