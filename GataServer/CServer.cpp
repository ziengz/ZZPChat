#include "CServer.h"


CServer::CServer(asio::io_context& ioc, unsigned short port):_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{
	
}

void CServer::Start()
{
	auto self = shared_from_this();
	auto& ioc = IOContextPool::GetIntance()->GetIO_Context();
	std::shared_ptr<HttpConnection>new_con = std::make_shared<HttpConnection>(ioc);
	_acceptor.async_accept(new_con->GetSocket(), [self,new_con](beast::error_code ec) {
		try {
			//出错则放弃这个链接
			if (ec) {
				self->Start();
				return;
			}
			new_con->Start();
			self->Start();
		}
		catch (std::exception& ec) {
			std::cout << "exception is " << ec.what() << std::endl;
			self->Start();
		}
		});
}
