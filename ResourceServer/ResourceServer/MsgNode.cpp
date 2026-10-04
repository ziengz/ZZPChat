#include "MsgNode.h"

recvNode::recvNode(int max_len, int msg_id):MsgNode(max_len),_msg_id(msg_id) {
	
}

sendNode::sendNode(const char* data, int msg_len, int msg_id) :MsgNode(HEAD_TOTAL_LEN + msg_len)
, _msg_id(msg_id)
{
	short msg_id_host = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	memcpy(_data, &msg_id_host, HEAD_ID_LEN);
	long msg_len_host = boost::asio::detail::socket_ops::network_to_host_long(msg_len);
	memcpy(_data + HEAD_ID_LEN, &msg_len_host, HEAD_DATA_LEN);
	memcpy(_data + HEAD_TOTAL_LEN, data, msg_len);
}