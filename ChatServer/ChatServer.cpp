// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <csignal>
#include "const.h"
#include "ConfigMgr.h"
#include "CServer.h"
#include "IOContextPool.h"
#include "RedisMgr.h"
#include <grpcpp/grpcpp.h>
#include "ChatServiceImpl.h"

using grpc::ServerBuilder;


int main()
{
    try {
        auto& config = ConfigMgr::Instance();
        std::string server_name = config["SelfServer"]["Name"];
        //将登录数设置为0
        RedisMgr::GetIntance()->HSet(LOGIN_COUNT, server_name, "0");

        std::string server_address(config["SelfServer"]["Host"] + ":" + config["SelfServer"]["RPCPort"]);
        ServerBuilder builder;
        ChatServiceImpl service;

        builder.AddListeningPort(server_address,grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server>server(builder.BuildAndStart());
        std::cout << "server listening on " << server_address << std::endl;

        std::thread grpc_server_thread([&server](){
            server->Wait();
        });


        auto pool = IOContextPool::GetIntance();
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, pool,&server](auto, auto) {
            pool->Stop();
            ioc.stop();
            server->Shutdown();
            });
        auto port = config["SelfServer"]["Port"];
        CServer s(ioc, atoi(port.c_str()));
        ioc.run();

        RedisMgr::GetIntance()->HDel(LOGIN_COUNT, server_name);
        RedisMgr::GetIntance()->Close();
        grpc_server_thread.join();
    }
    catch (std::exception&e) {
        std::cout << "Exception is " << e.what() << std::endl;
        return 0;
    }
}

