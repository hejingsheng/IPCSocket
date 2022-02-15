#ifndef _IPC_REMOTE_H_
#define _IPC_REMOTE_H_

#include <string>
#include <memory>
#include <functional>

using MsgCallback = std::function<void(std::string &, void*, int)>;

class IRemoteIPC
{
public:
	IRemoteIPC() = default;
	virtual void recvRemoteIPCMsg(void *data, int size) = 0;
	virtual ~IRemoteIPC() = default;
};

class RemoteIPCData : public IRemoteIPC 
{
public:
	RemoteIPCData(std::string remoteName, MsgCallback cb);
	virtual ~RemoteIPCData();

public:
	virtual void recvRemoteIPCMsg(void *data, int size);

private:
	std::string name_;
	MsgCallback msgCb_;
};

class RemoteIPCDataFactory
{
public:
	static std::shared_ptr<IRemoteIPC> CreateRemoteIPCData(std::string remoteName, MsgCallback cb);

private:
	~RemoteIPCDataFactory() = default;
};

#endif // !_IPC_REMOTE_H_
