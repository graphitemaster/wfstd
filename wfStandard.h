#ifndef WF_STDLIB_STANDARD_HDR
#define WF_STDLIB_STANDARD_HDR

/*
 * File: wfStandard
 *  Standard library support header
 *
 * Remarks:
 *  Includes some of the required headers utilized by the library into
 *  scope. While testing for C++11 compiler support and enabiling library
 *  features if present.  This is an internal header.  Which is not to be
 *  included directly.
 */
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include <ctype.h>
#define WF_PRAGMA(X) __pragma(X)

#ifdef WF_PC
#   define snprintf sprintf_s
#endif

/*
 * Macro: WF_FUNCTION
 * This macro implements C99s __func__ keyword, if certian conditions
 * are met, otherwise it expands to nothing.  This is useful for asserts
 */
#ifdef _MSC_VER
#   define WF_FUNCTION __FUNCTION__
#elif __STDC_VERSION >= 19901L
	// keyword in C99
#   define WF_FUNCTION __func__
#else
#   define WF_FUNCTION
#endif

/*
 * Macro: WF_RESTRICT
 * This macro implements C99s restrict keyword if certain conditions
 * are met, otherwise it expands to nothing.
 */
#if __STDC_VERSION__ >= 19901L
	// keyword in C99
#   define WF_RESTRICT restrict
#else
#   define WF_RESTRICT
#endif

/*
 * Macro: WF_INITIALIZER_THIS
 * By using this macro you agree you know why the warning exists!
 */
#ifdef _MSC_VER
#   define WF_INITIALIZER_THIS              \
		__pragma (warning (push))           \
		__pragma (warning (disable : 4355)) \
		this                                \
		__pragma (warning (pop))
#else
#   define WF_INITIALIZER_THIS this
#endif

/*
 * Macro: WF_STDLIB_CPP11
 * A macro which is defined if C++11 support has been detected
 */
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#   define WF_STDLIB_CPP11
#endif

/*
 * Macro: WF_ALIGN
 * A macro to precisely control the alignment of user-defined data.
 * (for example, static allocations or automatic data in a function).
 */
#define WF_ALIGN(X) __declspec(align(X))

#if __cplusplus == 201103L
#   define WF_STDLIB_CPP11
#endif


/*
 * Macro: isinf
 *  Test for infinity
 *
 * Returns:
 *  Returns a non-zero value if and only if it's argument has
 *  an infinite value.
 *
 * Remarks:
 *  determines weather its argument value is an infinity (positive or
 *  negative). First, an argument represented in a format wider than its
 *  semantic type is converted to its semantic type.  Then determination
 *  is based on the type of the argument.
 */
#if defined(WF_PC)
#   ifdef isinf
#       undef isinf
#   endif
#   define isinf(X) (!_finite(X))
#endif

/*
 * Macro: WF_UNUSED
 *  Used to mark an unused variable to supress warnings.
 */
#ifndef WF_UNUSED
#   ifdef WF_PC
#       define WF_UNUSED(X) {(void)(X);}
#   else
#       define WF_UNUSED(X) {(void)sizeof(X); (void)sizeof(X);}
#   endif
#endif

/*
 * Macro: REQUIRE_SEMICOLON
 *  Used to make macros that require a *:* (semicolon) for their use
 *  simply expands to (start code)do {} while (false)(end code)
 */
#define REQUIRE_SEMICOLON do {} while (0)

#define WF_ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
#if defined _MSC_VER
#   define WF_THREADLOCAL __declspec( thread )
#elif defined SPU
#   define WF_THREADLOCAL
#else
#   define WF_THREADLOCAL __thread
#endif

/* types */
#ifndef _MSC_VER
#   include <stdint.h>
#else
	typedef signed   __int8  int8_t;
	typedef unsigned __int8  uint8_t;
	typedef signed   __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef signed   __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef signed   __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#endif

/*
 * Types:
 * s8  - Signed 8-bit integer.
 * u8  - Unsigned 8-bit integer.
 * s16 - Signed 16-bit integer.
 * u16 - Unsigned 16-bit integer.
 * s32 - Signed 32-bit integer.
 * u32 - Unsigned 32-bit integer.
 * s64 - Signed 64-bit integer.
 * u64 - Unsigned 64-bit integer.
 */
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int64_t  s64;
typedef uint64_t u64;
typedef u16      f16;

/*
 * Types:
 * f32 - 32-bit floating point type.
 * f64 - 64-bit floating point type.
 */
typedef float    f32;
typedef double   f64;

/*
 * Function: wfSwapEndian
 * Performs endianess swap on data
 *
 * Paramaters:
 *  data - The data to be swapped (as a reference)
 *
 * Remarks:
 *  There exists template specializations for the following
 *  types:
 *  - s16
 *  - u16
 *  - s32
 *  - u32
 *  - s64
 *  - u64
 *
 *  If no template specialization exists for the type you're
 *  operating. A compiler error will occur. Opposed to providing
 *  overrides for function arguments (with different types) templates
 *  are used for wfSwapEndian to relax C++s implicit type conversions.
 */
template <typename T>
inline void wfSwapEndian(T&);

template<>
inline void wfSwapEndian<u16>(u16& data) {
	data = ((data & 0xFF00) >> 8) |
		   ((data & 0x00FF) << 8);
}
template<>
inline void wfSwapEndian<u32>(u32& data) {
	data = ((data & 0xFF000000) >> 24) |
		   ((data & 0x00FF0000) >> 8 ) |
		   ((data & 0x0000FF00) << 8 ) |
		   ((data & 0x000000FF) << 24);
}
template<>
inline void wfSwapEndian<u64>(u64& data) {
	data =((data >> 56)) |
		  ((data << 40) & 0x00FF000000000000LL) |
		  ((data << 24) & 0x0000FF0000000000LL) |
		  ((data << 8 ) & 0x000000FF00000000LL) |
		  ((data >> 8 ) & 0x00000000FF000000LL) |
		  ((data >> 24) & 0x0000000000FF0000LL) |
		  ((data >> 40) & 0x000000000000FF00LL) |
		  ((data << 56));
}

template<> inline void wfSwapEndian<s16>(s16& data) { wfSwapEndian<u16>(reinterpret_cast<u16&>(data)); }
template<> inline void wfSwapEndian<s32>(s32& data) { wfSwapEndian<u32>(reinterpret_cast<u32&>(data)); }
template<> inline void wfSwapEndian<s64>(s64& data) { wfSwapEndian<u64>(reinterpret_cast<u64&>(data)); }
template<> inline void wfSwapEndian<f32>(f32& data) { wfSwapEndian<u32>(reinterpret_cast<u32&>(data)); }
template<> inline void wfSwapEndian<f64>(f64& data) { wfSwapEndian<u64>(reinterpret_cast<u64&>(data)); }

/*
 * Function wfIsnan
 *  Tests if a floating-point value is NaN.
 *
 * Returns:
 *  true if NaN, otherwise false.
 *
 * Remarks:
 *  Implemented with template argument deduction opposed to overloaded
 *  arguments to relax C++s implicit type conversion.
 */
template <typename T> inline bool wfIsnan(T);
template <>           inline bool wfIsnan(f32         v) { return (v!=(v/1.0f)); }
template <>           inline bool wfIsnan(double      v) { return (v!=(v/1.0f)); }
template <>           inline bool wfIsnan(long double v) { return (v!=(v/1.0f)); }

/*
 * Function: wfSign
 *  Get the sign component of a value
 *
 * Returns:
 *  Returns the sign component of the value (if negative -1, otherwise 1)
 */
template <typename T> inline T wfSign(T value) {
	return (value >=0) ? static_cast<T>(1) : static_cast<T>(-1);
}

/*
 * Function: wfAbs
 *  Calculate absolute value.
 *
 * Parameters:
 *  n - Integer value.
 *
 * Returns:
 *  The <wfAbs> function returns the absolute value of its parameter.
 *
 * Remarks:
 *  Implemented as a template (not function overloads).  There exists
 *  unsigned template specializations to prevent useless comparisons.
 */
template <typename T> inline T wfAbs(T n) {
	return static_cast<T>(n >= 0 ? n : -n);
}

/*
 * Function: wfSaturate
 *  Clamps a value between 0 and 1
 *
 * Parameters:
 *  n - floating point value.
 */
template <typename T> inline T wfSaturate(T n) {
	return wfClamp( n, (T)0, (T)1 );
}

/* specializations for unsigned to prevent useless computation */
template<> inline unsigned char          wfAbs<unsigned char>         (unsigned char          n) { return n; }
template<> inline unsigned short int     wfAbs<unsigned short int>    (unsigned short int     n) { return n; }
template<> inline unsigned int           wfAbs<unsigned int>          (unsigned int           n) { return n; }
template<> inline unsigned long int      wfAbs<unsigned long int>     (unsigned long int      n) { return n; }
template<> inline unsigned long long int wfAbs<unsigned long long int>(unsigned long long int n) { return n; }

/*
 * Miniture implementation of wfHeap for standalone tests within this
 * library.
 */
struct wfHeap {
	static void *Alloc(size_t bytes) { return malloc(bytes); }
	static void  Free (void *ptr)    { return free  (ptr);   }
};
static wfHeap g_miscHeap;

namespace wfPrivate {
	template <typename T>
	struct wfHeapContainerManager {
		wfHeapContainerManager(wfHeap* heap) :
			m_heap(heap)
		{ }

		template <typename U>
		U *thisAllocateMemory(const size_t bytes) {
			return reinterpret_cast<U*>(m_heap->Alloc(bytes));
		}

		T *thisAllocateObject(const size_t items) {
			return reinterpret_cast<T*>(m_heap->Alloc(items * sizeof(T)));
		}
	private:
		wfHeap *m_heap;
	};
}

#endif // !WF_STDLIB_STANDARD_HDR
