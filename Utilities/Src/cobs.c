// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "cobs.h"

#include <stdint.h>
#include <memory.h>

size_t xCOBSStuff(const void *pvData, size_t xDataLengthBytes, void *pvCOBS) {
  size_t xCOBSLengthBytes = 0UL;
  while (xDataLengthBytes) {
    uint8_t uc = xDataLengthBytes < 0xffU ? xDataLengthBytes : 0xfeU;
    void *pv = memchr(pvData, 0x00U, uc);
    if (pv) uc = pv - pvData;
    *(uint8_t *)pvCOBS++ = uc + 0x01U;
    xCOBSLengthBytes++;
    if (uc) {
      if (pvCOBS) {
        (void)memcpy(pvCOBS, pvData, uc);
        pvCOBS += uc;
      }
      pvData += uc;
      xCOBSLengthBytes += uc;
      xDataLengthBytes -= uc;
    } else {
      pvData++;
      xDataLengthBytes--;
    }
  }
  if (pvCOBS) *(uint8_t *)pvCOBS = 0x00U;
  return xCOBSLengthBytes + 1UL;
}

size_t xCOBSUnStuff(const void *pvCOBS, size_t xCOBSLengthBytes, void *pvData) {
  size_t xDataLengthBytes = 0UL;
  while (xCOBSLengthBytes > 1UL) {
    uint8_t uc = *(const uint8_t *)pvCOBS++;
    if (uc == 0x00U || --uc >= xCOBSLengthBytes--) return 0UL;
    if (memchr(pvCOBS, 0x00U, uc)) return 0UL;
    if (uc) {
      if (pvData) {
        (void)memcpy(pvData, pvCOBS, uc);
        pvData += uc;
      }
      pvCOBS += uc;
      xCOBSLengthBytes -= uc;
      xDataLengthBytes += uc;
    }
    if (xCOBSLengthBytes == 0UL) return 0UL;
  }
  return *(const uint8_t *)pvCOBS == 0x00U ? xDataLengthBytes : 0UL;
}
