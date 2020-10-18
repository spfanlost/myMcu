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
 * Name:    Board_Audio.h
 * Purpose: Audio interface header file
 * Rev.:    1.1.0
 *----------------------------------------------------------------------------*/

#ifndef __BOARD_AUDIO_H
#define __BOARD_AUDIO_H

#include <stdint.h>
#include <stdbool.h>

/* Audio Stream identifier */
#define AUDIO_STREAM_OUT        0x00    ///< Output stream
#define AUDIO_STREAM_IN         0x80    ///< Input stream

/* Audio Channel identifier */
#define AUDIO_CHANNEL_MASTER    0       ///< Master channel
#define AUDIO_CHANNEL_LEFT      1       ///< Left channel (stereo)
#define AUDIO_CHANNEL_RIGHT     2       ///< Right channel (stereo)

/* Audio Data format */
#define AUDIO_DATA_8_MONO       0x01    ///<    8-bit samples (uint8_t)
#define AUDIO_DATA_16_MONO      0x02    ///<   16-bit samples (uint16_t)
#define AUDIO_DATA_32_MONO      0x04    ///<   32-bit samples (uint32_t)
#define AUDIO_DATA_8_STEREO     0x81    ///< 2* 8-bit stereo samples: L R (uint8_t)
#define AUDIO_DATA_16_STEREO    0x82    ///< 2*16-bit stereo samples: L R (uint16_t)
#define AUDIO_DATA_32_STEREO    0x84    ///< 2*32-bit stereo samples: L R (uint32_t)

/* Audio Volume */
#define AUDIO_VOLUME_AUTO_GAIN  0x80    ///< Automatic gain control

/* Audio Events */
#define AUDIO_EVENT_SEND_COMPLETE       (1UL << 0)  ///< Send completed
#define AUDIO_EVENT_RECEIVE_COMPLETE    (1UL << 1)  ///< Receive completed
#define AUDIO_EVENT_TX_UNDERFLOW        (1UL << 2)  ///< Transmit data not available
#define AUDIO_EVENT_RX_OVERFLOW         (1UL << 3)  ///< Receive data overflow

/**
  \fn          int32_t Audio_Initialize (Audio_SignalEvent_t cb_event)
  \brief       Initialize Audio Interface
  \param[in]   cb_event  pointer to event notification function
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_Uninitialize (void)
  \brief       De-initialize Audio Interface
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_SendData (const void *data, uint32_t num)
  \brief       Prepare for sending data to Audio output stream
  \param[in]   data  pointer to buffer with data to send
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_ReceiveData (void *data, uint32_t num)
  \brief       Prepare for receiving data from Audio input stream
  \param[out]  data  pointer to buffer for data to receive
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          uint32_t Audio_GetDataTxCount (void)
  \brief       Get transmitted data count
  \returns     number of data items transmitted
*/
/**
  \fn          uint32_t Audio_GetDataRxCount (void)
  \brief       Get received data count
  \returns     number of data items received
*/
/**
  \fn          int32_t Audio_Start (uint8_t stream)
  \brief       Start Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_Stop (uint8_t stream)
  \brief       Stop Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_Pause (uint8_t stream)
  \brief       Pause Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_Resume (uint8_t stream)
  \brief       Resume Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume)
  \brief       Set volume level for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   volume   volume level (0..100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_SetMute (uint8_t stream, uint8_t channel, bool mute)
  \brief       Set mute state for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   mute     mute state
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_SetDataFormat (uint8_t stream, uint8_t format)
  \brief       Set Audio data format
  \param[in]   stream  stream identifier
  \param[in]   format  data format
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
/**
  \fn          int32_t Audio_SetFrequency (uint8_t stream, uint32_t frequency)
  \brief       Set Audio stream frequency
  \param[in]   stream     stream identifier
  \param[in]   frequency  Audio frequency in Hz
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/

/**
  \fn          void Audio_SignalEvent (uint32_t event)
  \brief       Signal Audio Events
  \param[in]   event  notification mask
  \return      none
*/
typedef void (*Audio_SignalEvent_t) (uint32_t event);

extern int32_t  Audio_Initialize    (Audio_SignalEvent_t cb_event);
extern int32_t  Audio_Uninitialize  (void);
extern int32_t  Audio_SendData      (const void *data, uint32_t num);
extern int32_t  Audio_ReceiveData   (      void *data, uint32_t num);
extern uint32_t Audio_GetDataTxCount(void);
extern uint32_t Audio_GetDataRxCount(void);
extern int32_t  Audio_Start         (uint8_t stream);
extern int32_t  Audio_Stop          (uint8_t stream);
extern int32_t  Audio_Pause         (uint8_t stream);
extern int32_t  Audio_Resume        (uint8_t stream);
extern int32_t  Audio_SetVolume     (uint8_t stream, uint8_t channel, uint8_t volume);
extern int32_t  Audio_SetMute       (uint8_t stream, uint8_t channel, bool mute);
extern int32_t  Audio_SetDataFormat (uint8_t stream, uint8_t format);
extern int32_t  Audio_SetFrequency  (uint8_t stream, uint32_t frequency);

#endif /* __BOARD_AUDIO_H */
