//VfsFileInfo

#ifndef VFSFILEINFO_H
#define VFSFILEINFO_H

#include <string>
#include "VfsNodeInfo.h"
#include "VfsRepository.h"

class VfsFile;

class VfsFileInfo:public VfsNodeInfo{
	
	long file_bytes;
	int file_offset;
	VfsFile* vfsfile;
	VfsFileInfo *parent;
	
public:

	VfsFileInfo();
	VfsFileInfo(std::string, long, int, std::string);
	void getHeader(HeaderRecord& hr, std::string temp);
	void save(std::string, std::string);	
	//open rep(container)
	//get pointer to end
	//call saveinVfs
	void assignParent(VfsFileInfo*);
	void dexport(std::string, std::string);

};	

#endif
