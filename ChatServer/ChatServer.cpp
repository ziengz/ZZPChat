// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <csignal>
#include "const.h"
#include "ConfigMgr.h"
#include "CServer.h"
#include "IOContextPool.h"


int main()
{
    try {
        auto& config = ConfigMgr::Instance();
        auto pool = IOContextPool::GetIntance();
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, pool](auto, auto) {
            pool->Stop();
            ioc.stop();
            });
        auto port = config["SelfServer"]["Port"];
        CServer server(ioc, atoi(port.c_str()));
        ioc.run();
    }
    catch (std::exception&e) {
        std::cout << "Exception is " << e.what() << std::endl;
        return 0;
    }
}

