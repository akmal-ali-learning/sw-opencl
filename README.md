# sw-opencl
![CMake Build status](https://github.com/akmal-ali/sw-opencl/actions/workflows/cmake.yml/badge.svg)

Personal OpenCL learning repository

## Implemented
* Basic example for setting up OpenCL context and executing vectorAdd kernel.

## Future work
* Explore host+device execution of cl work
* Executing CL kernels on FPGA
* Executing CL kernels on NPU

## Setting up
* Require c++ compiler, cmake and opencl sdk.

### Install AMD OpenCL SDK

* https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases
* Installation should set OPENCL_ROOT environment variable which is used by CMAKE.