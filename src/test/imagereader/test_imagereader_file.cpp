/*
 * test_imagereader_file.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: peter
 */

#include "../../imagereader/imagereader.hpp"
int main() {
	ImageReader ir(-1);
	ir.readFile("00000006.png");
	ir.saveImage("out.png");
}


