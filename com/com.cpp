#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>

enum config:char//from arduino
{
    SERIAL_5N1,
    SERIAL_6N1,
    SERIAL_7N1,
    SERIAL_8N1,
    SERIAL_5N2,
    SERIAL_6N2,
    SERIAL_7N2,
    SERIAL_8N2,
    SERIAL_5E1,
    SERIAL_6E1,
    SERIAL_7E1,
    SERIAL_8E1,
    SERIAL_5E2,
    SERIAL_6E2,
    SERIAL_7E2,
    SERIAL_8E2,
    SERIAL_5O1,
    SERIAL_6O1,
    SERIAL_7O1,
    SERIAL_8O1,
    SERIAL_5O2,
    SERIAL_6O2,
    SERIAL_7O2,
    SERIAL_8O2
};

void*com_init(const config t=SERIAL_8N1)//default like arduino
{
#ifdef WIN32
    char path[7]={0};//how many bytes need for 'COM255'?
    void*com=(void*)0xffffffff;//0xffffffff it is nullptr?

    for(unsigned char i=1; ;i++)//1-255
    {
        sprintf(path,"COM%d",i);
        com=CreateFileA(path,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);//A for using 1 byte char
        if(size_t(com)!=0xffffffff){printf("connected on:%s\n",path);break;}//there is not available port
        CloseHandle(com);
        if(!i){printf("error:no available ports!\n");exit(1);}
    }

    DCB info;
    GetCommState(com,&info);
    switch(t)
    {
        case SERIAL_8N1:
            info.BaudRate=9600;
            info.ByteSize=8;
            info.StopBits=1;
            info.Parity=false;
            SetCommState(com,&info);
        break;
        default:printf("error:not supported this config of com!\n");exit(2);
    }
    return com;
#else

#endif
}

 inline
const size_t com_write(void*com,const char*message,size_t count=0)
{
#ifdef WIN32
     if(!count){for(const char*i=message;*i++;count++);}//if count=0 then send before '\0' was reached
     unsigned long how=0;
     WriteFile(com,message,count,&how,0);
     return how;
#else

#endif
}

 inline
bool com_close(void*com)
{
#ifdef WIN32
     return CloseHandle(com);
#else

#endif
}

int main()
{
    void*com=com_init();
    com_write(com,"test");
    com_close(com);

    return 0;
}
