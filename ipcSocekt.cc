#include <string>

#include "ipcCore.h"

int IPC_Init(std::string processName, MsgCallback callback)
{
	int ret;
	ret = CoreIPC::instance()->startCore(processName, callback);
	return ret;
}

int IPC_SendMsgToProcess(std::string remoteProcessName, void *data, int size)
{
	int ret;
	ret = CoreIPC::instance()->sendIPCMsg(remoteProcessName, data, size);
	return ret;
}