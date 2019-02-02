#ifndef PACKAGETCP_H_
#define PACKAGETCP_H_

#include <cstring>

struct PackHead 
{
    int type;
    int length;
};

static const int MAXBODY = 256;

struct PackageTCP 
{
    PackageTCP() 
    {
        head.type = -1;
        head.length = -1;
    }
    PackageTCP(int type_, const char* buffer) 
    {
        head.type = type_;
        int length = strlen(buffer) + 1;
        head.length = (length > 256) ? 255 : length;
        strncpy(body, buffer, head.length);
        if (head.length == 255) 
        {
            body[256] = '\0';
            head.length = 256;
        }
    }
    PackHead head;
    char body[MAXBODY];   
};

#endif