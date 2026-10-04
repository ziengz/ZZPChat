#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size): _io_contexts(size),_works(size),_nextIOService(0){
	//使用work的作用是防止io_context因无任务，而退出
	for (int i = 0; i < size; i++) {
		_works.emplace_back(std::make_unique<Work>(_io_contexts[i].get_executor()));
	}

	for (int i = 0; i < size; i++) {
		_threads.emplace_back([this, i]() {
			_io_contexts[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	stop();
	std::cout << "AsioIOService is destructed" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::getIOService() {
	auto& service = _io_contexts[_nextIOService++];
	if (_nextIOService == _io_contexts.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::stop() {
	// iocontext绑定读写了，需要手动stop该服务
	for (int i = 0; i < _io_contexts.size(); i++) {
		_io_contexts[i].stop();
		_works[i].reset();
	}
	for (auto& thread : _threads) {
		thread.join();
	}
}