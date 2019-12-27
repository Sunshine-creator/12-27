#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <winsock2.h>
#include<windows.h>
#pragma warning(disable : 4996)
/*��ӿ�ķ���������->����->����->����/��ģ�� �м���ws2_32.lib*/
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
    //��һ��������Ҫ����������ʼ��
    if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
    {
        return 0;
    }
    //�ڶ�������ȡ�׽���
    if ((listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("error:%d", WSAGetLastError());
        WSACleanup();//�ر��Ѿ���ʼ���Ļ���
        return 0;
    }
    printf("���ü����׽���\n");
    //����SOCKADDR_IN��ַ�ṹ
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(ADDR);
    //serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    printf("��\n");
    //���׽��ֵ���Ӧ�Ķ˿�
    if (bind(listensocket, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
        printf("error:%d", WSAGetLastError());
        goto ERR;
    }
    printf("�����������\n");
        //�����ȴ���������ʱ��������״̬
        if (listen(listensocket, 5); ());
        goto ERR;
    }
    //������
    goto NEXT;
ERR:
    closesocket(listensocket);
    WSACleanup();
    return 0;
NEXT:
    printf("���ý��������׽���\n");
    //�����µ��׽�����ͻ��˽���ͨ��
    if ((newconnection = accept(listensocket, (SOCKADDR*)&cliAddr,
        &cliAddrLen)) == INVALID_SOCKET) {
        printf("error:%d", WSAGetLastError());
        goto ERR;
    }
    //�رռ����׽��֡�Ҳ����ѭ�����������жಢ���������ر�
    closesocket(listensocket);
    printf("�շ����ݡ���");
    //����һ���̣߳������������ݡ�
    //windows������ô˺�����Linux��һ��
    CreateThread(NULL, NULL, ThreadProc, &newconnection, 0, NULL);

    //ѭ����������
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//����
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
//�̺߳�����ע�ⷵ��ֵ�Ͳ��������ǹ̶���
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    SOCKET* sk = (SOCKET*)lpParam;
    char buffer[1024];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//����
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


















