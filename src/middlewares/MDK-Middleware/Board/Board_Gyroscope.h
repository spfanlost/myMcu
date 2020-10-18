/*-----------------------------------------------------------------------------
 * Copyright (c) 2013 - 2019 Arm Limited (or its affiliates). All
 * rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1.Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   3.Neither the name of Arm nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * Name:    Board_Gyroscope.h
 * Purpose: Gyroscope interface header file
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

#ifndef __BOARD_GYROSCOPE_H
#define __BOARD_GYROSCOPE_H

#include <stdint.h>

/* Gyroscope state */
typedef struct _GYROSCOPE_STATE {
  int32_t x;                    ///< X Axis Angular Speed in mdps (1/1000 deg/sec)
  int32_t y;                    ///< Y Axis Angular Speed in mdps (1/1000 deg/sec)
  int32_t z;                    ///< Z Axis Angular Speed in mdps (1/1000 deg/sec)
} GYROSCOPE_STATE;

/**
  \fn          int32_t Gyroscope_Initialize (void)
  \brief       Initialize gyroscope
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Gyroscope_Uninitialize (void)
  \brief       De-initialize gyroscope
  \returns
   - \b 0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Gyroscope_GetState (GYROSCOPE_STATE *pState)
  \brief       Get gyroscope state
  \param[out]  pState  pointer to GYROSCOPE_STATE structure
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/

extern int32_t Gyroscope_Initialize   (void);
extern int32_t Gyroscope_Uninitialize (void);
extern int32_t Gyroscope_GetState     (GYROSCOPE_STATE *pState);

#endif /* __BOARD_GYROSCOPE_H */
