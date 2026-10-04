#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include "Singleton.h"
#include <boost/asio.hpp>

class AsioIOServicePool: public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	using WorkPtr = std::unique_ptr<Work>;

	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	~AsioIOServicePool();

	boost::asio::io_context& getIOService();
	void stop();

private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());

	std::vector<IOService> _io_contexts;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
};

