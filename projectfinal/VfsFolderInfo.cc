#include "VfsFolderInfo.h"
#include "VfsFileInfo.h"
#include "VfsNodeInfo.h"
#include "VfsRepository.h"
#include "PathString.h"

#include <iostream>

VfsFolderInfo::VfsFolderInfo(std::string path, std::string name): \
	VfsNodeInfo(name, path, 1){}

void VfsFolderInfo::getHeader(HeaderRecord &record){
	record.node_type = 1;
}

void VfsFolderInfo::assignParent(VfsFolderInfo *parent){
	parentfolder = parent;
}

void VfsFolderInfo::assignChildFolder(VfsFolderInfo *child){
	child_folder.push_back(child);
}

void VfsFolderInfo::assignChildFile(VfsFileInfo *child){
	child_file.push_back(child);
}

std::vector<std::string> VfsFolderInfo::getChild(){
	
	std::vector<std::string> v;
	for(int i = 0; i < child_folder.size(); i++){
		std::string temp = child_folder[i]->getName();
		//std::cout << temp;
		v.push_back(temp + "/");
		//std::cout <<  v[i];
		}
	return v;
}




