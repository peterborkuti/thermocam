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
#include <fstream>

std::string exec(const char* cmd)
{
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

using namespace ns;

int main(int argc, const char** argv)
{
	//std::string pwd = "pwd";
	//std::cout << exec(pwd.c_str()) << std::endl;
	NumberScanner ns(-1);
	std::ifstream file("output.txt");

	std::string line;
	int errorCount = 0;
	int picIndex = 6;
	int lineIndex = 1;

	while (std::getline(file, line))
	{
		char buf[100];
		sprintf(buf, "%08d.png", picIndex);
		std::string fileName(buf);

		ns.scanFile(fileName);
		ScannedStringValue sv = ns.getStringData();
		std::string check = sv.scan + sv.hold + sv.number;

		if (line.compare(check) != 0)
		{
			std::cout << "output.txt line " << lineIndex << " - "<< fileName <<
				":" << line << " != " << check << std::endl;

			errorCount++;
		}

		picIndex++;
		lineIndex++;
	}

	if (errorCount != 0) {
		std::cout << "Test done with errors!" << std::endl;
	}
	else {
		std::cout << "Test done without errors." << std::endl;
	}
}
