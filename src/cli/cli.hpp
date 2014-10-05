/*
 * cli.hpp
 *
 *  Created on: Nov 23, 2013
 *      Author: Peter Borkuti
 */

#ifndef CLI_HPP_
#define CLI_HPP_

class CLI {
private:
	const char * keys;
public:
	CLI();
	bool parse(int argc, const char** argv, std::string &dm_device,
			int &world_camera_number, int &screen_camera_number, int &fps);
};


#endif /* CLI_HPP_ */
