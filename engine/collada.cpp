
inline char*
skipWhitespace(char* string) {
    char* pResult = string;
    while (*pResult  == ' ') {
        ++pResult;
    }
    
    return pResult;
}

inline char*
readLineAndSkipWhitespace(char* buffer, u32 bufferSize, FILE* pFile) {
    fgets(buffer, bufferSize, pFile);
    char* pResult = skipWhitespace(buffer);
    
    return pResult;
}

inline char*
seekCharacter(char character, char* buffer) {
    char* pResult = 0;
    if (buffer != 0) {
        while (*buffer && *buffer != character) {
            ++buffer;
        }
        pResult = buffer;
    }
    return pResult;
}

inline char*
seekString(const char* string, char* buffer) {
    char* pResult = 0;
    u32 counter = 0;
    u32 strLength = (u32)strlen(string);
    if (buffer != 0 && strLength != 0) {
        while (*buffer) {
            counter = (*buffer == string[counter]) ? counter + 1 : 0;
            ++buffer;
            if (counter == strLength) {
                buffer -= strLength;
                break;
            }
        }
        pResult = buffer;
    }
    return pResult;
}


inline char*
seekNextString(char* string) {
    char* pResult = 0;
    
    if (string != 0) {
        b32 oldStringFinished = false;
        while (*string != 0 && *string++ != ' ') { }
        if (*string != 0) {
            while (*string != 0 && *string == ' ') {
                ++string;
            }
            pResult = string;
        }
    }
    
    return pResult;
}

const char geometryTag[] = "<geometry";
const char meshTag[] = "<mesh";
const char floatArrayTag[] = "<float_array";
const char accessorSourceTag[] = "<accessor source";
const char trianglesTag[] = "<triangles";
const char pTag[] = "<p>";


void
parseCollada(ModelData* pModelData, char* path) {
    const u32 kBufferSize = 2048;
    assert(pModelData);
    
    FILE* pFile = fopen(path, "r");
    char buffer[kBufferSize];
    
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        u32 size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        
        char* ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
        do {
            if (strncmp(ptr, geometryTag, strlen(geometryTag)) == 0) {
                ModelData modelData = {};
                ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                if (strncmp(ptr, meshTag, strlen(meshTag)) == 0) {
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, floatArrayTag, strlen(floatArrayTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    
                    const char countText[] = "count";
                    ptr = seekString(&countText[0], ptr);
                    ptr += strlen(countText) + 2 /* =" */;
                    u32 vertexCount = atoi(ptr) / 3;
                    modelData.vertexCount = vertexCount;
                    modelData.pVertices = (V3*)malloc(vertexCount*sizeof(V3));
                    
                    ptr = seekCharacter('>', ptr) + 1;
                    for (u32 iVertex = 0; iVertex < vertexCount; ++iVertex) {
                        V3 vertex = {};
                        vertex.x = (f32)atof(ptr);
                        ptr = seekNextString(ptr);
                        vertex.y = (f32)atof(ptr);
                        ptr = seekNextString(ptr);
                        vertex.z = (f32)atof(ptr);
                        ptr = seekNextString(ptr);
                        modelData.pVertices[iVertex] = vertex;
                    }
                    
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, accessorSourceTag, strlen(accessorSourceTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    
                    const char strideText[] = "stride";
                    ptr = seekString(&strideText[0], ptr);
                    ptr += strlen(strideText) + 2 /* =" */;
                    assert(atoi(ptr) == 3);
                    
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, floatArrayTag, strlen(floatArrayTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, floatArrayTag, strlen(floatArrayTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    ptr = seekString(&countText[0], ptr);
                    ptr += strlen(countText) + 2 /* =" */;
                    u32 uvCount = atoi(ptr) / 2;
                    modelData.uvCount = uvCount;
                    modelData.pUV = (V2*)malloc(uvCount*sizeof(V2));
                    
                    ptr = seekCharacter('>', ptr) + 1;
                    for (u32 iUV = 0; iUV < uvCount; ++iUV) {
                        V2 uv = {};
                        uv.u = (f32)atof(ptr);
                        ptr = seekNextString(ptr);
                        uv.v = (f32)atof(ptr);
                        ptr = seekNextString(ptr);
                        modelData.pUV[iUV] = uv;
                    }
                    
                    
                    
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, trianglesTag, strlen(trianglesTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    ptr = seekString(&countText[0], ptr);
                    ptr += strlen(countText) + 2 /* =" */;
                    u32 facesCount = atoi(ptr);
                    modelData.facesCount = facesCount;
                    modelData.pFaces = (Face*)malloc(facesCount*sizeof(Face));
                    
                    ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    while (strncmp(ptr, pTag, strlen(pTag)) != 0) {
                        ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
                    }
                    
                    ptr = seekCharacter('>', ptr) + 1;
                    for (u32 iFace = 0; iFace < facesCount; ++iFace) {
                        Face face = {};
                        face.vertexIndices[0] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        ptr = seekNextString(ptr);
                        face.uvIndices[0] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        face.vertexIndices[1] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        ptr = seekNextString(ptr);
                        face.uvIndices[1] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        face.vertexIndices[2] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        ptr = seekNextString(ptr);
                        face.uvIndices[2] = atoi(ptr);
                        ptr = seekNextString(ptr);
                        modelData.pFaces[iFace] = face;
                    }
                    
                    *pModelData = modelData;
                }
            }
            
            ptr = readLineAndSkipWhitespace(buffer, kBufferSize, pFile);
        } while (!feof(pFile));
        
        fclose(pFile);
    }
}
