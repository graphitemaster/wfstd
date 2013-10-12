#ifndef WF_STDLIB_STACKLIST_HDR
#define WF_STDLIB_STACKLIST_HDR
#include "wfStandard.h"
#include "wfNullPointer.h"

namespace wfPrivate {
	template <typename T>
	struct wfStackListNode {
		wfStackListNode(const T& data, wfStackListNode *below = wfNullPointer) :
			m_below(below),
			m_data (data)
		{ };

		wfStackListNode *m_below;
		T                m_data;
	};
};

/*
 * Struct: wfStackList
 * Standard LIFO Stack
 *
 * >#include "wfStackList.h"
 * 
 * Parameters:
 *  T - The element data type to be stored in the stack.
 *
 * Remarks:
 *  This container unlike many containers in the Wayfroward standard
 *  library lacks iterators. If iterators are required use <wfSingleList>
 * (a LIFO singly linked list).
 */   
template <typename T>
struct wfStackList {
	/*
	 * Constructor: wfStackList
	 *  Initializes the stack list.
	 */
	wfStackList() :
		m_topnode(wfNullPointer),
		m_length (0)
	{ }

	/*
	 * Destructor: wfStackList
	 *  Calls <PopBack> until the length is 0.  This effectivly deletes
	 *  all elements and frees memory.
	 */
	~wfStackList() {
		while (m_length > 0)
			PopBack();
	}

	/*
	 * Function: Length
	 *  Returns the current wfStackList length.
	 */   
	size_t Length() const { return m_length; }

	/*
	 * Function: Empty
	 *  Returns true if the stack contains no nodes, otherwise false.
	 */
	bool Empty() const { return (m_length == 0); }

	/*
	 * Function: Top
	 * Returns a reference to the top-most node on the stack.
	 */
	T& Top() { return m_topnode->data; }

	/*
	 * Function: Top
	 * Returns a const reference to the top-mode node on the stack.
	 */
	const T& Top() const { return m_topnode->data; }

	/*
	 * Function: PushBack
	 *  Creates a new top node with the data specified by the argument
	 *  and inserts it the back.
	 *
	 * Parameters:
	 *  data - The data to push back
	 */
	void PushBack(const T& data) {
		wfPrivate::wfStackListNode<T> *tmp = new (reinterpret_cast<wfPrivate::wfStackListNode<T>*>(g_miscHeap.Alloc(sizeof(wfPrivate::wfStackListNode<T>)))) wfPrivate::wfStackListNode<T>(data, m_topnode);
		m_topnode                          = tmp;

		m_length ++;
	}

	/*
	 * Function: PopBack
	 *  Pops out the current top node.
	 */
	void PopBack() {
		wfPrivate::wfStackListNode<T> *tmp = m_topnode;
		m_topnode                          = tmp->m_below;

		tmp->~wfPrivate::wfStackListNode<T>();
		g_miscHeap.Free(tmp);
		m_length --;
	}

	/*
	 * Function: Clear
	 *  Calls *PopBack* until the <wfStackList> is empty.  This effectivly
	 *  frees all memory as well.
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
	wfPrivate::wfStackListNode<T> *m_topnode;
	size_t                         m_length;
};

#endif
