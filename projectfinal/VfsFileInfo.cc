#include "VfsFileInfo.h"
#include "VfsFile.h"
#include "VfsFolderInfo.h"
#include <fstream>


VfsFileInfo::VfsFileInfo(std::string nodename, long bytes, int offset, std::string filepath):\
file_bytes(bytes), file_offset(offset), VfsNodeInfo(nodename, filepath, 0)
{}

void VfsFileInfo::getHeader(HeaderRecord &record){
	record.size = file_bytes;
	record.offset = file_offset;
	record.node_type = 0;
}

void VfsFileInfo::save(std::string extrn_path, std::string repo_file_path){

	std::fstream container;
	container.open(repo_file_path, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
	VfsFile *file = new VfsFile();
	container.seekp(0,std::ios::end);
	file->saveInVfs(container, extrn_path, file_offset, file_bytes);


}


void VfsFileInfo::dexport(std::string repo_file_path, std::string host_path){

	std::fstream container;
	container.open(repo_file_path, std::ios::in | std::ios::binary);
	VfsFile *file = new VfsFile();
	container.seekg(file_offset, std::ios::beg);
	file->exportOut(container, host_path, file_offset, file_bytes);


}