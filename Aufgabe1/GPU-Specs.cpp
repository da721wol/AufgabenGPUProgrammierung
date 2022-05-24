//
//  GPU-Specs.cpp
//  GPU-Programmierung
//
//  Created by Matthias Asche on 22.05.22.
//

#include "GPU-Specs.hpp"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

void clPrintDevInfo(cl_device_id device){
    char device_string[1024];
  
    
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_string), &device_string, NULL);
    printf("  CL_DEVICE_NAME:   %s\n", device_string);
   

    
}




void gpuspecs(){
    char cBuffer[1024];
    cl_platform_id clSelectedPlatform = NULL;
    cl_platform_id* clPlatformIDs;
    
    cl_uint num_platforms;
    
    /*
     Auslesen der OpenCL Plarformen
     Funktion gibt error code zurück, Daten stehen in num_platforms
     */
    cl_int clError = clGetPlatformIDs(0, NULL, &num_platforms);
    if(clError != CL_SUCCESS){
        printf("Error %i in clGetPlatformIDs \n", clError);
    } else {
        if (num_platforms == 0){
            printf("No OpenCL Plaform found \n");
        } else {
            //reservieren von speicher
            clPlatformIDs = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
        }
        
        printf("%d OpenCl platforms found \n", num_platforms);
        clError = clGetPlatformIDs(num_platforms, clPlatformIDs, NULL);
        for(cl_uint i = 0; i < num_platforms; ++i) {
            clError = clGetPlatformInfo (clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &cBuffer, NULL);
            if(clError == CL_SUCCESS) {
                clSelectedPlatform = clPlatformIDs[i];
                // Get OpenCL platform name and version
                clError = clGetPlatformInfo(clSelectedPlatform, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
                if (clError == CL_SUCCESS) {
                    printf(" CL_PLATFORM_NAME: \t%s\n", cBuffer);
                } else {
                    printf(" Error %i in clGetPlatformInfo Call !!!\n\n", clError);
                }
            }
        }
        
        cl_uint ciDeviceCount;
        cl_device_id *devices;
        
        /*
         Auslesen der openCl Devices
         */
        clError = clGetDeviceIDs (clSelectedPlatform, CL_DEVICE_TYPE_ALL, 0, NULL, &ciDeviceCount);
        if (ciDeviceCount == 0) {
            printf(" No devices found supporting OpenCL (return code %i)\n\n", clError);
            
        } else if (clError != CL_SUCCESS) {
            printf(" Error %i in clGetDeviceIDs\n", clError);
        } else {
            clError = clGetPlatformInfo (clSelectedPlatform, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
            printf(" %u devices found supporting OpenCL on: %s\n\n", ciDeviceCount, cBuffer);
            
            devices = (cl_device_id*)malloc(sizeof(cl_device_id) * ciDeviceCount);
            clError = clGetDeviceIDs (clSelectedPlatform, CL_DEVICE_TYPE_ALL, ciDeviceCount, devices, &ciDeviceCount);
            
            if (clError == CL_SUCCESS){
                for(unsigned int i = 0; i < ciDeviceCount; ++i){
                    clPrintDevInfo(devices[i]);
                }
            }


        }
    }
}



