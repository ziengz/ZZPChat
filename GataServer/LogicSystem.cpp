#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VarifyGrpcClient.h"
#include "RedisMgr.h"
#include "MySqlMgr.h"
#include "StatusGrpcClient.h"

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handles.find(path) == _get_handles.end())
	{
		return false;
	}
	_get_handles[path](con);  //调用函数
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection>con)
{
	if (_post_handles.find(path) == _post_handles.end())
	{
		return false;
	}
	_post_handles[path](con);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandle handle)
{
	_get_handles.insert(make_pair(url, handle));
}

void LogicSystem::RegPost(std::string url, HttpHandle handle)
{
	_post_handles.insert(make_pair(url, handle));
}

LogicSystem::LogicSystem()
{
	RegGet("/get_test", [](std::shared_ptr<HttpConnection>connection) {
		beast::ostream(connection->response_.body()) << "receive get_test reg" << std::endl;
		int i = 0;
		for (auto& elem : connection->_get_params) {
			i++;
			beast::ostream(connection->response_.body())
				<< "param" << i << " key is " << elem.first
				<< "," << "value is " << elem.second << std::endl;

		}
		});
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection>connection) {
		auto body_str = beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive data is " << body_str << std::endl;
		connection->response_.set(http::field::content_type, "text/json");
		Json::Reader reader;
		Json::Value root;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success)
		{
			std::cout << "failed to parse Json" << std::endl;
			root["error"] = Error_Codes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
		}

		std::string email = src_root["email"].asString();
		GetVarifyRsp rsp = VarifyGrpcClient::GetIntance()->GetVarifyCode(email);
		std::cout << "receive email is " << email << std::endl;
		root["error"] = rsp.error();
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->response_.body()) << jsonstr;
		});

	RegPost("/user_register", [](std::shared_ptr<HttpConnection>connection) {
		auto body_str = beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body_str is " << body_str << std::endl;
		Json::Reader reader;
		Json::Value root;
		Json::Value src_root;
		auto parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to Parse Json!" << std::endl;
			root["error"] = Error_Codes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}

		auto email = src_root["email"].asString();
		auto name = src_root["user"].asString();
		auto pwd = src_root["passwd"].asString();
		auto confirm = src_root["confirm"].asString();

		//先查找redis中email对应的验证码是否过期
		std::string varify_code;
		bool b_get_varify = RedisMgr::GetIntance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
		if (!b_get_varify) {
			std::cout << "get varify code expired" << std::endl;
			root["error"] = Error_Codes::VarifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}
		
		if (varify_code != src_root["varifycode"].asString()) {
			std::cout << "varify Code error" << std::endl;
			root["error"] = Error_Codes::VarifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
			return true;
		}

		int uid = MySqlMgr::GetIntance()->RegUser(name, email, pwd);
		if (uid == 0 || uid == -1) {
			std::cout << "user or email is exist" << std::endl;
			root["error"] = Error_Codes::UserExist;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->response_.body()) << jsonstr;
		}

		root["error"] = 0;
		root["uid"] = uid;
		root["email"] = src_root["email"];
		root["user"] = src_root["user"].asString();
		root["passwd"] = src_root["passwd"].asString();
		root["confirm"] = src_root["confirm"].asString();
		root["varifycode"] = src_root["varifycode"].asString();

		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->response_.body()) << jsonstr;
		return true;
		});

	RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection>connection) {
		auto body_str = beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body_str is " << body_str << std::endl;
		Json::Reader reader;
		Json::Value root;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			root["error"] = Error_Codes::Error_Json;
			std::string json_str = root.toStyledString();
			std::cout << "failed to parse json" << std::endl;
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}

		auto user = src_root["user"].asString();
		auto email = src_root["email"].asString();
		auto pwd = src_root["passwd"].asString();

		//从redis中读取判断是否过期
		std::string varify_code;
		bool b_get_varify = RedisMgr::GetIntance()->Get(CODEPREFIX + email, varify_code);
		if (!b_get_varify) {
			std::cout << "get varify code expire" << std::endl;
			root["error"] = Error_Codes::VarifyExpired;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}
		//判断验证码是否正确
		if (varify_code != src_root["varifycode"].asString()) {
			std::cout << "varify code is error" << std::endl;
			root["error"] = Error_Codes::VarifyCodeErr;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}

		bool b_email_valid = MySqlMgr::GetIntance()->CheckEmail(user, email);
		if (!b_email_valid) {
			std::cout << "user email not match" << std::endl;
			root["error"] = Error_Codes::EmailNotMatch;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}
		bool b_update = MySqlMgr::GetIntance()->UpdatePwd(user, pwd);
		if (!b_update) {
			std::cout << "failed to update password"  << std::endl;
			root["error"] = Error_Codes::PasswdUpFailed;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}

		std::cout << "successed to update password" << std::endl;
		root["error"] = 0;
		root["email"] = email;
		root["user"] = user;
		root["pwd"] = pwd;
		root["varifycode"] = src_root["varofycode"].asString();
		std::string json_str = root.toStyledString();
		beast::ostream(connection->response_.body()) << json_str;
		return true;

		});

	RegPost("/user_login", [](std::shared_ptr<HttpConnection> connection){
		std::string body_str = beast::buffers_to_string(connection->request_.body().data());
		std::cout << "receive body_str is " << body_str << std::endl;
		Json::Reader reader;
		Json::Value src_root;
		Json::Value root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "failed to parse json" << std::endl;
			root["error"] = Error_Codes::Error_Json;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}
		auto email = src_root["email"].asString();
		auto pwd = src_root["passwd"].asString();
		UserInfo userInfo;

		bool pwd_valid = MySqlMgr::GetIntance()->CheckPwd(email, pwd, userInfo);
		if (!pwd_valid) {
			std::cout << "pwd is error" << std::endl;
			root["error"] = Error_Codes::PasswdInvalid;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}
		auto reply = StatusGrpcClient::GetIntance()->GetChatServer(userInfo.uid);
		if (reply.error()) {
			std::cout << "grpc get chat server failed,error is " << reply.error() << std::endl;
			root["error"] = Error_Codes::RPCFailed;
			std::string json_str = root.toStyledString();
			beast::ostream(connection->response_.body()) << json_str;
			return true;
		}

		std::cout << "success to load userInfo,uid is " << userInfo.uid << std::endl;
		root["error"] = 0;
		root["email"] = email;
		root["uid"] = userInfo.uid;
		root["token"] = reply.token();
		root["host"] = reply.host();
		root["port"] = reply.port();
		std::string json_str = root.toStyledString();
		beast::ostream(connection->response_.body()) << json_str;
		return true;

	});
}
