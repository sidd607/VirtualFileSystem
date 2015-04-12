# VirtualFileSystem

A virtual file system written in C++,

Compile by running g++ *.cc -o vfs 

Using Vfs

- Run the program by vfs --interactive
- To create a file system -> create <file system name> <path to create the new vfs>
- To open an existing vfs -> open <file system neme> <path to the vfs>
- To create dirextories   -> mkdir <directory name>
- To list contents of the directory -> list <directory name>
- To add a file from the system to the vfs -> add <path of the file in the system> <path you want to store in your vfs>
