#pragma once
#include "Singleton.h"
#include "const.h"

class IOContextPool:public Singleton<IOContextPool>
{
	friend Singleton<IOContextPool>;
public:
	using IOContext = asio::io_context;
	using Work = asio::executor_work_guard<asio::io_context::executor_type>;
	using WorkPtr = std::unique_ptr<Work>;
	
	~IOContextPool();
	IOContextPool(const IOContextPool&) = delete;
	IOContextPool& operator=(const IOContextPool&) = delete;
	asio::io_context& GetIO_Context();
	void Stop();

private:
	IOContextPool(std::size_t size = std::thread::hardware_concurrency() );
	std::vector<IOContext> _io_contexts;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOContext;
};

