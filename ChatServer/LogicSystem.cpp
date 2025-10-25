#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "MySqlMgr.h"
#include "CServer.h"

LogicSystem::LogicSystem():_b_stop(false)
{
	RegisterCallBack();
	_work_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem()
{
	std::lock_guard<std::mutex>lock(mutex_);
	_b_stop = true;
	cond_.notify_all();
}

void LogicSystem::RegisterCallBack()
{
	_fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LogicHandler, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg)
{
	std::unique_lock<std::mutex>lock(mutex_);
	_msg_que.push(msg);
	if (_msg_que.size() == 1) {
		lock.unlock();
		cond_.notify_one();
	}
}

void LogicSystem::DealMsg()
{
	while (true) {
		std::unique_lock<std::mutex>lock(mutex_);
		//�������Ϊ���ҷ�����ûֹͣ��ȴ�����
		while (_msg_que.empty() && !_b_stop) {
			cond_.wait(lock);
		}
		//���ֹͣ������������Ƿ�������
		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msgnode = _msg_que.front();
				std::cout << "recv_msg id is " << msgnode->_recvnode->msg_id_ << std::endl;
				auto call_back_iter = _fun_callbacks.find(msgnode->_recvnode->msg_id_);
				if (call_back_iter == _fun_callbacks.end()) {
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msgnode->_session, msgnode->_recvnode->msg_id_,
					std::string(msgnode->_recvnode->data_,msgnode->_recvnode->cur_len_));
				_msg_que.pop();
			}
			break;
		}
		//���û��ͣ�����������ȡ
		auto msgnode = _msg_que.front();
		std::cout << "recv msg id is " << msgnode->_recvnode->msg_id_ << std::endl;
		auto call_back_iter = _fun_callbacks.find(msgnode->_recvnode->msg_id_);
		if (call_back_iter == _fun_callbacks.end()) {
			_msg_que.pop();
			std::cout << "msg id [" << msgnode->_recvnode->msg_id_ << "] handler not found" << std::endl;
			continue;
		}
		call_back_iter->second(msgnode->_session, msgnode->_recvnode->msg_id_,
			std::string(msgnode->_recvnode->data_, msgnode->_recvnode->cur_len_));
		_msg_que.pop();
	}
}

void LogicSystem::LogicHandler(std::shared_ptr<Session> session, const short& msg_id, const std::string &msg_data)
{
	Json::Reader reader;
	Json::Value src_root;
	reader.parse(msg_data, src_root);
	auto uid = src_root["uid"].asInt();
	auto token = src_root["token"].asString();
	std::cout << "user login uid is " << uid << " user token is "
		<< token << std::endl;

	auto rsp = StatusGrpcClient::GetIntance()->Login(uid, token);

	Json::Value returnValue;
	//�ú���������֮����Զ���������
	Defer defer([this,&returnValue,session]() {
		std::string return_str = returnValue.toStyledString();
		session->Send(return_str, MSG_CHAT_LOGIN_RSP);
		});

	returnValue["error"] = rsp.error();
	if (returnValue["error"] != Error_Codes::Success)
	{
		return;
	}

	//�ڴ��в�ѯ�û���Ϣ
	std::shared_ptr<UserInfo>user_info = nullptr;	
	auto find_iter = _users.find(uid);
	if (find_iter == _users.end()) {
		//����ڴ���û�鵽�������ݿ��в�ѯ
		user_info = MySqlMgr::GetIntance()->GetUser(uid);
		if (user_info == nullptr) {
			returnValue["error"] = Error_Codes::UidInvalid;
			return;
		}
		_users[uid] = user_info;
	}
	else {
		user_info = find_iter->second;
	}
	returnValue["uid"] = uid;
	returnValue["token"] = rsp.token();
	returnValue["name"] = user_info->name;
	
}
