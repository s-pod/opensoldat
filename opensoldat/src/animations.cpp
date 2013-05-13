/*
 Copyright (c) 2013, Stanislav Podolsky
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */
#include "global.h"
#include "animations.h"

animations::animations() {
	// TODO Auto-generated constructor stub

}

animations::~animations() {
	// TODO Auto-generated destructor stub
	unsigned int i;
	std::vector<std::string> v;
	for (std::map<std::string, std::vector<animFRAME> >::iterator it =
			anim_arr.begin(); it != anim_arr.end(); ++it) {
		for (i = 0; i < it->second.size(); i++) {
			delete (it->second[i].bodyParts);
		}
	}
}

void animations::loadAnimations() {
	DIR *dp;
	struct dirent *ep;

	dp = opendir("./anims/");
	if (dp == NULL) {
		std::cout << "cannot open animations directory" << std::endl;
		exit(1);
	}
	struct stat s;
	while ((ep = readdir(dp)) != NULL) {
		std::string *fname = new std::string;
		fname->clear();
		fname->append("./anims/");
		fname->append(ep->d_name);
		if (stat(fname->c_str(), &s) == 0) {
			if (s.st_mode & S_IFDIR) {
				//it's a directory
				delete fname;
				continue;
			}
		}
		std::vector<animFRAME> *anim = new std::vector<animFRAME>;

		loadAnimation(fname->c_str(), anim);
		fname->clear();
		fname->append(ep->d_name);
		anim_arr.insert(std::make_pair(*fname, *anim));
		delete (fname);
	}

	closedir(dp);
}

int animations::loadAnimation(const char *filename,
		std::vector<animFRAME> *anim) {
	int i, fnum = 0;
	std::string line;

	std::ifstream anim_file(filename);
	std::string anim_name = filename;
	if (!anim_file.is_open()) {
		std::cout << "cannot open animation file: " + anim_name << std::endl;
	}
	std::cout << "loading animation file: " + anim_name << std::endl;
	getline(anim_file, line);
	while (anim_file.good()) {
		if (line.compare("NEXTFRAME") == 0) {
			std::cout << "NEXTFRAME" << std::endl;
			fnum++;
			continue;
		}
		animFRAME frame;
		frame.bodyParts = new std::vector<VERTEX>;
		for (i = 0; i < 20; i++) {
			getline(anim_file, line);
			VERTEX v;
			v.x = atof(line.c_str());
			getline(anim_file, line);
			v.z = atof(line.c_str());
			getline(anim_file, line);
			v.y = atof(line.c_str());
			frame.bodyParts->push_back(v);
			getline(anim_file, line);
		}
		anim->push_back(frame);
	}
	anim_file.close();

	return 0;
}
