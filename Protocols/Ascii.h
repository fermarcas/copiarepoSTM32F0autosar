/**
 ******************************************************************************
 * @file    Ascii.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    19.01.2015
 * @brief   Library for ASCII operations.
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef ASCII_H
#define ASCII_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/
/**
 * Defines the length type.
 */
typedef uint8_t Ascii_LengthType;
/**
 * Defines the value type.
 */
typedef char Ascii_ValueType;

/**
 * Defines a fifo buffer.
 */
typedef struct
{
  /* A pointer to the buffer. */
  Ascii_ValueType* entries;
  /* The current read index in the buffer. */
  Ascii_LengthType rdIdx;
  /* The current write index in the buffer. */
  Ascii_LengthType wrIdx;
  /* The current space of the buffer. */
  Ascii_LengthType space;
  /* The size of the buffer. */
  const Ascii_LengthType size;
} Ascii_QueueType;

/**
 * ASCII operation results.
 */
typedef enum
{
  Ascii_OperationResultIncomplete, //!< Ascii_OperationResultIncomplete
  Ascii_OperationResultSucceeded,  //!< Ascii_OperationResultSucceeded
  Ascii_OperationResultFailed      //!< Ascii_OperationResultFailed
} Ascii_OperationResult;

/**
 * ASCII Number Base
 */
typedef enum
{
  Ascii_NumberBaseBin = 2u, //!< Ascii_NumberBaseBin
  Ascii_NumberBaseDec = 10u, //!< Ascii_NumberBaseDec
  Ascii_NumberBaseHex = 16u //!< Ascii_NumberBaseHex
} Ascii_NumberBase;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/**
 * Defines the maximum length.
 */
#define ASCII_INFINITE_LENGTH                                 ((Ascii_LengthType)-1)

/**
 * Checks if the value is a digit.
 * @param x Character to check.
 * @return TRUE if it is a digit; otherwise FALSE.
 */
#define Ascii_IsDigit(x)                                      \
    (((x) >= '0') && ((x) <= '9'))

/**
 * Converts the given value to an digit.
 * @param x Value to convert.
 * @return Converted value.
 */
#define Ascii_ToDigit(x)                                      \
    (Ascii_ValueType)((x) - '0')

/**
 * Converts the given value to an ASCII digit.
 * @param x Value to convert.
 * @return Converted value.
 */
#define Ascii_FromDigit(x)                                    \
  (Ascii_ValueType)(((x)<10)? ((x) + '0') : ((x) + 'A' - 10))

/**
 * Checks if the value is a negative sign.
 * @param x Character to check.
 * @return TRUE if it is a negative sign; otherwise FALSE.
 */
#define Ascii_IsNegativeSign(x)                               \
	(Ascii_ValueType)((x) == '-')

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Reads an 8-bit integer from a queue until the delimiter is reached or the expected length.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param value The destination value.
 * @param delimiter The delimiter.
 * @param expectedLen The expected length.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_ReadInt8(const Ascii_QueueType* queue,
                                     Ascii_LengthType* offset, int8_t* value,
                                     const Ascii_ValueType* delimiter,
                                     const Ascii_LengthType expectedLen);

/**
 * Reads an 16-bit integer from a queue until the delimiter is reached or the expected length.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param value The destination value.
 * @param delimiter The delimiter.
 * @param expectedLen The expected length.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_ReadInt16(const Ascii_QueueType* queue,
                                      Ascii_LengthType* offset, int16_t* value,
                                      const Ascii_ValueType* delimiter,
                                      const Ascii_LengthType expectedLen);

/**
 * Reads an 32-bit integer from a queue until the delimiter is reached or the expected length.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param value The destination value.
 * @param delimiter The delimiter.
 * @param expectedLen The expected length.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_ReadInt32(const Ascii_QueueType* queue,
                                      Ascii_LengthType* offset, int32_t* value,
                                      const Ascii_ValueType* delimiter,
                                      const Ascii_LengthType expectedLen);

/**
 * Reads an string from a queue until the delimiter is reached or the expected length.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param dest The destination string.
 * @param delimiter The delimiter.
 * @param expectedLen The expected length.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_ReadString(const Ascii_QueueType* queue,
                                       Ascii_LengthType* offset,
                                       Ascii_ValueType* dest,
                                       const Ascii_ValueType* delimiter,
                                       const Ascii_LengthType expectedLen);

/**
 * Searches in the queue, until the delimiter is reached.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param delimiter The searched delimiter.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_FindDelimiter(const Ascii_QueueType* queue,
                                          Ascii_LengthType* offset,
                                          const Ascii_ValueType* delimiter);

/**
 * Compares the given string with the characters in the queue.
 * @param queue The source queue.
 * @param offset The offset in the queue.
 * @param compareString The string which should be compared.
 * @param ignorePattern A ignore pattern. This characters will be ignored in the
 *  source queue.
 * @return Incomplete if not enough characters are available; Failed
 *  if an unexpected character was found; otherwise Succeeded.
 */
Ascii_OperationResult Ascii_CompareString(const Ascii_QueueType* queue,
                                          Ascii_LengthType* offset,
                                          const Ascii_ValueType* compareString,
                                          const Ascii_ValueType* ignorePattern);

/**
 * Calculates the 32-bit CRC of an ASCII string.
 * @param str The ASCII string.
 * @param resetCrc If this value is TRUE the CRC unit will be reseted.
 * @return The CRC32 check sum.
 */
uint32_t Ascii_CalcCrc32(const Ascii_ValueType* str, BOOL resetCrc);

/**
 * Calculates the 32-bit CRC over the given range of the queue.
 * @param queue A pointer to the ASCII queue.
 * @param offset The offset within the queue.
 * @param length The length of operation.
 * @param resetCrc If this value is TRUE the CRC unit will be reseted.
 * @return The CRC32 check sum.
 */
uint32_t Ascii_CalcQueueCrc32(const Ascii_QueueType* queue,
                              Ascii_LengthType offset, Ascii_LengthType length,
                              BOOL resetCrc);

/**
 * Converts an uint32_t value to an binary ASCII string.
 * @param buffer The destination buffer.
 * @param len The number of bits.
 * @param value The value which should be converted.
 * @return A pointer to the destination buffer.
 */
Ascii_ValueType* Ascii_DecToBin(Ascii_ValueType* buffer,
                                const Ascii_LengthType len,
                                const uint32_t value);

/**
 * Converts a decimal value to a string.
 * @param buffer The destination buffer.
 * @param bufferLength The destination buffer length.
 * @param value The value.
 * @param minimumLength The minimum length of the dest buffer. Leading zeros will be added
 *                        if the value is to small.
 * @return A pointer to the destination buffer.
 */
Ascii_ValueType* Ascii_DecToStr(Ascii_ValueType* buffer,
                                Ascii_LengthType bufferLength, int32_t value,
                                const Ascii_NumberBase base,
                                const uint32_t minimumLength);

/**
 * Reverse the given buffer.
 * @param buffer The buffer.
 * @param len The length of the buffer.
 * @return A pointer to the given buffer.
 */
Ascii_ValueType* Ascii_Reverse(Ascii_ValueType* buffer, Ascii_LengthType len);

/**
 * Checks if the given expected length was received
 * @param queue pointer to used queue
 * @param offset offset from read index
 * @param expectedLen expected length
 * @return Succeed if length is within queue, Incomplete if less, Failed
 *          otherwise
 */
Ascii_OperationResult Ascii_CheckLength(const Ascii_QueueType* queue,
                                        Ascii_LengthType* offset,
                                        const Ascii_LengthType expectedLen);

/**
 * Copies length characters from dest + destOffset to src + srcOffset
 * @param src The source buffer
 * @param srcOffset The source offset
 * @param dest The destination buffer
 * @param destOffset The destination buffer
 * @param length The length of data to be copied
 * @return The number of copied data
 */
Ascii_LengthType Ascii_CopyData(const Ascii_ValueType* src,
                                const Ascii_LengthType srcOffset,
                                Ascii_ValueType* dest,
                                const Ascii_LengthType destOffset,
                                Ascii_LengthType const length);

/**
 * Copies data from source queue to destination pointer
 * @param src pointer to the source queue
 * @param srcOffset offset within queue
 * @param dest pointer to the destination buffer
 * @param destOffset offset within buffer
 * @param length size of writeable buffer
 * @return
 */
Ascii_LengthType Ascii_CopyQueueData(const Ascii_QueueType* src,
                                     const Ascii_LengthType srcOffset,
                                     Ascii_ValueType* dest,
                                     const Ascii_LengthType destOffset,
                                     const Ascii_LengthType length);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

#endif
