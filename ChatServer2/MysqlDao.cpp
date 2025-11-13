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

bool MysqlDao::CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo)
{
	auto con = pool_->getConnection();
	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return false;
		}
		

		std::unique_ptr<sql::PreparedStatement> state(con->con_->prepareStatement("select * from user where email = ?"));
		state->setString(1, email);
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
		userInfo.email = email;
		userInfo.name = result->getString("name");
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

std::shared_ptr<UserInfo> MysqlDao::GetUser(int uid)
{
	auto con = pool_->getConnection();
	Defer defer([&con,this] {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return nullptr;
		}
		std::unique_ptr<sql::PreparedStatement>state(con->con_->prepareStatement("select * from user where uid = ?"));
		state->setInt(1, uid);

		//执行查询
		std::unique_ptr<sql::ResultSet>res(state->executeQuery());
		std::shared_ptr<UserInfo>user_info = nullptr;
		while (res->next()) {
			user_info.reset(new UserInfo);
			user_info->email = res->getString("email");
			user_info->name = res->getString("name");
			user_info->pwd = res->getString("pwd");
			user_info->uid = uid;
		}
		return user_info;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}

bool MysqlDao::AddFriendApply(const int& from, const int& to) {
	auto con = pool_->getConnection();
	Defer defer([&con,this]() {
		pool_->returnConnection(std::move(con));
	});
	try {
		if (con == nullptr) {
			return false;
		}
		std::unique_ptr<sql::PreparedStatement>pstmt(con->con_->prepareStatement("insert into friend_apply (from_uid,to_uid) values(?,?)"
			"ON DUPLICATE KEY UPDATE from_uid = from_uid, to_uid = to_uid"));
		pstmt->setInt(1, from);
		pstmt->setInt(2, to);
		int rowAffected = pstmt->executeUpdate();
		if (rowAffected < 0) {
			return false;
		}
		return true;
	}
	catch (sql::SQLException&e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
	return true;
}

bool MysqlDao::AuthFriendApply(const int& from, const int& to) {
	auto con = pool_->getConnection();
	Defer defer([&con,this]() {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr){
			return false;
		}
		std::shared_ptr<sql::PreparedStatement>pstmt(con->con_->prepareStatement("update friend_apply set status = 1 "
			"where from_uid = ? and to_uid = ?"));
		//因为是对方认证发过来的申请，所以要反过来设置from和to的uid
		pstmt->setInt(1, to);
		pstmt->setInt(2, from);
		int rowAffected = pstmt->executeUpdate();
		if (rowAffected < 0) {
			return false;
		}
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::AddFriend(const int& from, const int& to, std::string back_name) {
	auto con = pool_->getConnection();
	Defer defer([&con, this] {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return false;
		}
		//开始事务
		con->con_->setAutoCommit(false);

		// 准备第一个SQL语句, 插入认证方好友数据
		std::shared_ptr<sql::PreparedStatement>pstmt(con->con_->prepareStatement("insert ignore into friend(self_id,friend_id,back) "
			"values (?,?,?)"));
		pstmt->setInt(1, from);
		pstmt->setInt(2, to);
		pstmt->setString(3, back_name);
		int rowAffected = pstmt->executeUpdate();
		if (rowAffected < 0) {
			con->con_->rollback();
			std::cout << "executed failed" << std::endl;
			return false;
		}

		//准备第二个SQL语句，插入申请方好友数据
		std::unique_ptr<sql::PreparedStatement> pstmt2(con->con_->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) "
			"VALUES (?, ?, ?) "
		));
		//反过来的申请时from，验证时to
		pstmt2->setInt(1, to); // from id
		pstmt2->setInt(2, from);
		pstmt2->setString(3, "");
		// 执行更新
		int rowAffected2 = pstmt2->executeUpdate();
		if (rowAffected2 < 0) {
			con->con_->rollback();
			return false;
		}

		// 提交事务
		con->con_->commit();
		std::cout << "addfriend insert friends success" << std::endl;

		return true;
	}
	catch (sql::SQLException& e) {
		// 如果发生错误，回滚事务
		if (con) {
			con->con_->rollback();
		}
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
	return true;
}

std::shared_ptr<UserInfo> MysqlDao::GetUser(std::string name)
{
	auto con = pool_->getConnection();
	Defer defer([&con, this] {
		pool_->returnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return nullptr;
		}
		std::unique_ptr<sql::PreparedStatement>state(con->con_->prepareStatement("select * from user where name = ?"));
		state->setString(1, name);

		//执行查询
		std::unique_ptr<sql::ResultSet>res(state->executeQuery());
		std::shared_ptr<UserInfo>user_info = nullptr;
		while (res->next()) {
			user_info.reset(new UserInfo);
			user_info->email = res->getString("email");
			user_info->name = name;
			user_info->pwd = res->getString("pwd");
			user_info->uid = res->getInt("uid");
		}
		return user_info;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}
