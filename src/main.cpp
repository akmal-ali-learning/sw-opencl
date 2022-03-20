#include <iostream>
#include <CLManager/CLManager.h>
#include <memory>
#include <assert.h>

const char vector_add_kernel[] = R"cl(
__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    C[i] = A[i] + B[i];
}
)cl";

int 
main(int, char**) 
{   
    auto cl_manager = new CLManager();
    size_t num_elements = 1024;
    cl_int ret;
  
    std::vector<int> A = std::vector<int>(num_elements);
    std::vector<int> B = std::vector<int>(num_elements);
    for(int i = 0 ; i < A.size() ; ++i)
    {
        A[i] = i;
        B[i] = A.size() - i;
    }

    // Create memory buffers on the device for each vector 
    cl_mem a_mem_obj = clCreateBuffer(cl_manager->Context(), CL_MEM_READ_ONLY, 
            num_elements * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(cl_manager->Context(), CL_MEM_READ_ONLY,
            num_elements * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(cl_manager->Context(), CL_MEM_WRITE_ONLY, 
            num_elements * sizeof(int), NULL, &ret);

    ret = clEnqueueWriteBuffer(cl_manager->command_queues.front(), a_mem_obj, CL_TRUE, 0,
            A.size() * sizeof(A[0]), A.data(), 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(cl_manager->command_queues.front(), b_mem_obj, CL_TRUE, 0, 
            B.size() * sizeof(B[0]), B.data(), 0, NULL, NULL);

    size_t source_code_sizes[] = { sizeof(vector_add_kernel)};
    const char * source_codes[] = {vector_add_kernel};
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(
        cl_manager->Context(), 1, 
        source_codes, source_code_sizes,
        &ret
    );
 
    // Build the program
    ret = clBuildProgram(program, 1, &cl_manager->device, NULL, NULL, NULL);
 
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    size_t global_item_size = num_elements; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(cl_manager->command_queues.front(), kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);

    std::vector<int> C = std::vector<int>(num_elements);
    ret = clEnqueueReadBuffer( 
        cl_manager->command_queues.front(), c_mem_obj, 
        CL_TRUE, 0, C.size()*sizeof(C[0]), C.data(), 0, nullptr, nullptr);
    ret = clFlush(cl_manager->command_queues.front());
    ret = clFinish(cl_manager->command_queues.front());

    // Display the result to the screen
    for(int i = 0; i < num_elements; i++)
        assert(A[i] + B[i] == C[i]);

    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    delete cl_manager; cl_manager = nullptr;
}
