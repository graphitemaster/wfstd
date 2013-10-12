#ifndef WF_STDLIB_VECTOR_HDR
#define WF_STDLIB_VECTOR_HDR
#include "wfAlgorithm.h"    // wfMax
#include "wfTypeTraits.h"   // wfIsPOD
#include "wfIterator.h"     // wfReverseIterator
#include "wfNullPointer.h"  // wfNullPointer
#include "wfSorter.h"       // wfSorter
#include "wfArray.h"

namespace wfPrivate {
	enum {
		wfVectorMinimumSize = 8
	};
};

/*
 * Struct: wfVector
 *  wfVector is a container that holds a list of elements simillarily to
 *  an array, but it's generic, can easily be manipulated (resized, appended
 *  etc.) and its memory is automatically managed.
 */   
template <typename T>
struct wfVector : public wfPrivate::wfHeapContainerManager<T> {
	/*
	 * Type: Iterator
	 *  A type that provides a random-access iterator that can read or
	 *  modify any element in a <wfVector>.
	 */  
	typedef T*                               Iterator;

	/*
	 * Type: ConstIterator
	 *  A type that provides a random-aceess iterator that can read a
	 *  *const* element in a <wfVector>.
	 */  
	typedef const T*                         ConstIterator;

	/*
	 * Type: ReverseIterator
	 *  A type that provides a random-access iteratpr that can read or
	 *  modify any element in a reversed <wfVector>.
	 */  
	typedef wfReverseIterator<Iterator>      ReverseIterator;

	/*
	 * Type: ConstReverseIterator
	 *  A type that provides a random-access iterator that can read a
	 * *const* element in a reversed <wfVector>.
	 */  
	typedef wfReverseIterator<ConstIterator> ConstReverseIterator;


	/*
	 * Function: Begin
	 *  Returns a random-access iterator to the first element in the
	 *  vector.
	 *
	 * Returns:
	 *  A random-access iterator addressing the first element in the
	 *  <wfVector> or to the location succeeding an empty <wfVector>.
	 *  You should always compare the value returned with <End> to
	 *  ensure it's valid.
	 */
	Iterator             Begin()              { return m_buffer; }
	ConstIterator        Begin()        const { return m_buffer; }

	/*
	 * Function: ReverseBegin
	 *  Returns an iterator to the first element in a revered vector.
	 *
	 * Returns:
	 *  A reverse random-access iterator addressing the first element in
	 *  a reverded vector or addressing what had been the last element in
	 *  the unreversed vector.
	 */
	ReverseIterator      ReverseBegin()       { return ReverseIterator     (End()); }
	ConstReverseIterator ReverseBegin() const { return ConstReverseIterator(End()); }

	/*
	 * Function: End
	 * Returns the past-the-end iterator.
	 *
	 * Returns:
	 *  The past-the-end for the vector.  If the vector is empty, End()
	 *  will equal Begin().
	 */
	Iterator             End()                { return m_buffer + m_length; }
	ConstIterator        End()          const { return m_buffer + m_length; }

	/*
	 * Function: ReverseEnd
	 *  Returns an iterator that addresses the location succeeding the last
	 *  element in a reversed vector.
	 *
	 * Returns:
	 *  A reverse random-access iterator that addresses the location
	 *  succeeding the last element in a reversed vector (the location
	 *  that had preceded the first element in the unreversed vector).
	 *
	 * Remarks:
	 *  ReverseEnd is used with a reversed vector just as End is used with
	 *  ReverseEnd can be used to test wether a reverse iterator has reach
	 *  the end of its vector. The value returned by ReverseEnd should not
	 *  be dereferenced.
	 */  
	ReverseIterator      ReverseEnd()         { return ReverseIterator     (Begin()); }
	ConstReverseIterator ReverseEnd()   const { return ConstReverseIterator(Begin()); }

	/*
	 * Constructor: wfVector
	 *  Initialize an empty <wfVector>
	 */
	wfVector(wfHeap *heap = wfMemory::s_miscHeap) :
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_buffer                            (wfNullPointer),
		m_length                            (0),
		m_capacity                          (0)
	{ }

	/*
	 * Constructor: wfVector
	 *  Standard copy constructor.
	 */
	wfVector(const wfVector& vec) :
		m_buffer  (wfNullPointer),
		m_length  (0),
		m_capacity(0)
	{
		*this = vec;
	}


	/*
	 * Constructor: wfVector
	 *  Initialize a vector from an <wfArray>.
	 *
	 * Remarks:
	 *  This performs a copy of the current contents of the <wfArray>.
	 */
	template<size_t E>
	wfVector(const wfArray<T, E>& array, wfHeap *heap = wfMemory::s_miscHeap) :
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_buffer                            (wfNullPointer),
		m_length                            (E),
		m_capacity                          (E)
	{
		T*     last;
		size_t copy;

		m_buffer = this->thisAllocateObject(E);
		last     = m_buffer + m_length;


		if (!wfIsPOD<T>::value) {
			while (m_buffer != last)
				new (m_buffer++) T(array.Data()[copy++]);
			m_buffer -= m_length;
		} else {
			memcpy(
				reinterpret_cast<void*>(m_buffer),
				reinterpret_cast<const void*>(array.Data()),
				E * sizeof(T)
			);
		}
	}


	/*
	 * Constructor: wfVector
	 *  Initialize with a default size, and optionally
	 *  a default value to fill the elements with.
	 */
	wfVector(size_t size, const T& value = T(), wfHeap *heap = wfMemory::s_miscHeap) :
		//
		// in C++11 you can use delegating constructor to wfVector()
		// here to lower generated code size
		//
	#ifndef WF_STDLIB_CPP11
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_buffer                            (wfNullPointer),
		m_length                            (0),
		m_capacity                          (0)
	#else
		wfVector()
	#endif
	{

		T *last  = wfNullPointer;
		m_buffer = this->thisAllocateObject(size);
		m_length = m_capacity = size;


		if (!wfIsPOD<T>::value) {
			while (m_buffer != (last = m_buffer + size))
				new (m_buffer++) T(value);
			m_buffer -= size;
		} else {
			memmove (
				reinterpret_cast<void*>(m_buffer),
				reinterpret_cast<const void*>(value),
				sizeof (T) * m_length
			);
		}
	}

	~wfVector() {
		Clear();
	}


	bool operator == (const wfVector &vec) const { return *this == vec; }
	bool operator != (const wfVector &vec) const { return *this != vec; }


	/*
	 * Function: operator=
	 * Replaces the elements of the vector with a copy of another vector.
	 *
	 * Parameters:
	 *  vec - The vector class being copied into the <wfVector>
	 *
	 * Remarks:
	 *  After erasing any elements in a <wfVector>. <operator=> either
	 *  copies or moves the contents of *vec* into the <wfVector>.
	 */
	wfVector& operator=(const wfVector &vec) {
		if (this == &vec)
			return *this;

		if (m_capacity >= vec.Capacity()) {
			if (!wfIsPOD<T>::value) {
				T *last = m_buffer + m_length;
				while (m_buffer != last)
					(*m_buffer++).~T();
			}
			m_length = vec.m_length;
		} else {
			Clear();

			m_length   = vec.m_length;
			m_capacity = vec.m_capacity;
			m_buffer   = thisAllocateObject(m_capacity);
		}

		if (wfIsPOD<T>::value) {
			memcpy(
				reinterpret_cast<void*>(m_buffer),
				reinterpret_cast<const void*>(vec.m_buffer),
				m_length * sizeof(T)
			);
		} else {
			T *last = m_buffer + m_length;
			T *copy = vec.m_buffer;

			while (m_buffer != last)
				new (m_buffer++) T(*copy++);

			m_buffer -= m_length;
		}

		return *this;
	}

	/*
	 * Function: Resize
	 * Specifies a new size for a vector.
	 *
	 * Parameters:
	 *  size  - The new size of the vector.
	 *  value - The value of new elements added to the vector if the new
	 *          size is larger than the original size.  If the value is
	 *          omitted, the new objects are assigned the default value.
	 *
	 * Remarks:
	 *  If the containers size is less than the requested size, *size*,
	 *  elements are added to the vector until is reached the requested
	 *  size.  If the containers size is larger than the requested size,
	 *  the elements closest to the end of the container are deleted until
	 *  the container reaches the size *size*.  If the present size of the
	 *  container is the same as the requested size, no action is taken.
	 */                 
	void Resize(size_t size, const T& value = T()) {
		size_t length = m_length;
		Reserve(size);

		m_length = size;
		T *first = m_buffer + length;
		T *last  = m_buffer + m_length;

		while (first != last)
			new (first++) T(value);
	}


	/*
	 * Function: Length
	 * Returns the number of elements in the vector 
	 *
	 * Returns:
	 * The current length of the vector
	 */   
	size_t Length()   const { return m_length; }

	/*
	 * Function: Capacity
	 * Returns the number of elements the vector could contain without
	 * further allocating more storage.
	 *
	 * Returns:
	 * The current length of storage allocated for the vector.
	 *
	 * Remarks:
	 *  The member function <Resize> will be more efficient if sufficent
	 *  memory is allocated to accommodate it. Use the member function
	 *  <Reserve> to specify the amount of memory allocated.
	 */ 
	size_t Capacity() const { return m_capacity; }

	/*
	 * Function: Empty
	 * Tests if the vector is empty.
	 *
	 * Returns:
	 * True if the vector is empty; false if the vector is not empty.
	 */    
	bool   Empty()    const { return (m_length == 0); }

	/*
	 * Function: Reserve
	 * Reserves a minimum length of storage for a vector object, allocating
	 * space it necessary.
	 *
	 * Parameters:
	 *  size - The minimum length of storage to be allocated for the vector.
	 */       
	void Reserve(size_t size) {
		size_t old = m_capacity;

		if (!m_capacity) {
			m_capacity = wfMax ( 
				static_cast<size_t> (
					wfPrivate::wfVectorMinimumSize
				), size
			);
		} else {
			while (m_capacity < size) {
				m_capacity <<= 1;
			}
		}

		if (m_capacity <= old)
			return;

		T *temp = this->thisAllocateObject(m_capacity);
		if (old > 0) {
			if (wfIsPOD<T>::value)
				memcpy(temp, m_buffer, m_length * sizeof(T));
			else {
				T *current = m_buffer;
				T *last    = temp + m_length;

				while (temp != last) {
					new (temp++) T(*current);
					(*current++).~T();
				}

				temp -= m_length;
			}
			wfHeap::Free(m_buffer);
		}

		m_buffer = temp;
	}

	/*
	 * Function: Clear
	 * Erases the elements of the vector.
	 */   
	void Clear() {
		if (m_capacity == 0)
			return;

		if (!wfIsPOD<T>::value) {
			T *last = m_buffer + m_length;
			while (m_buffer != last)
				(*m_buffer++).~T();

			m_buffer -= m_length;
		}
		wfHeap::Free(m_buffer);
		m_length   = 0;
		m_capacity = 0;
	}

	/*
	 * Function: PushBack
	 * Adds an element to the end of the vector.
	 *
	 * Parameters:
	 *  data - The element to add to the end of the vector.
	 */      
	T& PushBack(const T& data=T()) {
		if (m_length >= m_capacity)
			Reserve(m_capacity + 1);
		if (!wfIsPOD<T>::value)
			new (&m_buffer[m_length]) T(data);
		else
			m_buffer[m_length] = data;

		return m_buffer[m_length++];
	}

	/*
	 * Function: PopBack
	 * Deletes the element at the end of the vector.
	 */   
	void PopBack() {
		if (!wfIsPOD<T>::value)
			m_buffer[--m_length].~T();
		else
			m_length --;
	}


	/*
	 * Function: operator[]
	 *  Returns a reference to the vector element at a specified position.
	 *
	 * Parameters:
	 *  index - The position of the vector element.
	 *
	 * Returns:
	 *  If the position is greater than or equal to the size of the
	 *  container, the result is undefined.  There exists a const cv-qualified
	 *  version of this operator as well.
	 */          
	T&       operator[](size_t index)       { return m_buffer[index]; }
	const T& operator[](size_t index) const { return m_buffer[index]; }

	/*
	 * Function: At
	 * Returns a reference to the element at a specified location in the vector.
	 *
	 * Parameters:
	 *  index - The subscript or position of the element to reference in the vector.
	 *
	 * Returns:
	 *  A reference to the elemened subscripted in the argument.  If *index* is greater
	 *  than the size of the vector, At will return a reference to the last element in
	 *  the vector.  There exists a const cv-qualified version of this operator as well.
	 */           
	T&       At(size_t index)       { return m_buffer[(index <= m_length) ? index : m_length]; }
	const T& At(size_t index) const { return m_buffer[(index <= m_length) ? index : m_length]; }

	/*
	 * Function: Erase
	 * Removes an element from the vector given a <ConstIterator>
	 *
	 * Parameters:
	 *	iterator - The iterator to the element in the vector to remove
	 */
	Iterator Erase(ConstIterator it) {
		// cast const away (standard std::vector erase returns Iterator)
		// but accepts const_iterator, I can't imagine how other implementations
		// can do it without casting away const.
		T*       pre = const_cast<T*>(&*it);
		Iterator rit = Iterator(pre);

		// very high performant move (rvalue if C++11, or memmove if trivially copy assignable, otherwise
		// slow-object-wise copy).
		wfMove(pre + 1, End(), pre);
		m_length--;

		return rit;
	}

	// const version reapplys const through cv-q
	ConstIterator Erase(ConstIterator it) const {
		return Erase(it); // modifys *this not the iterator, so it's safe
	}
	
	template <wfSortSelector S>
	void Sort() {
		wfSorter<wfVector<T>, wfFunctional::wfGreater<T, T>, S>() (
				Begin(),
				End  ()
		);
	}

private:
	T      *m_buffer;
	size_t  m_length;
	size_t  m_capacity;
};

#endif
