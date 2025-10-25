// StatusServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "const.h"
#include "MySqlMgr.h"
#include "RedisMgr.h"
#include "ConfigMgr.h"
#include "IOContextPool.h"
#include <grpcpp/grpcpp.h>
#include "StatusServiceImpl.h"

void RunServer() {
    auto& config = ConfigMgr::Instance();
    std::string server_address(config["StatusServer"]["Host"] + ":" + config["StatusServer"]["Port"]);
    
    StatusServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    //构建并启动grpc服务器
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;


    boost::asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&server](const boost::system::error_code& error, int signal_num) {
        if (!error) {
            std::cout << "Shutting down server..." << std::endl;
            server->Shutdown();
        }
    });
    std::thread([&io_context]() {io_context.run(); }).detach();

    server->Wait();
    io_context.stop();
}

int main()
{
    try {
        RunServer();
    }
    catch (std::exception&e) {
        std::cout << "Error:" << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
