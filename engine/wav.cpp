ENGINE_API s32
readWAV(WAV* pWAV) {
    FILE* pFile = fopen("../resources/moonglow.wav", "rb");
	char buffer[4];
    
    WAV wav = {};
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        u32 size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        if (strncmp(buffer, "RIFF", 4) != 0) { return -1; }
        strncpy(wav.riff, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.overall_size, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        if (strncmp(buffer, "WAVE", 4) != 0) { return -1; }
        strncpy(wav.wave, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        strncpy(wav.fmt_chunk_marker, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.length_of_fmt, buffer, 4);
        fread(buffer, sizeof(u8), 2, pFile);
        size -= 2;
        memcpy(&wav.format_type, buffer, 2);
        fread(buffer, sizeof(u8), 2, pFile);
        size -= 2;
        memcpy(&wav.channels, buffer, 2);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.sample_rate, buffer, 4);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.byterate, buffer, 4);
        fread(buffer, sizeof(u8), 2, pFile);
        size -= 2;
        memcpy(&wav.block_align, buffer, 2);
        fread(buffer, sizeof(u8), 2, pFile);
        size -= 2;
        memcpy(&wav.bits_per_sample, buffer, 2);
        fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.data_chunk_header, buffer, 4);
        size_t err = fread(buffer, sizeof(u8), 4, pFile);
        size -= 4;
        memcpy(&wav.data_size, buffer, 4);
        if (pWAV != 0) {
            *pWAV = wav;
            pWAV->pData = malloc(size);
            err = fread(pWAV->pData, sizeof(u8), pWAV->data_size , pFile);
            fclose(pFile);
        }
    } else {
        perror("Error opening file\n");
        return -1;
    }
    
    return 0;
}

ENGINE_API int
patestCallback(const void* inputBuffer, void* outputBuffer,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags,
               void* pUserData) {
    /* Cast data passed through stream to our structure. */
    f32* pData = (f32*)pUserData;
    f32* out = (f32*)outputBuffer;
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    for (u32 i = 0; i < framesPerBuffer; ++i) {
        *out++ = *pData;  /* left */
        pData++;
    }
    return 0;
}