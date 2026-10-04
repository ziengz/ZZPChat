#include "LogicSystem.h"

LogicSystem::LogicSystem() {
	for (int i = 0; i < LOGIC_WORKER_COUNT; i++) {
		_workers.push_back(std::make_shared<LogicWorker>());
	}
}

LogicSystem::~LogicSystem() {

}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> work, int index) {
	_workers[index]->PostTask(work);
}

void LogicSystem::AddMd5File(std::string md5, std::shared_ptr<FileInfo>fileInfo) {
	std::lock_guard<std::mutex>lck(_mtx);
	_map_md5_file[md5] = fileInfo;
}
std::shared_ptr<FileInfo> LogicSystem::GetFileInfo(std::string md5) {
	std::lock_guard<std::mutex>lck(_mtx);
	std::shared_ptr<FileInfo> fileInfo;
	auto it = _map_md5_file.find(md5);
	if (it != _map_md5_file.end()) {
		fileInfo = it->second;
	}
	return fileInfo;
}