#include "VfsNodeInfo.h"
#include "VfsRepository.h"

VfsNodeInfo::VfsNodeInfo(std::string nodename, std::string filepath, int type){

	node_name = nodename;
	file_path = filepath;
	if(type == 0){
		node_type = FILES;
	}
	else{
		node_type = FOLDER;
	}

}

VfsNodeInfo::VfsNodeInfo(){

}

std::string VfsNodeInfo::getPath(){
	return file_path;
}

std::string VfsNodeInfo::getName(){
	return node_name;
}



