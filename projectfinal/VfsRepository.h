//VfsRepository.h

#ifndef VFSREPOSITORY_H
#define VFSREPOSITORY_H

#include <string>
#include <vector>
#include <map>

const int MAX_PATH_LEN = 200;
const int MAX_NAME_LEN = 200;
const int MAX_NODES = 2000;



struct HeaderRecord
{
	char node_name[MAX_NAME_LEN];
	int node_type;
	int offset;
	int size;
	char folder_path[MAX_PATH_LEN];
};


class VfsNodeInfo;

class VfsRepository{

	std::string repo_name;

	enum repo_status { OPEN, CLOSED };

	std::string repo_file_path;

	std::vector <VfsNodeInfo*> vfsnodeinfo;

	std::map<std::string, VfsNodeInfo*> vfs_map;

	int node_count;

public:

	//VfsRepository(std::string name, std::string ext_path);
	VfsRepository();
	void create(std::string, std::string);
	void open(std::string, std::string);
	void close();
	
	void list(std::string temp, std::vector<std::string> &contents);
	void makeDir(std::string, std::string);
	void vfsMakeDir(std::string, std::string);
	void copyIn(std::string host_path , std::string repo_path);
	void copyOut(std::string, std::string);
};

#endif
