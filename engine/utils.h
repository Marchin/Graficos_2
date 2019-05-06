#ifndef UTILS_H
#define UTILS_H

enum FIRST_LAST {
    FIRST,
    LAST,
};

inline b32
findCharacter(const char* pString, char toFind, FIRST_LAST from, size_t* pIndex) {
    size_t length = strlen(pString)/sizeof(char);
    switch (from) {
        case FIRST: {
            for (size_t i = 0; i < length; ++i) {
                if (pString[i] == toFind) {
                    *pIndex = i;
                    return true;
                }
            }
        } break;
        case LAST: {
            for (size_t i = length - 1; i >= 0; --i) {
                if (pString[i] == toFind) {
                    *pIndex = i;
                    return true;
                }
            }
        } break;
    }
    return false;
}

#endif //UTILS_H