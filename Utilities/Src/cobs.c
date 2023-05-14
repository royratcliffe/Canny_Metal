// Copyright (c) Roy Ratcliffe, Northumberland, United Kingdom
// SPDX-License-Identifier: MIT

#include "cobs.h"

#include <stdint.h>
#include <memory.h>

ptrdiff_t xCOBSStuff(const void *pvData, size_t xDataLengthBytes, void *pvCOBS) {
  const uint8_t *pucData = pvData, *const pucEndOfData = pucData + xDataLengthBytes;
  uint8_t *pucCOBS = pvCOBS, *const pucStartOfCOBS = pucCOBS;
  uint8_t *pucStuff, ucStuff;
#define START_OF_STUFF()  \
  do {                    \
    pucStuff = pucCOBS++; \
    ucStuff = 0x01U;      \
  } while (0)
#define END_OF_STUFF()   \
  do {                   \
    *pucStuff = ucStuff; \
    START_OF_STUFF();    \
  } while (0)
  START_OF_STUFF();
  while (pucData < pucEndOfData) {
    if (*pucData == 0x00U)
      END_OF_STUFF();
    else {
      *pucCOBS++ = *pucData;
      if (++ucStuff == 0xffU) END_OF_STUFF();
    }
    pucData++;
  }
  END_OF_STUFF();
  return pucCOBS - pucStartOfCOBS;
}

size_t xCOBSMemStuff(const void *pvData, size_t xDataLengthBytes, void *pvCOBS) {
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

ptrdiff_t xCOBSUnStuff(const void *pvCOBS, size_t xCOBSLengthBytes, void *pvData) {
  const uint8_t *pucCOBS = pvCOBS, *const pucEndOfCOBS = pucCOBS + xCOBSLengthBytes;
  uint8_t *pucData = pvData, *const pucStartOfData = pucData;
  while (pucCOBS < pucEndOfCOBS) {
    const uint8_t ucStuff = *pucCOBS++;
    for (uint8_t uc = 0x01U; uc < ucStuff; uc++) {
      const uint8_t ucCOBS = *pucCOBS++;
      if (ucCOBS == 0x00U) return -1;
      *pucData++ = ucCOBS;
    }
    if (ucStuff < 0xffU) *pucData++ = 0x00U;
  }
  return pucData - pucStartOfData;
}

size_t xCOBSMemUnStuff(const void *pvCOBS, size_t xCOBSLengthBytes, void *pvData) {
  size_t xDataLengthBytes = 0UL;
  while (xCOBSLengthBytes > 1UL) {
    uint8_t uc = *(const uint8_t *)pvCOBS++;
    if (uc == 0x00U || --uc >= xCOBSLengthBytes-- || memchr(pvCOBS, 0x00U, uc)) return 0UL;
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
