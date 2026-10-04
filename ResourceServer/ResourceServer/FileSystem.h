#pragma once
#include "Singleton.h"
#include "FileWorker.h"

class FileSystem:public Singleton<FileSystem>
{
	friend class Singleton<FileSystem>;
public:
	~FileSystem();
	void PostMsgToQue(std::shared_ptr<FileTask> task, int index);
private:
	FileSystem();
	std::vector<std::shared_ptr<FileWorker>> _file_workers;
};

