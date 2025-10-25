#include "MysqlDao.h"

MysqlDao::MysqlDao()
{
	auto& config = ConfigMgr::Instance();
	const auto& host = config["Mysql"]["Host"];
	const auto& port = config["Mysql"]["Port"];
	const auto& pwd = config["Mysql"]["Passwd"];
	const auto& schema = config["Mysql"]["Schema"];
	const auto& user = config["Mysql"]["User"];
	pool_.reset(new MySqlPool(host + ":" + port, user, pwd, schema, 5));
}

MysqlDao::~MysqlDao()
{
	pool_->Close();
}


int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			//因为只有当b_stop状态关闭时才会出现con为空，所以不用重新获得了
			return false;
		}
		//准备调用存储过程
		std::unique_ptr<sql::PreparedStatement>stmt(con->con_->prepareStatement("CALL reg_user(?,?,?,@result)"));
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		//执行存储过程
		stmt->execute();
		std::unique_ptr<sql::Statement> stmtResult(con->con_->createStatement());
		std::unique_ptr<sql::ResultSet>res(stmtResult->executeQuery("select @result as result"));
		
		//为true，说明执行成功
		if (res->next()) {
			int result = res->getInt("result");
			std::cout << "Result:" << result << std::endl;
			pool_->returnConnection(std::move(con));
			return result;
		}
		pool_->returnConnection(std::move(con));
		return -1;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cout << "SQLException: " <<e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ",SQLState: " << e.getSQLState() << ")" << std::endl;
		return -1;
	}

}

bool MysqlDao::CheckEmail(const std::string& name, const std::string& email)
{
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}
		std::unique_ptr<sql::PreparedStatement>state(con->con_->prepareStatement("select email from user where name = ?"));
		state->setString(1, name);
		std::unique_ptr<sql::ResultSet> res(state->executeQuery());

		if (res->next()) {
			if (email != res->getString("email"))
			{
				pool_->returnConnection(std::move(con));
				return false;
			}
			pool_->returnConnection(std::move(con));
			return true;
		}
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::UpdatePwd(const std::string& name, const std::string& newpwd) {
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}
		std::unique_ptr<sql::PreparedStatement> state(con->con_->prepareStatement("update user set pwd = ? where name = ?"));
		state->setString(1, newpwd);
		state->setString(2, name);

		int updataCount = state->executeUpdate();
		std::cout << "updated rows:" << updataCount << std::endl;
		pool_->returnConnection(std::move(con));
		return true;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo)
{
	auto con = pool_->getConnection();
	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return false;
		}
		

		std::unique_ptr<sql::PreparedStatement> state(con->con_->prepareStatement("select * from user where name = ?"));
		state->setString(1, name);
		std::unique_ptr<sql::ResultSet> result(state->executeQuery());
		std::string origin_pwd = "";

		while (result->next()) {
			origin_pwd = result->getString("pwd");
			std::cout << "pwd is " << origin_pwd << std::endl;
			break;
		}
		if (pwd != origin_pwd) {
			return false;
		}
		userInfo.email = result->getString("email");
		userInfo.name = name;
		userInfo.uid = result->getInt("uid");
		userInfo.pwd = origin_pwd;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}
