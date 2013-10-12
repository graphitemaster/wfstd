#ifndef WF_STDLIB_SMALLLIST_HDR
#define WF_STDLIB_SMALLLIST_HDR
#include "wfNullPointer.h"
#include "wfIterator.h"

/*
 * Class: wfSmallList
 * Double ended queue
 *
 * Parameters:
 *  T - The element data type to be stored in the <wfSmallList>.
 *
 * Remarks:      
 *  <wfSmallList> arranges elements of a given type in a linear arrangement
 *  and, like vectors, allows fast random access to any element and efficent
 *  insertion and deletion at the back of the container.  However, unlike
 *  a vector, the <wfSmallList> (double-ended-queue), also supports efficent insersion
 *  and deletion at the front of the container. 
 */
template <typename T>
struct wfSmallList;

template <typename T>
struct wfSmallListIterator;

template <typename T>
struct wfSmallListConstIterator;

namespace wfPrivate {
	template <typename T>
	struct wfSmallListNode {		
		wfSmallListNode() :
			m_next(wfNullPointer),
			m_prev(wfNullPointer)
		{ };

		wfSmallListNode(const T& data, wfSmallListNode *prev, wfSmallListNode *next) :
			m_data(data),
			m_prev(prev),
			m_next(next)
		{ };

	private:
		T                m_data;
		wfSmallListNode *m_next;
		wfSmallListNode *m_prev;

		template <typename U> friend struct ::wfSmallListIterator;
		template <typename U> friend struct ::wfSmallListConstIterator;
		template <typename U> friend struct ::wfSmallList;
	};
};

/*
 * Class: wfSmallListIterator
 *  An iterator for the doubly-linked list.  This is a bidirectional
 *  iterator.
 */  
template <typename T>
struct wfSmallListIterator {
	typedef ptrdiff_t DifferenceType;
	typedef T         ValueType;
	typedef T*        PointerType;
	typedef T&        ReferenceType;

	wfSmallListIterator() :
		m_end(wfNullPointer)
	{ };

	wfSmallListIterator(wfPrivate::wfSmallListNode<T> *end) :
		m_end(end)
	{ };

	wfSmallListIterator(const wfSmallListIterator& end) :
		m_end(end.m_end)
	{ };


	ReferenceType operator *  () const { return  m_end->m_data; }
	PointerType   operator -> () const { return &m_end->m_data; }

	wfSmallListIterator& operator++() {
		m_end = m_end->m_next;
		return *this;
	}

	wfSmallListIterator& operator--() {
		m_end = m_end->m_prev;
		return *this;
	}

	wfSmallListIterator operator++(int) {
		wfSmallListIterator tmp = WF_STDLIB_RVALUE_MOVE(*this);
		m_end = m_end->m_next;
		return tmp;
	}

	wfSmallListIterator operator--(int) {
		wfSmallListIterator tmp = WF_STDLIB_RVALUE_MOVE(*this);
		m_end = m_end->m_prev;
		return tmp;
	}


	template <typename U>
	friend bool operator != (
		const wfSmallListIterator   & lhs,
		const wfSmallListIterator<U>& rhs
	) { return lhs.m_end != rhs.m_end; }

protected:
	wfPrivate::wfSmallListNode<T> *m_end;

	template<typename U> friend struct wfSmallListIterator;
	template<typename U> friend struct wfSmallListConstIterator;
};

/*
 * Class: wfSmallListConstIterator
 *  A *const* iterator for the doubly-linked list.
 *  This is a bidirectional iterator.
 */
template <typename T>
struct wfSmallListConstIterator : wfSmallListIterator<T> {
private:
	typedef wfSmallListIterator<T> Base;

public:
	typedef ptrdiff_t         DifferenceType;
	typedef const T*          PointerType;
	typedef const T&          ReferenceType;
	typedef T                 ValueType;

	wfSmallListConstIterator(const Base& it) {
		Base::m_end = it.m_end;
	}

	ReferenceType operator *  () const { return  Base::m_end->m_data; }
	PointerType   operator -> () const { return &Base::m_end->m_data; }
};

template <typename T>
struct wfSmallList {
	/*
	 * Type: Iterator
	 *  A type that provides a bidirectional iterator that can read or
	 *  modify any element in a <wfSmallList>.
	 * 
	 * Remarks:
	 *  A type <Iterator> can be used to modify the value of an element
	 */  
	typedef wfSmallListIterator<T>           Iterator;

	/*
	 * Type: ConstIterator
	 *  A type that provides a bidirectional iterator that can read a
	 *  *const* element in a <wfSmallList>. 
	 * 
	 * Remarks:
	 *  A type <ConstIterator> cannot be used to modify the value of an
	 *  element
	 */ 
	typedef wfSmallListConstIterator<T>      ConstIterator;

	/*
	 * Type: ReverseIterator
	 *  A type that provides a bidirectional iterator that can read or
	 *  modify an element in a reversed <wfSmallList>.
	 *
	 * Remarks:
	 *  A type <ReverseIterator> is used to iterate through the <wfSmallList>
	 *  in reverse.    
	 */
	typedef wfReverseIterator<Iterator>      ReverseIterator;

	/*
	 * Type: ConstReverseIterator
	 *  A type that provides a bidirectional iterator that can read any
	 *  *const* element in a <wfSmallList>.
	 *
	 * Remarks:
	 *  A type <ConstReverseIterator> cannot modify the value of an element
	 *  and is used to iterate through the <wfSmallList> in reverse.     
	 */    
	typedef wfReverseIterator<ConstIterator> ConstReverseIterator;

	
	wfSmallList() :
		m_first (wfNullPointer),
		m_last  (new (g_miscHeap.Alloc(sizeof(wfPrivate::wfSmallListNode<T>))) wfPrivate::wfSmallListNode<T>),
		m_length(0)
	{ };

	~wfSmallList() {
		while (m_length > 0)
			PopBack();

		g_miscHeap.Free(m_last);
	}

	/*
	 * Function: Begin
	 *  Returns an iterator addressin the first element in a <wfSmallList>.
	 *
	 * Returns:
	 *  A bidirectional iterator addressing the first element in the <wfSmallList> or to the location
	 *  succeeding an empty <wfSmallList>.
	 *
	 * Remarks:
	 *  If the return value of *Begin* is assigned to a <wfConstIterator>, the elements in the
	 *  <wfSmallList> object cannot be modified.  If the return of *Begin* is assigned to an <wfIterator>,
	 *  the elements in the <wfSmallList> can be modified.  There is a const cv-qualified version of this
	 *  function as well.
	 */
	Iterator             Begin()              { return Iterator            (m_first); }
	ConstIterator        Begin()        const { return ConstIterator       (m_first); }

	/*
	 * Function: End
	 *  Returns an iterator that addresses the location succeeding the last element in a <wfSmallList>.
	 *
	 * Returns:
	 *  A bidirectiuonal iterator that addresses the location succeeding the last element in the
	 *  <wfSmallList>.  If the list is empty, then End == Begin.
	 *
	 * Remarks:
	 *  *End* is used to test whether an iterator has reached the end of its <wfSmallList>.  There is a
	 *  const cv-qualified version of this function as well.
	 */
	Iterator             End()                { return Iterator            (m_last);  }
	ConstIterator        End()          const { return ConstIterator       (m_last);  }

	/*
	 * Function: ReverseBegin
	 *  Returns an iterator addressing the first element in a reversed <wfSmallList>.
	 *
	 * Returns:
	 *  A reverse bidirectional iterator addressing the first element in a reverse <wfSmallList>, or
	 *  addressing what had been the last element in the unreversed <wfSmallList..
	 *
	 * Remarks:
	 *  *ReverseBegin* is used with a reversed <wfSmallList> just as *Begin* is used with a <wfSmallList>.
	 *  If the return value of *ReverseBegin* is assigned to a *ConstReverseIterator*, the list object
	 *  cannot be modified.  If the return value of *ReverseBegin* is assigned to a *ReverseIterator*, the
	 *  list object can be modified.  *ReverseBegin* can be used to iterate through a <wfSmallList> backwards.
	 *  There also exists a const cv-qualified version of this function as well.
	 */
	ReverseIterator      ReverseBegin()       { return ReverseIterator     (End());   }
	ConstReverseIterator ReverseBegin() const { return ConstReverseIterator(End());   }

	/*
	 * Function: ReveseEnd
	 *  Returns an iterator that addresses the location succeeding the last element in a reversed <wfSmallList>.
	 *
	 * Returns:
	 *  A reverse bidirectional iterator that addresses the location succeeding the last element in a reversed
	 *  <wfSmallList>, or the element that had preceded the first element in the unreversed <wfSmallList>.
	 *
	 * Remarks:
	 *  *ReverseEnd* is used with a reverse <wfSmallList> just as *End* is used with a <wfSmallList>.
	 *  If the return value of *ReverseEnd* is assigned to a *ConstReverseIterator*, the list object cannot
	 *  be modified.  If the return value of *ReverseEnd* is assigned to a *ReverseIterator*, the list object
	 *  can be modified.  *ReverseEnd* can be used to test whether a reversed iterate has reached the end of its
	 *  <wfSmallList>.  The value returned by *ReverseEnd* should not be dereferenced.  There also exists a
	 *  const cv-qualified version of this function as well.
	 */
	ReverseIterator      ReverseEnd()         { return ReverseIterator     (Begin()); }
	ConstReverseIterator ReverseEnd()   const { return ConstReverseIterator(Begin()); }

	/*
	 * Function: Length
	 *  Returns the number of elements in a <wfSmallList>.
	 *
	 * Returns:
	 *  The current length of the <wfSmallList>
	 */
	size_t Length() const { return m_length; }
	
	/*
	 * Function: Empty
	 *  Tests if a <wfSmallList> is empty.
	 *
	 * Returns:
	 *  *true* if the <wfSmallList> is empty; *false* otherwise.
	 */
	bool   Empty () const { return (m_length == 0); }

	
	/*
	 * Function: PushBack
	 *  Adds an element to the end of a <wfSmallList>.
	 *
	 * Parameters:
	 *  data - The element to add to the end of the <wfSmallList>.
	 */
	T& PushBack(const T& data) {
		wfPrivate::wfSmallListNode<T> *node = new (g_miscHeap.Alloc(sizeof(wfPrivate::wfSmallListNode<T>))) wfPrivate::wfSmallListNode<T>(data, m_last->m_prev, m_last);

		if (!m_last->m_prev) m_first = node;
		else m_last->m_prev->m_next  = node;

		m_last->m_prev = node;
		++m_length;

		return node->m_data;
	}

	/*
	 * Function: PushFront
	 *  Adds an element to the beginning of a <wfSmallList>.
	 *
	 * Parameters:
	 *  data - The element to add to the beginning of the <wfSmallList>.
	 */
	T& PushFront(const T& data) {
		wfPrivate::wfSmallListNode<T> *node = new (g_miscHeap.Alloc(sizeof(wfPrivate::wfSmallListNode<T>))) wfPrivate::wfSmallListNode<T>(data, wfNullPointer, m_first);

		if (!m_first) m_last->m_prev = node;
		else m_first->m_prev         = node;

		m_first = node;
		++m_length;

		return node->m_data;
	}

	/*
	 * Function: PopBack
	 *  Deletes an element at the end of a <wfSmallList>.
	 *
	 * Remarks:
	 *  The last element must not be empty.  *PopBack* doesn't protect
	 *  out-of-bounds deletions.
	 */
	void PopBack() {
		wfPrivate::wfSmallListNode<T> *node = m_last->m_prev;
		if (!node) return;

		m_last->m_prev = node->m_prev;
		if (m_last->m_prev)
			m_last->m_prev->m_next = node->m_next;

		if (m_length == 1)
			m_first = wfNullPointer;

		if (m_last->m_prev && !m_last->m_prev->m_prev) {
			m_first         = m_last->m_prev;
			m_first->m_prev = wfNullPointer;
		}

		g_miscHeap.Free(node);
		-- m_length;
	}

	/*
	 * Function: PopFront
	 *  Deletes the element at the beginning of a <wfSmallList>.
	 *
	 * Remarks:
	 *  The first element must not be empty.  *PopFront* doesn't protect
	 *  out-of-bounds deletions.
	 */
	void PopFront() {
		wfPrivate::wfSmallListNode<T> *node = m_first;
		if (!node) return;

		if (m_length == 1)
			m_first = wfNullPointer;
		else {
			m_first         = node->m_next;
			m_first->m_prev = wfNullPointer;
		}

		g_miscHeap.Free(node);
		-- m_length;
	}

	/*
	 * Function: Clear
	 *  Erases all the elements of a <wfSmallList>
	 *
	 * Remarks:
	 *  *Clear* is functionally equivlant to:
	 *  (start code)
	 *  while (!Empty()) PopBack();
	 *  (end code)
	 */
	void Clear() {
		while (m_length > 0)
			PopBack();
	}
	
private:
	wfPrivate::wfSmallListNode<T> *m_first;
	wfPrivate::wfSmallListNode<T> *m_last;
	size_t                         m_length;
};

#endif
