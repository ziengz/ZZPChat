#pragma once
#include "const.h"

template <typename T>
class Singleton
{
protected:
	Singleton<T>() = default;
	Singleton& operator=(const Singleton<T>&) = delete;
	Singleton(const Singleton<T>&) = delete;
	static std::shared_ptr<T> _instance;

public:
	static std::shared_ptr<T> GetIntance() {
		static std::once_flag flags;
		std::call_once(flags, [&]() {
			_instance = std::shared_ptr<T>(new T);
			});
		return _instance;
	}
	void PrintAddress()
	{
		std::cout << _instance.get() << std::endl;
	}
	~Singleton() {
		std::cout << "this is singleton destruct" << std::endl;
	}
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;