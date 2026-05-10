#include <cz.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_lszb32(void) {
  assert(0x00000000U == lszb32(0xffffffffU));
  assert(0x00000001U == lszb32(0x00000000U));
  assert(0x00000002U == lszb32(0x00000001U));
  assert(0x00000004U == lszb32(0x00000003U));
  assert(0x00000008U == lszb32(0x00000007U));
}

int main(void) {
  (void)printf("Hello, World from %s!!!\n", "cz_test");

  test_lszb32();

  _exit(0);
  return 0;
}
