#include <iostream>
#include <fstream>


#include "VfsFile.h"
#include "VfsFileInfo.h"

VfsFile::VfsFile(){

}

void VfsFile::exportfile(std::string external_path){

}

//void VfsFile::saveInVfs(std::fstream& repository, int offset){

//}


void VfsFile::saveInVfs(std::fstream& repository, std::string extn_path,int &file_offset, long &bytes){

	std::fstream host;
	host.open(extn_path, std::ios::in | std::ios::out | std::ios::binary);
	//std::cout << "FILE OPENED" << std::endl;
	repository.seekp(0,std::ios::end);
	file_offset = repository.tellp();
	//std::cout << file_offset << "\n";
	int ext;
	int count = 0;
	char * buffer = new char[1024];
	while(!host.eof()){
		host.read(buffer, 1024);
		ext = host.gcount();
	//	std::cout << host<<"\t" << ext << "\n";
		if(ext == 1024){
			repository.write(buffer, 1024);
			count += 1024;
		}
		else{
	//		std::cout << buffer << "\t" <<ext << "\n";
			repository.write(buffer,ext);
			count += ext;
			bytes = count;
			break;
		}
	}

}




void VfsFile::exportOut(std::fstream &container, std::string host_path, int& offset, long& bytes){

	std::fstream out;
	char * buffer = new char[1024];
	out.open(host_path, std::ios::out | std::ios::binary);
	out.clear();
	container.clear();
	container.seekg(offset, std::ios::beg);
	//std::cout << offset <<"\n";
	int count = bytes/1024;
	int left = bytes % 1024;

	//std::cout << host_path <<"\t" <<  count <<"\t" << left << "\n";


	for(int i = 0; i< count ; i++){

		container.read(buffer, 1024);
		//std::cout << buffer;
		out.write(buffer, 1024);
	}
	
	container.read(buffer, left);
	//std::cout << buffer;
	out.write(buffer,left);

}