MusicVisualizerConfig* gpMusicVisualizerConfig;

s32
readWAV(WAV* pWAV, const char* pPath) {
    FILE* pFile = fopen(pPath, "rb");
	char buffer[4];
    
    WAV wav = {};
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        u32 size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        fread(buffer, sizeof(u8), 4, pFile);
        if (strncmp(buffer, "RIFF", 4) != 0) { return -1; }
        strncpy(wav.riff, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.overall_size, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        if (strncmp(buffer, "WAVE", 4) != 0) { return -1; }
        strncpy(wav.wave, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        strncpy(wav.fmt_chunk_marker, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.length_of_fmt, buffer, 4);
        fread(buffer, sizeof(u8), 2, pFile);
        memcpy(&wav.format_type, buffer, 2);
        fread(buffer, sizeof(u8), 2, pFile);
        memcpy(&wav.channels, buffer, 2);
        fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.sample_rate, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.byterate, buffer, 4);
        fread(buffer, sizeof(u8), 2, pFile);
        memcpy(&wav.block_align, buffer, 2);
        fread(buffer, sizeof(u8), 2, pFile);
        memcpy(&wav.bits_per_sample, buffer, 2);
        fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.data_chunk_header, buffer, 4);
        size_t err = fread(buffer, sizeof(u8), 4, pFile);
        memcpy(&wav.data_size, buffer, 4);
        if (pWAV != 0) {
            *pWAV = wav;
            pWAV->pData = malloc(pWAV->data_size);
            err = fread(pWAV->pData, sizeof(u8), pWAV->data_size , pFile);
            fclose(pFile);
        }
    } else {
        perror("Error opening file\n");
        return -1;
    }
    
    return 0;
}

global b32 gPaused = true;
global u8* gpPlaying;
global u32 gAudioBlockAlign;
global u32 gMusicBytesLeft;

void
setMusicData(u32 audioBlockAlign, u32 musicSizeInBytes) {
    gAudioBlockAlign = audioBlockAlign;
    gMusicBytesLeft = musicSizeInBytes;
}

void playMusic() {
    gPaused = false;
}

global f32 value;
global f32 gVolume = 0.5f;
global f32 gFFTMod[FRAMES_PER_BUFFER];

int
patestCallback(const void* inputBuffer, void* outputBuffer,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags,
               void* pUserData) {
    /* Cast data passed through stream to our structure. */
    if (gPaused) { return paContinue; }
    
    if (gpPlaying == 0) { gpPlaying = (u8*)pUserData; }
    s16* out = (s16*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    u32 byteCountToSend = gAudioBlockAlign*framesPerBuffer;
    
    if (byteCountToSend > gMusicBytesLeft) {
        byteCountToSend = gMusicBytesLeft;
    }
    gMusicBytesLeft -= byteCountToSend;
    
    memcpy(outputBuffer, gpPlaying, byteCountToSend);
    
    if (byteCountToSend < gMusicBytesLeft) {
        gpPlaying += byteCountToSend;
        return paContinue;
    } else {
        return paComplete;
    }
}

kiss_fft_cfg gMycfg = kiss_fft_alloc(FRAMES_PER_BUFFER * TWO_CHANNELS, 0, NULL, NULL);

int
fftCallback(const void* inputBuffer, void* outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void* pUserData) {
    /* Cast data passed through stream to our structure. */
    if (gPaused) { return paContinue; }
    
    if (gpPlaying == 0) { gpPlaying = (u8*)pUserData; }
    s16* out = (s16*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    u32 byteCountToSend = gAudioBlockAlign*FRAMES_PER_BUFFER;
    
    if (byteCountToSend > gMusicBytesLeft) {
        byteCountToSend = gMusicBytesLeft;
    }
    gMusicBytesLeft -= byteCountToSend;
    
    memcpy(outputBuffer, gpPlaying, byteCountToSend);
    
    kiss_fft_cpx buffer[FRAMES_PER_BUFFER] = {};
    s32* pHelper = (s32*)outputBuffer;
    for (u32 i = 0; i < FRAMES_PER_BUFFER * TWO_CHANNELS; ++i) {
        s16* pLR = (s16*)pHelper++;
        buffer[i].r = (f32)(pLR[0]);
        //buffer[i].r = (f32)((s32)pLR[0] + (s32)pLR[1])*0.5f;
    }
    
    kiss_fft_cpx output[FRAMES_PER_BUFFER];
    
    kiss_fft(gMycfg, buffer, output);
    for (u32 i = 0; i < FRAMES_PER_BUFFER * TWO_CHANNELS; ++i) {
        gFFTMod[i] = sqrt(output[i].r*output[i].r + output[i].i*output[i].i);
    }
    printf("NO");
    if (byteCountToSend < gMusicBytesLeft) {
        gpPlaying += byteCountToSend;
        return paContinue;
    } else {
        return paComplete;
    }
}

int
sinCallback(const void* inputBuffer, void* outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void* pUserData) {
    (u8*)pUserData;
    f32* out = (f32*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */
    local_persist f32 wave_period = 440.f/44100.f;
    
    kiss_fft_cpx buffer[FRAMES_PER_BUFFER] = {};
    for (u32 i = 0; i < FRAMES_PER_BUFFER; ++i) {
        buffer[i].r = sinf(2*PI32*value)*gVolume;
        *out++ = buffer[i].r;
        value += wave_period;
        if (value >= 1.f) {
            value = 1.f;
            wave_period = -wave_period;
        }
    }
    
    kiss_fft_cpx output[FRAMES_PER_BUFFER];
    
    kiss_fft(gMycfg, buffer, output);
    f32 mod[FRAMES_PER_BUFFER] = {};
    for (u32 i = 0; i < FRAMES_PER_BUFFER; ++i) {
        mod[i] = output[i].r*output[i].r + output[i].i*output[i].i;
    }
    
    return paContinue;
}

f32*
getFFTModResult() {
    return gFFTMod;
}

u32
getFFTModSize() {
    return arrayCount(gFFTMod);
}

void
setMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig) {
    gpMusicVisualizerConfig = pMusicVisualizerConfig;
}