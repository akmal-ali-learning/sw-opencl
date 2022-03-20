#include <iostream>
#include <CLManager/CLManager.h>
#include <memory>
int 
main(int, char**) 
{
    auto cl_manager = std::make_shared<CLManager>();
    // Create a command queue
    // cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
}
