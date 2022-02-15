#ifndef _IPC_SOCKET_H_
#define _IPC_SOCKET_H_

#include <string>
#include <functional>

using MsgCallback = std::function<void(std::string &, void*, int)>;

int IPC_Init(std::string processName, MsgCallback callback);

int IPC_SendMsgToProcess(std::string remoteProcessName, void *data, int size);

#endif // !_IPC_SOCKET_H_
