#ifndef WAV_H
#define WAV_H

#define SAMPLE_RATE 2048
#define HALF_SAMPLE_RATE (SAMPLE_RATE/2 + 1)

// WAV file header format
struct WAV {
    char riff[4]; // RIFF string
	u32 overallSize; // overall size of file in bytes
    char wave[4]; // WAVE string
	char fmtChunkMarker[4]; // fmt string with trailing null char
	u32 lengthOfFmt; // length of the format data
	u16 formatType; // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	u16 channels; // no.of channels
	u32 sampleRate; // sampling rate (blocks per second)
	u32 byterate; // SampleRate * NumChannels * BitsPerSample/8
	u16 blockAlign; // NumChannels * BitsPerSample/8
	u16 bitsPerSample; // bits per sample, 8- 8bits, 16- 16 bits etc
	char dataChunkHeader[4]; // DATA string or FLLR string
	u32 dataSize; // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
    void* pData;
};

struct MusicData {
    f32* pFFTMod;
    kiss_fftr_cfg cfg;
    kiss_fft_scalar* pTBuffer;
    kiss_fft_cpx* pFBuffer;
    void* musicData;
    u8* pPlaying;
    u32 audioBlockAlign;
    u32 musicBytesLeft;
    b32 paused;
    b32 changed;
};

ENGINE_API s32 readWAV(WAV* pWAV, const char* pPath);
ENGINE_API int fftCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData);
ENGINE_API void initMusicData(MusicData* pMusicData, WAV* pWAV);
#endif //WAV_H