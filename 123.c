#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <winsock2.h>
#pragma warning(disable : 4996)
/*添加库的方法：工程->设置->连接->对象/库模块 中加入ws2_32.lib*/
#pragma comment(lib,"ws2_32.lib")
#define PORT  8888
#define ADDR "127.0.0.1"
                //函数声明
DWORD WINAPI ThreadProc(LPVOID lpParam);
//主函数 
int main() 
{
    SOCKET scoket;
    SOCKADDR_IN serAddr;
    int i = 0;
    char buffer[1024];
    int nRet = 0;
    WSADATA wsock;    //第一步，很重要。做环境初始化
    if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
    {
        return 0;
    }
    //第二步，获取套接字
    if ((scoket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        WSACleanup();//获取套接字失败后，需要关闭已经初始化的环境
        return 0;
    }
    //设置SOCKADDR_IN地址结构
    serAddr.sin_family = AF_INET;//ipv4协议簇
    serAddr.sin_port = htons(PORT);//设置端口
    serAddr.sin_addr.s_addr = inet_addr(ADDR); //设置IP地址

                    //第三步，进行连接
    if ((connect(scoket, (SOCKADDR*)&serAddr, sizeof(serAddr))) == SOCKET_ERROR)
    {
        printf("error:%d", WSAGetLastError());
        return 0;
    }
    //创建一个线程，用来接收数据。
    //windows里面调用此函数，Linux不一定
    CreateThread(NULL, NULL, ThreadProc, &scoket, 0, NULL);
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//缓存清零
        gets(buffer);
        if(strcmp("exit", buffer) == 0)
            goto EXIT;
           
        if ((nRet = send(scoket, buffer, strlen(buffer), 0)) == SOCKET_ERROR)
        {
            printf("error:%d", WSAGetLastError());
            goto EXIT;
        }
    }

    //错误处理
EXIT:
    closesocket(scoket);/*关闭不使用的套接字，跟文件操作一样，网络也是稀缺资源。*/
    WSACleanup(); //关闭已经初始化的环境
    return 0;

}
//线程函数，注意返回值和参数类型是固定的
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    SOCKET* sk = (SOCKET*)lpParam;
    char buffer[1024];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        if (recv(*sk, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        {
            printf("error:%d", WSAGetLastError());
            closesocket(*sk);
            WSACleanup();
            return 0;
        }
        if (strcmp(buffer, "exit") == 0) 
        {
            return 0;
        }
        puts(buffer);
    }
}



















 