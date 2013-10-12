#ifndef WF_STDLIB_ALGORITHM_HDR
#define WF_STDLIB_ALGORITHM_HDR
#include "wfIterator.h"
#include "wfTypeTraits.h"

/*
 * File: wfAlgorithm
 *  The algorithms library defines functions for a variety of purposes
 *  that operate on ranges of elements. The very odd case there exists
 *  functions that operate on individual objects.
 *
 * >#include "wfAlgorithm.h"  
 */  

/*
 * Function: wfMax
 * Compares two objects and returns the larger of the two.
 *
 * Paramaters:
 *  a - The first of the two objects being compared.
 *  b - The second of the two objects being compared.
 *
 * Returns:
 * The greater of the two objects unless neither is greater, in which
 * case it returns the first of the two objects.
 */
template <typename T>
inline T wfMax(T a, T b) { return (a < b) ? b : a; }

/*
 * Function: wfMin
 * Compares two objects and returns the lesser of the two.
 *
 * Paramaters:
 *  a - The first of the two objects being compared.
 *  b - The second of the two objects being compared.
 *
 * Returns:
 * The lesser of the two objects unless neither is greater, in which
 * case it returns the first of the two objects.
 */
template <typename T>
inline T wfMin(T a, T b) { return (b < a) ? b : a; }

/*
 * Function: wfMax
 * Compares three objects and returns the greater of the three.
 *
 * Paramaters:
 *  x1 - The first of the three objects being compared.
 *  x2 - The second of the three objects being compared.
 *  x3 - The third of the three objects being compared.
 *
 * Returns:
 * The greater of the three objects unless none are greater, in which
 * case it returns the first of the three objects.
 */
template<typename T>
inline T wfMax(T x1, T x2, T x3) { return wfMax( x1, wfMax( x2, x3 ) ); }

/*
 * Function: wfMin
 * Compares three objects and returns the lesser of the three.
 *
 * Paramaters:
 *  x1 - The first of the three objects being compared.
 *  x2 - The second of the three objects being compared.
 *  x3 - The third of the three objects being compared.
 *
 * Returns:
 * The lesser of the three objects unless none are lesser, in which
 * case it returns the first of the three objects.
 */
template<typename T>
inline T wfMin(T x1, T x2, T x3) { return wfMin( x1, wfMin( x2, x3 ) ); }

/*
 * Function: wfClamp
 * Clamps to nearest available value.
 *
 * Paramaters:
 *  a - Value to clamp
 *  b - Minimum value to clamp to.
 *  c - Maximum value to clamp to.
 *
 * Returns:
 *  The nearest available value
 *   
 * Pesudo Code: 
 * (start code)
 * if a > max then a := max
 * if a < min then a := min
 * (end code)
 */
template <typename T, typename U>
inline T wfClamp(T a, U b, U c) {
	return wfMax(T(b), wfMin(a, T(c)));
}


#ifdef WF_STDLIB_CPP11
	namespace wfPrivate {
		template<typename T> struct RemoveR      { typedef T value_type; };
		template<typename T> struct RemoveR<T&>  { typedef T value_type; };
		template<typename T> struct RemoveR<T&&> { typedef T value_type; };

		template<typename T>
		inline typename RemoveR<T>::value_type& wfRValueMove(T&& value) {
			return static_cast<typename RemoveR<T>::value_type&&>(value);
		}
	}
#   define WF_STDLIB_RVALUE_MOVE(X) (wfPrivate::wfRValueMove((X)))
#else
	/*
	 * Macro: WF_STDLIB_RVALUE_MOVE(X)
	 *  Obtains an rvalue reference to its argument when C++11 is present.
	 *  Otherwise expands to its passed argument.
	 *
	 * Remarks:
	 *  Rvalue references are otherwise only produced by temporary objects.
	 *  So library code that's passed an rvalue reference to a resource-owning
	 *  object has the option (but isn't required) to *move* the resource
	 *  out of the argument in order to run more quickly, leaving the argument with
	 *  an empty value.  The library code is required to leave a valid value
	 *  in the argument, but unless the type or function documents otherwise,
	 *  there are no other constraints on the resulting argument value.  This
	 *  means that it's generally wisest to avoid using a moved from argument
	 *  again.  If you have to use it again, be sure to re-initialize it with
	 *  a known value before doing so.
	 */     
#   define WF_STDLIB_RVALUE_MOVE(X) (X)
#endif

/*
 * Function: wfSwap
 * Exchanges the values of the elements between two types of objects,
 * assigning the contents of the first object to the second object and
 * the contents of the second to the first.
 *
 * Parameters:
 *  lhs - The first object to have its elements exchanged.
 *  rhs - The second object to have its elements exchanged
 *
 * Example:
 *  (start code)
 *  int x = 10;
 *  int y = 20;
 *
 *  wfSwap(x, y);
 *  // x now contains: 20
 *  // y now contains: 10
 *
 *  wfVector<int> a(4, x); // a: 20 20 20 20
 *  wfVector<int> b(6, y); // b: 10 10 10 10 10 10
 *
 *  wfSwap(a, b);
 *  // a now contains: 10 10 10 10 10 10
 *  // b now contains: 20 20 20 20
 *  (end code)
 *
 *  Complexity:
 *  the complexity is constant as it performs exactly one copy construction
 *  and two assigments (although each of these operations work on their own
 *  complexity).
 */
template <typename T>
inline void wfSwap(T& lhs, T& rhs) {
	T rvalue = WF_STDLIB_RVALUE_MOVE(lhs);
	lhs      = WF_STDLIB_RVALUE_MOVE(rhs);
	rhs      = WF_STDLIB_RVALUE_MOVE(rvalue);
}

/*
 * Function: wfSwapRanges
 *  Exchanges the elements of one range with the elements of another,
 *  equal sized range.
 *
 * Parameters:
 *  lhs   - A forward iterator pointing to the first position of the first range
 *          whos elements are to be exchanged.
 *  rhs   - A forward iterator pointing to one past the final position of the first
 *          range whoes elements are to be exchanged.
 *  range - A forward iterator pointing to the first position of the second range
 *          whoes elements are to be exchanged.
 *
 * Returns:
 *  A forward iterator pointing to one past the final position of the second range
 *  whoes elements are to be exchanged.
 *           
 * Remarks:
 *  The ranges referenced must be valid; all pointers must be dereferencable and
 * within each sequence the last position is reachable from the first by incrementation.
 * The second range has to be as large as the first range.
 *
 *  The complexity is linear with *lhs-rhs* swaps performed.  If elements from containers
 * of the same type are being swapped, then the *Swap* member function from that
 * container should be used, because the member function typically has constant complexity.
 *
 * Example:
 *  (start code)
 *  wfVector<int>           a(5, 10); // a: 10 10 10 10 10
 *  wfVector<int>           b(5, 33); // b: 33 33 33 33 33
 *  wfVector<int>::Iterator i;
 *
 *  wfSwapRanges(a.Begin()+1, a.End()-1, b.Begin());
 *
 *  // a now contains: 10 33 33 33 10
 *  // b now contains: 10 10 10 33 33
 *  (end code)
 *
 * Complexity:
 *  The complexity is linear as it performs as many swaps as elements are
 *  in the range *[lhs,rhs)*.  
 */
template <typename T, typename U>
U wfSwapRanges (T lhs, T rhs, U range) {
	while (lhs != rhs)
		wfSwap(*lhs++, *range++);

	return range;
}

namespace wfPrivate {
	template <bool T>
	struct wfIteratorSwap {
		template<typename U, typename V>
		static void Swap(U a, V b) {
			typename ::wfIteratorTraits<U>::ValueType t = *a;

			*a = *b;
			*b =  t;
		}
	};

	template<>
	struct wfIteratorSwap<true> {
		template<typename U, typename V>
		static void Swap(U a, V b) {
			wfSwap(*a, *b);
		}
	};
}

namespace wfPrivate {
	template <typename T, typename C>
	C wfMove(T first, T last, C result) {
		for(; first != last; ++first, ++result) {
			*result = WF_STDLIB_RVALUE_MOVE(*first);
		}
		return result;
	}

	template <typename T, typename C>
	typename wfEnableIf<
		wfIsSameType<typename wfRemoveConst<T>::type, C>::value &&
		wfIsTriviallyCopyAssignable<C>::value,
		C*
	>::type wfMove(T *first, T *last, C *result) {
		const size_t n = static_cast<size_t>(last - first);
		memmove(result, first, n * sizeof(C));
		return result + n;
	}
}

/*
 * Function: wfMove
 * Assigns the values of each elements from a source range to a destination range,
 * iterating through the source sequence of elements and assigning them new positions
 * in a forward direction.
 *
 * Parmeters:
 *	first  - An input iterator addressing the position of the first element in the
 *           source range.
 *	last   - An input iterator addressing the position that is one-past the final 
 *           element in the source range.
 *  result - An output iterator addressing the position of the first element in the
 *           destination range.
 *
 * Returns:
 *	An output iterator addressing the position that is one-past the final element
 *	in the destination range, that is, the iterator address result + (last - first).
 *
 * Remarks:
 *	The source range must be valid and there must be sufficent space at the destination
 *	to hold all the elements being copied.
 *
 *	Because the algorithm copies the source elements in order beginning with the first
 *	element, the destination range can overlap with the source range provided the last
 *	position of the source range is not contained in the destination range. <wfMove> can
 *	be used to shift elements to the left but not right, unless there is no overlap 
 *	between the source and destination ranges.
 *
 *	The <wfMove> algorithm only modifies values point to by the iterators, assigning new
 *	valyes to elements in the destination range. It cannot be used to create new 
 *	elements and cannot insert elements into an empty container directly.
 */
template <typename T, typename C, typename O>
O wfMove(T first, T last, C result) {
	return wfPrivate::wfMove(first.Base(), last.Base(), result.Base());
}

/*
 * Function: wfIteratorSwap
 * Exchanges two values referred to by a pair of specified iterators.
 *
 * Parameters:
 *  a - One of the forward iterators whos value is to be exchanged.
 *  b - the second of the forward iterator whose value is to be exchanged.
 *
 * Example:
 *  (start code)
 *  int           a[] = { 10, 20, 30, 40, 50); // a: 10 20 30 40 50
 *  wfVector<int> b(4,99);                     // b: 99 99 99 99
 *
 *  wfIteratorSwap(a, b.Begin());
 *  // a now contains: [99] 20 30 40 50
 *  // b now contains: [10] 99 99 99
 *
 *  wfIteratorSwap(a+3, b.Begin()+2);
 *  // a now contains: 99 20  30  [99]
 *  // b now conrains: 10 99 [40]  99
 *  (end code)
 *
 * Complexity:
 *  The complexity is constant as it performs exactly one copy construction
 *  and two assignments (although each of these operations work on their own
 *  complexity).              
 */
template <typename T, typename U>
inline void wfIteratorSwap(T a, T b) {
	wfPrivate::wfIteratorSwap <
		wfIsSameType <
			typename wfIteratorTraits<T>::ValueType,
			typename wfIteratorTraits<U>::ValueType
		>::value && wfIsSameType <
			typename wfIteratorTraits<T>::ReferenceType,
			typename wfIteratorTraits<T>::ValueType&
		>::value && wfIsSameType <
			typename wfIteratorTraits<U>::ReferenceType,
			typename wfIteratorTraits<U>::ValueType&
		>::value
	>::Swap(a, b);
}
#endif
