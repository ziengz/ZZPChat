
#ifndef SINGLETON_H
#define SINGLETON_H
#include <global.h>

//单例类
template <typename T>
class singleton
{
protected:
    singleton() = default;       //禁止外部构造
    singleton(const singleton<T>&) = delete; //禁止外部拷贝构造
    singleton<T>& operator=(const singleton<T>&) = delete; //禁止外部赋值
    static std::shared_ptr<T> _instance;        //singleton在析构该成员时，share_ptr会找到T中的析构函数，所以T的析构函数需要设置为共有
public:
    //获取单例实例
    static std::shared_ptr<T> getInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag,[&](){
           _instance = std::shared_ptr<T>(new T);  //因为这里会调用T的构造函数，但是T的构造函数只能是私有的，
                                                  //所以T需要被设置为友元friend
        });
        return _instance;
    }
    void paintAddress(){
        std::cout<<_instance.get()<<std::endl;
    }
    ~singleton()
    {
        std::cout<<"the singleton is destruct"<<std::endl;
    }

};

template <typename T>
std::shared_ptr<T>singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
