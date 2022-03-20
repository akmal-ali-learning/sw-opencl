#include <iostream>
#include <CLManager/CLManager.h>
#include <memory>
#include <assert.h>
#include <chrono>

const char vector_add_kernel[] = R"cl(
__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    C[i] = A[i] + B[i];
}
)cl";

class VectorAddTest
{
    public:
    VectorAddTest(size_t num_elements, CLManager * in_cl_manager)
    {
        cl_manager = in_cl_manager;
        A = std::vector<int>(num_elements);
        B = std::vector<int>(num_elements);
        for(int i = 0 ; i < A.size() ; ++i)
        {
            A[i] = i;
            B[i] = A.size() - i;
        }
        cl_int ret;

        // Create memory buffers on the device for each vector 
        dev_mem_A = clCreateBuffer(cl_manager->Context(), CL_MEM_READ_ONLY, num_elements * sizeof(int), NULL, &ret);
        dev_mem_B = clCreateBuffer(cl_manager->Context(), CL_MEM_READ_ONLY, num_elements * sizeof(int), NULL, &ret);
        dev_mem_C = clCreateBuffer(cl_manager->Context(), CL_MEM_WRITE_ONLY, num_elements * sizeof(int), NULL, &ret);

        {
            size_t source_code_sizes[] = { sizeof(vector_add_kernel)};
            const char * source_codes[] = {vector_add_kernel};
            // Create a program from the kernel source
            program = clCreateProgramWithSource(
                cl_manager->Context(), 1, 
                source_codes, source_code_sizes,
                &ret
            );
            // Build the program
            ret = clBuildProgram(program, 1, &cl_manager->device, NULL, NULL, NULL);
            // Create the OpenCL kernel
            kernel = clCreateKernel(program, "vector_add", &ret);
        }

    }

    void run()
    {
        size_t global_item_size = A.size(); // Process the entire lists
        size_t local_item_size = 64; // Divide work items into groups of 64
        cl_int ret;
        // Buffer data.
        ret = clEnqueueWriteBuffer(cl_manager->command_queues.front(), dev_mem_A, CL_TRUE, 0,
            A.size() * sizeof(int), A.data(), 0, NULL, NULL);
        ret = clEnqueueWriteBuffer(cl_manager->command_queues.front(), dev_mem_B, CL_TRUE, 0,
             B.size() * sizeof(int), B.data(), 0, NULL, NULL);

        // Set the arguments of the kernel
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&dev_mem_A);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dev_mem_B);
        ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&dev_mem_C);

        ret = clEnqueueNDRangeKernel(cl_manager->command_queues.front(), kernel, 1, NULL, 
                &global_item_size, &local_item_size, 0, NULL, NULL);

        C = std::vector<int>(A.size());
        ret = clEnqueueReadBuffer( cl_manager->command_queues.front(), dev_mem_C, 
            CL_TRUE, 0, C.size()*sizeof(int), C.data(), 0, nullptr, nullptr);
        // Display the result to the screen
        for(int i = 0; i < C.size(); i++)
        {
            if( A[i] + B[i] != C[i] )
            {
                printf("%d + %d = %d\n", A[i], B[i], C[i] );
            }
            assert(A[i] + B[i] == C[i]);
        }
    }

    ~VectorAddTest()
    {
        cl_int ret;
        ret = clFlush(cl_manager->command_queues.front());
        ret = clFinish(cl_manager->command_queues.front());
        ret = clReleaseKernel(kernel);
        ret = clReleaseProgram(program);
        ret = clReleaseMemObject(dev_mem_A);
        ret = clReleaseMemObject(dev_mem_B);
        ret = clReleaseMemObject(dev_mem_C);
    }

    private:
    CLManager * cl_manager;
    std::vector<int> A;
    std::vector<int> B;
    std::vector<int> C;
    cl_mem dev_mem_A;
    cl_mem dev_mem_B;
    cl_mem dev_mem_C;
    cl_program program;
    cl_kernel kernel;
};



int 
main(int, char**) 
{
    auto start = std::chrono::high_resolution_clock::now();
    auto cl_manager = new CLManager();

    size_t num_elements =1024*1024;
    auto test = new VectorAddTest(num_elements, cl_manager);
    for(int i = 0 ; i < 100; ++i)
    {
        test->run();
    }

    delete test;             test = nullptr;
    delete cl_manager; cl_manager = nullptr;
    auto finish = std::chrono::high_resolution_clock::now();

    double time_taken_ms = (finish - start).count()/(1000.0*1000.0);
    printf("Ran succesfully in %f ms\n", time_taken_ms);
}
