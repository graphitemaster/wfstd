#ifndef WF_STDLIB_SYSTEMINFO_HDR
#define WF_STDLIB_SYSTEMINFO_HDR
#include "wfStandard.h"

/*
 * File: wfSystemInfo
 *  Used to retrive system capabilities at runtime, works for the
 *  following architctures:
 * 
 *  PPC
 *  PP64
 *  i386 .. x86
 *  AMD64/x86_64
 *
 * Uses inline assembly when and where it can by making assumptions
 * of the compiler.  Currently this supports the following compilers:
 *
 *  MSVC (pretty much any version, including 64 bit versions)
 * 
 *  GCC / Or any GCC-like compiler (includes some console compilers,
 *  clang, mingw, and other variations) and newer Intel compilers like
 *  XE studio.
 *  
 *  SUNC, most of (if not all of IBMs compilers, and older Intel
 *  compiler for C, also the standard "retargetable lcc" compiler too.)
 */  

/*
 * Function: wfCPUHasRDTSC
 *  Used to determine if the running host supports read time stamp counter
 * 
 * Returns:
 *  True if the CPU supports RDTSC, otherwise false.
 */  
inline bool wfCPUHasRDTSC();

/*
 * Function: wfCPUHasALTIVEC
 *   Used to determine if the running host supports AltiVec instruction
 *   set. 
 *
 * Returns:
 *  True if the CPU supports AltiVec, otherwise false.
 */    
inline bool wfCPUHasALTIVEC();

/*
 * Function: wfCPUHasMMX
 *  Used to determine if the running host supports MMX instruction set.
 *
 * Returns:
 *  True if the CPU supports MMX, otherwise false.
 */     
inline bool wfCPUHasMMX();

/*
 * Function: wfCPUHas3DNOW
 *  Used to determine if the running host supports 3DNow instruction set.
 * Returns:
 *  True if the CPU supports 3DNow, otherwise false.
 */ 
inline bool wfCPUHas3DNOW();

/*
 * Function: wfCPUHasSSE
 *  Used to determine if the running host supports SSE instruction set.
 *
 * Returns:
 *  True if the CPU supports SSE, otherwise false.
 */     
inline bool wfCPUHasSSE();

/*
 * Function: wfCPUHasSSE2
 *  Used to determine if the running host supports SSE2 instruction set.
 *
 * Returns:
 *  True if the CPU supports SSE2, otherwise false.
 */  
inline bool wfCPUHasSSE2();

/*
 * Function: wfCPUHasSSE3
 *  Used to determine if the running host supports SSE3 instruction set.
 *
 * Returns:
 *  True if the CPU supports SSE3, otherwise false.
 */  
inline bool wfCPUHasSSE3();

/*
 * Function: wfCPUHasSSE41
 *  Used to determine if the running host supports SSE 4.1 instruction set.
 *
 * Returns:
 *  True if the CPU supports SSE 4.1, otherwise false.
 */  
inline bool wfCPUHasSSE41();

/*
 * Function: wfCPUHasSSE42
 *  Used to determine if the running host supports SSE 4.2 instruction set.
 *
 * Returns:
 *  True if the CPU supports SSE 4.2, otherwise false.
 */  
inline bool wfCPUHasSSE42();

/*
 * Function: wfCPUHasNEON
 *  Used to determine if the running host supports NEON instruction set.
 *
 * Returns:
 *  True if the CPU supports NEON, otherwise false.
 */  
inline bool wfCPUHasNEON();

/* =================================================================== */
/* =================================================================== */
/* ============= READ THE COMMENT BLOW IF YOU DARE =================== */
/* =================================================================== */
/* =================================================================== */

/*
 * This is the implementation of all the runtime/compiler time checks
 * for testing system information.  None of this matters to the user,
 * the function prototypes above are what is exposed.  Everything from
 * here on down is not exposed, and is low-level magic to be ignored by
 * virgin eyes. 
 */ 
#if (defined(__ppc__) || defined(__ppc64__))
#   include <sys/sysctl.h>
#   include <sys/signal.h>
#endif
 
#define CPUID_I386_GNUC(STORE)                                         \
    __asm__ (                                                          \
        "   pushfl                          # Get old EFLAGS       \n" \
        "   popl    %%eax                                          \n" \
        "   movl    %%eax,       %%ecx                             \n" \
        "   xorl    $0x200000,   %%eax      # ID bit flip in EFLAGS\n" \
        "   pushl   %%eax                   # Save EFLAGS on stack \n" \
        "   popfl                           # Replace EFLAGS       \n" \
        "   pushfl                          # Get the new EFLAGS   \n" \
        "   popl    %%eax                   # Store EFLAGS in EAX  \n" \
        "   xorl    %%ecx,       %%eax      # Toggle ID bit        \n" \
        "   jz      1f                      # Test processor 80486 \n" \
        "   movl    $1,          %0         # CPUID support!       \n" \
        "1:                                                        \n" \
            : "=m"(STORE)    /* store boolean back */                  \
            :                /* nothing to do here */                  \
            : "%eax", "%ecx" /* clobbers EAX & ECX */                  \
    )
#define CPUID_AMD64_GNUC(STORE)                                        \
    __asm__ (                                                          \
        "   pushfq                          # Get old EFLAGS       \n" \
        "   popq    %%rax                                          \n" \
        "   movq    %%rax,       %%rcx                             \n" \
        "   xorl    $0x200000,   %%eax      # ID bit flip in EFLAGS\n" \
        "   pushq   %%rax                   # Save EFLAGS on stack \n" \
        "   popfq                           # Replace EFLAGS       \n" \
        "   pushfq                          # Get the new EFLAGS   \n" \
        "   popq    %%rax                   # Store EFLAGS in EAX  \n" \
        "   xorl    %%ecx,       %%eax      # Toggle ID bit        \n" \
        "   jz      1f                      # Test process 80486   \n" \
        "   movl    $1,          %0         # CPUID support!       \n" \
        "1:                                                        \n" \
            : "=m"(STORE)    /* store boolean back */                  \
            :                /* nothing to do here */                  \
            : "%rax", "%rcx" /* clobbers RAX & RCX */                  \
    ) 
#define CPUID_I386_MSVC(STORE)                                         \
    do {                                                               \
        __asm popfd                         ; Get old EFLAGS           \
        __asm pop   eax                                                \
        __asm mov   ecx,        eax                                    \
        __asm xor   eax,        200000h     ; ID bit flip in EFLAGS    \
        __asm push  eax                     ; Save EFLAGS on stack     \
        __asm popfd                         ; Replace EFLAGS           \
        __asm pushfd                        ; Get the new EFLAGS       \
        __asm pop   eax                     ; Store EFLAGS in EAX      \
        __asm xor   eax,        ecx         ; Toggle ID bit            \
        __asm jz    done                    ; Test processor 80486     \
        __asm mov   STORE,      1           ; CPUID support!           \
        __asm done:                                                    \
    } while (0)
/*
 * CPUID: is supported if you're targeting amd64 with MSVC, it's even
 * supported for the GCC stuff above, but the assembly is there for
 * consistency.
 */   
#define CPUID_AMD64_MSVC(STORE)                                        \
        ((STORE) = true)

#define CPUID_I386_SUNC(STORE)                                         \
    __asm (                                                            \
        "   pushfl                          # Get old EFLAGS       \n" \
        "   popl    %eax                                           \n" \
        "   movl    %eax,        %ecx                              \n" \
        "   xorl    $0x200000,   %eax       # ID bit flip in EFLAGS\n" \
        "   pushl   %eax                    # Save EFLAGS on stack \n" \
        "   popfl                           # Replace EFLAGS       \n" \
        "   pushfl                          # Get the new EFLAGS   \n" \
        "   popl    %eax                    # Store EFLAGS in EAX  \n" \
        "   xorl    %ecx,        %eax       # Toggle ID bit        \n" \
        "   jz      1f                      # Test proessor 80486  \n" \
        "   movl    $1,          -8(%ebp)   # CPUID support!       \n" \
        "1:                                                        \n" \
    )
#define CPUID_AMD64_SUNC(STORE)                                        \
    __asm (                                                            \
        "   pushfq                          # Get old EFLAGS       \n" \
        "   popq    %rax                                           \n" \
        "   movq    %rax,        %rcx                              \n" \
        "   xorl    $0x200000,   %eax       # ID bit flip in EFLAGS\n" \
        "   pushq   %rax                    # Save EFLAGS on stack \n" \
        "   popfq                           # Replace EFLAGS       \n" \
        "   pushfq                          # Get the new EFLAGS   \n" \
        "   popq    %eax                    # Store EFLAGS in EAX  \n" \
        "   xorl    %ecx,        %eax       # Toggle ID bit        \n" \
        "   jz      1f                      # Test proessor 80486  \n" \
        "   movl    $1,          -8(%ebp)   # CPUID support!       \n" \
        "1:                                                        \n" \
    )


/*
 * CPUID macros for filling in the four components as mandated by the
 * CPUID specification.
 */
#define CPUID_LOAD_I386_GNUC(FUNC, A1, A2, A3, A4)                     \
    __asm__ __volatile__ (                                             \
        "   pushl   %%ebx                                          \n" \
        "   cpuid                                                  \n" \
        "   movl    %%ebx,       %%esi                             \n" \
        "   popl    %%ebx                                          \n" \
            :                                                          \
                "=a"(A1),                                              \
                "=S"(A2),                                              \
                "=c"(A3),                                              \
                "=d"(A4)                                               \
            :                                                          \
                "a" (FUNC)                                             \
    )
#define CPUID_LOAD_AMD64_GNUC(FUNC, A1, A2, A3, A4)                    \
   __asm__ __volatile__ (                                              \
        "   pushq   %%rbx                                          \n" \
        "   cpuid                                                  \n" \
        "   movq    %%rbx,       %%rsi                             \n" \
        "   popq    %%rbx                                          \n" \
            :                                                          \
                "=a"(A1),                                              \
                "=S"(A2),                                              \
                "=c"(A3),                                              \
                "=d"(A4)                                               \
            :                                                          \
                "a" (FUNC)                                             \
    )
#define CPUID_LOAD_I386_MSVC(FUNC, A1, A2, A3, A4)                     \
    do {                                                               \
        __asm mov   eax,         FUNC                                  \
        __asm cpuid                                                    \
        __asm mov   A1,          eax                                   \
        __asm mov   A2,          ebx                                   \
        __asm mov   A3,          ecx                                   \
        __asm mov   A4,          edx                                   \
    } while (0)

#define CPUID_LOAD_AMD64_MSVC(FUNC, A1, A2, A3, A4)                    \
    do {                                                               \
        int load[4] = {-1};                                            \
        __cpuid(load, FUNC);                                           \
        A1 = load[0];                                                  \
        A2 = load[1];                                                  \
        A3 = load[2];                                                  \
        A4 = load[3];                                                  \
    } while (0)

/*
 * Select the proper assembly path for reading CPUID lanes (which are
 * just eax,ebx,ecx,edx or the extended version for amd64 based on the
 * compiler and target.
 */   
#if defined(__GNUC__)
#   if defined(i386)
#       define CPUID_LOAD(FUNC, L)                                     \
            CPUID_LOAD_I386_GNUC(FUNC, L[0], L[1], L[2], L[3])
#   elif defined(__x86_64__)
#       define CPUID_LOAD(FUNC, L)                                     \
            CPUID_LOAD_AMD64_GNUC(FUNC, L[0], L[1], L[2], L[3])
#   endif /*! defined(i386) */
#elif defined(_MSC_VER)
#   if defined(_M_IX86)
#       define CPUID_LOAD(FUNC, L)                                     \
            CPUID_LOAD_I386_MSVC(FUNC, L[0], L[1], L[2], L[3])
#   elif defined(_M_X64)
#       define CPUID_LOAD(FUNC, L)                                     \
            CPUID_LOAD_AMD64_MSVC(FUNC, L[0], L[1], L[2], L[3])
#   endif /*! defined (_M_IX86) */
#endif /*! defined (__GNUC__) */

/*
 * Most likely just AltiVec (i.e PPC), we can safely null
 * this out.
 */
#ifndef CPUID_LOAD
#	define CPUID_LOAD(FUNC, L)
#endif

/*
 * Brute force method at determining if AltiVec is supported by issuing
 * an illegal instruction and catching the signal.  This is probably
 * the only portable method of doing this I can think of currently.  I'd
 * like to thank libmpeg2 for this trick.
 */    
#if defined(__ppc__) || defined(__PPC__)
#   ifdef __cplusplus
        extern "C" {
#   endif /*! __cplusplus */
#   include <setjmp.h>
#	include <signal.h>
    /*
     * These need to be static global inorder to cleanly handle the singal
     * I'm unsure if the ABI mandates anything outside the C spec for this
     */ 
    static jmp_buf wfPrivatewfAltiVecCheckBuffer;
    static void    wfPrivatewfAltiVecCheckIllegalInstruction(int) {
        longjmp(wfPrivatewfAltiVecCheckBuffer, 1);
    }
#   ifdef __cplusplus
    }
#   endif /*! __cplusplus */
#endif /*! defined(__ppc__) || defined(__ppc64__) */

/* Obvious reasons */
#ifdef NS
#   undef NS
#endif

/* prevent them from hittin the global scope if they don't need to */
#ifdef __cplusplus
#   define NS wfPrivate::
namespace wfPrivate {
#else
#   define NS
#endif

    enum {
        kCpuFeatureRDTSC    = 1 << 1,
        kCpuFeatureALTIVEC  = 1 << 2,
        kCpuFeatureMMX      = 1 << 3,
        kCpuFeature3DNOW    = 1 << 4,
        kCpuFeatureSSE      = 1 << 5,
        kCpuFeatureSSE2     = 1 << 6,
        kCpuFeatureSSE3     = 1 << 7,
        kCpuFeatureSSE41    = 1 << 8,
        kCpuFeatureSSE42    = 1 << 9,
		kCpuFeatureNEON     = 1 << 10
    };

    static uint32_t wfSystemInfoCPUSupport = 0xFFFFFFFF;

    bool wfSystemInfoHasCPUID() {
        int ret = false;
#       if defined(__GNUC__)
#           if defined(i386)
                CPUID_I386_GNUC(ret);
#           elif defined(__x86_64__)
                CPUID_AMD64_GNUC(ret);
#           endif /*! defined(i386) */
#       elif defined(__sun)
#           if defined(__i386)
                CPUID_I386_SUNC(ret);
#           elif defined(__amd64)
                CPUID_AMD64_SUNC(ret);
#           endif /*! defined(__i386) */
#       elif defined(_MSC_VER)
#           if defined(_M_IX86)
                CPUID_I386_MSVC(ret);
#           elif defined(_M_X64)
                CPUID_AMD64_MSVC(ret);
#           endif /*! defined(_M_IX86) */
#       endif /*! defined(__GNUC__) */
        return !!ret;
    }

    int wfSystemInfoGetFeatures() {
        int features = 0;
        int lanes[4];

        CPUID_LOAD(0, lanes);
        if (lanes[0] >= 1) {
            CPUID_LOAD(1, lanes);
            features = lanes[3];
        }

        return features;
    }

#   define IMPLEMENT_BASIC_CHECK(NAME, VAL)                            \
        bool wfSystemInfoHas##NAME(void) {                             \
            if (wfSystemInfoHasCPUID())                                \
                return !!(wfSystemInfoGetFeatures() & (VAL));          \
            return false;                                              \
        }
#   define IMPLEMENT_CPUID_CHECK(NAME, FUN1, CMP, FUN2, IDX, VAL)      \
        bool wfSystemInfoHas##NAME(void) {                             \
            if (wfSystemInfoHasCPUID()) {                              \
                int lanes[4];                                          \
                CPUID_LOAD(FUN1, lanes);                               \
                if (lanes[0] >= CMP) {                                 \
                    CPUID_LOAD(FUN2, lanes);                           \
                    return (lanes[IDX] & VAL);                         \
                }                                                      \
            }                                                          \
            return false;                                              \
        }
                

    IMPLEMENT_BASIC_CHECK(RDTSC, 0x00000010)
    IMPLEMENT_BASIC_CHECK(MMX,   0x00800000)
    IMPLEMENT_BASIC_CHECK(SSE,   0x02000000)
    IMPLEMENT_BASIC_CHECK(SSE2,  0x04000000)
    IMPLEMENT_CPUID_CHECK(3DNOW, 0x80000000, 0x80000001, 0x80000001, 3, 0x80000000)
    IMPLEMENT_CPUID_CHECK(SSE3,  0,          1,          1,          2, 0x00000001)
    IMPLEMENT_CPUID_CHECK(SSE41, 1,          1,          1,          2, 0x00080000)
    IMPLEMENT_CPUID_CHECK(SSE42, 1,          1,          1,          2, 0x00100000)

    /*
     * Lets NOT allow these into the global namespace since that can cause
     * some mild confusion.
     */ 
    #undef IMPLEMENT_BASIC_CHECK
    #undef IMPLEMENT_CPUID_CHECK

    bool wfSystemInfoHasALTIVEC(void) {
        /* runtime check */
#       if defined(__ppc__) || defined(__PPC__)
            bool   ret           = false;
            void (*handler)(int) = signal(SIGILL, &wfPrivatewfAltiVecCheckIllegalInstruction);
            if (setjmp(wfPrivatewfAltiVecCheckBuffer) == 0) {
                /* volatile to prevent compile away of */
                asm volatile (
                    "mtspr 265,         %0                           \n"
                    "vand  %%v0,        %%v0,           %%v0         \n"
                    :: "r"(-1)
                );

                /*
                 * if we made it here it means altivec _IS_ supported
                 * otherwise the illegal handler is invoked and we jump
                 * over this.
                 */
                ret = true;
            }
            signal(SIGILL, handler);
			return ret;
#       endif /*! defined(__ppc__) || defined(__ppc64__) */
        return false;
    }
	
	bool wfSystemInfoHasNEON(void) {
#		if defined(__ARM_NEON__)
			return true;
#		endif /*! defined(__ARM_NEON__) */

		return false;
	}

    uint32_t wfSystemInfoGetSupport() {
        if (wfSystemInfoCPUSupport != 0xFFFFFFFF) {
            return wfSystemInfoCPUSupport;
        }

        wfSystemInfoCPUSupport = 0;
        if (wfSystemInfoHasRDTSC())   wfSystemInfoCPUSupport |= kCpuFeatureRDTSC;
        if (wfSystemInfoHasALTIVEC()) wfSystemInfoCPUSupport |= kCpuFeatureALTIVEC;
        if (wfSystemInfoHasMMX())     wfSystemInfoCPUSupport |= kCpuFeatureMMX;
        if (wfSystemInfoHas3DNOW())   wfSystemInfoCPUSupport |= kCpuFeature3DNOW;
        if (wfSystemInfoHasSSE())     wfSystemInfoCPUSupport |= kCpuFeatureSSE;
        if (wfSystemInfoHasSSE2())    wfSystemInfoCPUSupport |= kCpuFeatureSSE2;
        if (wfSystemInfoHasSSE3())    wfSystemInfoCPUSupport |= kCpuFeatureSSE3;
        if (wfSystemInfoHasSSE41())   wfSystemInfoCPUSupport |= kCpuFeatureSSE41;
        if (wfSystemInfoHasSSE42())   wfSystemInfoCPUSupport |= kCpuFeatureSSE42;
		if (wfSystemInfoHasNEON())    wfSystemInfoCPUSupport |= kCpuFeatureNEON;

        return wfSystemInfoCPUSupport;
    }

#ifdef __cplusplus
} /* namespace wfPrivate */
#endif

inline bool wfCPUHasRDTSC()   { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureRDTSC);   }
inline bool wfCPUHasALTIVEC() { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureALTIVEC); }
inline bool wfCPUHasMMX()     { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureMMX);     }
inline bool wfCPUHas3DNOW()   { return (NS wfSystemInfoGetSupport() & NS kCpuFeature3DNOW);   }
inline bool wfCPUHasSSE()     { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureSSE);     }
inline bool wfCPUHasSSE2()    { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureSSE2);    }
inline bool wfCPUHasSSE3()    { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureSSE3);    }
inline bool wfCPUHasSSE41()   { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureSSE41);   }
inline bool wfCPUHasSSE42()   { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureSSE42);   }
inline bool wfCPUHasNEON()    { return (NS wfSystemInfoGetSupport() & NS kCpuFeatureNEON);    }

/* undef the "namespace" C/C++ agnostic macro defined earlier */
#undef NS

/*
 * Lets NOT allow these into the global namespace since that can cause
 * some mild confusion.
 */  
#undef CPUID_AMD64_GNUC
#undef CPUID_AMD64_SUNC
#undef CPUID_I386_GNUC
#undef CPUID_I386_MSVC
#undef CPUID_I386_SUNC
#undef CPUID_LOAD_AMD64_GNUC
#undef CPUID_LOAD_I386_GNUC
#undef CPUID_LOAD_I386_MSVC
#undef CPUID_LOAD

#endif /*! WF_STDLIB_SYSTEMINFO_HDR */
