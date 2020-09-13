/**
 * @file common_define.h
 * @author meng_yu (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-06
 * 
 * @copyright imyumeng@qq.com Copyright (c) 2020
 * 
 */
#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

//#define FAILED (0)
//#define SUCCEED (1)

//#define DISABLE (0)
//#define ENABLE (1)

#define TRUE (1)
#define FALSE (0)

#ifndef NULL
#define NULL (0)
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define CONTAINER_OF(ptr, type, member) (type *)((char *)(ptr) - (char *)&((type *)0)->member)
#define TYPE_OFFSET(type, member) ((dword_t)(&(((type *)0)->member)))

#define GET_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define GET_MIN(x, y) (((x) < (y)) ? (x) : (y))

//2^n for addr and align
#if 0
#define MEM_ALIGN(addr, align) \
#if ((align / 2) * 2 != align) #error "MEM_ALIGN align not 2^n" #endif((addr + align - 1) & (align - 1))
#else
//align is 2^n
#define MEM_ALIGN(addr, align) ((addr + align - 1) & ~(dword_t)(align - 1))
#endif

#define PRINT_IF printf

#define INLINE static __inline
#define _AT_(s) __attribute__((at(s)))

#if defined(__CC_ARM)
//#define _VAL(x) #x
//#define _STR(x) _VAL(x)

//#define __PASTE2(x, y) x##y
//#define __PASTE(x, y) __PASTE2(x, y)
#endif
//#define __INT64_C_SUFFIX__ ll
//#define __INT64_C(x)  (__PASTE(x, __INT64_C_SUFFIX__))
//#define __UINT64_C(x)  (__PASTE(x ## u, __INT64_C_SUFFIX__))

#define REG_GET(n) (*(volatile dword_t *)(n))
#define MEM32_GET(n) (*(volatile dword_t *)(n))
#define MEM16_GET(n) (*(volatile word_t *)(n))
#define MEM8_GET(n) (*(volatile byte_t *)(n))

#define RAND_INIT() (srand(time(NULL)))
#define BYTE_RAND() ((byte_t)rand())
#define WORD_RAND() ((word_t)rand())
#define DWORD_RAND() ((dword_t)rand() << 16 | rand())
#define RAND_RANGE(s, e) (rand() % ((e) - (s) + 1) + (s))

#define BYTE_REG(n) (*(volatile byte_t *)(n))
#define BYTE_BIT(n) ((byte_t)1 << (n))
#define BYTE_SHR(m, n) ((byte_t)(m) >> (n))
#define BYTE_SHL(m, n) ((byte_t)(m) << (n))
#define BYTE_IF1(m, n) (((byte_t)(m)&BYTE_BIT(n)) != 0)
#define BYTE_IF0(m, n) (((byte_t)(m)&BYTE_BIT(n)) == 0)
#define BYTE_SET(m, n) ((m) |= BYTE_BIT(n))
#define BYTE_CLR(m, n) ((m) &= ~BYTE_BIT(n))
#define BYTE_BITS (8)
#define BYTE_MASK (0xFF)

#define WORD_REG(n) (*(volatile word_t *)(n))
#define WORD_BIT(n) ((word_t)1 << (n))
#define WORD_SHR(m, n) ((word_t)(m) >> (n))
#define WORD_SHL(m, n) ((word_t)(m) << (n))
#define WORD_IF1(m, n) (((word_t)(m)&WORD_BIT(n)) != 0)
#define WORD_IF0(m, n) (((word_t)(m)&WORD_BIT(n)) == 0)
#define WORD_SET(m, n) ((m) |= WORD_BIT(n))
#define WORD_CLR(m, n) ((m) &= ~WORD_BIT(n))
#define WORD_BITS (16)
#define WORD_MASK (0xFFFF)
#define WORD_BYTE0(m) ((byte_t)(m))
#define WORD_BYTE1(m) ((byte_t)((m) >> BYTE_BITS))

#define DWORD_ARY(n) ((volatile dword_t *)(n))
#define DWORD_REG(n) (*(volatile dword_t *)(n))
#define DWORD_BIT(n) ((dword_t)1 << (n))
#define DWORD_SHR(m, n) ((dword_t)(m) >> (n))
#define DWORD_SHL(m, n) ((dword_t)(m) << (n))
#define DWORD_IF1(m, n) (((dword_t)(m)&DWORD_BIT(n)) != 0)
#define DWORD_IF0(m, n) (((dword_t)(m)&DWORD_BIT(n)) == 0)
#define DWORD_SET(m, n) ((m) |= DWORD_BIT(n))
#define DWORD_CLR(m, n) ((m) &= ~DWORD_BIT(n))
#define DWORD_BITS (32)
#define DWORD_MASK (0xFFFFFFFF)
#define DWORD_BYTE0(m) ((byte_t)(m))
#define DWORD_BYTE1(m) ((byte_t)((m) >> BYTE_BITS))
#define DWORD_BYTE2(m) ((byte_t)((m) >> WORD_BITS))
#define DWORD_BYTE3(m) ((byte_t)((m) >> (WORD_BITS + BYTE_BITS)))
#define DWORD_WORD0(m) ((word_t)(m))
#define DWORD_WORD1(m) ((word_t)((m) >> WORD_BITS))

#define QWORD_REG(n) (*(volatile qword_t *)(n))
#define QWORD_BIT(n) ((qword_t)1 << (n))
#define QWORD_SHR(m, n) ((qword_t)(m) >> (n))
#define QWORD_SHL(m, n) ((qword_t)(m) << (n))
#define QWORD_IF1(m, n) (((qword_t)(m)&QWORD_BIT(n)) != 0)
#define QWORD_IF0(m, n) (((qword_t)(m)&QWORD_BIT(n)) == 0)
#define QWORD_SET(m, n) ((m) |= QWORD_BIT(n))
#define QWORD_CLR(m, n) ((m) &= ~QWORD_BIT(n))
#define QWORD_BITS (64)
#define QWORD_MASK (0xFFFFFFFFFFFFFFFF)
#define QWORD_DWORD0(m) ((dword_t)(m))
#define QWORD_DWORD1(m) ((dword_t)((m) >> DWORD_BITS))

#define QWORD_BYTE0(m) ((byte_t)(m))
#define QWORD_BYTE1(m) ((byte_t)((m) >> BYTE_BITS))
#define QWORD_BYTE2(m) ((byte_t)((m) >> WORD_BITS))
#define QWORD_BYTE3(m) ((byte_t)((m) >> (WORD_BITS + BYTE_BITS)))
#define QWORD_BYTE4(m) ((byte_t)((m) >> DWORD_BITS))
#define QWORD_BYTE5(m) ((byte_t)((m) >> (DWORD_BITS + BYTE_BITS)))
#define QWORD_BYTE6(m) ((byte_t)((m) >> (DWORD_BITS + WORD_BITS)))
#define QWORD_BYTE7(m) ((byte_t)((m) >> (DWORD_BITS + WORD_BITS + BYTE_BITS)))

#define QWORD_WORD0(m) ((word_t)(m))
#define QWORD_WORD1(m) ((word_t)((m) >> WORD_BITS))
#define QWORD_WORD2(m) ((word_t)((m) >> DWORD_BITS))
#define QWORD_WORD3(m) ((word_t)((m) >> (DWORD_BITS + WORD_BITS)))

#endif
