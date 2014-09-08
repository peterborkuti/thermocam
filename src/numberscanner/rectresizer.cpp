#include "rectresizer.hpp"

RectResizer::RectResizer(cv::Size origSize, cv::Size newSize) {
	dx = (float)newSize.width / (float)origSize.width;
	dy = (float)newSize.height / (float)origSize.height;
}
cv::Rect RectResizer::resize(cv::Rect r) {
	cv::Rect s;
	s.x = ((float) r.x) * dx;
	s.y = ((float)r.y) * dy;
	s.height = ((float)r.height) * dy;
	s.width = ((float)r.width) * dx;
	return s;
}
