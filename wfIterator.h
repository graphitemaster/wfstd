#ifndef WF_STDLIB_ITERATOR_HDR
#define WF_STDLIB_ITERATOR_HDR
#include "wfStandard.h"
#include "wfTypeTraits.h"

/*
 * Class: wfIteratorTraits
 *  A template helper structure used to specify all the critical type
 *  definitions that an iterator should have.
 *
 * Parameters:
 *  T - The iterator type
 */
template <typename T>
struct wfIteratorTraits;

namespace wfPrivate {
	struct wfInputIteratorTag                                    { };
	struct wfOutputIteratorTag                                   { };
	struct wfForwardIteratorTag      : wfInputIteratorTag        { };
	struct wfBidirectionlIteratorTag : wfForwardIteratorTag      { };
	struct wfRandomAccessIteratorTag : wfBidirectionlIteratorTag { };

	template <typename T, bool>
	struct wfIteratorTraitsCore { };

	template <typename T>
	struct wfIteratorTraitsCore<T, true> {
		typedef typename T::DifferenceType   DifferenceType;
		typedef typename T::ValueType        ValueType;
		typedef typename T::Pointer          Pointer;
		typedef typename T::Reference        Reference;
		typedef typename T::IteratorCategory IteratorCategory;
	};

	template <typename T, bool>
	struct wfIteratorTraits { };

	template <typename T>
	struct wfIteratorTraits<T, true> : 
		wfIteratorTraitsCore<
			T,
			wfIsConvertible<typename T::IteratorCategory, wfInputIteratorTag>::value ||
			wfIsConvertible<typename T::IteratorCategory, wfOutputIteratorTag>::value
		>
	{ };

	template <typename T>
	struct wfIteratorCategoryTest {
	private:
		struct Sized {
			char _Ignore1;
			char _Ignore2;
		};

		template <typename U> static Sized Test(...);
		template <typename U> static char  Test(typename U::IteratorCategory* = 0);
	public:

		// this is basic SFINAE, if T has an IteratorCategory, the selected Test method
		// will return char which sizeof will == 1, otherwise it will return a "Sized"
		// structure which is sizeof == 2 (ergo this will evaluate false in such cases).
		static const bool value = (sizeof(Test<T>(0)) == 1);
	};
}

template <typename T>
struct wfIteratorTraits : 
	wfPrivate::wfIteratorTraits<
		T,
		wfPrivate::wfIteratorCategoryTest<T>::value
	>
{ };

template <typename T>
struct wfIteratorTraits<T*> {
	typedef ptrdiff_t                             DifferenceType;
	typedef typename wfRemoveConst<T>::type       ValueType;
	typedef T*                                    Pointer;
	typedef T&                                    Reference;
	typedef wfPrivate::wfRandomAccessIteratorTag  IteratorCategory;
};

namespace wfPrivate {
	template <typename T>
	void wfAdvance(
		T&                                             iterator,
		typename ::wfIteratorTraits<T>::DifferenceType difference,
		wfInputIteratorTag
	) {
		for (; difference > 0; --difference)
			++iterator;
	}

	template <typename T>
	void wfAdvance(
		T&                                             iterator,
		typename ::wfIteratorTraits<T>::DifferenceType difference,
		wfBidirectionlIteratorTag
	) {
		if (difference >= 0) {
			for (; difference > 0; --difference)
				++iterator;
		} else {
			for (; difference < 0; ++difference)
				--iterator;
		}
	}

	template <typename T>
	void wfAdvance(
		T&                                             iterator,
		typename ::wfIteratorTraits<T>::DifferenceType difference,
		wfRandomAccessIteratorTag
	){
		iterator += difference;
	}
}

template <typename T>
void wfAdvance(T &iterator, typename wfIteratorTraits<T>::DifferenceType difference) {
	wfPrivate::wfAdvance(iterator, difference, typename wfIteratorTraits<T>::IteratorCategory());
}

/*
 * Class: wfReverseIterator
 *  Iterator adaptor that describes a reverse iterator object
 *
 * Arguments:
 *  T - The type that represents the iterator to be adapbted to operate in reverse
 *
 * Remarks:
 *  Tempalte class is an iterator adaptor that describes a reverse iterator object
 *  that behaves like a random-access or bidirectional iterator, only in reverse.
 *  It enables the backward traversal of a range.
 */
template<typename T>
struct wfReverseIterator {
	/*
	 * Type: DifferenceType
	 *  A type that provides the difference between two <b>ResvereIterator</b>s reffering to elements
	 *  within the same container.
	 *
	 * Remarks:
	 *  The ReverseIterator difference type is the same as the iterator difference type.  The type is
	 *  a synonym for the iterator typename <b>wfIteratorTraits&gt<i>random-access-iterator</i>&lt::DifferenceType</b>.
	 */
	typedef typename wfIteratorTraits<T>::DifferenceType DifferenceType;
	
	/*
	 * Type: ValueType
	 *  A type that provides the underlying iterator for a <b>wfReverseIterator</b>.
	 *
	 * Remarks:
	 *  The type is a synonym for the iterator typename
	 *  *wfIteratorTraits&lt;<i>random-access-iterator</i>&gt;::ValueType.*
	 */
	typedef typename wfIteratorTraits<T>::ValueType      ValueType;
	
	/*
	 * Type: PointerType
	 *  A type that provides a pointer to an element addressed by a ReverseIterator
	 *
	 * Remarks:
	 *  The type is a synonym for the iterator trait typename
	 *  *wfIteratorTraits&lt;<i>random-access-iterator</i>&gt;::PointerType*.
	 */
	typedef typename wfIteratorTraits<T>::PointerType    PointerType;
	
	/*
	 * Type: ReferenceType
	 *  A type that provides a reference to an element addresses by a *wfReverseIterator*.
	 *
	 * Remarks:
	 *  The type is a synonym for the iterator trait typename
	 *  *wfIteratorTraits&lt;<i>random-access-iterator</i>&gt;::ReferenceType*.
	 */
	typedef typename wfIteratorTraits<T>::ReferenceType  ReferenceType;

	/*
	 * Constructor: wfReverseIterator
	 *  Constructs a default *wfReverseIterator* or a *wfReverseIterator* from
	 *  an underlying iterator.
	 *
	 * Returns:
	 *  A default *wfReverseIterator*
	 */
	wfReverseIterator() :
		m_base(T())
	{ }

	/*
	 * Constructor: wfReverseIterator
	 *  Constructs a default *wfReverseIterator* or a *wfReverseIterator* from
	 *  an underlying iterator.
	 *
	 * Parameters:
	 *	it - The iterator that is to be adapted to a *wfReverseIterator*
	 *
	 * Returns:
	 *  A default *wfReverseIterator* or a *wfReverseIterator* adapting an
	 *  underlying iterator.
	 */
	wfReverseIterator(T it) :
		m_base(it)
	{ }
	
	/*
	 * Constructor: wfReverseIterator
	 *  Constructs a default *wfReverseIterator* or a *wfReverseIterator* from
	 *  an underlying iterator.
	 *
	 * Parameters:
	 *	it - The iterator that is to be adapted to a *wfReverseIterator*.
	 *
	 * Returns:
	 *  A default *wfReverseIterator* or a *wfReverseIterator* adapting an
	 *  underlying iterator.
	 */
	template<typename U>
	wfReverseIterator(const wfReverseIterator<U> &it) :
		m_base(it.Base())
	{ }

	/*
	 * Function: Base
	 *  Recovers the underlying iterator from its *wfReverseIterator*.
	 *
	 * Returns:
	 *  The iterator underlying the *wfReverseIterator*.
	 *
	 * Remarks:
	 *  The identity that relates all reverse iterators to their underlying
	 *  iterators is.
	 * (start code)
	 * &*(wfReverseIterator(iterator)) == &*(iterator - 1)
	 * (end code)
	 *
	 *  In practice, this means that in the reversed sequence the *wfReverseIterator*
	 *  will refer to the element one position beyond (to the right of) the element that
	 *  the iterator had referred to in the original sequence.  So if an iterator addressed
	 *  the element <i>6</i> in the sequence <i>(2, 4, 6, 8)</i>, then the *wfReverseIterator*
	 *  will address the element <i>4</i> in the reversed sequence <i>(8, 6, 4, 2)</i>.
	 */
	T Base() const { return m_base; }


	/*
	 * Function: operator *
	 *  Returns the element that a *wfReverseIterator* adresses.
	 *
	 * Returns:
	 *  The value of the element addresses by the *wfReverseIterator*.
	 *
	 * Remarks:
	 *  The operator is functionally equivlant to:
	 *  (start code)
	 *  *(iterator - 1)
	 *  (end code)
	 */
	ReferenceType operator *() const {
		T temp(m_base);
		return *--temp;
	}

	/*
	 * Function: operator +
	 *  Adds an offset to an iterator and returns the new *wfReverseIterator*
	 *  addressing the inserted element at the new offset position.
	 *
	 * Parameters:
	 *  off - The offset to be added to the *wfReverseIterator*.
	 *
	 * Returns:
	 *  A *wfReverseIterator* addressing the offset element.
	 */
	wfReverseIterator operator +(DifferenceType off) const {
		return wfReverseIterator(m_base - off);
	}
	
	/*
	 * Function: operator -
	 *  Subtracts an offset from a *wfReverseIterator* and returns a
	 *  *wfReverseIterator* addressing the element at the offset position.
	 *
	 * Parameters:
	 *  off - The offset to be subtracted to the <b>wfReverseIterator</b>.
	 *
	 * Returns:
	 *  A *wfReverseIterator* addressing the offset element.
	 */
	wfReverseIterator operator -(DifferenceType off) const {
		return wfReverseIterator(m_base + off);
	}
	
	/*
	 * Function: operator +=
	 *  Adds a specified offset from a *wfReverseIterator*
	 *
	 * Parameters:
	 *  off - The offset by which to increment the iterator.
	 *
	 * Returns:
	 *  A reference to the element addresses by the *wfReverseIterator*.
	 */
	wfReverseIterator& operator +=(DifferenceType off) {
		m_base -= off;
		return *this;
	}

	/*
	 * Function: operator -=
	 *  Subtracts a specified offset from a *wfReverseIterator*
	 *
	 * Parameters:
	 *  off - The offset by which to decrement the iterator.
	 *
	 * Returns:
	 *  A reference to the element addresses by the *wfReverseIterator*.
	 */
	wfReverseIterator& operator -=(DifferenceType off) {
		m_base += off;
		return *this;
	}

	/*
	 * Function: operator ++
	 *  Increments the *wfReverseIterator* to the previous element.
	 *
	 * Returns:
	 *  The preincremented *wfReverseIterator*.
	 */
	wfReverseIterator& operator ++() { return (operator +=(1)); }
	
	/*
	 * Function: operator --
	 *  Decrements the *wfReverseIterator* to the previous element.
	 *
	 * Returns:
	 *  The predecremented *wfReverseIterator*.
	 */
	wfReverseIterator& operator --() { return (operator -=(1)); }

	/*
	 * Function: operator ++
	 *  Increments the *wfReverseIterator* to the previous element.
	 *
	 * Returns:
	 *  A copy of the incremented *wfReverseIterator*.
	 */
	wfReverseIterator operator ++(int) {
		wfReverseIterator temp(*this);
		--m_base;
		return temp;
	}
	
	/*
	 * Function: operator --
	 *  Increments the *wfReverseIterator* to the previous element.
	 *
	 * Returns:
	 *  A copy of the decremented *wfReverseIterator*.
	 */
	wfReverseIterator operator --(int) {
		wfReverseIterator temp(*this);
		++m_base;
		return temp;
	}
	

	/*
	 * Function: operator ->
	 *  Returns a pointer to the element addressed by the *wfReverseIterator*.
	 *
	 * Returns:
	 *  A pointer to the element addressed by the *wfReverseIterator*.
	 *
	 * Remarks:
	 *  The operator is functionally equivlant to:
	 *  (start code)
	 *  &**this
	 *  (end code)
	 */
	PointerType operator -> () const {
		return &(operator*());
	}

	/*
	 * Function: operator []
	 *  Returns a reference to an element offset from the element addressed by a
	 * *wfReverseIterator* by a specified number of positions.
	 *
	 * Parameters:
	 *  nth - The offset from the *wfReverseIterator* address.
	 *
	 * Returns:
	 *  The reference to the element offset.
	 *
	 * Remarks:
	 *  The operator is functionally equivlant to:
	 *  (start code)
	 *  *(*this + nth)
	 *  (end code)
	 */
	ReferenceType operator [](size_t nth) const {
		return *(m_base + nth);
	}

	//
	// Friended method for operators == and != this allows for
	// very small and nice (guranteed inlineable operators).
	// Sadly this only works in C++11
	//
	#ifdef WF_STDLIB_CPP11
		template<typename U>
		friend bool operator == (
			const wfReverseIterator   & lhs,
			const wfReverseIterator<U>& rhs
		) {
			return lhs.Base() == rhs.Base();
		}

		template<typename U>
		friend bool operator != (
			const wfReverseIterator   & lhs,
			const wfReverseIterator<U>& rhs
		) {
			return lhs.Base() != rhs.Base();
		}
	#endif

protected:
	T m_base;
};

#ifndef WF_STDLIB_CPP11
	//
	// Global operator methods if not C++ (this produces more code
	// bloat per instantiation (also more to instantiate too). We're
	// also not guranteed inlinability.)
	//
	template<typename U, typename T>
	bool operator == (
		const wfReverseIterator<T>& lhs,
		const wfReverseIterator<U>& rhs
	) { return lhs.Base() == rhs.Base(); }

	template<typename U, typename T>
	bool operator != (
		const wfReverseIterator<T>& lhs,
		const wfReverseIterator<U>& rhs
	) { return lhs.Base() != rhs.Base(); }
#endif // !WF_STDLIB_CPP11

#endif
