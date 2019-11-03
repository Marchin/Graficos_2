#ifndef WAV_H
#define WAV_H

#define SAMPLE_RATE 2048
#define HALF_SAMPLE_RATE (SAMPLE_RATE/2 + 1)

// WAV file header format
struct WAV {
    char riff[4]; // RIFF string
	u32 overall_size; // overall size of file in bytes
    char wave[4]; // WAVE string
	char fmt_chunk_marker[4]; // fmt string with trailing null char
	u32 length_of_fmt; // length of the format data
	u16 format_type; // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	u16 channels; // no.of channels
	u32 sample_rate; // sampling rate (blocks per second)
	u32 byterate; // SampleRate * NumChannels * BitsPerSample/8
	u16 block_align; // NumChannels * BitsPerSample/8
	u16 bits_per_sample; // bits per sample, 8- 8bits, 16- 16 bits etc
	char data_chunk_header[4]; // DATA string or FLLR string
	u32 data_size; // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
    void* pData;
};

ENGINE_API s32 readWAV(WAV* pWAV, const char* pPath);
ENGINE_API void playPauseMusic();
ENGINE_API void setMusicData(u32 audioBlockAlign, u32 musicSizeInBytes);
ENGINE_API int patestCallback(const void* inputBuffer, void* outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData);
ENGINE_API int fftCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData);
ENGINE_API int sinCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* pUserData);
ENGINE_API f32* getFFTModResult();
ENGINE_API u32 getFFTModSize();
ENGINE_API b32 hasMusicBufferChanged();
ENGINE_API void clearMusicBufferChanged();
#endif //WAV_H