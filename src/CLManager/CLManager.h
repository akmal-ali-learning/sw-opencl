#pragma once
#include <CL/cl.h>
#include <vector>
class CLManager
{
    public:
    CLManager();
    ~CLManager();

    void open();
    void close();
    private:
    cl_device_id device;
    cl_context   context;
    std::vector<cl_command_queue> command_queues;
};