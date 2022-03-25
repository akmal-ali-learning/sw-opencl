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
std::vector<cl_platform_id> 
get_platforms()
{
    cl_platform_id platform_id = nullptr;
    cl_uint num_platforms = 0;
    clGetPlatformIDs(0, nullptr, &num_platforms);
    std::vector<cl_platform_id> platforms(num_platforms,nullptr);
    cl_int ret = clGetPlatformIDs(platforms.size(), platforms.data(), &num_platforms);
    printf("%d platforms found\n", num_platforms);
    return platforms;
}

//! @brief Get OpenCL Device ID.
cl_device_id 
get_device()
{
    cl_uint         num_devices;
    cl_platform_id  platform = get_platforms().front();
    cl_int ret = clGetDeviceIDs( platform, CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &num_devices);
    std::vector<cl_device_id> devices(num_devices);
    ret = clGetDeviceIDs( platform, CL_DEVICE_TYPE_DEFAULT, devices.size(), devices.data(), &num_devices);
    printf("%d device found\n", num_devices);
    cl_device_id device = devices.front();
    return device;
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