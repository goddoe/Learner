// Author : Sung-ju Kim
// Email  : goddoe2@gmail.com
// github : https://github.com/goddoe
// The MIT License (MIT)
// Copyright (c) 2016 Sung-ju Kim

#include "DirControl.h"

namespace sj
{
	DirControl::DirControl()
	{
	}

	void DirControl::makeFileList( char* _dir, std::vector<char*>& _fileList)
	{
		QDir dir(_dir);
		QFileInfoList list = dir.entryInfoList();
		for(int i = 0 ; i < list.size() ; ++i)
		{
			QFileInfo fileInfo = list.at(i);

			char* str = new char[50];
			//qPrintable(fileInfo.fileName());
			strcpy(str, qPrintable(fileInfo.fileName()));
			_fileList.push_back( str );
		}
	}

	void DirControl::makeFileList( char* _dir, std::vector<char*>& _fileList, char* filter)
	{
		QDir dir(_dir);
		dir.setNameFilters(QStringList()<<filter);
		QFileInfoList list = dir.entryInfoList();
        printf("Directory : %s , Number of Files : %d\n", _dir, list.size());
		for(int i = 0 ; i < list.size() ; ++i)
		{
			QFileInfo fileInfo = list.at(i);

			char* str = new char[50];
			//qPrintable(fileInfo.fileName());
			strcpy(str, qPrintable(fileInfo.fileName()));
			_fileList.push_back( str );
		}
	}
}
