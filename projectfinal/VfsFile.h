//VfsFile.h

#ifndef VFSFILE_H
#define VFSFILE_H

#include<string>
#include<fstream>

class VfsFileInfo;

class VfsFile{
	char* data_bytes;
	VfsFileInfo* vfsfile;
public:
	VfsFile(char*);
	void exportfile( std::string external_path);
	void saveInVfs(std::fstream& repository, int file_offset);
};

#endif
