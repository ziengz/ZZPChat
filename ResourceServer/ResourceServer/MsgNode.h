#pragma once
#include <string>
#include <iostream>
#include "const.h"
#include <boost/asio.hpp>

class MsgNode
{
public:
	MsgNode(int max_len) :_total_len(max_len), _cur_len(0) {
		_data = new char[_total_len + 1];
		_data[_total_len] = '\0';
	}
	~MsgNode() {
		std::cout << "destruct MsgNode" << std::endl;
		delete[] _data;
	}

	void clear() {
		::memset(_data, 0, _total_len);
		_cur_len = 0;
	}

public:
	int _total_len;
	int _cur_len;
	char* _data;
};

class recvNode: public MsgNode{
public:
	recvNode(int max_len, int msg_id);
	short _msg_id;
};

class sendNode : public MsgNode {
public:
	sendNode(const char* msg,int max_len, int msg_id);
	short _msg_id;
};