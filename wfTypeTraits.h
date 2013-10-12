#ifndef WF_STDLIB_TYPETRAITS_HDR
#define WF_STDLIB_TYPETRAITS_HDR
#include "wfNullPointer.h"
/*
 * File: wfTypeTraits
 * Defines templates that provide compile-time constants that relay
 * information about the properties of their type arguments.
 * 
 * >#include "wfTypeTraits.h"
 */

// Remove stuff
template <typename T> struct wfRemoveConst                { typedef T  type; };
template <typename T> struct wfRemoveConst<const T&>      { typedef T& type; };
template <typename T> struct wfRemoveVolatile             { typedef T  type; };
template <typename T> struct wfRemoveVolatile<volatile T> { typedef T  type; };

template <typename T> struct wfRemoveReference            { typedef T  type; };
template <typename T> struct wfRemoveReference<T&>        { typedef T  type; };
#ifdef WF_STDLIB_CPP11
template <typename T> struct wfRemoveReference<T&&>       { typedef T  type; };
#endif //!WF_STDLIB_CPP11

template <typename T> struct wfRemoveCV;
template <typename T> struct wfRemoveCV {
	typedef /* { */
		typename wfRemoveVolatile <
			typename wfRemoveConst<
				// more layers for removal can be added if C++ introduces some more qualification
				T 
			>::type
		>::type
	/* }*/ type;
};

namespace wfPrivate {
    /* Compile time integral constant */
    template <typename T, T v>
    struct wfIntegralConstant {
        typedef T                       ValueType;
        typedef wfIntegralConstant<T,v> type;

        
        static const T value = v;
    };

    typedef wfIntegralConstant<bool, true>  wfCompileTrue;
    typedef wfIntegralConstant<bool, false> wfCompileFalse;

    /*
     * This is such an annoyance and should/could be designed
     * better.
     */  
    template<typename T, T v>
    const T wfIntegralConstant<T, v>::value;
};

/*
 * Type: wfIsIntegral
 *  Tests if type is integral.
 *
 * Parameters:
 *  T - The type to query.
 *
 * Remarks:
 *  An instance of the type predicate holds true if the type T is one
 *  of the integral types: bool, char, unsigned char, signed char, short
 *  unsigned short, int, unsigned int, long, unsigned long, otherwise it
 *  holds false.
 *
 * Example:
 *  (start code)
 *      bool test1 = wfIsIntegral<int>::value;     // true
 *      bool test2 = wfIsIntegral<bool>::value;    // true
 *      bool test3 = wfIsIntegral<float>::value;   // false
 *      bool test4 = wfIsIntegral<double>::value;  // false
 *      bool test5 = wfIsIntegral<void*>::value;   // false 
 *  (end code)
 */

template <typename T> struct wfIsIntegral;

/*
 * Type: wfIsFloating
 *  Tests if type is floating point.
 *
 * Parameters:
 *  T - The type to query.
 *
 * Remarks:
 *  An instance of the type predicate holds true if the type T is one of
 *  the floating point types: float, double, long double, otherwise it
 *  hold false.
 *
 * Example:
 *  (start code)
 *      bool test1 = wfIsFloating<int>::value;    // false
 *      bool test2 = wfIsFloating<bool>::value;   // false
 *      bool test3 = wfIsFloating<float>::value;  // true
 *      bool test4 = wfIsFloating<doubke>::value; // true
 *      bool test5 = wfIsFloating<void*>::value;  // false 
 *  (end code)    
 */
template <typename T> struct wfIsFloating;

/*
 * Type: wfIsPointer
 *  Tests if the type is a pointer
 *
 * Paramaters:
 *  T - The type to query
 *
 * Remarks:
 *  An instance of the type predicate holds true if the type *T* is a
 *  pointer to *void*, a pointer to an object, or a pointer to a function,
 *  or a cv-qualified form of one of them, otherwise it holds false.
 *
 * Example:
 *  (start code) 
 *      bool test1 = wfIsPointer<int>::value;     // false
 *      bool test2 = wfIsPointer<bool>::value;    // false
 *      bool test3 = wfIsPointer<float>::value;   // false
 *      bool test4 = wfIsPointer<double>::value;  // false
 *      bool test5 = wfIsPointer<void*>::value;   // true 
 *  (end code) 
 */
template <typename T> struct wfIsPointer;

/*
 * Type: wfIsClass
  *	Tests if a type is a class type
 *
 * Parameters:
 *	T - The type to query
 */
template <typename T>
struct wfIsClass;

/*
 * Type: wfIsSameType
 *  Tests if two types are the same.
 *
 * Parameters:
 *  T - The first type to query.
 *  U - The second type to query.
 *
 * Remarks:
 *  An instance of the type predicate holes true if the types T and U
 *  are the same type, otherwise it holds false.
 */      
template <typename T, typename U> struct wfIsSameType;

// specializations
namespace wfPrivate {
	template <typename T>               struct wfIsIntegral                           : wfCompileFalse { };
	template<>                          struct wfIsIntegral<bool>                     : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<char>                     : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<unsigned char>            : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<signed char>              : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<short>                    : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<unsigned short>           : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<int>                      : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<unsigned int>             : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<long>                     : wfCompileTrue  { };
	template<>                          struct wfIsIntegral<unsigned long>            : wfCompileTrue  { };

	template<typename T>                struct wfIsFloating                           : wfCompileFalse { };
	template<>                          struct wfIsFloating<float>                    : wfCompileTrue  { };
	template<>                          struct wfIsFloating<double>                   : wfCompileTrue  { };
	template<>                          struct wfIsFloating<long double>              : wfCompileTrue  { };

	template<typename T>                struct wfIsPointer                            : wfCompileFalse { };
	template<typename T>                struct wfIsPointer<T*>                        : wfCompileTrue  { };

	template<typename T>                struct wfIsReference                          : wfCompileFalse { };
	template<typename T>                struct wfIsReference<T&>                      : wfCompileTrue  { };
#	ifdef WF_STDLIB_CPP11
	template <typename T>               struct wfIsReference<T&&>                     : wfCompileTrue  { };
#	endif /*! WF_SRDLIB_CPP11 */

	template <typename T>               struct wfIsMemberPointer                      : wfCompileFalse { };
	template <typename T, typename C>   struct wfIsMemberPointer<T C::*>              : wfCompileTrue  { };

	template <typename T>               struct wfIsNullPointerType                    : wfCompileFalse { };
	template <>                         struct wfIsNullPointerType<wfNullPointerType> : wfCompileTrue  { };

	template <typename T>               struct wfIsVoid                               : wfCompileFalse { };
	template <>                         struct wfIsVoid<void>                         : wfCompileTrue  { };

	template <typename T>               struct wfIsArray                              : wfCompileFalse { };
	template <typename T>               struct wfIsArray<T[]>                         : wfCompileTrue  { };
	template <typename T, size_t E>     struct wfIsArray<T[E]>                        : wfCompileTrue  { };

	// twice because dependent name lookup
	struct ClassTestTwo    { char _Ignore1, _Ignore2; };
	struct FunctionTestTwo { char _Ignore1, _Ignore2; };

	template <typename T>               char             wfIsClassTest(int T::*);
	template <typename T>               ClassTestTwo     wfIsClassTest(...);

	template <typename T>               char             wfIsFunctionTest(T*);
	template <typename T>               FunctionTestTwo  wfIsFunctionTest(...);
	template <typename T>               T&               wfIsFunctionSource();

	//
	// TODO: wfIsUnion test for C++ stdlib conformance
	// otherwise this is slightly 'incorrect'
	//
	template <
		typename T,
		bool = wfIsClass<T>::value     ||
		       wfIsVoid<T>::value      ||
			   wfIsReference<T>::value ||
			   wfIsSameType<T, wfNullPointerType>::value
	>
	struct wfIsFunction :
		wfIntegralConstant <
			bool,
			sizeof(wfIsFunctionTest<T>(wfIsFunctionSource<T>())) == 1
		>
	{ };

	// false if true .. eh
	template <typename T>
	struct wfIsFunction<T, true> : wfCompileFalse {
		// TODO: extend with type of function typedefs
		// so we can do:
		// wfIsFunction<T>::type (on success) which is like
		// wfEnableIf (but for functions)
	};
}

template <typename T> struct wfIsIntegral        : wfPrivate::wfIsIntegral        <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsFloating        : wfPrivate::wfIsFloating        <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsPointer         : wfPrivate::wfIsPointer         <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsReference       : wfPrivate::wfIsReference       <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsMemberPointer   : wfPrivate::wfIsMemberPointer   <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsNullPointerType : wfPrivate::wfIsNullPointerType <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsVoid            : wfPrivate::wfIsVoid            <typename wfRemoveCV<T>::type> { };
template <typename T> struct wfIsArray           : wfPrivate::wfIsArray           <typename wfRemoveCV<T>::type> { };

template <typename T>
struct wfIsClass :
	wfPrivate::wfIntegralConstant <
		bool,
		sizeof(wfPrivate::wfIsClassTest<T>(0)) == 1
	>
{ };

/*
 * Type: wfIsFunction
  *	Tests if a type is a function
 *
 * Parameters:
 *	T - The type to query
 */
template <typename T>
struct wfIsFunction;

template <typename T>
struct wfIsFunction : wfPrivate::wfIsFunction<T> { };

/*
 * Type: wfRemoveAllExtents
 *	Removes all extents from a type.
 *
 * Parameters:
 *	T - The type to remove extents from
 */
template <typename T>           struct wfRemoveAllExtents       { typedef T type; };
template <typename T>           struct wfRemoveAllExtents<T[]>  { typedef T type; };
template <typename T, size_t E> struct wfRemoveAllExtents<T[E]> { typedef T type; };

/*
 * Type: wfIsArithmetic
 *	Tests if type is arithmetic.
 *
 * Parameters:
 *	T - The type to query.
 *
 * Remarks:
 *	An instance of the type predicate holds true of the type *T* is an arithmetic
 *	type, that is, an integral type or a floating point type, or a cv-qualified
 *	form of one of them, otherwise it holds false.
 */
template <typename T> struct wfIsArithmetic;
template <typename T> struct wfIsArithmetic:
	wfPrivate::wfIntegralConstant<
		bool,
		// cv-q-free
		wfIsIntegral<T>::value ||
		wfIsFloating<T>::value
	>
{ };

/*
 * Type: wfIsScalar
 *	Tests if a type is scalar
 *
 * Parameters:
 *	T - The type to query
 *
 * Remarks:
 *	An instance of the type predicate holds true if the type *T* is an integral
 *	type, floating point type, pointer type, or a pointer to member type, or a
 *	cv-qualified form of one of them, otherwise holds false.
 *
 * Todo:
 *	Currently this doesn't support the test for enumerators, which means it's
 *	unaligned with the C++ standard.
 */
template <typename T> struct wfIsScalar;
template <typename T> struct wfIsScalar:
	wfPrivate::wfIntegralConstant<
		bool,
		wfIsArithmetic<T>::value      ||
		wfIsMemberPointer<T>::value   ||
		wfIsPointer<T>::value         ||
		wfIsNullPointerType<T>::value
	>
{ };

// specialization for wfNullPointerType
template <>
struct wfIsScalar<wfNullPointerType> :
	wfPrivate::wfCompileTrue
{ };

/*
 * Type: wfIsTriviallyAssignable
 *	Tests if a type is trivially assignable to another
 *
 * Parameters:
 *	T - The type to assign to
 *	U - The type to query (is assignable to T)
 *
 * Remarks:
 *	An instance of the type predicate holds true if the type *U* is trivially
 *	assignable to *T*.
 */
template <typename T, typename C> struct wfIsTriviallyAssignable;
template <typename T, typename C> struct wfIsTriviallyAssignable               : wfPrivate::wfCompileFalse { };
template <typename T>             struct wfIsTriviallyAssignable<T&, T>        : wfPrivate::wfIntegralConstant<bool, wfIsScalar<T>::value> { };
template <typename T>             struct wfIsTriviallyAssignable<T&, T&>       : wfPrivate::wfIntegralConstant<bool, wfIsScalar<T>::value> { };
template <typename T>             struct wfIsTriviallyAssignable<T&, const T&> : wfPrivate::wfIntegralConstant<bool, wfIsScalar<T>::value> { };
#ifdef WF_STDLIB_CPP11
template <typename T>             struct wfIsTriviallyAssignable<T&, T&&>      : wfPrivate::wfIntegralConstant<bool, wfIsScalar<T>::value> { };
#endif

template <typename T> struct wfAddLvalueReference                      { typedef T&                  type;}; // standard base lvalue add
template <typename T> struct wfAddLvalueReference<T&>                  { typedef T&                  type;}; // needed for bad compilers
template <>           struct wfAddLvalueReference<void>                { typedef void                type;}; // the rest are voids
template <>           struct wfAddLvalueReference<const void>          { typedef const void          type;}; // since void cannot have lvalue
template <>           struct wfAddLvalueReference<volatile void>       { typedef volatile void       type;}; // no matter the type of void
template <>           struct wfAddLvalueReference<const volatile void> { typedef const volatile void type;}; // they're simply not lvalues

/*
 * Type: wfIsTriviallyCopyAssignable
  *	Tests if a type is trivially copy assignable
 *
 * Parameters:
 *	T - The type to query
 */
template <typename T> struct wfIsTriviallyCopyAssignable;
template <typename T> struct wfIsTriviallyCopyAssignable:
	wfIsTriviallyAssignable<
		typename wfAddLvalueReference<T>::type,
		const typename wfAddLvalueReference<T>::type
	>
{ };

/*
 * Type: wfEnableIf
 *	Makes an instance of type or an undefined type.
 *
 * Parameters:
 *	B - The value that determines the resulting type.
 *	T - The type to evaluate and modify.
 *
 * Remarks:
 *	An instance of this type modifier holds a modified-type that is
 *	either *T* if *B* is true, or undefined if *B* is false.
 */
template <bool B, typename T = void> struct wfEnableIf          { };
template <typename T>                struct wfEnableIf<true, T> { typedef T type; };

//
// up to now all these specializations where implemented to implement
// our own isPOD.  Now this isn't the _same_ as what the C++ standard
// defines as plain-old-data, since in C++ user-defined structure are
// still considered POD so long as they don't contain any constructors
// and it's members don't either.  There is no sane way to test this
// in C++ without support from the compiler.
//
// This just assumes anything that is an integral/float or pointer is
// POD, which is fine for what we actually use it for.
//
template<typename T> struct wfIsPOD : wfPrivate::wfIntegralConstant
    <bool, (wfIsIntegral<T>::value ||
            wfIsFloating<T>::value ||
            wfIsPointer <T>::value)>
{ };

template <typename T, typename U> struct wfIsSameType      : wfPrivate::wfCompileFalse { };
template <typename T>             struct wfIsSameType<T,T> : wfPrivate::wfCompileTrue  { };

// all this for a wfIsConvertible<T, T>::value
// shoot me now
namespace wfPrivate {
	struct Sized { char _Ignore1, _Ignore2; };

	template <typename T> char                                 wfIsConvertibleTest(T);
	template <typename T> Sized                                wfIsConvertibleTest(...);
	template <typename T> typename wfRemoveReference<T>::type &wfIsConvertibleSource();

	// test variants outside types (arrays, functions, voids)
	template <
		typename T,
		bool     TestType_Array    = ::wfIsArray<T>::value,
		bool     TestType_Function = ::wfIsFunction<T>::value,
		bool     TestType_Void     = ::wfIsVoid<T>::value
	>
	struct wfIsConvertibleTestVariants : wfIntegralConstant<int, 0> { };

	template <typename T> struct wfIsConvertibleTestVariants<T, true,  false, false> : wfIntegralConstant<int, 1> { };
	template <typename T> struct wfIsConvertibleTestVariants<T, false, true,  false> : wfIntegralConstant<int, 2> { };
	template <typename T> struct wfIsConvertibleTestVariants<T, false, false, true>  : wfIntegralConstant<int, 3> { };

	template <
		typename T,
		unsigned = wfIsConvertibleTestVariants<typename wfRemoveReference<T>::type>::value
	>
	struct wfIsConvertibleCheck {
		static const size_t size = 0;
	};

	template <typename T>
	struct wfIsConvertibleCheck<T, 0> {
		static const size_t size = sizeof(T);
	};

	template <
		typename T1,
		typename T2,
		unsigned I1 = wfIsConvertibleTestVariants<T1>::value,
		unsigned I2 = wfIsConvertibleTestVariants<T2>::value
	>
	struct wfIsConvertible :
		public wfIntegralConstant<
			bool,
			sizeof(wfIsConvertibleTest<T2>(wfIsConvertibleSource<T1>())) == 1
		>
	{ };

	// specializations
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2,                  1, 0> : wfCompileFalse { };
	template <typename T1>              struct wfIsConvertible<T1, const T1&,           1, 0> : wfCompileTrue  { };

#	ifdef WF_STDLIB_CPP11
	template <typename T1>              struct wfIsConvertible<T1, T1&&,                1, 0> : wfCompileTrue  { };
	template <typename T1>              struct wfIsConvertible<T1, const T1&&,          1, 0> : wfCompileTrue  { };
	template <typename T1>              struct wfIsConvertible<T1, volatile T1&&,       1, 0> : wfCompileTrue  { };
	template <typename T1>              struct wfIsConvertible<T1, const volatile T1&&, 1, 0> : wfCompileTrue  { };
#	endif

	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2*, 1, 0> :
		wfIntegralConstant<bool, wfIsConvertible<typename wfRemoveAllExtents<T1>::type*, T2*>::value>
	{ };
	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2* const, 1, 0> : 
		wfIntegralConstant<bool, wfIsConvertible<typename wfRemoveAllExtents<T1>::type*, T2* const>::value>
	{ };
	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2* volatile, 1, 0> :
		wfIntegralConstant<bool, wfIsConvertible<typename wfRemoveAllExtents<T1>::type*, T2* volatile>::value>
	{ };
	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2* const volatile, 1, 0> :
		wfIntegralConstant<bool, wfIsConvertible<typename wfRemoveAllExtents<T1>::type*, T2* const volatile>::value>
	{ };


	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2, 2, 0> : wfCompileFalse { };

	// others
#	ifdef WF_STDLIB_CPP11
	template <typename T> struct wfIsConvertible<T, T&&, 2, 0> : wfCompileTrue { };
#	endif

	template <typename T> struct wfIsConvertible<T, T*,                2, 0> : wfCompileTrue { };
	template <typename T> struct wfIsConvertible<T, T* const,          2, 0> : wfCompileTrue { };
	template <typename T> struct wfIsConvertible<T, T* volatile,       2, 0> : wfCompileTrue { };
	template <typename T> struct wfIsConvertible<T, T* const volatile, 2, 0> : wfCompileTrue { };

	// and one last class of them
	template <typename T1, typename T2>
	struct wfIsConvertible<T1, T2, 3, 0> : wfCompileFalse { };

	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 0, 1> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 1, 1> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 2, 1> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 3, 1> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 0, 2> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 1, 2> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 2, 2> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 3, 2> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 0, 3> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 1, 3> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 2, 3> : wfCompileFalse { };
	template <typename T1, typename T2> struct wfIsConvertible<T1, T2, 3, 3> : wfCompileTrue  { }; // only valid one
}

template <typename T1, typename T2>
struct wfIsConvertible;

template <typename T1, typename T2>
struct wfIsConvertible : wfPrivate::wfIsConvertible<T1, T2> {
private:
	// tests
	static const size_t not_convertible_1 = wfPrivate::wfIsConvertibleCheck<T1>::size;
	static const size_t not_convertible_2 = wfPrivate::wfIsConvertibleCheck<T2>::size;
};

#endif