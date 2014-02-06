/*
 * digit.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: peter
 */

#include "digit.hpp"
#include <iostream>


Digit::Digit(cv::Rect r) {
	rect = r;
	value = 0u;
	isDigitized = false;

	int h = r.height / 7;
	int w = r.width / 3;
	int h2 = 2*h;

	//horiz top
	segments.push_back(Segment(cv::Rect(r.x+w, r.y+0*h, w, h )) );
	//horiz middle
	segments.push_back(Segment(cv::Rect(r.x+w, r.y+3*h, w, h )) );
	//horiz bottom
	segments.push_back(Segment(cv::Rect(r.x+w, r.y+6*h, w, h )) );

	//top left
	segments.push_back(Segment(cv::Rect(r.x, r.y+1*h, w, h2 )) );
	//bottom left
	segments.push_back(Segment(cv::Rect(r.x, r.y+4*h, w, h2 )) );
	//top right
	segments.push_back(Segment(cv::Rect(r.x+2*w, r.y+1*h, w, h2 )) );
	//bottom right
	segments.push_back(Segment(cv::Rect(r.x+2*w, r.y+4*h, w, h2 )) );

	//segments: top|middle|botton|lefttop|leftbottom|righttop|rightbottom
	decoderMap[0b01011111u] = '0';
	decoderMap[0b00000011u] = '1';
	decoderMap[0b01110110u] = '2';
	decoderMap[0b01110011u] = '3';
	decoderMap[0b00101011u] = '4';
	decoderMap[0b01111001u] = '5';
	decoderMap[0b01111101u] = '6';
	decoderMap[0b01000011u] = '7';
	decoderMap[0b01111111u] = '8';
	decoderMap[0b01111011u] = '9';
	decoderMap[0b00100000u] = '-';

}

ValueType Digit::read(cv::Mat grayImage) {
	isDigitized = true;

	value = 0;

	for (int i = 0; i < 7; i++) {
		if (segments[i].read(grayImage) == 1) {
			value |= 1 << (6-i); //sets the i. bit to 1
		}
	}

	return value;
}

void Digit::draw(cv::Mat &image) {

	for (int i = 0; i < 7; i++) {
		segments[i].draw(image);
	}

	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));
}

ValueType Digit::getValue() {
	return value;
}

void Digit::printMap() {
	for (DecoderMapIterator it = decoderMap.begin(); it != decoderMap.end(); ++it) {
		std::cout << std::hex << it->first << "," << it->second << std::endl;
	}

}

char Digit::decode() {
	char c = 'X';

	if (decoderMap.count(value) == 1) {
		c = decoderMap.at(value);
	}

	return c;
}

char Digit::decode2() {
	char c = 'X';
	for (DecoderMapIterator it = decoderMap.begin(); it != decoderMap.end(); ++it) {
		if (it->first == value) {
			c = it->second;
			break;
		}
	}

	return c;
}