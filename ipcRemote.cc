#include "ipcRemote.h"

RemoteIPCData::RemoteIPCData(std::string remoteName, MsgCallback cb) : name_(remoteName), msgCb_(cb)
{

}

RemoteIPCData::~RemoteIPCData()
{
	msgCb_ = nullptr;
}

void RemoteIPCData::recvRemoteIPCMsg(void *data, int size)
{
	if (msgCb_)
	{
		msgCb_(name_, data, size);
	}
}

std::shared_ptr<IRemoteIPC> RemoteIPCDataFactory::CreateRemoteIPCData(std::string remoteName, MsgCallback cb)
{
	return std::make_shared<RemoteIPCData>(remoteName, cb);
}