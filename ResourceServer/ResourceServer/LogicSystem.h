#pragma once
#include "Singleton.h"
#include "MsgNode.h"
#include "const.h"
#include <unordered_map>
#include "LogicWorker.h"

class FileInfo {
public:
	FileInfo(int seq = 0, std::string name = "", int total_size = 0, int trans_size = 0,
		std::string file_path_str = "")
		: _seq(seq),_name(name),_total_size(total_size),_trans_size(total_size),_file_path_str(file_path_str)
	{}
	int _seq = 0;
	std::string _name;
	int _total_size;
	int _trans_size;
	std::string _file_path_str;
};


class LogicSystem:public Singleton<LogicSystem>{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();

	void AddMd5File(std::string md5, std::shared_ptr<FileInfo>fileinfo);
	void PostMsgToQue(std::shared_ptr<LogicNode> work,int index);
	std::shared_ptr<FileInfo> GetFileInfo(std::string md5);
private:
	LogicSystem();

	std::vector<std::shared_ptr<LogicWorker>> _workers;
	std::unordered_map<std::string, std::shared_ptr<FileInfo>> _map_md5_file;
	std::mutex _mtx;
};

