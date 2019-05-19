#pragma once 

#include "FServer.h"

class FHServer 
{
public:
    FHServer()
    {}
    ~FHServer();

private:
    std::unique_ptr<FServer> server_;
};