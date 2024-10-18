#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <cuda.h>
#include <gmp.h>
#include <cgbn/cgbn.h>
#include <utility/cpu_support.h>
#include <utility/cpu_simple_bn_math.h>
#include <utility/gpu_support.h>

#include "../TimeMeasuring.hpp"

// IMPORTANT:  DO NOT DEFINE TPI OR BITS BEFORE INCLUDING CGBN
#define TPI 32
#define BITS 256
#define INSTANCES 10000000

struct instance_t {
  cgbn_mem_t<BITS> a;
  cgbn_mem_t<BITS> b;
  cgbn_mem_t<BITS> m;
};

// helpful typedefs for the kernel
typedef cgbn_context_t<TPI> context_t;
typedef cgbn_env_t<context_t, BITS> env_t;

// the actual kernel
__global__ void kernel_add(cgbn_error_report_t *report, instance_t* instances, cgbn_mem_t<BITS>* results, uint32_t count) {
  // decode an instance number from the blockIdx and threadIdx
  int32_t instance=(blockIdx.x*blockDim.x + threadIdx.x)/TPI;
  if(instance>=count)
    return;

  context_t      bn_context(cgbn_report_monitor, report, instance);   // construct a context
  env_t          bn_env(bn_context.env<env_t>());                     // construct an environment for 1024-bit math
  env_t::cgbn_t  a, b, r, m;                                          // define a, b, r as 1024-bit bignums

  cgbn_load(bn_env, a, &(instances[instance].a));      // load my instance's a value
  cgbn_load(bn_env, b, &(instances[instance].b));      // load my instance's b value
  cgbn_load(bn_env, m, &(instances[instance].m));      // load my instance's b value

  // convert a and b to Montgomery space
  uint32_t np0 = cgbn_bn2mont(bn_env, a, a, m);
  cgbn_bn2mont(bn_env, b, b, m);

  cgbn_mont_mul(bn_env, r, a, b, m, np0); // a * b mod n

  // convert r back to normal space
  cgbn_mont2bn(bn_env, r, r, m, np0);

  cgbn_store(bn_env, &(results[instance]), r); // store r into mul
}

extern "C" void a_x_b_mod_m(uint8_t const* data, uint8_t* result, uint32_t count) {
  TAKEN_TIME();
  CUDA_CHECK(cudaSetDevice(0));

  cgbn_error_report_t *report;
  CUDA_CHECK(cgbn_error_report_alloc(&report));

  instance_t *gpuInstances;
  cgbn_mem_t<BITS> *gpuResults;
  CUDA_CHECK(cudaMalloc((void **)&gpuInstances, sizeof(instance_t)*count));
  CUDA_CHECK(cudaMalloc((void **)&gpuResults, sizeof(cgbn_mem_t<BITS>)*count));
  CUDA_CHECK(cudaMemcpy(gpuInstances, data, sizeof(instance_t)*count, cudaMemcpyHostToDevice));

  // launch with 32 threads per instance, 128 threads (4 instances) per block
  kernel_add<<<(INSTANCES + 3) / 4, 128>>>(report, gpuInstances, gpuResults, count);

  // error report uses managed memory, so we sync the device (or stream) and check for cgbn errors
  CUDA_CHECK(cudaDeviceSynchronize());
  CGBN_CHECK(report);

  // copy the instances back from gpuMemory
  CUDA_CHECK(cudaMemcpy(result, gpuResults, sizeof(cgbn_mem_t<BITS>) * count, cudaMemcpyDeviceToHost));

  CUDA_CHECK(cudaFree(gpuInstances));
  CUDA_CHECK(cudaFree(gpuResults));
  CUDA_CHECK(cgbn_error_report_free(report));
}

