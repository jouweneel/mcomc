#ifndef _MCOM_TYPES_H_
#define _MCOM_TYPES_H_

#include <stdint.h>

#include "mcom_util.h"

/** 
 * ByteSchema data types
 * 1st half byte = data UNIT length
 * 2nd half byte = 1 bit for array or not, 3 bits for ID
 **/
typedef enum DataType_t {
  DATA_TYPE_VOID = 0x00,

/** 1-byte types */
  DATA_TYPE_U8 = 0x10,
  DATA_TYPE_I8 = 0x11,
  DATA_TYPE_BOOL = 0x12,
  DATA_TYPE_CHAR = 0x13,

/** 1-byte array types */
  DATA_TYPE_U8_A = 0x18,
  DATA_TYPE_I8_A = 0x19,
  DATA_TYPE_BOOL_A = 0x1a,
  DATA_TYPE_STRING = 0x1b,
  DATA_TYPE_JSON = 0x1c,

/** 2-byte types */
  DATA_TYPE_U16 = 0x20,
  DATA_TYPE_I16 = 0x21,

/** 2-byte array types */
  DATA_TYPE_U16_A = 0x28,
  DATA_TYPE_I16_A = 0x29,

/** 3-byte types */
  DATA_TYPE_RGB = 0x30,
  DATA_TYPE_HSV = 0x31,

/** 3-byte array types */
  DATA_TYPE_RGB_A = 0x38,
  DATA_TYPE_HSV_A = 0x39,

/** 4-byte types */
  DATA_TYPE_U32 = 0x40,
  DATA_TYPE_I32 = 0x41,
  DATA_TYPE_FLOAT = 0x42,
  DATA_TYPE_RGBW = 0x43,

/** 4-byte array types */
  DATA_TYPE_U32_A = 0x48,
  DATA_TYPE_I32_A = 0x49,
  DATA_TYPE_FLOAT_A = 0x4a,
  DATA_TYPE_RGBW_A = 0x4b,

/** 8-byte types */
  DATA_TYPE_U64 = 0x80,
  DATA_TYPE_I64 = 0x81,
  DATA_TYPE_TIMESTAMP = 0x82,
  DATA_TYPE_DOUBLE = 0x83,

/** 8-byte array types */
  DATA_TYPE_U64_A = 0x88,
  DATA_TYPE_I64_A = 0x89,
  DATA_TYPE_TIMESTAMP_A = 0x8a,
  DATA_TYPE_DOUBLE_A = 0x8b
} DataType_t;

typedef struct McomMsg_t {
  uint8_t cmd;
  uint8_t type;
  uint16_t length;
  uint8_t *data;
} McomMsg_t;

typedef McomMsg_t *(* DecodeFn_t)(Buf_t *buf);
typedef Buf_t *(* EncodeFn_t)(McomMsg_t *msg);

typedef struct Protocol_t {
  DecodeFn_t decode;
  EncodeFn_t encode;
} Protocol_t;

#endif
