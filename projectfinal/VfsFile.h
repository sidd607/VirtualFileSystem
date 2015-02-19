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
	VfsFile();
	void exportfile( std::string external_path);
	void saveInVfs(std::fstream& repository, std::string extn_path, int &file_offset, long &bytes);
	void exportOut(std::fstream& container,  std::string host_path, int &file_offset, long &file_bytes);



};

#endif
