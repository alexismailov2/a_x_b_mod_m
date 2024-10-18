#include <oismailov/a_x_b_mod_m.h>

#include <stdio.h>
#include <malloc.h>

void print(void* val) {
  uint8_t const* ptr = (uint8_t const*)val;
  printf("0x");
  for (int j = 256/8-1; j >=0 ; --j) {
    printf("%02X", ptr[j]);
  }
}

#define INSTANCES 10000000

int main(int argc, char* argv[]) {
#ifdef _cplusplus
  auto results = std::vector<cgbn_mem_t<BITS>>(INSTANCES);
  auto instances = std::vector<instance_t>(INSTANCES);
  for(auto& instance : instances) {
    instance.a._limbs[0] = 0xA8212CC1;
    instance.a._limbs[1] = 0x866C79A3;
    instance.a._limbs[2] = 0xC21C7960;
    instance.a._limbs[3] = 0xF7A7CDF2;
    instance.a._limbs[4] = 0xD40C4699;
    instance.a._limbs[5] = 0xD419E99E;
    instance.a._limbs[6] = 0x1A8CA025;
    instance.a._limbs[7] = 0x16800631;
    //
    instance.b._limbs[0] = 0xEFFFFFFE;
    instance.b._limbs[1] = 0x43E1F593 ;
    instance.b._limbs[2] = 0x79B97091;
    instance.b._limbs[3] = 0x2833E848;
    instance.b._limbs[4] = 0x8181585D;
    instance.b._limbs[5] = 0xB85045B6;
    instance.b._limbs[6] = 0xE131A029;
    instance.b._limbs[7] = 0x30644E72;
    //
    instance.m._limbs[0] = 0xF0000001;
    instance.m._limbs[1] = 0x43E1F593;
    instance.m._limbs[2] = 0x79B97091;
    instance.m._limbs[3] = 0x2833E848;
    instance.m._limbs[4] = 0x8181585D;
    instance.m._limbs[5] = 0xB85045B6;
    instance.m._limbs[6] = 0xE131A029;
    instance.m._limbs[7] = 0x30644E72;
  }
  printf("A*B mod N:\n");
  print(instances[0].a._limbs);
  printf("\n x \n");
  print(instances[0].b._limbs);
  printf("\n mod \n");
  print(instances[0].m._limbs);
  printf("\n");

  a_x_b_mod_m((uint8_t*)instances.data(), (uint8_t*)results.data(), instances.size());

  printf("\n = \n");
  print(results[0]._limbs);
#else
  uint32_t *results = (uint32_t *)malloc((256/8)*INSTANCES);
  uint32_t *instances = (uint32_t *)malloc((256/8*3)*INSTANCES);
  for(int ii = 0; ii < INSTANCES; ++ii) {
    uint32_t i = ii*24;
    instances[i] = 0xA8212CC1;
    instances[i+1] = 0x866C79A3;
    instances[i+2] = 0xC21C7960;
    instances[i+3] = 0xF7A7CDF2;
    instances[i+4] = 0xD40C4699;
    instances[i+5] = 0xD419E99E;
    instances[i+6] = 0x1A8CA025;
    instances[i+7] = 0x16800631;
    //
    instances[i+8] = 0xEFFFFFFE;
    instances[i+9] = 0x43E1F593;
    instances[i+10] = 0x79B97091;
    instances[i+11] = 0x2833E848;
    instances[i+12] = 0x8181585D;
    instances[i+13] = 0xB85045B6;
    instances[i+14] = 0xE131A029;
    instances[i+15] = 0x30644E72;
    //
    instances[i+16] = 0xF0000001;
    instances[i+17] = 0x43E1F593;
    instances[i+18] = 0x79B97091;
    instances[i+19] = 0x2833E848;
    instances[i+20] = 0x8181585D;
    instances[i+21] = 0xB85045B6;
    instances[i+22] = 0xE131A029;
    instances[i+23] = 0x30644E72;
  }
  printf("A*B mod N:\n");
  print(&instances[0]);
  printf("\n x \n");
  print(&instances[8]);
  printf("\n mod \n");
  print(&instances[16]);
  printf("\n");

  a_x_b_mod_m((uint8_t const*)instances, (uint8_t*)results, INSTANCES);

  printf("\n = \n");
  print(&results[0]);
  free(results);
  free(instances);
#endif
  return 0;
}
