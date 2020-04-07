/******************************************************************************
    Copyright (C) 2014 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#if OBS
#include <obs.h>
#endif

#define MILLISECOND_DEN 1000

enum obs_encoder_type {
	OBS_ENCODER_AUDIO, /**< The encoder provides an audio codec */
	OBS_ENCODER_VIDEO  /**< The encoder provides a video codec */
};

enum { OBS_NAL_PRIORITY_DISPOSABLE = 0,
       OBS_NAL_PRIORITY_LOW = 1,
       OBS_NAL_PRIORITY_HIGH = 2,
       OBS_NAL_PRIORITY_HIGHEST = 3,
};

struct encoder_packet {
	uint8_t *data; /**< Packet data */
	size_t size;   /**< Packet size */

	int64_t pts; /**< Presentation timestamp */
	int64_t dts; /**< Decode timestamp */

	int32_t timebase_num; /**< Timebase numerator */
	int32_t timebase_den; /**< Timebase denominator */

	enum obs_encoder_type type; /**< Encoder type */

	bool keyframe; /**< Is a keyframe */

	/* ---------------------------------------------------------------- */
	/* Internal video variables (will be parsed automatically) */

	/* DTS in microseconds */
	int64_t dts_usec;

	/* System DTS in microseconds */
	int64_t sys_dts_usec;

	/**
	 * Packet priority
	 *
	 * This is generally use by video encoders to specify the priority
	 * of the packet.
	 */
	int priority;

	/**
	 * Dropped packet priority
	 *
	 * If this packet needs to be dropped, the next packet must be of this
	 * priority or higher to continue transmission.
	 */
	int drop_priority;

	/** Audio track index (used with outputs) */
	size_t track_idx;
};

static int32_t get_ms_time(struct encoder_packet *packet, int64_t val)
{
	return (int32_t)(val * MILLISECOND_DEN / packet->timebase_den);
}

extern void write_file_info(FILE *file, int64_t duration_ms, int64_t size);

extern bool flv_meta_data(/*obs_output_t *context,*/ uint8_t **output, size_t *size,
			  bool write_header, size_t audio_idx);
extern void flv_packet_mux(struct encoder_packet *packet, int32_t dts_offset,
			   uint8_t **output, size_t *size, bool is_header);
