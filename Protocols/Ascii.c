/**
 ******************************************************************************
 * @file    Ascii.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    19.01.2015
 * @brief   Library for ASCII operations.
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Ascii.h"

#include "Services/CrcIf.h"

#include <string.h>

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

Ascii_OperationResult Ascii_ReadInt8(const Ascii_QueueType* queue,
                                     Ascii_LengthType* offset, int8_t* value,
                                     const Ascii_ValueType* delimiter,
                                     const Ascii_LengthType expectedLen)
{
  /* Local variables */
  Ascii_OperationResult res;
  int32_t tempVal;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  tempVal = 0;

  /* Only check value, because the other parameters are check in the
   * Ascii_ReadInt32(...) function */
  if (value != NULL_PTR)
  {
    res = Ascii_ReadInt32(queue, offset, &tempVal, delimiter, expectedLen);

    if (res == Ascii_OperationResultSucceeded)
    {
      /* Convert value to correct data type */
      *value = (int8_t) tempVal;
    }
  }

  return res;
}

Ascii_OperationResult Ascii_ReadInt16(const Ascii_QueueType* queue,
                                      Ascii_LengthType* offset, int16_t* value,
                                      const Ascii_ValueType* delimiter,
                                      const Ascii_LengthType expectedLen)
{
  /* Local variables */
  Ascii_OperationResult res;
  int32_t tempVal;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  tempVal = 0;

  /* Only check value, because the other parameters are check in the
   * Ascii_ReadInt32(...) function */
  if (value != NULL_PTR)
  {
    res = Ascii_ReadInt32(queue, offset, &tempVal, delimiter, expectedLen);

    if (res == Ascii_OperationResultSucceeded)
    {
      /* Convert value to correct data type */
      *value = (int16_t) tempVal;
    }
  }

  return res;
}

Ascii_OperationResult Ascii_ReadInt32(const Ascii_QueueType* queue,
                                      Ascii_LengthType* offset, int32_t* value,
                                      const Ascii_ValueType* delimiter,
                                      const Ascii_LengthType expectedLen)
{
  /* Local variables */
  Ascii_OperationResult res;
  int32_t tempVal;
  int32_t negativeFactor;
  Ascii_LengthType i;
  Ascii_LengthType j;
  Ascii_LengthType idx;
  Ascii_LengthType len;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  tempVal = 0;
  negativeFactor = 1;
  i = 0u;
  j = 0u;
  idx = 0u;
  len = 0u;

  /* Check parameters */
  if ((queue != NULL_PTR) && (offset != NULL_PTR) && (value != NULL_PTR))
  {
    /* Update return value to incomplete */
    res = Ascii_OperationResultIncomplete;
    /* Calculate correct values of queue */
    idx = (Ascii_LengthType) ((queue->rdIdx + *offset) % queue->size);
    len = (Ascii_LengthType) (queue->size - queue->space - *offset);

    for (i = 0u; (res == Ascii_OperationResultIncomplete) && (i < len); i++)
    {
      if (i >= expectedLen)
      {
        /* Mark that the operation is succeeded */
        res = Ascii_OperationResultSucceeded;
        /* Decrement to get correct offset */
        i--;
      }
      else if (delimiter != NULL_PTR)
      {
        for (j = 0u; delimiter[j] != '\0'; j++)
        {
          if (queue->entries[idx] == delimiter[j])
          {
            /* Mark that the operation is succeeded */
            res = Ascii_OperationResultSucceeded;
            /* Decrement to get correct offset */
            i--;
            break;
          }
        }
      }

      /* Only convert values, if we are in incomplete state */
      if (res == Ascii_OperationResultIncomplete)
      {
        /* Check if the entry is a digit */
        if (Ascii_IsDigit(queue->entries[idx]))
        {
          tempVal =
            (int32_t) (tempVal * 10 + Ascii_ToDigit(queue->entries[idx]));
        }
        /* Negative sign is only allowed on the first position */
        else if ((i == 0u) && Ascii_IsNegativeSign(queue->entries[idx]))
        {
          negativeFactor = -1;
        }
        else
        {
          /* Not allowed, operation failed */
          res = Ascii_OperationResultFailed;
        }
      }

      /* Update queue index */
      idx = (Ascii_LengthType) ((idx + 1u) % queue->size);
    }

    /* Check if length reached */
    if ((res != Ascii_OperationResultFailed) && (i >= expectedLen))
    {
      /* Mark that the operation is succeeded */
      res = Ascii_OperationResultSucceeded;
    }

    /* Update offset */
    *offset = (Ascii_LengthType) (*offset + i);

    if (res == Ascii_OperationResultSucceeded)
    {
      /* Update value */
      *value = tempVal * negativeFactor;
    }
  }

  return res;
}

Ascii_OperationResult Ascii_ReadString(const Ascii_QueueType* queue,
                                       Ascii_LengthType* offset,
                                       Ascii_ValueType* dest,
                                       const Ascii_ValueType* delimiter,
                                       const Ascii_LengthType expectedLen)
{
  /* Local variables */
  Ascii_OperationResult res;
  Ascii_LengthType i;
  Ascii_LengthType j;
  Ascii_LengthType idx;
  Ascii_LengthType len;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  i = 0u;
  j = 0u;
  idx = 0u;
  len = 0u;

  /* Check parameters */
  if ((queue != NULL_PTR) && (offset != NULL_PTR) && (dest != NULL_PTR))
  {
    /* Update return value to incomplete */
    res = Ascii_OperationResultIncomplete;
    /* Calculate correct values of queue */
    idx = (Ascii_LengthType) ((queue->rdIdx + *offset) % queue->size);
    len = (Ascii_LengthType) (queue->size - queue->space - *offset);

    for (i = 0u; (res == Ascii_OperationResultIncomplete) && (i < len); i++)
    {
      if (i >= expectedLen)
      {
        /* Mark that the operation is succeeded */
        res = Ascii_OperationResultSucceeded;
        /* Decrement to get correct offset */
        i--;
      }
      else if (delimiter != NULL_PTR)
      {
        for (j = 0u; delimiter[j] != '\0'; j++)
        {
          if (queue->entries[idx] == delimiter[j])
          {
            /* Mark that the operation is succeeded */
            res = Ascii_OperationResultSucceeded;
            /* Decrement to get correct offset */
            i--;
            break;
          }
        }
      }

      /* Only convert values, if we are in incomplete state */
      if (res == Ascii_OperationResultIncomplete)
      {
        /* Copy current character */
        dest[j++] = queue->entries[idx];
      }

      /* Update queue index */
      idx = (Ascii_LengthType) ((idx + 1u) % queue->size);
    }

    /* Check if length reached */
    if (i >= expectedLen)
    {
      /* Mark that the operation is succeeded */
      res = Ascii_OperationResultSucceeded;
    }

    /* Add null termination */
    dest[j] = '\0';
    /* Update offset */
    *offset = (Ascii_LengthType) (*offset + i);
  }

  return res;
}

Ascii_OperationResult Ascii_FindDelimiter(const Ascii_QueueType* queue,
                                          Ascii_LengthType* offset,
                                          const Ascii_ValueType* delimiter)
{
  /* Local variables */
  Ascii_OperationResult res;
  Ascii_LengthType i;
  Ascii_LengthType j;
  Ascii_LengthType idx;
  Ascii_LengthType len;
  Ascii_LengthType delimiterLen;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  i = 0u;
  j = 0u;
  idx = 0u;
  len = 0u;
  delimiterLen = 0u;

  /* Check parameters */
  if ((queue != NULL_PTR) && (offset != NULL_PTR) && (delimiter != NULL_PTR))
  {
    /* Update return value to incomplete */
    res = Ascii_OperationResultIncomplete;
    /* Calculate correct values of queue */
    idx = (Ascii_LengthType) ((queue->rdIdx + *offset) % queue->size);
    len = (Ascii_LengthType) (queue->size - queue->space - *offset);
    /* Calculate delimiter length */
    delimiterLen = (Ascii_LengthType) strlen(delimiter);

    for (i = 0u;
      (res == Ascii_OperationResultIncomplete) && (i < len)
      && (j < delimiterLen); i++)
    {
      for (j = 0u; (j < delimiterLen) && ((i + j) < len); j++)
      {
        idx =
          (Ascii_LengthType) ((queue->rdIdx + *offset + i + j) % queue->size);

        if (queue->entries[idx] != delimiter[j])
        {
          break;
        }
      }

      /* If delimiter was not found */
      if (j < delimiterLen)
      {
        /* Calculate next read index */
        idx = (Ascii_LengthType) ((queue->rdIdx + *offset + i) % queue->size);
      }
      else
      {
        res = Ascii_OperationResultSucceeded;
      }
    }

    if (res == Ascii_OperationResultSucceeded)
    {
      /* Calculate new offset */
      *offset = (Ascii_LengthType) (*offset + i + j - 1);
    }
    else
    {
      *offset = (Ascii_LengthType) (*offset + i);
    }
  }

  return res;
}

Ascii_OperationResult Ascii_CompareString(const Ascii_QueueType* queue,
                                          Ascii_LengthType* offset,
                                          const Ascii_ValueType* compareString,
                                          const Ascii_ValueType* ignorePattern)
{
  /* Local variables */
  Ascii_OperationResult res;
  Ascii_LengthType i;
  Ascii_LengthType j;
  Ascii_LengthType idx;
  Ascii_LengthType len;
  Ascii_LengthType compareStringLen;

  /* Initialize local variables */
  res = Ascii_OperationResultFailed;
  i = 0u;
  j = 0u;
  idx = 0u;
  len = 0u;
  compareStringLen = 0u;

  /* Check parameters */
  if ((queue != NULL_PTR) && (offset != NULL_PTR)
      && (compareString != NULL_PTR))
  {
    /* Update return value to incomplete */
    res = Ascii_OperationResultIncomplete;
    /* Calculate correct values of queue */
    idx = (Ascii_LengthType) ((queue->rdIdx + *offset) % queue->size);
    len = (Ascii_LengthType) (queue->size - queue->space - *offset);
    /* Calculate delimiter length */
    compareStringLen = (Ascii_LengthType) strlen(compareString);

    for (i = 0u, j = 0u;
      (res == Ascii_OperationResultIncomplete) && (i < len)
      && (j < compareStringLen); i++)
    {
      /* If  */
      if (queue->entries[idx] == compareString[j])
      {
        j++;
      }
      else if (ignorePattern != NULL_PTR)
      {
        /* Iterate through loop, until the value is found or the end
         * of the pattern is found */
        for (j = 0u;
          (ignorePattern[j] != '\0') && (queue->entries[idx] == ignorePattern[j]);
          j++)
          ;

        /* If character was not found in ignore pattern */
        if (ignorePattern[j] == '\0')
        {
          res = Ascii_OperationResultFailed;
        }
      }
      else
      {
        res = Ascii_OperationResultFailed;
      }

      idx = (Ascii_LengthType) ((idx + 1u) % queue->size);
    }

    /* Calculate new offset */
    *offset = (Ascii_LengthType) (*offset + i);

    if (j == compareStringLen)
    {
      /* Operation succeeded */
      res = Ascii_OperationResultSucceeded;
    }
    /* TODO: check compare string length */
  }

  return res;
}

uint32_t Ascii_CalcCrc32(const Ascii_ValueType* str, BOOL resetCrc)
{
  /* Local variables */
  uint32_t crc;
  Ascii_LengthType length;

  /* Initialize local variables */
  crc = 0u;
  length = (Ascii_LengthType) strlen(str);

  /* Reset CRC unit */
  if (resetCrc)
  {
    CrcIf_Reset();
  }

  /* Calculate CRC */
  CrcIf_CalcBlock8((const uint8_t*) str, length);

  /* Get CRC */
  crc = CrcIf_GetCrc();

  return crc;
}

uint32_t Ascii_CalcQueueCrc32(const Ascii_QueueType* queue,
                              Ascii_LengthType offset, Ascii_LengthType length,
                              BOOL resetCrc)
{
  /* Local variables */
  uint32_t crc;
  Ascii_LengthType startIdx;
  Ascii_LengthType endIdx;

  /* Initialize local variables */
  crc = 0u;
  startIdx = (Ascii_LengthType) ((queue->rdIdx + offset) % queue->size);
  endIdx = (Ascii_LengthType) ((queue->rdIdx + offset + length) % queue->size);

  /* Reset CRC unit */
  if (resetCrc)
  {
    CrcIf_Reset();
  }

  /* Calculate CRC */
  if (startIdx < endIdx)
  {
    CrcIf_CalcBlock8((const uint8_t*) (queue->entries + startIdx),
                     (uint32_t) (endIdx - startIdx));
  }
  else
  {
    CrcIf_CalcBlock8((const uint8_t*) (queue->entries + startIdx),
                     (uint32_t) (queue->size - startIdx));
    CrcIf_CalcBlock8((const uint8_t*) queue->entries, endIdx);
  }

  /* Get CRC */
  crc = CrcIf_GetCrc();

  return crc;
}

Ascii_ValueType* Ascii_DecToBin(Ascii_ValueType* buffer,
                                const Ascii_LengthType len,
                                const uint32_t value)
{
  /* Local variables */
  int8_t i;
  Ascii_ValueType* workingBuffer;

  /* Initialize local variables */
  i = 0u;
  workingBuffer = buffer;

  for (i = (int8_t) (len - 1u); i >= 0; i--)
  {
    *workingBuffer = Ascii_FromDigit((value >> i) & 1);
    workingBuffer++;
  }

  /* Add null termination */
  *workingBuffer = '\0';

  return buffer;
}

Ascii_ValueType* Ascii_Reverse(Ascii_ValueType* buffer, Ascii_LengthType len)
{
  /* Local variables */
  Ascii_LengthType i;
  Ascii_ValueType tmp;
  Ascii_ValueType* bufferPtr;

  /* Initialize local variables */
  tmp = 0u;
  bufferPtr = buffer + len - 1;
  len = (Ascii_LengthType) (len / 2u);

  for (i = 0; i < len; i++)
  {
    tmp = *bufferPtr;
    *bufferPtr = buffer[i];
    buffer[i] = tmp;

    bufferPtr--;
  }

  return buffer;
}

Ascii_ValueType* Ascii_DecToStr(Ascii_ValueType* buffer,
                                Ascii_LengthType bufferLength, int32_t value,
                                const Ascii_NumberBase base,
                                const uint32_t minimumLength)
{
  /* Local variables */
  Ascii_LengthType i;
  Ascii_ValueType temp;
  Ascii_ValueType* workingBuffer;

  /* Initialize local variables */
  i = 0u;
  temp = 0u;
  workingBuffer = buffer;

  if (value == 0u)
  {
    *workingBuffer = Ascii_FromDigit(value);
    workingBuffer++;
  }

  for (i = 0u; (value != 0) && (i < bufferLength); i++)
  {
    *workingBuffer = Ascii_FromDigit(value % base);
    value /= base;

    workingBuffer++;
  }

  for (; (i < bufferLength) && (i < minimumLength); i++)
  {
    *workingBuffer = '0';
    workingBuffer++;
  }

  /* Add null termination */
  *workingBuffer = '\0';
  workingBuffer--;
  bufferLength = (Ascii_LengthType) (i / 2);

  for (i = 0u; i < bufferLength; i++)
  {
    temp = buffer[i];
    buffer[i] = *workingBuffer;
    *workingBuffer = temp;
    workingBuffer--;
  }

  return buffer;
}

Ascii_OperationResult Ascii_CheckLength(const Ascii_QueueType* queue,
                                        Ascii_LengthType* offset,
                                        const Ascii_LengthType expectedLen)
{
  /* local variables */
  Ascii_OperationResult result = Ascii_OperationResultFailed;
  
  /* check parameter */
  if (queue != NULL_PTR && offset != NULL_PTR)
  {
    /* check queue space */
    if ((queue->size - queue->space - *offset) >= expectedLen)
    {
      /* set offset and result */
      *offset = (Ascii_LengthType) ((*offset) + expectedLen);
      result = Ascii_OperationResultSucceeded;
    }
    else
    {
      /* set result */
      result = Ascii_OperationResultIncomplete;
    }
  }
  
  return result;
}

Ascii_LengthType Ascii_CopyData(const Ascii_ValueType* src,
                                const Ascii_LengthType srcOffset,
                                Ascii_ValueType* dest,
                                const Ascii_LengthType destOffset,
                                Ascii_LengthType const length)
{
  /* local variables */
  Ascii_LengthType i = 0;

  for (; i < length; i++)
  {
    dest[i + destOffset] = src[i + srcOffset];
  }

  return i;
}

Ascii_LengthType Ascii_CopyQueueData(const Ascii_QueueType* src,
                                  const Ascii_LengthType srcOffset,
                                  Ascii_ValueType* dest,
                                  const Ascii_LengthType destOffset,
                                  const Ascii_LengthType length)
{
  /* local variables */
  Ascii_LengthType i = 0;
  Ascii_LengthType tail = (Ascii_LengthType)(src->size - src->rdIdx - srcOffset);
  
  
  /* check if there is an overflow within queue */
  if (length < tail)
  {
    /* copy data directly from queue to destination buffer */
    for (; i < length; i++)
    {
      dest[i + destOffset] = src->entries[src->rdIdx + i + srcOffset];
    }
  }
  else
  {
    /* copy first part of data */
    for (i = 0; i < tail; i++)
    {
      dest[i + destOffset] = src->entries[src->rdIdx + i + srcOffset];
    }
    
    /* copy last part of data */
    for(i = tail; i < length; i++)
    {
      dest[i + destOffset] = src->entries[i - tail];
    }
  }

  return i;
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

