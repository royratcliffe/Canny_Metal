/*
 * stm32xx_can_msgpack.c
 */

#include "stm32xx_can_msgpack.h"

BaseType_t xCANRxPack(const struct CANRx *pxCANRx, msgpack_packer *pxPacker) {
  return msgpack_pack_uint32(pxPacker, pxCANRx->ulRxFifo) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.StdId) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.ExtId) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.IDE) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.RTR) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.DLC) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.Timestamp) == 0 &&
         msgpack_pack_uint32(pxPacker, pxCANRx->xRxHeader.FilterMatchIndex) == 0 &&
         msgpack_pack_bin_with_body(pxPacker, pxCANRx->ucRxData, sizeof(pxCANRx->ucRxData)) == 0;
}
