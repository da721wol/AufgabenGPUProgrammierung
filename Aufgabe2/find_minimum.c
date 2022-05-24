#ifdef __APPLE__
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#include <stdio.h>

#define NUM_VALUES 4096
#define WORKGROUP_SIZE 256
#define NUM_WORKGROUPS (NUM_VALUES / WORKGROUP_SIZE)

cl_float* useMultipleWorkGroups();
cl_float useSingleWorkGroup(cl_float* values);

char* read_source(const char* filename) {
  FILE* h = fopen(filename, "r");
  if (!h) {
    fprintf(stderr, "Unable to open file %s", filename);
    exit(1);
  }
  fseek(h, 0, SEEK_END);
  size_t s = ftell(h);
  rewind(h);
  char* program = (char*)malloc(s + 1);
  fread(program, sizeof(char), s, h);
  program[s] = '\0';
  fclose(h);
  return program;
}

void fill(cl_float array[], size_t size) {
  int z = 5;
  for (int i = 0; i < size; ++i) {
    array[i] = (cl_float)z;
    z = z +1;
  }
}

int main() {

  cl_float* results = useMultipleWorkGroups();

  cl_float result;

  // TODO

  printf("Result: %f\n", result);
  return 0;
}

cl_float* useMultipleWorkGroups() {

  cl_platform_id platform;
  clGetPlatformIDs(1, &platform, NULL);

  cl_device_id device;
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

  cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

  cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

  char* source = read_source("find_minimum.cl");
  cl_program program = clCreateProgramWithSource(context, 1,
    (const char**)&source, NULL, NULL);
  free(source);

  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

  cl_kernel kernel = clCreateKernel(program, "find_minimum", NULL);

  cl_float values[NUM_VALUES];
  fill(values, NUM_VALUES);

  cl_mem valuesBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(cl_float) * NUM_VALUES, values, NULL);
  cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
    sizeof(cl_float) * NUM_WORKGROUPS, NULL, NULL);

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &valuesBuffer);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &resultBuffer);
  clSetKernelArg(kernel, 2, sizeof(cl_float) * WORKGROUP_SIZE, NULL);

  size_t work_units[] = {NUM_VALUES};
  size_t workgroup_size[] = {WORKGROUP_SIZE};
  clEnqueueNDRangeKernel(queue, kernel, 1, NULL, work_units, workgroup_size, 0, NULL, NULL);

  cl_float* results = malloc (sizeof (cl_float) * NUM_WORKGROUPS);
  clEnqueueReadBuffer(queue, resultBuffer, CL_TRUE, 0, sizeof(cl_float) * NUM_WORKGROUPS, results, 0, NULL, NULL);

  clReleaseMemObject(resultBuffer);
  clReleaseMemObject(valuesBuffer);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  return results;
}

cl_float useSingleWorkGroup(cl_float* values) {

  // TODO
}
