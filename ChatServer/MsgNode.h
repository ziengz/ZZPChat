#pragma once
#include "const.h"

class LogicSystem;
class MsgNode
{
public:
	MsgNode(int max_len):total_len_(max_len),cur_len_(0) {
		data_ = new char[total_len_ + 1];
		data_[total_len_] = '\0';
	}
	~MsgNode() {
		std::cout << "destory data_" << std::endl;
		delete[] data_;
	}
	void Clear() {
		memset(data_, 0, total_len_);
		cur_len_ = 0;
	}

	char* data_;
	int cur_len_;
	int total_len_;
};


class RecvNode :public MsgNode
{
	friend class LogicSystem;
public:
	RecvNode(short max_len, short msg_id);
private:
	short msg_id_;
};

class SendNode :public MsgNode {
	friend class LogicSystem;
public:
	SendNode(const char* data, short msg_len, short msg_id);
private:
	short msg_id_;
};