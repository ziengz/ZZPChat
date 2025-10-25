#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(asio::io_context&ioc):_socket(ioc)
{
}

//char 转为16进制

// 输入 x 是一个 0 - 15 之间的数字（表示十六进制的一位）
// 如果 x > 9（即 10 - 15），需要转换为字母 'A' - 'F'
// ASCII 码中，'A' 的十进制值是 65
// 所以 x + 55：10 + 55 = 65 → 'A'，11 + 55 = 66 → 'B'，依此类推
// 如果 x <= 9（即 0 - 9），需要转换为数字字符 '0' - '9'
// ASCII 码中，'0' 的十进制值是 48
// 所以 x + 48：0 + 48 = 48 → '0'，1 + 48 = 49 → '1'，依此类推
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

//从16进制转为十进制的char

//如果 x 是大写字母 'A' - 'Z'（ASCII 65 - 90）
//减去 'A' (65) 得到 0 - 25，然后加上 10 得到 10 - 35
//例如：'B' (66) → 66 - 65 + 10 = 11
//如果 x 是小写字母 'a' - 'z'（ASCII 97 - 122）
//减去 'a' (97) 得到 0 - 25，然后加上 10 得到 10 - 35
//例如：'b' (98) → 98 - 97 + 10 = 11
//如果 x 是数字 '0' - '9'（ASCII 48 - 57）
//减去 '0' (48) 直接得到 0 - 9
//例如：'5' (53) → 53 - 48 = 5
//如果 x 不是有效的十六进制字符，触发断言错误
unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
//编码
std::string UrlEncode(const std::string& str) {
	std::string strTemp = "";
	std::size_t length = str.length();
	for (int i = 0; i < length; i++) {
		//判断是否仅有数字和字母组成
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else {
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i]& 0x0F);
		}
	}
	return strTemp;
}
//解码
std::string UrlDeCode(const std::string& str)
{
	std::string strTemp = "";
	std::size_t length = str.length();
	for (int i = 0; i < length; i++)
	{
		if (str[i] == '+')strTemp = ' ';
		else if (str[i] == '%') {
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp = high * 16 + low;
		}
		else
			strTemp += str[i];
	}
	return strTemp;
}



void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, request_, [self](beast::error_code ec,std::size_t bytes_transfer) {
		try {
			if (ec) {
				std::cout << "http read err is " << ec.what() << std::endl;
				return;
			}
			boost::ignore_unused(bytes_transfer);
			self->process_request();
			self->check_deadline();
		}
		catch (std::exception& ec) {
			std::cout << "exception is " << ec.what() << std::endl;
			return;
		}
		});
}

void HttpConnection::PreParseGetParam()
{
	auto url = request_.target();   // 获取完整URI: "/path?key1=value1&key2=value2"
	//查找‘？’的位置
	auto query_pos = url.find('?');
	if (query_pos == std::string::npos) {
		_get_url = url;     //如果没有查到，就把整个字符串给他
		return;
	}
	_get_url = url.substr(0, query_pos);   //获取/path
	std::string query_string = url.substr(query_pos + 1); //获取key1=value1&key2=value2
	std::size_t pos = 0;
	std::string key;
	std::string value;
	while ((pos = query_string.find('&')) != std::string::npos)
	{
		auto pair = query_string.substr(0, pos);   //key1=value1
		std::size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos)
		{
			key = UrlDeCode(pair.substr(0, eq_pos));
			value = UrlDeCode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);           //key2=value2
	}
	//如果没有&符，并且还有数据
	if (!query_string.empty())
	{
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDeCode(query_string.substr(0, eq_pos));
			value = UrlDeCode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}


void HttpConnection::process_request()
{
	response_.version(request_.version());
	response_.keep_alive(false);
	PreParseGetParam();
	if (request_.method() == http::verb::get) {
		bool b_success = LogicSystem::GetIntance()->HandleGet(_get_url, shared_from_this());
		if (!b_success) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\r\n";
		}
		response_.result(http::status::ok);
		response_.set(http::field::server, "GataServer");
		WriteResponse();
		return;
	}
	else if (request_.method() == http::verb::post) {
		bool b_success = LogicSystem::GetIntance()->HandlePost(request_.target(), shared_from_this());
		if (!b_success) {
			response_.result(http::status::not_found);
			response_.set(http::field::content_type, "text/plain");
			beast::ostream(response_.body()) << "url not found\r\n";
		}
		response_.result(http::status::ok);
		response_.set(http::field::server, "GataServer");
		WriteResponse();
		return;
	}
	
}

void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	response_.content_length(response_.body().size());
	http::async_write(_socket, response_,[self](beast::error_code ec,std::size_t bytes_transfer) {
		self->_socket.shutdown(tcp::socket::shutdown_send, ec);
		self->_deadline.cancel();
		});
}


void HttpConnection::check_deadline()
{
	auto self = shared_from_this();
	_deadline.async_wait([self](beast::error_code ec) {
		if (!ec) {
			self->_socket.close();
		}
		});
}

