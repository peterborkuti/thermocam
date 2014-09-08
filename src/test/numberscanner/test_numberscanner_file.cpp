/*
 * test_numberscanner.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: peter
 */


#include "../../numberscanner/numberscanner.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}


int main(int argc, const char** argv) {
	std::cout << exec("pwd") << std::endl;
	std::cout << "test_numberscanner_file started" << std::endl;
	NumberScanner ns(-1);
	ScannedValue v = ns.scanFile("00000022.png");
	cv::namedWindow("image", cv::WINDOW_AUTOSIZE );
	cv::imshow("image", ns.getProcessedImage());
	cv::waitKey(0);
	cv::destroyAllWindows();
	std::cout << v.error << "," << v.value << std::endl;
	ScannedStringValue sv = ns.getStringData();
	std::cout << sv.scan + " " + sv.scan + " " + sv.number << std::endl;
	std::cout << "test_numberscanner_file ended" << std::endl;

}
