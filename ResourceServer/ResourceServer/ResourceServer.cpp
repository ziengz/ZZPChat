// ResourceServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
#include "ConfigMgr.h"
#include "CServer.h"


using namespace std;
bool bStop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	auto& config = ConfigMgr::Inst();
	auto server_name = config["ResourceServer"]["Name"];
	
	std::shared_ptr<AsioIOServicePool> pool = nullptr;
	try {
		pool = AsioIOServicePool::GetInstance();
		
		boost::asio::io_context ioc;
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

		signals.async_wait([&ioc, pool](auto, auto) {
			ioc.stop();
			pool->stop();
			});
		auto port = config["ResourceServer"]["Port"];
		CServer server(ioc, atoi(port.c_str()));
		ioc.run();
	}
	catch (std::exception& ec) {
		std::cerr << "exception is " << ec.what() << std::endl;
	}
}

