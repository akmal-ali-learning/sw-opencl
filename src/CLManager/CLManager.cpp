#include "CLManager.h"
#include <iostream>
#include <assert.h>

CLManager::CLManager()
{
    open();
}
CLManager::~CLManager()
{
    close();
}


//! @brief Get OpenCL Platform ID.
cl_platform_id 
get_platform_id()
{
    cl_platform_id platform_id = nullptr;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    return platform_id;
}

//! @brief Get OpenCL Device ID.
cl_device_id 
get_device()
{
    cl_uint         ret_num_devices;
    cl_device_id    device_id;
    cl_int ret = clGetDeviceIDs( get_platform_id() , CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    return device_id;
}

cl_context
get_cl_context(const cl_device_id& device)
{
    // Get platform and device information
    cl_int ret;
    cl_context context = clCreateContext( NULL, 1, &device, NULL, NULL, &ret);
    assert(context);
    return context;
}


void 
CLManager::open()
{
    command_queues = {};
    device  = get_device();
    context = get_cl_context(device);
    cl_int ret;
    cl_command_queue command_queue = clCreateCommandQueue(context, device, 0, &ret);
    command_queues.push_back(command_queue);
}


void 
CLManager::close()
{
    for(auto& cmd_queue : command_queues)
    {
        clReleaseCommandQueue(cmd_queue);
    }
    cl_int ret = clReleaseContext(context);
    context = nullptr;
}