/**
 * @file audio.h
 * @brief Provides audio interface for streamer
 * @copyright Copyright (C) 2017 Elphel Inc.
 * @author AUTHOR <EMAIL>
 *
 * @par License:
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AUDIO__H_
#define _AUDIO__H_

#include <string>
#ifdef __cplusplus
	#undef __cplusplus
#endif
#define __cplusplus 1
#include <alsa/asoundlib.h>
#include <pthread.h>

#include "rtp_stream.h"
#include "parameters.h"

using namespace std;

#define SAMPLE_RATE	44100
#define SAMPLE_CHANNELS 2

class Audio : public RTP_Stream {
public:
	Audio(int port, bool enable, Parameters *pars, string &dev_name, int sample_rate = SAMPLE_RATE, int channels = SAMPLE_CHANNELS);
	virtual ~Audio(void);
	long sample_rate(void) { return _sample_rate; };
	long channels(void) { return _channels; };
	bool present(void) { return _present; };
	long volume(void) { return _volume; };
	void set_volume(long volume) { set_capture_volume(volume); }

	void Start(string ip, long port, int ttl = -1);
	void Stop(void);
protected:
	snd_pcm_t *capture_handle;                                  //< PCM handle, returned from snd_pcm_open
	short *sbuffer;                                             //< buffer for sound data
	long sbuffer_len;
	bool _present;                                              //< flag indicating that audio interface has been initialized
	long _sample_rate;
	long _channels;
	long _volume;

	long capture(void);
	long process(void);
	long process_send(long sample_len);
	void set_capture_volume(int volume);

	uint64_t timestamp_rtcp;
	long delta_fpga_sys;	                                    //< A/V clocks delta for RTCP
	long long delta_fpga_alsa;                                  //< time delta between FPGA time and time reported by ALSA, in microseconds
	unsigned char *packet_buffer;                               //< buffer for RTP packet data (header plus PCM samples)
private:
	long long get_delta_fpga_alsa(void);                        //< return time delta between FPGA time and time stamps reported by ALSA, in microseconds
};

#endif // _AUDIO__H_
