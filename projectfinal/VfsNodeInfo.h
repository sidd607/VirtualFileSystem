//VfsNodeInfo

#ifndef VFSNODEINFO_H
#define VFSNODEINFO_H

#include <string>
#include "VfsRepository.h"

class VfsRepository;

class VfsNodeInfo{

	std::string node_name;
	std::string file_path;
	enum type{ FILES, FOLDER } node_type;

public:
	VfsNodeInfo();
	VfsNodeInfo(std::string, std::string, int);
	virtual void getHeader(HeaderRecord&, std::string) = 0;
	std::string getPath();
	std::string getName();
	


};

#endif