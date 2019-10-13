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
setupMaterial(const char* pMaterialPath, u32 type) {
	FILE* pFile = fopen(pMaterialPath, "r");
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
initMaterial(Material* pMaterial, const char* pName,
             const char* pVertexPath, const char* pFragmentPath,
             const char* pGeometryPath, 
             const char* pTessControlPath, const char* pTessEvaluationPath) {
    
	u32 vertex = 0;
	u32 fragment = 0;
	u32 geometry = 0;
	u32 tessellationControl = 0;
	u32 tessellationEvaluation = 0;
    
    strcpy(pMaterial->name, pName);
    
	vertex = setupMaterial(pVertexPath, GL_VERTEX_SHADER);
	fragment = setupMaterial(pFragmentPath, GL_FRAGMENT_SHADER);
	if (pGeometryPath != nullptr) {
		geometry = setupMaterial(pGeometryPath, GL_GEOMETRY_SHADER);
	}
	if (pTessControlPath != nullptr) {
		tessellationControl = setupMaterial(pTessControlPath, GL_TESS_CONTROL_SHADER);
	}if (pTessEvaluationPath != nullptr) {
		tessellationEvaluation = setupMaterial(pTessEvaluationPath, 
                                               GL_TESS_EVALUATION_SHADER);
	}
    
	glCall(pMaterial->id = glCreateProgram());
	glCall(glAttachShader(pMaterial->id, vertex));
	glCall(glAttachShader(pMaterial->id, fragment));
	if (pGeometryPath != 0) {
		glCall(glAttachShader(pMaterial->id, geometry));
	}
	if (pTessControlPath != 0) {
		glCall(glAttachShader(pMaterial->id, tessellationControl));
	}if (pTessEvaluationPath != 0) {
		glCall(glAttachShader(pMaterial->id, tessellationEvaluation));
	}
	glCall(glLinkProgram(pMaterial->id));
	checkShaderCompileErrors(pMaterial->id, "PROGRAM");
    
	glCall(glDeleteShader(vertex));
	glCall(glDeleteShader(fragment));
	if (pGeometryPath != 0) {
		glCall(glDeleteShader(geometry));
	}
    
    for (int iUniform = 0; iUniform < UNIFORMS_MAX; ++iUniform) {
        pMaterial->uniformLocationCache[iUniform] = -1;
    }
    
}

ENGINE_API inline void 
materialBindID(u32 materialID) {
	glCall(glUseProgram(materialID));
}

ENGINE_API s32 
getUniformLocation(Material* pMaterial, const char* pName) {
    meow_hash hash = MeowHash_Accelerated(0, sizeof(pName), (const void*)pName);
    b32 found = false;
    s32 iHash;
    s32 firstFreeSlotIndex = -1;
    for (iHash = 0; iHash < UNIFORMS_MAX; ++iHash) {
        if (MeowHashesAreEqual(pMaterial->hashLocationCache[iHash], hash)) {
            found = true;
            break;
        } else {
            if (firstFreeSlotIndex == -1 && pMaterial->uniformLocationCache[iHash] == -1) {
                firstFreeSlotIndex = iHash;
            }
        }
    }
    if (found) {
        return pMaterial->uniformLocationCache[iHash];
    } else {
        glCall(s32 location = glGetUniformLocation(pMaterial->id, pName));
		if (location == -1) {
            printf("Warning: uniform '%s' doesn't exist\n", pName);
        }
        if (firstFreeSlotIndex == -1) {
            printf("Error: You exceeded the max number of uniforms in a shader allowed\n");
        } else {
            pMaterial->hashLocationCache[firstFreeSlotIndex] = hash;
            pMaterial->uniformLocationCache[firstFreeSlotIndex] = location;
        }
        return location;
    }
}

ENGINE_API inline void 
shaderSetBool(Material* pMaterial, const char* pName, b32 value) {
	glCall(glUniform1i(getUniformLocation(pMaterial, pName), value));
}

ENGINE_API inline void
shaderSetInt(Material* pMaterial, const char* pName, s32 value) {
	glCall(glUniform1i(getUniformLocation(pMaterial, pName), value));
}

ENGINE_API inline void
shaderSetFloat(Material* pMaterial, const char* pName, f32 value) {
	glCall(glUniform1f(getUniformLocation(pMaterial, pName), value));
}

ENGINE_API inline void
shaderSetVec3(Material* pMaterial, const char* pName, hmm_vec3* pVector) {
	glCall(glUniform3fv(getUniformLocation(pMaterial, pName), 1, &pVector->x));
}

ENGINE_API inline void
shaderSetMat4(Material* pMaterial, const char* pName, hmm_mat4* pMat4) {
	glCall(glUniformMatrix4fv(getUniformLocation(pMaterial, pName), 
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
    assert(pCamera != 0);
    initCamera(pCamera, HMM_Vec3(0.f, 3.f, 20.f));
    pRenderer->pCamera = pCamera;
    pRenderer->pWindow = pWindow;
    //glCall(glEnable(GL_BLEND));
    glCall(glEnable(GL_DEPTH_TEST));
    glCullFace(GL_BACK);
    //glDepthFunc(GL_EQUAL);
    //glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    
    return true;
}

ENGINE_API inline b32 
stopRenderer() {
    return true;
}

inline void 
clearRenderer() {
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

inline void
fillColor(f32 red, f32 green, f32 blue) {
    glCall(glClearColor(red, green, blue, 1.f));
}

inline void
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

ENGINE_API inline hmm_mat4 
getModelView(Renderer* pRenderer) {
    return (getViewMatrix(pRenderer->pCamera) * pRenderer->pCamera->model);
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
