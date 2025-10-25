#pragma once
#include "const.h"

class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(asio::io_context& ioc);
	void Start();
	void PreParseGetParam();
	tcp::socket& GetSocket(){
		return _socket;
	}

private:
	void check_deadline();
	void WriteResponse();
	void process_request();

	tcp::socket _socket;
	http::request<http::dynamic_body> request_;     //��������
	http::response<http::dynamic_body> response_;   //�ظ��ͻ���
	beast::flat_buffer _buffer{ 8192 };             //��������
	asio::steady_timer _deadline{                   //����Ƿ�ʱ
		_socket.get_executor(),std::chrono::seconds(60)
	};

	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;   //���ڴ洢key��value
};

