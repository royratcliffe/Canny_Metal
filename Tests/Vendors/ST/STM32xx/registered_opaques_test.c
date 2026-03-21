#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <registered_opaques.h>
#include <dimof.h>

int should_register_and_retrieve_opaques(void) {
  void *opaques[1] = {NULL};
  struct RegisteredOpaques registered_opaques = {
      .ppvOpaques = opaques, .xNumberOfOpaques = DIM_OF(opaques), .pxHashOfOpaqueFunction = NULL};
  assert(xRegisteredCardinalOfOpaque(&registered_opaques, (void *)0xdeadbeefUL) == 0);
  assert(xOpaqueIsRegistered(&registered_opaques, (void *)0xdeadbeefUL));
  assert(xRegisteredCardinalOfOpaque(&registered_opaques, (void *)0x12345678UL) == -ENOMEM);
  return 0;
}

int main(void) {
  (void)printf("Hello, World from %s!!!\n", "registered_opaques_test");

  assert(should_register_and_retrieve_opaques() == 0);

  _exit(0);
  return 0;
}
