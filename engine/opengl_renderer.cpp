ENGINE_API inline f32 
getTime() {
    return (f32)glfwGetTime();
}

internal inline void 
glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

internal inline bool
glLogCall(const char* pFunction, const char* pFile, s32 line) {
    while (GLenum error = glGetError()) {
        printf("[OpenGL Error] (%d): %s %s: %d\n", error, pFunction, pFile, line);
        
        return false;
    }
    
    return true;
}

internal void 
checkShaderCompileErrors(u32 shader, const char* pType) {
	s32 success;
	char infoLog[1024];
	if (strcmp(pType, "PROGRAM") != 0) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n\
                   -------------------------------------------------------\n", pType, infoLog);
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n\
                       -------------------------------------------------------\n", 
                       pType, infoLog);
			}
		}
	}
}

internal u32 
setupShader(const char* pShaderPath, u32 type) {
	FILE* pFile = fopen(pShaderPath, "r");
    char* pShaderCode;
	u32 id;
    
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        u32 size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        pShaderCode = (char*)malloc(size + 1);
        if (pShaderCode && size) {
            fread(pShaderCode, sizeof(char), size, pFile);
        }
        while (pShaderCode[size - 1] < '\0' && size > 0) {
            --size; // NOTE(Marchin): for some reason some text files have 
            //non-valid characters at EOF, so we get rid of them
        }
        pShaderCode[size] = '\0'; 
        fclose(pFile);
    } else {
        perror("Error opening file\n");
    }
	glCall(id = glCreateShader(type));
	glCall(glShaderSource(id, 1, &pShaderCode, NULL));
	glCall(glCompileShader(id));
    
    char* pVertexMsg = "VERTEX";
    char* pFragmentMsg = "FRAGMENT";
    char* pGeometryMsg = "GEOMETRY";
    char* msg = 0;
	if (type == GL_VERTEX_SHADER) msg = pVertexMsg;
	else if (type == GL_FRAGMENT_SHADER) msg = pFragmentMsg;
	else if (type == GL_GEOMETRY_SHADER) msg = pGeometryMsg;
	checkShaderCompileErrors(id, msg);
    
	return id;
}

ENGINE_API void
initShader(Shader* pShader,
           const char* pVertexPath, const char* pFragmentPath,
           const char* pGeometryPath, 
           const char* pTessControlPath, const char* pTessEvaluationPath) {
    
	u32 vertex = 0;
	u32 fragment = 0;
	u32 geometry = 0;
	u32 tessellationControl = 0;
	u32 tessellationEvaluation = 0;
    
	vertex = setupShader(pVertexPath, GL_VERTEX_SHADER);
	fragment = setupShader(pFragmentPath, GL_FRAGMENT_SHADER);
	if (pGeometryPath != nullptr) {
		geometry = setupShader(pGeometryPath, GL_GEOMETRY_SHADER);
	}
	if (pTessControlPath != nullptr) {
		tessellationControl = setupShader(pTessControlPath, GL_TESS_CONTROL_SHADER);
	}if (pTessEvaluationPath != nullptr) {
		tessellationEvaluation = setupShader(pTessEvaluationPath, 
                                             GL_TESS_EVALUATION_SHADER);
	}
    
	glCall(pShader->id = glCreateProgram());
	glCall(glAttachShader(pShader->id, vertex));
	glCall(glAttachShader(pShader->id, fragment));
	if (pGeometryPath != 0) {
		glCall(glAttachShader(pShader->id, geometry));
	}
	if (pTessControlPath != 0) {
		glCall(glAttachShader(pShader->id, tessellationControl));
	}if (pTessEvaluationPath != 0) {
		glCall(glAttachShader(pShader->id, tessellationEvaluation));
	}
	glCall(glLinkProgram(pShader->id));
	checkShaderCompileErrors(pShader->id, "PROGRAM");
    
	glCall(glDeleteShader(vertex));
	glCall(glDeleteShader(fragment));
	if (pGeometryPath != 0) {
		glCall(glDeleteShader(geometry));
	}
    
    for (int iUniform = 0; iUniform < UNIFORMS_MAX; ++iUniform) {
        pShader->uniformLocationCache[iUniform] = -1;
    }
    
}

ENGINE_API inline void 
shaderBindID(u32 shaderID) {
	glCall(glUseProgram(shaderID));
}

ENGINE_API s32 
getUniformLocation(Shader* pShader, const char* pName) {
    meow_hash hash = MeowHash_Accelerated(0, sizeof(pName), (const void*)pName);
    b32 found = false;
    s32 iHash;
    s32 firstFreeSlotIndex = -1;
    for (iHash = 0; iHash < UNIFORMS_MAX; ++iHash) {
        if (MeowHashesAreEqual(pShader->hashLocationCache[iHash], hash)) {
            found = true;
            break;
        } else {
            if (firstFreeSlotIndex == -1 && pShader->uniformLocationCache[iHash] == -1) {
                firstFreeSlotIndex = iHash;
            }
        }
    }
    if (found) {
        return pShader->uniformLocationCache[iHash];
    } else {
        glCall(s32 location = glGetUniformLocation(pShader->id, pName));
		if (location == -1) {
            printf("Warning: uniform '%s' doesn't exist\n", pName);
        }
        if (firstFreeSlotIndex == -1) {
            printf("Error: You exceeded the max number of uniforms in a shader allowed\n");
        } else {
            pShader->hashLocationCache[firstFreeSlotIndex] = hash;
            pShader->uniformLocationCache[firstFreeSlotIndex] = location;
        }
        return location;
    }
}

ENGINE_API inline void 
shaderSetBool(Shader* pShader, const char* pName, b32 value) {
	glCall(glUniform1i(getUniformLocation(pShader, pName), value));
}

ENGINE_API inline void
shaderSetInt(Shader* pShader, const char* pName, s32 value) {
	glCall(glUniform1i(getUniformLocation(pShader, pName), value));
}

ENGINE_API inline void
shaderSetFloat(Shader* pShader, const char* pName, f32 value) {
	glCall(glUniform1f(getUniformLocation(pShader, pName), value));
}

ENGINE_API inline void
shaderSetVec3(Shader* pShader, const char* pName, hmm_vec3* pVector) {
	glCall(glUniform3fv(getUniformLocation(pShader, pName), 1, &pVector->x));
}

ENGINE_API inline void
shaderSetMat4(Shader* pShader, const char* pName, hmm_mat4* pMat4) {
	glCall(glUniformMatrix4fv(getUniformLocation(pShader, pName), 
                              1, GL_FALSE, (f32*)pMat4));
}

ENGINE_API void
initTexture(Texture* pTexture, u32 width, u32 height) {
    pTexture->width = width;
    pTexture->height = height;
    glCall(glGenTextures(1, &pTexture->id));
    glCall(glBindTexture(GL_TEXTURE_2D, pTexture->id));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

ENGINE_API void
initTexture(Texture* pTexture,
            const char* pImgPath, b32 flipVertical, 
            s32 TextureWrap_S, s32 TextureWrap_T,
            s32 TextureMinFilter, s32 TextureMagFilter) {
    
    glCall(glGenTextures(1, &pTexture->id));
    glCall(glBindTexture(GL_TEXTURE_2D, pTexture->id));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrap_S));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrap_T));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureMinFilter));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureMagFilter));
    stbi_set_flip_vertically_on_load(flipVertical);
    
    s32 nrChannels;
    u8* pData = stbi_load(pImgPath, 
                          &pTexture->width, &pTexture->height, 
                          &nrChannels, 0);
    if (pData) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        } else {
            format = 0; // NOTE(Marchin): i arbitrary put 0 here, i should check later if it doesn't cause problems
        }
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, format, 
                            pTexture->width, pTexture->height,
                            0, format, GL_UNSIGNED_BYTE, pData));
        glCall(glGenerateMipmap(GL_TEXTURE_2D));
    } else {
        printf("Failed to load texture\n");
    }
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(pData);
}

ENGINE_API inline void
freeTexture(u32* pTextureID) {
    glCall(glDeleteTextures(1, pTextureID));
}

ENGINE_API inline void
textureBindID(u32 textureID, u32 slot) {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, textureID));
}

ENGINE_API inline void
initEB(u32* pEBObject,  u32* pData, u32 count) {
    glCall(glGenBuffers(1, pEBObject));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBObject));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                        count * sizeof(u32), 
                        pData, 
                        GL_STATIC_DRAW));
}

ENGINE_API inline void
freeEB(u32* pEBObject) {
    glCall(glDeleteBuffers(1, pEBObject));
}

ENGINE_API inline void 
ebBind(u32 ebObject) {
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebObject));
}

ENGINE_API inline void 
ebUnbind() {
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

ENGINE_API inline u32 
vbElementGetSizeOfType(u32 type) {
    u32 result = 0;
    switch (type) {
        case GL_FLOAT: {
            return 4;
        } break;
        case GL_UNSIGNED_INT: {
            return 4;
        } break;
        case GL_UNSIGNED_BYTE: {
            return 1;
        } break;
        default: {
            assert(false);
        }
    }
    return 0;
}

ENGINE_API void 
vbLayoutPushFloat(VertexBufferLayout* pVBLayout, u32 count) {
    if (pVBLayout->elementsCount >= pVBLayout->elementsMaxSize) {
        perror("You exceeded the max amount of elements in a VBLayout\n");
    } else {
        *(pVBLayout->pElements + pVBLayout->elementsCount)
            = { GL_FLOAT, count, GL_FALSE };
        pVBLayout->stride += count * vbElementGetSizeOfType(GL_FLOAT);
        ++pVBLayout->elementsCount;
    }
}

ENGINE_API void
vbLayoutPushUint(VertexBufferLayout* pVBLayout, u32 count) {
    if (pVBLayout->elementsCount >= pVBLayout->elementsMaxSize) {
        perror("You exceeded the max amount of elements in a VBLayout\n");
    } else {
        *(pVBLayout->pElements + pVBLayout->elementsCount)
            = { GL_UNSIGNED_INT, count, GL_FALSE };
        pVBLayout->stride += count * vbElementGetSizeOfType(GL_UNSIGNED_INT);
        ++pVBLayout->elementsCount;
    }
}

ENGINE_API void 
vbLayoutPushUchar(VertexBufferLayout* pVBLayout, u32 count) {
    if (pVBLayout->elementsCount >= pVBLayout->elementsMaxSize) {
        perror("You exceeded the max amount of elements in a VBLayout\n");
    } else {
        *(pVBLayout->pElements + pVBLayout->elementsCount)
            = { GL_UNSIGNED_BYTE, count, GL_FALSE };
        pVBLayout->stride += count * vbElementGetSizeOfType(GL_UNSIGNED_BYTE);
        ++pVBLayout->elementsCount;
    }
}

ENGINE_API inline void
initVB(u32* pVB) {
    glCall(glGenBuffers(1, pVB));
}

ENGINE_API inline void
initVB(u32* pVB, const void* pData, u32 size) {
    glCall(glGenBuffers(1, pVB));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, *pVB));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW));
}

ENGINE_API inline void
freeVB(u32* pVB) {
    glCall(glDeleteBuffers(1, pVB));
}

ENGINE_API inline void 
vbBind(u32 vb) {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
}

ENGINE_API inline void 
vbUnbind() {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

ENGINE_API inline void 
vbSetData(u32 vb, const void* pData, u32 size) {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW));
}

ENGINE_API inline void 
initVA(u32* pVA) {
    glCall(glGenVertexArrays(1, pVA));
}

ENGINE_API inline void 
freeVA(u32* pVA) {
    glCall(glDeleteVertexArrays(1, pVA));
}

ENGINE_API inline void
vaBind(u32 va) {
    glCall(glBindVertexArray(va));
}

ENGINE_API inline void 
vaUnbind() {
    glCall(glBindVertexArray(0));
}

ENGINE_API void 
vaAddBuffer(u32 va, u32 vb, VertexBufferLayout* pLayout) {
    vaBind(va);
    vbBind(vb);
    VertexBufferElement* pElements = pLayout->pElements;
    size_t offset = 0;
    for (u32 iElement = 0; iElement < pLayout->elementsCount; ++iElement) {
        VertexBufferElement* pElement = pElements + iElement;
        glCall(glVertexAttribPointer(iElement, 
                                     pElement->count, pElement->type, pElement->normalized,
                                     pLayout->stride, (void*)offset));
        glCall(glEnableVertexAttribArray(iElement));
        
        offset += pElement->count * vbElementGetSizeOfType(pElement->type);
    }
}

ENGINE_API void 
vaAddBufferByLocation(u32 va, u32 vb, VertexBufferLayout* pLayout, u32 location) {
    vaBind(va);
    vbBind(vb);
    VertexBufferElement* pElements = pLayout->pElements;
    size_t offset = 0;
    VertexBufferElement* pElement = pElements;
    glCall(glVertexAttribPointer(location, 
                                 pElement->count, pElement->type, pElement->normalized,
                                 0, (void*)offset));
    glCall(glEnableVertexAttribArray(location));
}

////////////////////////////////
//WINDOW
////////////////////////////////

inline void
framebufferSizeCallback(GLFWwindow* pWindow, s32 width, s32 height) {
    glViewport(0, 0, width, height);
}

ENGINE_API b32 
startWindow(Window* pWindow) {
    printf("Window::Start()\n");
	if (!glfwInit()) {
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	pWindow->pInstance = glfwCreateWindow(pWindow->width, pWindow->height, 
                                          pWindow->pName, NULL, NULL);
	if (pWindow->pInstance == 0) {
        printf("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent((GLFWwindow*)pWindow->pInstance);
    glfwSetFramebufferSizeCallback((GLFWwindow*)pWindow->pInstance, 
                                   framebufferSizeCallback);
    glfwSetInputMode((GLFWwindow*)pWindow->pInstance, 
                     GLFW_CURSOR, 
                     GLFW_CURSOR_DISABLED);  
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return false;
	}
	return true;
}

ENGINE_API b32  
stopWindow(Window* pWindow) {
    printf("Window::Stop()\n");
	if (pWindow->pInstance != 0) {
		glfwDestroyWindow((GLFWwindow*)pWindow->pInstance);
	}
    pWindow->pInstance = 0;
    
	glfwTerminate();
	return true;
}

ENGINE_API inline b32 
windowShouldClose(Window* pWindow) {
	if (pWindow->pInstance) {
		return glfwWindowShouldClose((GLFWwindow*)pWindow->pInstance);
	} else {
		return true;
	}
}

ENGINE_API inline void 
pollEventsFromWindow(Window* pWindow) {
	if (glfwGetKey((GLFWwindow*)pWindow->pInstance, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glCall(glfwSetWindowShouldClose((GLFWwindow*)pWindow->pInstance, true));
	}
    glCall(glfwPollEvents());
}

ENGINE_API inline b32
isKeyPressed(Renderer* pRenderer, u32 key) {
    if (glfwGetKey((GLFWwindow*)pRenderer->pWindow->pInstance, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

ENGINE_API inline void
getMousePos(Window* pWindow, f64* pX, f64* pY) {
    glfwGetCursorPos((GLFWwindow*)pWindow->pInstance, pX, pY);
}

////////////////////////////////
//RENDERER
////////////////////////////////

ENGINE_API inline b32 
startRenderer(Renderer* pRenderer, Window* pWindow, Camera* pCamera) {
    printf("Start()\n");
    assert(pCamera != 0);
    initCamera(pCamera, HMM_Vec3(0.f, 0.f, 6.f));
    pRenderer->pCamera = pCamera;
    pRenderer->pWindow = pWindow;
    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //glCall(glfwSwapInterval(0));
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    
    return true;
}

ENGINE_API inline b32 
stopRenderer() {
    printf("Stop()\n");
    
    return true;
}

ENGINE_API inline void 
clearRenderer() {
    glCall(glClear(GL_COLOR_BUFFER_BIT));
}

ENGINE_API inline void
fillColor(f32 red, f32 green, f32 blue) {
    glCall(glClearColor(red, green, blue, 1.f));
}

ENGINE_API inline void
swapBuffers(Window* pWindow) {
    glCall(glfwSwapBuffers((GLFWwindow*)pWindow->pInstance));
}

ENGINE_API inline void
drawBuffer(u32 offset, u32 count) {
    glCall(glDrawArrays(GL_TRIANGLES, offset, count));
}

ENGINE_API inline void 
drawBufferStrip(u32 offset, u32 count) {
    glCall(glDrawArrays(GL_TRIANGLE_STRIP, offset, count));
}

ENGINE_API inline void
drawBufferFan(u32 offset, u32 count) {
    glCall(glDrawArrays(GL_TRIANGLE_FAN, offset, count));
}

ENGINE_API inline void
drawElements(u32 count) {
    glCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
}

ENGINE_API inline void
resetModelMatrix(Renderer* pRenderer) {
    pRenderer->pCamera->model = HMM_Mat4d(1.f);
}

ENGINE_API inline void
multiplyModelMatrix(Renderer* pRenderer, hmm_mat4* pTransformation) {
    pRenderer->pCamera->model = pRenderer->pCamera->model * *pTransformation;
}

ENGINE_API inline hmm_mat4 
getModelViewProj(Renderer* pRenderer) {
    return (pRenderer->pCamera->projection * getViewMatrix(pRenderer->pCamera) * pRenderer->pCamera->model);
}

ENGINE_API inline hmm_vec3
getCameraPosition(Renderer* pRenderer) {
    return pRenderer->pCamera->position;
}

ENGINE_API inline f32 
getCameraWidth(Renderer* pRenderer) {
    return pRenderer->pCamera->halfCamWidth * 2.f;
}

ENGINE_API inline f32 
getCameraHeight(Renderer* pRenderer) {
    return pRenderer->pCamera->halfCamHeight * 2.f;
}

////////////////////////////////

//Models

////////////////////////////////

ENGINE_API void 
initMesh(Mesh* pMesh) {
    initVA(&pMesh->va);
    vaBind(pMesh->va);
    
    initVB(&pMesh->vb, pMesh->pVertices, pMesh->verticesCount*sizeof(Vertex));
    vbBind(pMesh->vb);
    
    initEB(&pMesh->eb, pMesh->pIndices, pMesh->indicesCount*sizeof(u32));
    ebBind(pMesh->eb);
    
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 3;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    vbLayoutPushFloat(&layout, 3);
    vbLayoutPushFloat(&layout, 2);
    
    vaAddBuffer(pMesh->va, pMesh->vb, &layout);
    free(layout.pElements);
    
    vaUnbind();
}

ENGINE_API void
drawMesh(Mesh* pMesh) {
    u32 diffuseNr = 1;
    u32 specularNr = 1;
    u32 normalNr = 1;
    u32 reflectionNr = 1;
    u32 texturesSize = pMesh->texturesCount*sizeof(Texture);
    meow_hash diffuseHash = MeowHash_Accelerated(0, sizeof(gpDiffuse), (void*)gpDiffuse);
    meow_hash specularHash = MeowHash_Accelerated(0, sizeof(gpSpecular), (void*)gpSpecular);
    meow_hash normalHash = MeowHash_Accelerated(0, sizeof(gpNormal),(void*)gpNormal);
    meow_hash reflectionHash = 
        MeowHash_Accelerated(0, sizeof(gpReflection),(void*)gpReflection);
    const char* pTypes[] = { gpDiffuse, gpSpecular, gpNormal, gpReflection }; 
    
    char* pName = (char*)malloc(sizeof(char) * 512);
    for (u32 i = 0; i < texturesSize; ++i) {
        glCall(glActiveTexture(GL_TEXTURE0 + i));
        s32 number = 0;
        s32 typeIndex = 0;
        meow_hash type = pMesh->pModelTextures[i]->typeHash;
        
        if (MeowHashesAreEqual(type, diffuseHash)) {
            number = diffuseNr++;
            typeIndex = 0;
        } else if (MeowHashesAreEqual(type, specularHash)) {
            number = specularNr++;
            typeIndex = 1;
        } else if (MeowHashesAreEqual(type, normalHash)) {
            number = normalNr++;
            typeIndex = 2;
        } else if (MeowHashesAreEqual(type, reflectionHash)) {
            number = reflectionNr++;
            typeIndex = 3;
        }
        
        strcpy(pName, pTypes[typeIndex]);
        s32 iEndOfString = sizeof(pTypes[typeIndex])/sizeof(char); 
        // NOTE(Marchin): I assume no model will have more that 128 textures
        assert(number < 128);
        pName[iEndOfString++] = (char)number;
        pName[iEndOfString++] = '\0';
        shaderBindID(pMesh->pMaterial->id);
        shaderSetInt(pMesh->pMaterial, pName, i);
        glCall(glBindTexture(GL_TEXTURE_2D, pMesh->pModelTextures[i]->id));
    }
    vaBind(pMesh->va);
    drawElements(pMesh->indicesCount*(sizeof(u32)));
    vaUnbind();
    glCall(glActiveTexture(GL_TEXTURE0));
}

ENGINE_API void
drawModel(Model* pModel) {
    Mesh* pMeshes = pModel->pMeshes;
    u32 meshesCount = pModel->meshesCount;
    for (u32 i = 0; i < meshesCount; ++i) {
        drawMesh(&pMeshes[i]);
    }
}

ENGINE_API u32 
textureFromFile(const char* pTextureName, const char* pModelPath) {
    size_t textureNameCount = strlen(pTextureName);
    size_t modelPathCount = strlen(pModelPath);
    char* pTexturePath = (char*)malloc(textureNameCount + modelPathCount + 1);
    memcpy(pTexturePath, pModelPath, modelPathCount);
    memcpy(&pTexturePath[modelPathCount], pTextureName, textureNameCount);
    pTexturePath[textureNameCount + modelPathCount] = '\0';
    Texture* pTexture = (Texture*)malloc(sizeof(Texture*));
    initTexture(pTexture, pTexturePath, true);
    
    glCall(glBindTexture(GL_TEXTURE_2D, pTexture->id));
    
    return pTexture->id;
}

internal void
loadMaterialsTextures(Model* pModel, Mesh* pMesh, aiMaterial* pMaterial, 
                      aiTextureType type, const char* pTypeName) {
    
    u32 texturesCount = pMaterial->GetTextureCount(type);
    u32 modelTexturesCount = pModel->texturesCount;
    //ModelTexture* pTextures = (ModelTexture*)malloc(texturesCount*sizeof(ModelTexture));
    //u32 textureIndex = 0;
    
    for (u32 i = 0; i < texturesCount; ++i) {
        b32 skip = false;
        aiString str;
        pMaterial->GetTexture(type, i, &str);
        for (u32 j = 0; j < modelTexturesCount; ++j) {
            if (strcmp(pModel->pLoadedTextures[j].pPath, str.C_Str()) == 0) {
                skip = true;
                
                *(pMesh->pModelTextures + pMesh->texturesCount + texturesCount - i - 1) = 
                    &pModel->pLoadedTextures[j];
                break;
            }
        }
        
        if (!skip) {
            ModelTexture modelTexture = {};
            modelTexture.id = textureFromFile(str.C_Str(), pModel->pPath);
            modelTexture.typeHash = 
                MeowHash_Accelerated(0, sizeof(pTypeName), (void*)pTypeName);
            strcpy(modelTexture.pPath, str.C_Str());
            pModel->pLoadedTextures[pModel->texturesCount] = modelTexture;
            *(pMesh->pModelTextures + pMesh->texturesCount + texturesCount - i - 1) = 
                &pModel->pLoadedTextures[pModel->texturesCount++];
        }
    }
}

ENGINE_API void 
setupModelVertex(aiMesh* pAiMesh, Vertex* pVertex) {
    hmm_vec3 vector;
    u32 verticesCount = pAiMesh->mNumVertices;
    
    for (u32 iVertex = 0; iVertex < verticesCount; ++iVertex) {
        vector.x = pAiMesh->mVertices[iVertex].x;
        vector.y = pAiMesh->mVertices[iVertex].y;
        vector.z = pAiMesh->mVertices[iVertex].z;
        pVertex[iVertex].pos = vector;
        
        vector.x = pAiMesh->mNormals[iVertex].x;
        vector.y = pAiMesh->mNormals[iVertex].y;
        vector.z = pAiMesh->mNormals[iVertex].z;
        pVertex[iVertex].normal = vector;
        
        if (pAiMesh->mTextureCoords[0]) {
            hmm_vec2 vec;
            vec.x = pAiMesh->mTextureCoords[0][iVertex].x;
            vec.y = pAiMesh->mTextureCoords[0][iVertex].y;
            pVertex[iVertex].uv = vec;
        } else {
            pVertex[iVertex].uv = hmm_vec2{0.0f, 0.0f};
        }
    }
}

ENGINE_API void
processMeshes(Model* pModel, const aiScene* pScene) {
    u32 meshesCount = pModel->meshesCount;
    for (u32 iMesh = 0; iMesh < meshesCount; ++iMesh) {
        Mesh* pMesh = &pModel->pMeshes[iMesh];
        aiMesh* pAiMesh = pScene->mMeshes[iMesh];
        u32 verticesCount = pAiMesh->mNumVertices;
        u32 facesCount = pAiMesh->mNumFaces;
        u32 indicesCount = 0;
        
        pMesh->pVertices = (Vertex*)pushToStack(gpMeshComponentsPool->vertices, 
                                                &gpMeshComponentsPool->verticesOffset, 
                                                MAX_VERTICES, 
                                                sizeof(Vertex), 
                                                verticesCount);
        setupModelVertex(pAiMesh, pMesh->pVertices);
        pMesh->verticesCount = verticesCount;
        
        for (u32 i = 0; i < facesCount; ++i) {
            indicesCount += pAiMesh->mFaces[i].mNumIndices;
        }
        
        pMesh->pIndices = (u32*)pushToStack(gpMeshComponentsPool->indices, 
                                            &gpMeshComponentsPool->indicesOffset, 
                                            MAX_INDICES, 
                                            sizeof(u32), 
                                            indicesCount);
        u32 iIndex = 0;
        for (u32 i = 0; i < facesCount; ++i) {
            aiFace face = pAiMesh->mFaces[i];
            u32 faceIndicesCount = face.mNumIndices;
            memcpy(&pMesh->pIndices[iIndex], face.mIndices, faceIndicesCount*sizeof(u32));
            iIndex += faceIndicesCount;
        }
        
        pMesh->indicesCount = indicesCount;
        
        u32 materialIndex = pAiMesh->mMaterialIndex;
        aiMaterial* pMaterial = pScene->mMaterials[materialIndex];
        
        u32 texturesCount = 0;
        pMesh->texturesCount = 0;
        ModelTexture*** pMeshTextures = &pMesh->pModelTextures;
        if (materialIndex >= 0) {
            texturesCount = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
            *pMeshTextures = (ModelTexture**)pushToStack(gpMeshComponentsPool->pTextures,
                                                         &gpMeshComponentsPool->texturesOffet, 
                                                         MAX_TEXTURES_POINTERS, 
                                                         sizeof(ModelTexture*), 
                                                         texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial, 
                                  aiTextureType_DIFFUSE, gpDiffuse);
            pMesh->texturesCount += texturesCount;
            
            pMeshTextures += texturesCount;
            texturesCount = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
            *pMeshTextures = (ModelTexture**)pushToStack(gpMeshComponentsPool->pTextures, 
                                                         &gpMeshComponentsPool->texturesOffet, 
                                                         MAX_TEXTURES_POINTERS, 
                                                         sizeof(ModelTexture*), 
                                                         texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_SPECULAR, gpSpecular);
            pMesh->texturesCount += texturesCount;
            
            pMeshTextures += texturesCount;
            texturesCount = pMaterial->GetTextureCount(aiTextureType_NORMALS);
            *pMeshTextures = (ModelTexture**)pushToStack(gpMeshComponentsPool->pTextures, 
                                                         &gpMeshComponentsPool->texturesOffet, 
                                                         MAX_TEXTURES_POINTERS, 
                                                         sizeof(ModelTexture*), 
                                                         texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_NORMALS, gpNormal);
            pMesh->texturesCount += texturesCount;
            
            pMeshTextures += texturesCount;
            texturesCount = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
            *pMeshTextures = (ModelTexture**)pushToStack(gpMeshComponentsPool->pTextures, 
                                                         &gpMeshComponentsPool->texturesOffet, 
                                                         MAX_TEXTURES_POINTERS, 
                                                         sizeof(ModelTexture*), 
                                                         texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_AMBIENT, gpReflection);
            pMesh->texturesCount += texturesCount;
        }
        
        pMesh->pMaterial = &pModel->material;
        
        initMesh(pMesh);
    }
}
/*
internal void
processNode(Model* pModel, aiNode* pNode, const aiScene* pScene) {
    u32 meshesCount = pNode->mNumMeshes;
    for (u32 i = 0; i < meshesCount; ++i) {
        pModel->pMeshes[i] = processMesh(pModel, pNode->mMeshes[i], pScene);
    }
    u32 nodeChildCount = pNode->mNumChildren;
    for (u32 i = 0; i < nodeChildCount; ++i) {
        processNode(pModel, pNode->mChildren[i], pScene);
    }
}
*/
ENGINE_API void 
loadModel(Model* pModel, const char* pPath) {
    Assimp::Importer importer;
    const aiScene* pScene = 
        importer.ReadFile(pPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
        printf(importer.GetErrorString());
    }
    
    size_t index;
    size_t size;
    if (findCharacter(pPath, '/', LAST, &index)) {
        size = sizeof(char)*(index + 1);
    } else {
        size = sizeof(pPath);
    }
    memcpy(pModel->pPath, pPath, size);
    pModel->pPath[size] = '\0';
    // TODO(Marchin): LOAD TEXTURES IN OTHER FUNCTION, ITS FROM MODEL, THE COPY INDICES TO MESH
    pModel->meshesCount = pScene->mNumMeshes;
    pModel->texturesCount = pScene->mNumTextures;
    pModel->pMeshes = (Mesh*)malloc(pModel->meshesCount*sizeof(Mesh));
    pModel->pLoadedTextures = 
        (ModelTexture*)malloc(pModel->texturesCount*sizeof(ModelTexture));
    //processNode(pModel, pScene->mRootNode, pScene);
    
    
    processMeshes(pModel, pScene);
}
