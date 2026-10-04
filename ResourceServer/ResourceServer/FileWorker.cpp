#include "FileWorker.h"
#include "base64.h"
#include "ConfigMgr.h"

FileWorker::FileWorker():_b_stop(false) {
	_task_thread = std::thread([this]() {
		while (!_b_stop) {
			std::unique_lock<std::mutex>lck(_mtx);
			_cv.wait(lck, [this] {
				if (_b_stop)
					return true;
				if (_task_que.empty())
					return false;
				return true;
			});
			if (_b_stop)
				break;
			auto task = _task_que.front();
			task_callback(task);
			_task_que.pop();
		}
	});

}
FileWorker::~FileWorker() {
	_b_stop = true;
	_cv.notify_all();
	_task_thread.join();
}
void FileWorker::PostTask(std::shared_ptr<FileTask> task) {
	{
		std::lock_guard<std::mutex> lck(_mtx);
		_task_que.push(task);
	}
	_cv.notify_one();
}

void FileWorker::task_callback(std::shared_ptr<FileTask> task) {
	// 执行下载逻辑
	std::string decode = base64_decode(task->_file_data);

	auto name = task->_name;
	auto file_path = ConfigMgr::Inst().GetFileOutPath();
	auto file_path_str = (file_path / name).string();
	auto last = task->_last;
	
	std::cout << file_path_str << std::endl;
	std::ofstream ofs;
	// 第一个包
	if (task->_seq == 1) {
		// 覆盖存在的文件
		ofs.open(file_path_str, std::ios::trunc | std::ios::binary);
	}
	else {
		// 追加
		ofs.open(file_path_str, std::ios::app | std::ios::binary);
	}

	if (!ofs) {
		std::cerr << "无法打开文件进行写入。" << std::endl;
		return ;
	}

	ofs.write(decode.data(), decode.size());
	if (!ofs) {
		std::cout << "写入文件失败" << std::endl;
		return;
	}

	ofs.close();
	if (last) {
		std::cout << "写入文件成功" << std::endl;
	}
}
