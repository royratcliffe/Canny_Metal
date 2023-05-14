/*
 * stm32xx_can_unifier.c
 */

#include "stm32xx_can_unifier.h"

#include <memory.h>

BaseType_t xMsgUnifyCANRx(MsgUnifierHandle_t xMsgUnifier, struct CANRx *pxCANRx) {
  const void *pvBin;
  size_t xBinLengthBytes;
  BaseType_t xPass =
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->ulRxFifo) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.StdId) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.ExtId) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.IDE) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.RTR) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.DLC) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.Timestamp) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess &&
      xMsgUnifyUInt32(xMsgUnifier, &pxCANRx->xRxHeader.FilterMatchIndex) &&
      xMsgUnify(xMsgUnifier) == eMsgUnifySuccess && xMsgUnifyBin(xMsgUnifier, &pvBin, &xBinLengthBytes) &&
      xBinLengthBytes == sizeof(pxCANRx->ucRxData);
  if (xPass) (void)memcpy(pxCANRx->ucRxData, pvBin, sizeof(pxCANRx->ucRxData));
  return xPass;
}
