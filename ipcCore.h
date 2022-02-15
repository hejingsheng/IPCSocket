#ifndef _IPC_CORE_H_
#define _IPC_CORE_H_

#include <sys/un.h> 

#include "singleton.h"
#include "ipcRemote.h"

#include <string>
#include <unordered_map>
#include <thread>
#include <memory> // shared_ptr

class CoreIPC : public core::Singleton<CoreIPC>
{
public:
	CoreIPC();
	~CoreIPC();

public:
	int startCore(std::string localname, MsgCallback callback);
	int sendIPCMsg(std::string remoteProcessName, void *data, int size);

private:
	void IPC_MsgRecvThread();

private:
	int localsocket_;
	MsgCallback callback_;
	std::thread recvThread_;
	std::unordered_map<std::string, std::shared_ptr<IRemoteIPC>> remoteIpcMap_;
};

#endif