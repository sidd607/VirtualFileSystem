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
#include <sys/stat.h>




//VfsRepository::VfsRepository(std::string reponame):repo_name(reponame){
//}
bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
	return true;
    }
    return false;
}


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
//		std::cout << node_count << std::endl;	


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

	if(!fileExists(repo_file_path))
		throw VFS_OPEN_01;

	else{
		std::fstream container;
		repo_name = name;
		repo_file_path = ext_path;
		int x;
		container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

		container.seekg(0,std::ios::beg);
		container.read((char*)&node_count, sizeof(int));
		HeaderRecord header;
//	std::cout << "NODES : "<< node_count <<"\n";
		x = node_count;
		if(node_count > 0)
			makeDir("","");

		for(int i = 0 ;i < x - 1; i ++){
			container.read((char*)&header, sizeof(HeaderRecord));
	//	std::cout << node_count << " " << i <<" " << header.folder_path << "/" << header.node_name <<"\n";
			if(header.node_type == 1){
				makeDir(header.folder_path, header.node_name);
			}
			else{
	//		std::cout << "HELLO";

				VfsFileInfo *file = new VfsFileInfo(header.node_name, header.size, header.offset, header.folder_path);
				std::map<std::string, VfsNodeInfo*>::iterator it;
				it = vfs_map.find(header.folder_path);
				std::string full_path = std::string(header.folder_path) + "/" + std::string(header.node_name);
	//		dynamic_cast<VfsFolderInfo*>(it->second)->assignChildFile(file);
			}


		}


		container.close();
	}
}




void VfsRepository::close(){

	vfs_map.erase("");
	std::map<std::string , VfsNodeInfo*>::iterator it1;
	
	//for(it1 = vfs_map.begin(); it1!=vfs_map.end(); it1++){
	//	std::cout << it1->first << std::endl;
	//}

	std::fstream container;
	container.open(repo_file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

	int num_node = vfs_map.size();
	container.seekp(0, std::ios::beg);

	container.write((char*)&num_node, sizeof(int));
	HeaderRecord header;
	std::map<std::string, VfsNodeInfo*>::iterator it;
	
	it = vfs_map.begin();
	for( it; it != vfs_map.end(); it++){

		if(it->first != "/"){
			
				it->second -> getHeader(header, it->first);

	//			std::cout << "i" <<" " << header.folder_path << "/" << header.node_name <<"\n";
				container.write((char*)&header, sizeof(HeaderRecord));
			
			
/*
				VfsFileInfo *file = new VfsFileInfo(header.node_name, header.size, header.offset, header.folder_path);
				std::map<std::string, VfsNodeInfo*>::iterator it;
				it = vfs_map.find(header.folder_path);
				std::string full_path = std::string(header.folder_path) + "/" + std::string(header.node_name);
				//
				dynamic_cast<VfsFolderInfo*>(it->second)->assignChildFile(file);
*/			
		}
	}	

	container.clear();
	container.close();
	//_map.clear();
	
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
			//std::cout << "CASE 1 " <<  full_path << std::endl;
		}

		else if(path == "" && name == ""){
		//std::cout << "CASE II";
			VfsFolderInfo *folder = new VfsFolderInfo(name, "");
			vfs_map[""] = folder;
			//std::cout << "CASE 2 " << vfs_map[""]->getName() << std::endl;
		}

		else if(path == "" && name != ""){
		//std::cout << "CASE III";
			VfsFolderInfo *root = new VfsFolderInfo("", "");
			vfs_map[""] = root;
			VfsFolderInfo *child = new VfsFolderInfo(name, "/" + name);
			vfs_map["/" + name] = child;
			root->assignChildFolder(child);
			child->assignParent(root);
	//		std::cout << "CASE 3 " << vfs_map["/"+name]->getName() << std::endl;
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
		if(!vfs_map[repo_path])
			vfs_map[repo_path] = file;
		else
			throw VFS_COPYIN_02;

		dynamic_cast<VfsFolderInfo*>(it->second)->assignChildFile(file);
	}
	else
		throw VFS_COPYIN_01;


}




void VfsRepository::copyOut(std::string repo_path, std::string host_path){

	
	if(vfs_map[repo_path]){
		dynamic_cast<VfsFileInfo*>(vfs_map[repo_path]) -> dexport(repo_file_path, host_path);
	}
	else{
		
		throw VFS_COPYOUT_01;
	}

}