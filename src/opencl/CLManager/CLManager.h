#pragma once
#include <CL/cl.h>
#include <vector>
class CLManager
{
    public:
    CLManager();
    ~CLManager();

    cl_context Context() const {return context;};

    void open();
    void close();
    public:
    cl_device_id device;
    cl_context   context;
    std::vector<cl_command_queue> command_queues;
};