//VfsFileInfo

#ifndef VFSFILEINFO_H
#define VFSFILEINFO_H

#include "VfsNodeInfo.h"
#include "VfsRepository.h"

class VfsFile;

class VfsFileInfo:public VfsNodeInfo{
	
	long file_bytes;
	int file_offset;
	VfsFile* vfsfile;
	
public:

	VfsFileInfo();
	VfsFileInfo(std::string, long, int, std::string);
	void getHeader(HeaderRecord&);
	void save(std::string full_path);

};

#endif
