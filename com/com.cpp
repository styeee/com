#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>

 template<typename type=int>
class stack
{
    struct node
    {
        type value;
        node*prev;
    };
protected:
    node*self;
    size_t count=0;
public:
    stack(const stack&s):self(s.self),count(s.count){}
    stack(){}

    ~stack()
    {while(self)pop();}

    stack&push(const type i)
    {
        self=new node{i,self};
        count++;
        return*this;
    }

    const type pop()
    {
        if(!count)return 0;
        node*p=self->prev;
        const type v=self->value;
        delete self;
        self=p;
        count--;
        return v;
    }

    inline stack&operator+(const type i)
    {return push(i);}

    inline const type operator+()
    {return pop();}

    operator type*()
    {
        if(!count)return 0;
        type*p=new type[count];
        while(count)p[count]=pop();
        return p;
    }

    inline const size_t size()const
    {return count;}

    inline operator size_t()const
    {return count;}
};

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

namespace com
{

const char*find()
{
#ifdef WIN32
    stack<char>all;
    char path[7]={0};//how many bytes need for 'COM255'?
    void*com=(void*)0xffffffff;//0xffffffff it is nullptr?

    for(unsigned char i=1;i++;)//1-255
    {
        sprintf(path,"COM%d",i);
        com=CreateFileA(path,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);//A for using 1 byte char
        if(size_t(com)!=0xffffffff)all.push(i);//add it cuz it is available
        CloseHandle(com);
    }

    all.push(0);//push 0 at end cuz it will turn in cstr
    return all;//aware for memory leak! (keep in mind that need for remove in future)
#else
    //for linux
#endif
}

void*init(const char id,const config t=SERIAL_8N1)//default like arduino
{
#ifdef WIN32
    char path[7]={0};//how many bytes need for 'COM255'?
    sprintf(path,"COM%d",id);
    void*com=CreateFileA(path,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

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
    //for linux
#endif
}

 inline
const size_t write(void*com,const char*message,size_t count=0)
{
#ifdef WIN32
     if(!count){for(const char*i=message;*i++;count++);}//if count=0 then send before '\0' was reached
     unsigned long how=0;
     WriteFile(com,message,count,&how,0);
     return how;
#else
     //for linux
#endif
}

 inline
bool finish(void*com)
{
#ifdef WIN32
     return CloseHandle(com);
#else
     //for linux
#endif
}

};

int main()
{
    const char*all=com::find();
    if(!all)return 2;
    printf("connected on: COM%d\n",*all);
    void*com=com::init(*all);
    printf("%d bytes was sended\n",com::write(com,"test"));

    return!com::finish(com);
}
