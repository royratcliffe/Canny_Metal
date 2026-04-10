/*!
 * \file can_frame.h
 * \brief CAN frame structure definition.
 * \details This header file defines the structure of a CAN frame, which is used
 * in Controller Area Network (CAN) communication. The CAN frame structure
 * includes fields for the CAN identifier, payload length, and the payload data
 * itself.
 */
#ifndef CAN_FRAME_H
#define CAN_FRAME_H 1

#include <stdint.h>

#define CAN_MAX_DLEN    8  /*!< Maximum data length for a CAN frame */
#define CAN_MAX_RAW_DLC 15 /*!< Maximum raw DLC value */

/*!
 * \brief CAN frame structure.
 * \details This structure represents a CAN frame, which consists of a 32-bit
 * CAN identifier plus EFF/RTR/ERR flags, a payload length in bytes, and the
 * payload data itself. The `can_id` field contains the CAN identifier along
 * with flags for extended frame format (EFF), remote transmission request
 * (RTR), and error frame (ERR). The `len` field indicates the length of the
 * payload data, which can be between 0 and CAN_MAX_DLEN bytes. The `len8_dlc`
 * field is an optional data length code that can be used when the payload
 * length exceeds CAN_MAX_DLEN, allowing for lengths up to CAN_MAX_RAW_DLC. The
 * `data` array holds the actual payload data, which is aligned to 8 bytes for
 * efficient access. The structure is designed to be used in CAN communication,
 * where frames are transmitted and received over a CAN bus. The `can_frame`
 * structure provides a standardised way to represent the data being sent and
 * received, making it easier to work with CAN messages in software
 * applications.
 */
struct can_frame {
  uint32_t can_id;                                        /*!< 32 bit CAN identifier plus EFF/RTR/ERR flags */
  uint8_t len;                                            /*!< Frame payload length in bytes (0 .. CAN_MAX_DLEN) */
  uint8_t __pad;                                          /*!< Padding, reserved for future use */
  uint8_t __res0;                                         /*!< Reserved */
  uint8_t len8_dlc;                                       /*!< Optional DLC (9..15) */
  uint8_t data[CAN_MAX_DLEN] __attribute__((aligned(8))); /*!< Payload data, up to CAN_MAX_DLEN bytes */
};

/*!
 * \brief Check if the len8_dlc field of a CAN frame is valid.
 * \param frame Pointer to the CAN frame.
 * \return true if the len8_dlc field is valid (greater than CAN_MAX_DLEN and
 * less than or equal to CAN_MAX_RAW_DLC), false otherwise.
 */
static inline bool can_frame_len8_dlc_valid(const struct can_frame *frame) { return frame->len8_dlc > CAN_MAX_DLEN && frame->len8_dlc <= CAN_MAX_RAW_DLC; }

/*!
 * \brief Get the data length code (DLC) of a CAN frame.
 * \param frame Pointer to the CAN frame.
 * \return The DLC of the CAN frame, which is either the length of the data (if
 * it is less than or equal to CAN_MAX_DLEN) or the len8_dlc value (if the
 * length is greater than CAN_MAX_DLEN and len8_dlc is valid).
 */
static inline uint8_t can_frame_dlc(const struct can_frame *frame) {
  return frame->len == CAN_MAX_DLEN && can_frame_len8_dlc_valid(frame) ? frame->len8_dlc : frame->len;
}

#endif /* CAN_FRAME_H */
