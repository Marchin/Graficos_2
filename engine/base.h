#ifndef BASE_H
#define BASE_H

#define internal static
#define local_persist static
#define global static

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef s8       b8;
typedef s32      b32;
typedef float    f32;
typedef double   f64;
typedef size_t memory_index;

#define Ps32 3.14159265359f

// NOTE(Marchin): if expression is false, write into 0 (invalid memory) for a
//plaftorm independent break
#if SLOW
#define Assert(expression) if(!(expression)) {__debugbreak();} 
#else
#define Assert(expression) 
#endif

#define INVALID_CODE_PATH Assert(!"INVALID_CODE_PATH");

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)
#define ArrayCount(array) (sizeof(array)/sizeof((array)[0]))

#endif //BASE_H