#include "LogicWorker.h"
#include "json/value.h"
#include "json/reader.h"
#include "json/json.h"
#include "CSession.h"
#include "LogicSystem.h"
#include "ConfigMgr.h"
#include "FileSystem.h"

LogicWorker::LogicWorker():_b_stop(false) {
	RegisterCallBack();
	_thread = std::thread([this]() {
		while (!_b_stop) {
			std::unique_lock<std::mutex> lck(_mtx);
			_cv.wait(lck, [this] {
				if (_b_stop) {
					return true;
				}
				if (_task_que.empty()) {
					return false;
				}
				return true;
			});
			if (_b_stop)
				return;
			auto task = _task_que.front();
			task_callback(task);
			_task_que.pop();
		}
	});
}
LogicWorker::~LogicWorker() {
	_b_stop = true;
	_cv.notify_all();
	_thread.join();
}

void LogicWorker::PostTask(std::shared_ptr<LogicNode> logicNode) {
	std::lock_guard<std::mutex>lck(_mtx);
	_task_que.push(logicNode);
	_cv.notify_one();
}
void LogicWorker::RegisterCallBack() {
	_fun_callbacks[ID_TEST_MSG_REQ] = [this](std::shared_ptr<CSession> session,const short& msg_id,
		const std::string& msg_data) {
			Json::Reader reader;
			Json::Value root;
			reader.parse(msg_data, root);
			auto data = root["data"].asString();
			std::cout << "recv test data is " << data << std::endl;
			
			Json::Value rtvalue;
			Defer defer([this,session,&rtvalue] {
				std::string return_str = rtvalue.toStyledString();
				session->Send(return_str, ID_TEST_MSG_RSP);
				});
			rtvalue["error"] = ErrorCodes::Success;
			rtvalue["data"] = data;
		};
	_fun_callbacks[ID_UPLOAD_FILE_REQ] = [this](std::shared_ptr<CSession>session,const short&msg_id,
		const std::string&msg_data) {
			Json::Reader reader;
			Json::Value root;
			reader.parse(msg_data, root);
			auto md5 = root["md5"].asString();
			auto seq = root["seq"].asInt();
			auto name = root["name"].asString();
			auto total_size = root["total_size"].asInt();
			auto trans_size = root["trans_size"].asInt();
			auto last = root["last"].asInt();
			auto file_data = root["data"].asString();
			auto file_path = ConfigMgr::Inst().GetFileOutPath();
			auto file_path_str = (file_path / name).string();

			Json::Value rtvalue;
			Defer defer([this, session, &rtvalue]() {
				auto return_str = rtvalue.toStyledString();
				session->Send(return_str, ID_UPLOAD_FILE_RSP);
				});

			// 使用hash对文件名进行哈希
			std::hash<std::string> hash_fn;
			std::size_t hash_value = hash_fn(name);
			int index = hash_value % FILE_WORKER_COUNT;
			std::cout << "hash value: " << hash_value << std::endl;

			if (seq == 1) {
				auto fileInfo = std::make_shared<FileInfo>();
				fileInfo->_name = name;
				fileInfo->_seq = seq;
				fileInfo->_total_size = total_size;
				fileInfo->_trans_size = trans_size;

				LogicSystem::GetInstance()->AddMd5File(md5, fileInfo);
			}
			else {
				std::shared_ptr<FileInfo> fileInfo = LogicSystem::GetInstance()->GetFileInfo(md5);
				if (!fileInfo) {
					rtvalue["error"] = ErrorCodes::FileNotExists;
					return;
				}
				fileInfo->_seq = seq;
				fileInfo->_trans_size = trans_size;
			}

			// 投递到文件系统中
			// 投递文件要确认哪一个会话、文件信息，以及哪个线程
			FileSystem::GetInstance()->PostMsgToQue(
				std::make_shared<FileTask>(session, name,
					seq, total_size, trans_size, last,
					file_data)
				, index
			);

			rtvalue["error"] = ErrorCodes::Success;
			rtvalue["total_size"] = total_size;
			rtvalue["trans_size"] = trans_size;
			rtvalue["name"] = name;
			rtvalue["seq"] = seq;
			rtvalue["md5"] = md5;
			rtvalue["last"] = last;
		};
	_fun_callbacks[ID_SYNC_FILE_REQ] = [this](std::shared_ptr<CSession> session, const short& msgid, 
		const std::string& msg_data) {
			Json::Reader reader;
			Json::Value root;
			reader.parse(msg_data, root);

			Json::Value rtvalue;
			Defer defer([session,this,&rtvalue]() {
					auto return_str = rtvalue.toStyledString();
					session->Send(return_str, ID_SYNC_FILE_RSP);
				});
			auto md5 = root["md5"].asString();
			
			std::shared_ptr<FileInfo> fileInfo = LogicSystem::GetInstance()->GetFileInfo(md5);
			if (!fileInfo) {
				rtvalue["error"] = ErrorCodes::FileNotExists;
				return;
			}

			rtvalue["error"] = ErrorCodes::Success;
			rtvalue["trans_size"] = fileInfo->_trans_size;
			rtvalue["seq"] = fileInfo->_seq;
			rtvalue["name"] = fileInfo->_name;
			rtvalue["total_size"] = fileInfo->_total_size;
			rtvalue["md5"] = md5;

		};
}

void LogicWorker::task_callback(std::shared_ptr<LogicNode> task) {
	short msg_id = task->_recvNode->_msg_id;
	auto iter = _fun_callbacks.find(msg_id);
	if (iter == _fun_callbacks.end()) {
		return;
	}
	iter->second(task->_session, msg_id,
		std::string(task->_recvNode->_data, task->_recvNode->_cur_len));
}




