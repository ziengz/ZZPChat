#include "MsgNode.h"

RecvNode::RecvNode(short max_len, short msg_id):MsgNode(max_len),msg_id_(msg_id)
{

}

SendNode::SendNode(const char* data, short msg_len, short msg_id)
    : MsgNode(msg_len + HEAD_TOTAL_LEN), msg_id_(msg_id)
{
    short network_msg_id = asio::detail::socket_ops::host_to_network_short(msg_id);
    memcpy(data_, &network_msg_id, HEAD_ID_LEN);
    short network_msg_len = asio::detail::socket_ops::host_to_network_short(msg_len);
    memcpy(data_ + HEAD_ID_LEN, &network_msg_len, HEAD_DATA_LEN);
    memcpy(data_ + HEAD_TOTAL_LEN, data, msg_len);
}
