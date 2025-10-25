#include "IOContextPool.h"

IOContextPool::IOContextPool(std::size_t size):_io_contexts(size),_works(size), _nextIOContext(0)
{
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



IOContextPool::~IOContextPool()
{
	Stop();
	std::cout << "IOContextPool is destroy" << std::endl;
}

asio::io_context& IOContextPool::GetIO_Context()
{
	auto& io_context = _io_contexts[_nextIOContext++];
	if (_nextIOContext == _io_contexts.size()) {
		_nextIOContext = 0;
	}
	return io_context;
}

void IOContextPool::Stop()
{
	for (int i = 0; i < _io_contexts.size(); i++) {
		_io_contexts[i].stop();
		_works[i].reset();
	}
	for (auto&thread:_threads) {
		thread.join();
	}
}



