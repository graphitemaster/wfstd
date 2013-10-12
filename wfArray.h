#ifndef WF_STDLIB_ARRAY_HDR
#define WF_STDLIB_ARRAY_HDR
#include "wfAlgorithm.h"
#include "wfIterator.h"

/*
 * Class: wfArray
 * Describes an object that controls a sequence of length *N* of elements
 * of type *T*.  The sequence is sored as an array of *T*, contained
 * in the *wfArray<T, N>* object.
 *
 * >#include "wfArray.h" 
 *
 * Parameters:
 *  T - The type of an element.
 *  N - The number of elements.
 *
 * Remarks:
 *  The type has a default constructor *wfArray()* and a default assignment
 *  operator *operator=*, and satisfies the requirement for an aggregate.
 *  Therefor, objects of type *wfArray<T, N>* can be initialized by
 *  using an aggregate initializer.  Sadly this feature is only availble
 *  in C++11.  To enable support for this please ensure your target console
 *  compiler supports C++11, and *#define WF_STDLIB_CPP11* to enable support
 *  for aggregate initialization on *wfArray*.  Enabling support for aggregate
 *  initialization will allow for the following:
 * 
 *  (start code)
 *  wfArray<int 4> name = { 1, 2, 3 };
 *  (end code)
 */   
template<typename T, size_t N>
struct wfArray {
	/*
	 * Type: Iterator
	 *  The type of an iterator for the controlled sequence.
	 *
	 * Remarks:
	 *  The type describes an object that can serve as a random-access
	 *  iterator for the controlled sequence.
	 */   
	typedef T*                               Iterator;

	/*
	 * Type: ConstIterator
	 *  The type of a constant iterator for the controlled sequence.
	 *
	 * Remarks:
	 *  The type describes an object that can serve as a constant
	 *  random-access iterator for the controlled sequence.
	 */  
	typedef const T*                         ConstIterator;

	/*
	 * Type: ReverseIterator
	 *  The type of a reverse iterator for the controlled sequence.
	 *
	 * Remarks:
	 *  The type describes an object that can serve as a reverse iterator
	 *  for the controlled sequence.
	 */       
	typedef wfReverseIterator<Iterator>      ReverseIterator;

	/*
	 * Type: ConstReverseIterator
	 *  The type of a constant reverse iterator for the controlled sequence.
	 *
	 * Remarks:
	 *  The type describes an object that can serve as a constant reverse
	 *  iterator for the controlled sequence.
	 */  
	typedef wfReverseIterator<ConstIterator> ConstReverseIterator;

	/*
	 * Function: Begin
	 *  Designates the beginning of the controlled sequence.
	 *
	 * Remarks:
	 *  The member functions return a random-access iterator that points
	 *  at the first element of the sequence (or just beyond the end of
	 *  and empty sequence).
	 */        
	Iterator             Begin       ()       { return m_data;   }
	ConstIterator        Begin       () const { return m_data;   }

	/*
	 * Function: End
	 *  Designates the end of the controlled sequence.
	 *
	 * Remarks:
	 *  The member functions return a random-access iterator that points
	 *  beyond the end of the sequence.
	 */ 
	Iterator             End         ()       { return m_data+N; }
	ConstIterator        End         () const { return m_data+N; }


	/*
	 * Function: ReverseBegin
	 *  Designates the beginning of the reversed controlled sequence.
	 *
	 * Remarks:
	 *  The member functions return a reverse iterator that points just
	 *  beyond the end of the controlled sequence.  Hence, it designates
	 *  the beginning of the reversed sequence.
	 */        
	ReverseIterator      ReverseBegin()       { return ReverseIterator     (End());   }
	ConstReverseIterator ReverseBegin() const { return ConstReverseIterator(End());   }


	/*
	 * Function: ReverseEnd
	 *  Designates the end of the reversed controlled sequence.
	 *
	 * Remarks:
	 *  The member functions return a reverse iterator that points at the
	 *  first element of the sequence (or just beyond the end of an empty
	 *  sequence). Hence, it designates the end of the reversed sequence.
	 */        
	ReverseIterator      ReverseEnd  ()       { return ReverseIterator     (Begin()); }
	ConstReverseIterator ReverseEnd  () const { return ConstReverseIterator(Begin()); }
  
	/*
	 * Operator: operator[]
	 *  Accesses an element at a specified position.
	 *
	 * Parameters:
	 *  index - Position of element to access.
	 *
	 * Remarks:
	 *  The member functions return a reference to the element of the
	 *  controlled sequence at position *index*.  If that position is
	 *  invalid, the behaviour is undefined; unless *WF_STDLIB_DEBUG*
	 *  is defined, in which case an assertion will be invoked.
	 */  
	T& operator[](size_t index) {
#       ifdef WF_STDLIB_DEBUG
			WF_STDLIB_ASSERT(index < N);
#       endif
		return m_data[index];
	}
	const T& operator[](size_t index) const {
#       ifdef WF_STDLIB_DEBUG
			WF_STDLIB_ASSERT(index < N);
#       endif
		return m_data[index];
	}

	/*
	 * Function: At
	 *  Accesses an element at a specified position.
	 *
	 * Parameters:
	 *  index - Position of element to access.
	 *   
	 * Remarks:
	 *  The member functions return a reference to the element of the
	 *  controlled sequence at position *index*.  If that position is
	 *  invalid, the function returns a reference to the last element
	 *  of the controlled sequence.
	 */         
	T&       At(size_t index)       { return m_data[(index < N) ? index : (N - 1)]; }
	const T& At(size_t index) const { return m_data[(index < N) ? index : (N - 1)]; }

	/*
	 * Function: Front
	 *  Accesses the first element.
	 * 
	 * Remarks:
	 *  The member functions return a reference to the first element of
	 *  the controlled sequence, which must be non-empty.
	 */     
	T&       Front()       { return m_data[0];   }
	const T& Front() const { return m_data[0];   }

	/*
	 * Function: Back
	 *  Accesses the last element.
	 *
	 * Remarks:
	 *  The member functions return a reference to the last element of
	 *  the controlled sequence, which must be non-empty.
	 */ 
	T&       Back ()       { return m_data[N-1]; }
	const T& Back () const { return m_data[N-1]; }

	/*
	 * Function: Swap
	 *  Swaps the contents of two *wfArrays*
	 *
	 * Parameters:
	 *  data - Array to swap contents with.
	 *
	 * Remarks:
	 *  The member function swaps the controlled sequences between * *this*
	 *  and *data*. It perrforms a number of element assignments and
	 *  constructor calls proportional to *N*.  This is acomplished with
	 *  <wfSwapRanges>.
	 */            
	void Swap(wfArray<T, N>& data) {
		wfSwapRanges(data.Begin(), data.End(), data, Begin());
	}

	/*
	 * Function: Length
	 *  Counts the number of elements.
	 * 
	 * Remarks:
	 *  The member function returns *N*.
	 */   
	static size_t Length  () { return N;     }

	/*
	 * Function: Capacity
	 *  Counts the maximum number of elements.
	 * 
	 * Remarks:
	 *  The member function returns *N*.
	 */   
	static size_t Capacity() { return N;     }

	/*
	 * Function: Empty
	 *  Tests whether no elements are present.
	 * 
	 * Remarks:
	 *  The member function returns true only if *N == 0*.
	 */   
	static bool   Empty   () { return (N == 0); }

	/*
	 * Function: Data
	 *  Gets the address of the first element.
	 *
	 * Remarks:
	 *  The member functions return the address of the first element
	 *  in the controlled sequence.
	 */       
	const T* Data() const { return m_data; }
	T*       Data()       { return m_data; }


	// this cannot be privatized for aggregate initialization in C++11
	// we just keep this public as a result.
	T m_data[N];
};
#endif
