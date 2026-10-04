#include "FileSystem.h"
#include "const.h"

FileSystem::FileSystem() {
	for (int i = 0; i < FILE_WORKER_COUNT; i++) {
		_file_workers.push_back(std::make_shared<FileWorker>());
	}
}


FileSystem::~FileSystem() {
	_file_workers.clear();
}
void FileSystem::PostMsgToQue(std::shared_ptr<FileTask> task,int index) {
	_file_workers[index]->PostTask(task);
}
