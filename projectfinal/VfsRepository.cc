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
		std::cout << node_count << std::endl;	


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

	std::fstream container;
	repo_name = name;
	repo_file_path = ext_path;

	container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary);
	container.read((char*)&node_count, sizeof(int));
	HeaderRecord header;

	for(int i = 0 ;i < node_count; i ++){
		container.read((char*)&header, sizeof(HeaderRecord));
		std::cout << i <<" " << header.folder_path << "/" << header.node_name <<"\n";


	}


	container.close();
}




void VfsRepository::close(){

	std::fstream container;
	container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary);

	int num_node = vfs_map.size();

	container.write((char*)&num_node, sizeof(int));
	HeaderRecord header;
	std::map<std::string, VfsNodeInfo*>::iterator it;
	for(it = vfs_map.begin() ; it != vfs_map.end(); it++){

		std::cout << "temp: " << it-> first <<std::endl;
		if(it->first != "/"){
			it->second -> getHeader(header);
			container.write((char*)&header, sizeof(HeaderRecord));
		}
	}	

	container.clear();
	container.close();
	
}

void VfsRepository::makeDir(std::string path, std::string name){

	std::map<std::string, VfsNodeInfo*>::iterator it;
	it = vfs_map.find(path);


	if(vfs_map[path + "/" + name]){

		//Program terminates after catching errors
		throw VFS_MAKEDIR_02;
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
			std::cout << "CASE 1 " <<  full_path << std::endl;
		}

		else if(path == "" && name == ""){
		//std::cout << "CASE II";
			VfsFolderInfo *folder = new VfsFolderInfo(name, "");
			vfs_map[""] = folder;
			std::cout << "CASE 2 " << vfs_map[""]->getName() << std::endl;
		}

		else if(path == "" && name != ""){
		//std::cout << "CASE III";
			VfsFolderInfo *root = new VfsFolderInfo("", "");
			vfs_map[""] = root;
			VfsFolderInfo *child = new VfsFolderInfo(name, "/" + name);
			vfs_map["/" + name] = child;
			root->assignChildFolder(child);
			child->assignParent(root);
			std::cout << "CASE 3 " << vfs_map["/"+name]->getName() << std::endl;
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















void VfsRepository::copyIn(std::string host_path, std::string repo_path){

	PathString p(repo_path, "/");
	
	std::string path = p.excludeLast();
	std::string name = p.getLast();

	if(path == ""){
		std::cout << "HELLO";	
	}
	else
		std::cout << "PATH NOT FOUND";

	std::map<std::string, VfsNodeInfo*>::iterator it;
	it = vfs_map.find(path);
	if(it != vfs_map.end()){
		VfsFileInfo *file = new VfsFileInfo(name, 0, 0, path);
		file->save(host_path, repo_file_path);
		vfs_map[repo_path] = file;
		dynamic_cast<VfsFolderInfo*>(it->second)->assignChildFile(file);
	}


}




void VfsRepository::copyOut(std::string repo_path, std::string host_path){

	
	if(vfs_map[repo_path]){
		dynamic_cast<VfsFileInfo*>(vfs_map[repo_path]) -> dexport(repo_file_path, host_path);
	}
	else{
		std::cout << "404 FILE NOT FOUND" << std::endl;
	}

}