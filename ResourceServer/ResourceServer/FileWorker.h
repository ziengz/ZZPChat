#pragma once
#include "const.h"

class CSession;
struct FileTask {
	FileTask(std::shared_ptr<CSession> session, std::string name,
		int seq, int total_size, int trans_size, int last,
		std::string file_data) :_session(session),
		_seq(seq), _name(name), _total_size(total_size),
		_trans_size(trans_size), _last(last), _file_data(file_data)
	{
	}
	~FileTask() {}
	std::shared_ptr<CSession> _session;

	int _seq;
	std::string _name;
	int _total_size;
	int _trans_size;
	int _last;
	std::string _file_data;
};

class FileWorker
{
public:
	FileWorker();
	~FileWorker();
	void PostTask(std::shared_ptr<FileTask> task);
private:
	void task_callback(std::shared_ptr<FileTask> task);
	std::thread _task_thread;
	std::mutex _mtx;
	std::atomic<bool> _b_stop;
	std::condition_variable _cv;
	std::queue<std::shared_ptr<FileTask>> _task_que;
};

