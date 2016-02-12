// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

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
