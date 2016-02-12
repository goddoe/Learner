#ifndef LIBRARY_DIRCONTROL_H
#define LIBRARY_DIRCONTROL_H
#include <iostream>
#include <vector>
#include <QDir>

namespace sj
{
	class DirControl
	{
	public:
		DirControl();
		static void makeFileList( char* _dir, std::vector<char*>&fileList );
		static void makeFileList( char* _dir, std::vector<char*>& _fileList, char* filter);
	};
}


#endif // LIBRARY_DIRCONTROL_H
