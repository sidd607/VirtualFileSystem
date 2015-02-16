#include "VfsFileInfo.h"
#include "VfsFile.h"
#include "VfsFolderInfo.h"


VfsFileInfo::VfsFileInfo(std::string nodename, long bytes, int offset, std::string filepath):\
file_bytes(bytes), file_offset(offset), VfsNodeInfo(nodename, filepath, 0)
{}

void VfsFileInfo::getHeader(HeaderRecord &record){
	record.size = file_bytes;
	record.offset = file_offset;
	record.node_type = 0;
}
