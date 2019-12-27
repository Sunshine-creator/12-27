#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <winsock2.h>
#include<windows.h>
#pragma warning(disable : 4996)
/*添加库的方法：工程->设置->连接->对象/库模块 中加入ws2_32.lib*/
#pragma comment(lib,"WS2_32.lib")
#define PORT 8888
#define ADDR "127.0.0.1"

DWORD WINAPI ThreadProc(LPVOID lpParam);
int main(int argc, char* argv[]) 
{
    WSADATA wsock;
    SOCKET listensocket, newconnection;
    SOCKADDR_IN serAddr, cliAddr;
    int     cliAddrLen = sizeof(cliAddr);
    int     nRet = 0;
    char     buffer[1024];
    //第一步，很重要。做环境初始化
    if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
    {
        return 0;
    }
    //第二步，获取套接字
    if ((listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("error:%d", WSAGetLastError());
        WSACleanup();//关闭已经初始化的环境
        return 0;
    }
    printf("设置监听套接字\n");
    //设置SOCKADDR_IN地址结构
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(ADDR);
    //serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    printf("绑定\n");
    //绑定套接字到相应的端口
    if (bind(listensocket, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
        printf("error:%d", WSAGetLastError());
        goto ERR;
    }
    printf("进入监听……\n");
        //监听等待，无连接时处于阻塞状态
        if (listen(listensocket, 5); ());
        goto ERR;
    }
    //错误处理
    goto NEXT;
ERR:
    closesocket(listensocket);
    WSACleanup();
    return 0;
NEXT:
    printf("设置接收连接套接字\n");
    //引用新的套接字与客户端进行通信
    if ((newconnection = accept(listensocket, (SOCKADDR*)&cliAddr,
        &cliAddrLen)) == INVALID_SOCKET) {
        printf("error:%d", WSAGetLastError());
        goto ERR;
    }
    //关闭监听套接字。也可以循环监听，进行多并发处理。不关闭
    closesocket(listensocket);
    printf("收发数据……");
    //创建一个线程，用来接收数据。
    //windows里面调用此函数，Linux不一定
    CreateThread(NULL, NULL, ThreadProc, &newconnection, 0, NULL);

    //循环发送数据
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//清零
        gets(buffer);
        if (strcmp(buffer, "exit") == 0) {
            goto EXIT;
        }
        if ((nRet = send(newconnection, buffer, strlen(buffer), 0)) == SOCKET_ERROR) {
            printf("error:%d", WSAGetLastError());
            goto EXIT;
        }
    }

EXIT:
    closesocket(newconnection);
    WSACleanup();
    return 0;

}
//线程函数，注意返回值和参数类型是固定的
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    SOCKET* sk = (SOCKET*)lpParam;
    char buffer[1024];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//清零
        if (recv(*sk, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        {
            printf("error:%d", WSAGetLastError());
            closesocket(*sk);
            WSACleanup();
            return 0;
        }
        if (strcmp(buffer, "exit") == 0) {
            return 0;
        }
        puts(buffer);
    }
}


















