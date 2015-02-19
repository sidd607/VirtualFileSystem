//VfsFolderInfo

#ifndef VFSFOLDERINFO_H
#define VFSFOLDERINFO_H

#include "VfsNodeInfo.h"

#include <string>
#include <vector>

class VfsFileInfo;

class VfsFolderInfo: public VfsNodeInfo{

	std::string node_path;
	
	std::vector<VfsFileInfo*> child_file;

	std::vector<VfsFolderInfo*> child_folder;
	
	VfsFolderInfo* parentfolder;
	
	public:
		
		VfsFolderInfo(std::string node_path, std::string node_name);
		void getHeader(HeaderRecord&);
		void assignParent(VfsFolderInfo*);
		void assignChildFolder(VfsFolderInfo*);
		void assignChildFile(VfsFileInfo*);
		std::vector<std::string> getChild();

		
		//void getName();

};

#endif
