#ifndef WF_STDLIB_SINGLELIST_HDR
#define WF_STDLIB_SINGLELIST_HDR
#include "wfNullPointer.h"

namespace wfPrivate {
	template <typename T>
	struct wfSingleListNode {
		wfSingleListNode(const T& data, wfSingleListNode *next) :
			m_data(data),
			m_next(next)
		{ };

		T                 m_data;
		wfSingleListNode *m_next;
	};
}

/*
 * Class: wfSingleList
 *  Singly linked list
 *
 * Parameters:
 *  T - The element data type to be stored in the <wfSingleList>.
 *
 * Remarks:      
 *  <wfSingleList> arranges elements of a given type in a linear arrangement
 *  and, like vectors, allows fast random access to any element and efficent
 *  insertion and deletion at the back of the container.
 */
template <typename T>
struct wfSingleList;

/*
 * Class: wfSingleListIterator
 *  An iterator for the singly-linked list.  This is a bidirectional
 *  iterator.
 */ 
template <typename T>
struct wfSingleListIterator {
	typedef ptrdiff_t DifferenceType;
	typedef T         ValueType;
	typedef T*        PointerType;
	typedef T&        ReferenceType;

	wfSingleListIterator() :
		m_end(wfNullPointer)
	{ };

	wfSingleListIterator(wfPrivate::wfSingleListNode<T> *end) :
		m_end(end)
	{ };

	wfSingleListIterator(const wfSingleListIterator& end) :
		m_end(end.m_end)
	{ };


	ReferenceType operator *  () const { return  m_end->m_data; }
	PointerType   operator -> () const { return &m_end->m_data; }

	wfSingleListIterator& operator++() {
		m_end = m_end->m_next;
		return *this;
	}

	wfSingleListIterator operator++(int) {
		wfSingleListIterator tmp = *this;
		m_end = m_end->m_next;
		return tmp;
	}

	template <typename U>
	friend bool operator != (
		const wfSingleListIterator   & lhs,
		const wfSingleListIterator<U>& rhs
	) { return lhs.m_end != rhs.m_end; }

protected:
	wfPrivate::wfSingleListNode<T> *m_end;

	template<typename U> friend struct wfSingleListIterator;
	template<typename U> friend struct wfSingleListConstIterator;
};

/*
 * Class: wfSmallListConstIterator
 *  A *const* iterator for the singly-linked list.
 *  This is a bidirectional iterator.
 */
template <typename T>
struct wfSingleListConstIterator : wfSingleListIterator<T> {
private:
	typedef wfSingleListIterator<T> Base;
public:
	typedef ptrdiff_t         DifferenceType;
	typedef const T*          PointerType;
	typedef const T&          ReferenceType;
	typedef T                 ValueType;

	wfSingleListConstIterator(const Base& it) {
		Base::m_end = it.m_end;
	}

	ReferenceType operator *  () const { return  Base::m_end->m_data; }
	PointerType   operator -> () const { return &Base::m_end->m_data; }
};

template <typename T>
struct wfSingleList {
	/*
	 * Type: Iterator
	 *  A type that provides a bidirectional iterator that can read or
	 *  modify any element in a <wfSingleList>.
	 * 
	 * Remarks:
	 *  A type <Iterator> can be used to modify the value of an element.
	 */ 
	typedef wfSingleListIterator<T>      Iterator;
	
	/*
	 * Type: ConstIterator
	 *  A type that provides a bidirectional iterator that can read a
	 *  *const* element in a <wfSingleList>. 
	 * 
	 * Remarks:
	 *  A type <ConstIterator> cannot be used to modify the value of an
	 *  element.
	 */ 
	typedef wfSingleListConstIterator<T> ConstIterator;

	wfSingleList() :
		m_head  (wfNullPointer),
		m_length(0)
	{ };

	wfSingleList(const wfSingleList& list) :
		m_head (wfNullPointer),
		m_length(0)
	{
		for (ConstIterator i = list.Begin(); i != list.End(); ++i)
			PushFront(*i);
		Reverse();
	}

	~wfSingleList() { Clear(); }

	/*
	 * Function: Reverse
	 *  Reverses the contents of a <wfSingleList>.
	 */
	void Reverse() {
		wfPrivate::wfSingleListNode<T> *p = wfNullPointer;
		wfPrivate::wfSingleListNode<T> *i = m_head;
		wfPrivate::wfSingleListNode<T> *n;

		while (i) {
			n         = i->m_next;
			i->m_next = p;
			p         = i;
			i         = n;
		}
		m_head = p;
	}

	/*
	 * Function: Begin
	 *  Returns an iterator addressing the first element in a <wfSingleList>.
	 *
	 * Returns:
	 *  A bidirectional iterator addressing the first element in the <wfSingleList> or to the location
	 *  succeeding an empty <wfSingleList>.
	 *
	 * Remarks:
	 *  If the return value of *Begin* is assigned to a <wfConstIterator>, the elements in the
	 *  <wfSingleList> object cannot be modified.  If the return of *Begin* is assigned to an <wfIterator>,
	 *  the elements in the <wfSingleList> can be modified.  There is a const cv-qualified version of this
	 *  function as well.
	 */
	Iterator      Begin()       { return Iterator     (m_head); }
	ConstIterator Begin() const { return ConstIterator(m_head); }
	
	/*
	 * Function: End
	 *  Returns an iterator that addresses the location succeeding the last element in a <wfSingleList>.
	 *
	 * Returns:
	 *  A bidirectiuonal iterator that addresses the location succeeding the last element in the
	 *  <wfSingleList>.  If the list is empty, then End == Begin.
	 *
	 * Remarks:
	 *  *End* is used to test whether an iterator has reached the end of its <wfSingleList>.  There is a
	 *  const cv-qualified version of this function as well.
	 */
	Iterator      End()         { return Iterator     ();       }
	ConstIterator End()   const { return ConstIterator();       }

	T&       Front()       { return *Begin(); }
	const T& Front() const { return *Begin(); }

	wfSingleList& operator = (const wfSingleList& lst) {
		wfSingleList tmp(lst);
		Swap        (tmp);
		return *this;
	}

	/*
	 * Function: Clear
	 *  Erases all the elements of a <wfSingleList>
	 *
	 * Remarks:
	 *  *Clear* is functionally equivlant to:
	 *  (start code)
	 *  while (!Empty()) PopBack();
	 *  (end code)
	 */
	void Clear() {
		while (m_length > 0)
			PopFront();
	}

	/*
	 * Function: PushFront
	 *  Adds an element to the beginning of a <wfSingleList>.
	 *
	 * Parameters:
	 *  data - The element to add to the beginning of the <wfSingleList>.
	 */
	void PushFront(const T& data) {
		wfPrivate::wfSingleListNode<T> *tmp = new (g_miscHeap.Alloc(sizeof(wfPrivate::wfSingleListNode<T>))) wfPrivate::wfSingleListNode<T>(data, m_head);
		tmp->m_next                         = m_head;
		m_head                              = tmp;

		m_length ++;
	}

	/*
	 * Function: PopFront
	 *  Deletes the element at the beginning of a <wfSingleList>.
	 */
	void PopFront() {
		if (!m_head) {
			return;
		}

		wfPrivate::wfSingleListNode<T> *head = m_head->m_next;

		// call dtor then frees
		//m_head->~wfPrivate::wfSingleListNode<T>();
		g_miscHeap.Free(m_head);
		m_head = head;

		m_length --;
	}

	/*
	 * Function: Length
	 *  Returns the number of elements in a <wfSingleList>.
	 *
	 * Returns:
	 *  The current length of the <wfSingleList>
	 */
	size_t Length() const { return  m_length;       }
	
	/*
	 * Function: Empty
	 *  Tests if a <wfSingleList> is empty.
	 *
	 * Returns:
	 *  *true* if the <wfSingleList> is empty; *false* otherwise.
	 */
	bool   Empty () const { return (m_length == 0); }
	
private:
	wfPrivate::wfSingleListNode<T> *m_head;
	size_t                          m_length;
};

#endif