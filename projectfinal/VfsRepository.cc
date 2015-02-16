#include "VfsRepository.h"
#include "VfsNodeInfo.h"
#include "VfsFolderInfo.h"
#include "VfsFileInfo.h"
#include "PathString.h"
#include "vfs_errors.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <map>




//VfsRepository::VfsRepository(std::string reponame):repo_name(reponame){
//}


VfsRepository::VfsRepository(){

}


void VfsRepository::create(std::string name, std::string ext_path)
{
	repo_name = name;
	repo_file_path = ext_path;

	std::fstream container;

	container.open(repo_file_path, std::ios::out | std::ios::binary);
	if( !container.good()){
		throw VFS_CREATE_01;
	}
	else{

	node_count = 0;

	container.write((char*)&node_count, sizeof(int));

	HeaderRecord header;
	header.node_type = 0;
	header.offset = 0;
	header.size = 0;

	for (int i = 0; i < MAX_NODES; i++)
		container.write((char*)&header, sizeof(HeaderRecord));
	container.close();
	}
}


void VfsRepository::open(std::string name, std::string ext_path){

	repo_name = name;
	repo_file_path = ext_path;
	int num_node;
	//repo_status = 1;
	HeaderRecord header;

	std::fstream container;
	
	//std::cout << repo_file_path <<"\t"<< repo_name << std::endl;
	
	container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary);
	//std::cout << "File Opened\n";
	if(!container.good())
		throw VFS_OPEN_01;
	else{
	container.read((char*)&num_node, sizeof(num_node));
	//std::cout << num_node << std::endl;

	node_count = num_node;
	
	for(int i = 0; i < node_count ;i++){
		container.read((char*)&header, sizeof(header));
		//std::cout << header.node_type;
		
		if(header.node_type == 1){ // 1 represents folder			

			VfsFolderInfo *folder = new VfsFolderInfo((std::string)header.folder_path, (std::string)header.node_name);

			std::string full_path = (std::string)header.folder_path + "/" + (std::string)header.node_name;

			vfs_map[full_path] = folder;

			

		}
		else{

			VfsFileInfo *file = new VfsFileInfo((std::string)header.node_name, (long)header.size,\
				(int)header.offset, (std::string)header.folder_path);
			
			std::string full_path = (std::string)header.folder_path + "/" + (std::string)header.node_name;
			
			vfs_map[full_path] = file;

		}


		container.clear();

		container.seekg(0, std::ios::beg);

		container.close();



	}

}

}




void VfsRepository::close(){
	
	std::fstream container;
	container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary);
	
	int temp = vfs_map.size();
	container.write((char*)&temp, sizeof(int));
	std::map<std::string, VfsNodeInfo*>::iterator it;
	HeaderRecord record;
	std::string path = "", name = "";
	for(it = vfs_map.begin(); it != vfs_map.end(); it++){
		it->second->getHeader(record);

		if(it->first == ""){

			strcpy(record.node_name, "");
			strcpy(record.folder_path,"/");
			container.write((char*)&record, sizeof(record));
		}
		else{
			PathString p (it->first, "/");
			name = p.getLast();
			path = p.excludeLast();
			strcpy(record.node_name, name.c_str());
			strcpy(record.folder_path, path.c_str());
			container.write((char*)&record, sizeof(record));
		}
	}

	container.close();
	
}

void VfsRepository::makeDir(std::string path, std::string name){

	std::map<std::string, VfsNodeInfo*>::iterator it;
	it = vfs_map.find(path);


	if(vfs_map[path + "/" + name]){

		//Program terminates after catching errors
		//sthrow VFS_MAKEDIR_02;
	}
	else{
	if(it != vfs_map.end()){
		//std::cout << "CASE I";
		VfsFolderInfo *folder = new VfsFolderInfo(path + "/" + name, name);
		folder->assignParent(dynamic_cast<VfsFolderInfo*>(it->second));
		std::string full_path = path + "/" + name;
		node_count++;
		dynamic_cast<VfsFolderInfo*>(it->second)->assignChildFolder(folder);
		vfs_map[full_path] = folder;
		//std::cout << "directory created! "<< std::endl;
		//std::cout << "DSAD " << vfs_map[full_path]->getName() << std::endl;
	}

	else if(path == "" && name == ""){
		//std::cout << "CASE II";
		VfsFolderInfo *folder = new VfsFolderInfo(name, "");
		vfs_map[""] = folder;
		//std::cout << "DSAD " << vfs_map[""]->getName() << std::endl;
	}

	else if(path == "" && name != ""){
		//std::cout << "CASE III";
		VfsFolderInfo *root = new VfsFolderInfo("", "");
		vfs_map[""] = root;
		VfsFolderInfo *child = new VfsFolderInfo(name, "/" + name);
		vfs_map["/" + name] = child;
		root->assignChildFolder(child);
		child->assignParent(root);
		//std::cout << "DSAD " << vfs_map["/"+name]->getName() << std::endl;
	}
	else 
		throw VFS_MAKEDIR_01;
	}	
}

void VfsRepository::list(std::string file_path, std::vector<std::string> &contents){

	if(file_path == "/")
		file_path = "";
	std::map<std::string, VfsNodeInfo*>::iterator it;
	it = vfs_map.find(file_path);
	if(it != vfs_map.end()){
		std::vector<std::string> children;
		children = dynamic_cast<VfsFolderInfo*>(it->second)->getChild();
		for(int i = 0; i < children.size(); i++){
			children[i] = file_path + "/" + children[i];
			//std::cout <<  children[i] <<"\n";
		}
		contents = children;
	}
	else
		throw VFS_LISTDIR_01;

}