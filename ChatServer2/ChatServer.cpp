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
#include "LogicSystem.h"

using grpc::ServerBuilder;


int main()
{
    try {
        auto& config = ConfigMgr::Instance();
        auto server_name = config["SelfServer"]["Name"];

        auto pool = IOContextPool::GetIntance();
        //将登录数设置为0
        RedisMgr::GetIntance()->HSet(LOGIN_COUNT, server_name, "0");
        Defer defer([&server_name]() {
			RedisMgr::GetIntance()->HDel(LOGIN_COUNT, server_name);
			RedisMgr::GetIntance()->Close();
            });

        boost::asio::io_context ioc;
        auto port = config["SelfServer"]["Port"];
        auto server_ptr = std::make_shared<CServer>(ioc, atoi(port.c_str()));
        //启动定时器
        server_ptr->StartTimer();

        //定义GrpcServer
        std::string server_address(config["SelfServer"]["Host"] + ":" + config["SelfServer"]["RPCPort"]);
        ServerBuilder builder;
        ChatServiceImpl service;
        //监听端口和添加服务
        builder.AddListeningPort(server_address,grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        service.RegisterServer(server_ptr);

        std::unique_ptr<grpc::Server>server(builder.BuildAndStart());
        std::cout << "server listening on " << server_address << std::endl;

        //单独启动一个线程处理grpc服务
        std::thread grpc_server_thread([&server](){
            server->Wait();
        });


        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, pool,&server](auto, auto) {
            pool->Stop();
            ioc.stop();
            server->Shutdown();
            });
        
        //将CServer注册给逻辑类方便后续清除链接
        LogicSystem::GetIntance()->SetServer(server_ptr);

        CServer s(ioc, atoi(port.c_str()));
        ioc.run();

        grpc_server_thread.join();
        server_ptr->StopTimer();
    }
    catch (std::exception&e) {
        std::cout << "Exception is " << e.what() << std::endl;
        return 0;
    }
}

