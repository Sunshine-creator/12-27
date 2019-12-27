#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <winsock2.h>
#pragma warning(disable : 4996)
/*��ӿ�ķ���������->����->����->����/��ģ�� �м���ws2_32.lib*/
#pragma comment(lib,"ws2_32.lib")
#define PORT  8888
#define ADDR "127.0.0.1"
                //��������
DWORD WINAPI ThreadProc(LPVOID lpParam);
//������ 
int main() 
{
    SOCKET scoket;
    SOCKADDR_IN serAddr;
    int i = 0;
    char buffer[1024];
    int nRet = 0;
    WSADATA wsock;    //��һ��������Ҫ����������ʼ��
    if (WSAStartup(MAKEWORD(2, 2), &wsock) != 0)
    {
        return 0;
    }
    //�ڶ�������ȡ�׽���
    if ((scoket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        WSACleanup();//��ȡ�׽���ʧ�ܺ���Ҫ�ر��Ѿ���ʼ���Ļ���
        return 0;
    }
    //����SOCKADDR_IN��ַ�ṹ
    serAddr.sin_family = AF_INET;//ipv4Э���
    serAddr.sin_port = htons(PORT);//���ö˿�
    serAddr.sin_addr.s_addr = inet_addr(ADDR); //����IP��ַ

                    //����������������
    if ((connect(scoket, (SOCKADDR*)&serAddr, sizeof(serAddr))) == SOCKET_ERROR)
    {
        printf("error:%d", WSAGetLastError());
        return 0;
    }
    //����һ���̣߳������������ݡ�
    //windows������ô˺�����Linux��һ��
    CreateThread(NULL, NULL, ThreadProc, &scoket, 0, NULL);
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));//��������
        gets(buffer);
        if(strcmp("exit", buffer) == 0)
            goto EXIT;
           
        if ((nRet = send(scoket, buffer, strlen(buffer), 0)) == SOCKET_ERROR)
        {
            printf("error:%d", WSAGetLastError());
            goto EXIT;
        }
    }

    //������
EXIT:
    closesocket(scoket);/*�رղ�ʹ�õ��׽��֣����ļ�����һ��������Ҳ��ϡȱ��Դ��*/
    WSACleanup(); //�ر��Ѿ���ʼ���Ļ���
    return 0;

}
//�̺߳�����ע�ⷵ��ֵ�Ͳ��������ǹ̶���
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



















 