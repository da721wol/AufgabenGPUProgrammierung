__kernel void find_minimum(__global const float* values,
                           __global float* results,
                           __local float* scratch) {

  int i = get_local_id(0);
  int n = get_local_size(0);

  scratch[i] = values[get_global_id(0)];

  barrier(CLK_LOCAL_MEM_FENCE);

  for (int j = n / 2; j > 0; j /= 2) {
    if (i < j)
      scratch[i] = min(scratch[i], scratch[i + j]);
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if (i == 0) {
    results[get_group_id(0)] = scratch[0];
  }
}