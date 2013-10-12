#ifndef WF_STDLIB_SET_HDR
#define WF_STDLIB_SET_HDR
#include "wfAlgorithm.h"
#include "wfFunctional.h"
#include "wfNullPointer.h"

namespace wfPrivate {

	template <typename T>
	struct wfSetNode {
		wfSetNode() :
			m_level (0),
			m_parent(this),
			m_left  (this),
			m_right (this)
		{ };
		
		wfSetNode(const T& data, wfSetNode *sides, wfSetNode *parent) :
			m_data  (data),
			m_level (1),
			m_parent(parent),
			m_left  (sides),
			m_right (sides)
		{ };

	public:

		// (1:54:14 PM) graphitemaster: right now .. I'd suggest just making m_left/m_right and m_data in wfSetNode unprotected, because this looks like a compiler bug
		// (2:03:20 PM) graphitemaster: https://connect.microsoft.com/VisualStudio/feedback/details/708928/cannot-access-private-member-declared-in-class
		// (2:12:50 PM) graphitemaster: so yeah, aparently in VS, any private/protected accesses through an object passed in through a function that isn't "this", instantly becomes illegal
		// (2:20:21 PM) graphitemaster: so yeah just throew them into public, put a nice big comment yelling at VS and anyone who dares touch them (they're sort of impossible to access from any of the actual classes .. so)
		T          m_data;
		wfSetNode *m_left;
		wfSetNode *m_right;
		size_t     m_level;
		wfSetNode *m_parent;
		
	protected:
		
		template <typename U>             friend struct wfSetIterator;
		template <typename U>             friend struct wfSetConstIterator;
		template <typename U>             friend struct wfSet;
		template <typename U, typename V> friend struct wfMap;
		template <typename U, typename V> friend struct wfPair;
	};
}

/*
 * Class: wfSet
 *  An efficent associative container using AA tree
 *
 * Remarks:
 *  An efficent associative container using AA tree.  Can be used for
 *  efficent key-value associations that are ordered, while retaining
 *  efficent insertion and search time.
 */
template <typename T>
struct wfSet;

/*
 * Class: wfSetIterator
 *  Bidirectional iterator, no offsets.
 */
template<typename T>
struct wfSetIterator {
	typedef ptrdiff_t DifferenceType;
	typedef T         ValueType;
	typedef T&        ReferenceType;
	typedef T*        PointerType;
	
	wfSetIterator() :
		m_node(wfNullPointer)
	{ };
	
	wfSetIterator(wfPrivate::wfSetNode<T> *node) :
		m_node(node)
	{ };
	
	wfSetIterator(const wfSetIterator& it) :
		m_node(it.m_node)
	{ };
	
	ReferenceType operator * () const { return  m_node->m_data; }
	PointerType   operator ->() const { return &m_node->m_data; }
	
	wfSetIterator& operator ++() {
		if (m_node->m_right->m_level != 0) {
			m_node = m_node->m_right;
			while (m_node->m_left->m_level != 0) {
				m_node = m_node->m_left;
			}
		} else {
			wfPrivate::wfSetNode<T> *node = m_node->m_parent;
			while (m_node == node->m_right) {
				m_node = node;
				node   = node->m_parent;
			}
			
			if (m_node->m_right != node)
				m_node = node;
		}
		
		return *this;
	}
	
	wfSetIterator& operator --() {
		if (m_node->m_left->m_level != 0) {
			m_node = m_node->m_left;
			while (m_node->m_right->m_level != 0) {
				m_node = m_node->m_right;
			}
		} else {
			wfPrivate::wfSetNode<T> *node = m_node->m_parent;
			while (m_node->m_level != 0 && m_node == node->m_left) {
				m_node = node;
				node   = node->m_parent;
			}
			
			if (m_node->m_left != node)
				m_node = node;
		}
		
		return *this;
	}
	
	wfSetIterator operator ++(int) {
		wfSetIterator tmp(*this);
		operator++();
		return tmp;
	}
	
	wfSetIterator operator --(int) {
		wfSetIterator tmp(*this);
		operator--();
		return tmp;
	}
	
	friend bool operator == (
		const wfSetIterator &a,
		const wfSetIterator &b
	) { return a.m_node == b.m_node; }
	
	friend bool operator != (
		const wfSetIterator &a,
		const wfSetIterator &b
	) { return a.m_node != b.m_node; }
	
protected:
	wfPrivate::wfSetNode<T> *m_node;
	
	template <typename U> friend struct wfSetIterator;
	template <typename U> friend struct wfSetConstIterator;
};

template <typename T>
struct wfSetConstIterator : wfSetIterator<T> {
	typedef typename wfIteratorTraits<T>::DifferenceType DifferenceType;
	typedef typename wfIteratorTraits<T>::ValueType      ValueType;

    typedef const T* PointerType;
    typedef const T& ReferenceType;
	
	wfSetConstIterator(const wfSetIterator<T> &it) {
		this->m_node = it.m_node;
	}
	
	ReferenceType operator * () const { return  this->m_node->m_data; }
	PointerType   operator ->() const { return &this->m_node->m_data; }
};

template <typename T>
struct wfSet {
	/*
	 * Type: Iterator
	 *  A type that provides a bidirectional iterator that can read or modify any
	 *  element in a <wfSet>.
	 *
	 * Remarks:
	 *  A type *Iterator* can be used to modify the value of an element.
	 */
	typedef wfSetIterator<T>                 Iterator;
	
	/*
	 * Type: ConstIterator
	 *  A type that propvides a bidirectional iterator that can read a *const* element
	 *  in the <wfSet>.
	 *
	 * Remarks:
	 *  A type *ConstIterator* cannot be used to modify the value of an element.
	 */
	typedef wfSetConstIterator<T>            ConstIterator;
	
	/*
	 * Type: ReverseIterator
	 *  A type that provides a bidirectional iterator that can read or modify an element
	 *  in a reversed <wfSet>.
	 *
	 * Remarks:
	 *  A type *ReverseIterator* is used to iterate through the set in reverse.
	 */
	typedef wfReverseIterator<Iterator>      ReverseIterator;
	
	/*
	 * Type: ConstReverseIterator
	 *  A type that provides a bidirectional iterator that can read any *const* element in
	 *  the <wfSet>.
	 *
	 * Remarks:
	 *  A type *ConstReverseIterator* cannot modify the value of an element and is used to
	 *  iterate through the <wfSet> in reverse.
	 */
	typedef wfReverseIterator<ConstIterator> ConstReverseIterator;
	
	wfSet() :
		m_root  (new (reinterpret_cast<wfPrivate::wfSetNode<T>*>(g_miscHeap.Alloc(sizeof(wfPrivate::wfSetNode<T>)))) wfPrivate::wfSetNode<T>),
		m_nil   (wfNullPointer),
		m_length(static_cast<size_t>(0))
	{
		m_nil  = m_root;
	}
	
	~wfSet() {
		this->DestroyNode(m_root);

		m_nil->wfPrivate::wfSetNode<T>::~wfSetNode();
		g_miscHeap.Free(m_nil);
	}
	
	/*
	 * Function: Length
	 *  Returns the number of elements in the <wfSet>.
	 *
	 * Returns
	 *  The current length of the <wfSet>.
	 */
	size_t Length() const { return m_length;      }
	
	/*
	 * Function: Empty
	 *  Tests if a <wfSet> is empty.
	 *
	 * Returns:
	 *  *true* if the <wfSet> is empty; *false* otherwise.
	 */
	bool   Empty () const { return m_length == 0; }
	
	/*
	 * Function: Begin
	 *  Returns an iterator that addresses the first element in the set.
	 *
	 * Returns:
	 *  A bidirectional iterator addressing the first element in the set or the location
	 *  succeeding an empty set.
	 *
	 * Remarks:
	 *  If the return value of *Begin* is assigned to a ConstIterator, the elements in
	 *  the set object cannot be modified.  If the return value of *Begin* is assigned
	 *  to an Iterator, the elements in the set object can be modified.  There exists
	 *  a const cv-qualified version of this function as well.
	 */
	Iterator Begin() {
		wfPrivate::wfSetNode<T> *node = m_root;
		while (node != m_nil) {
			if (node->m_left == m_nil)
				break;
				
			node = node->m_left;
		}
		
		return Iterator(node);
	}
	ConstIterator Begin() const {
		wfPrivate::wfSetNode<T> *node = m_root;
		while (node != m_nil) {
			if (node->m_left == m_nil)
				break;
				
			node = node->m_left;
		}
		
		return ConstIterator(node);
	}
	
	/*
	 * Function: ReverseBegin
	 *  Returns an iterator addressing the first element in a reversed set.
	 *
	 * Returns:
	 *  A reverse bidirectional iterator addressing the first element in a reversed set or
	 *  addressing what had been the last element in the unreversed set.
	 *
	 * Remarks:
	 *  *ReverseBegin* is used with a reversed <wfSet>just as *Begin* is used with a <wfSet>.
	 *  If the return value of *ReverseBegin* is assigned to a *ConstReverseIterator*, then the
	 *  <wfSet> object cannot be modified.  If the return value of *ReverseBegin* is assigned to
	 *  a *ReverseIterator*, then the <wfSet> object can be modified.  *ReverseBegin* can be
	 *  used to iterate through a <wfSet> backwards.  There exists a const cv-qualified version
	 *  of this function as well.
	 */
	ReverseIterator      ReverseBegin()       { return ReverseIterator     (End()); }
	ConstReverseIterator ReverseBegin() const { return ConstReverseIterator(End()); }
	
	/*
	 * Function: End
	 *  Returns an iterator that addresses the location succeeding the last element in a <wfSet>.
	 * 
	 * Returns:
	 *  A bidirectional iterator that addresses the location succeeding the last element in a <wfSet>.
	 *  If the <wfSet> is empty, then <i>wfSet::End == wfSet::Begin</i>.
	 *
	 * Remarks:
	 *  *End* is used to test whether an iterator has reached the end of its <wfSet>.  The value returned
	 *  by *End* should not be derferenced.  There exists a const cv-qualified version of this function
	 *  as well.
	 */
	Iterator End() {
		wfPrivate::wfSetNode<T> *node = m_root;
		while (node != m_nil)
			node = node->m_right;
			
		return Iterator(node);
	}
	ConstIterator End() const {
		wfPrivate::wfSetNode<T> *node = m_root;
		while (node != m_nil)
			node = node->m_right;
			
		return ConstIterator(node);
	}
	
	/*
	 * Function: ReverseEnd
	 *  Returns an iterator that addresses the location succeeding the last element in a
	 *  reversed <wfSet>.
	 *
	 * Returns:
	 *  A reverse bidirectional iterator that addresses the location succeeding the last element
	 *  in a reversed set, or the location that had preceded the first element in the unreversed
	 *  <wfSet>.
	 *
	 * Remarks:
	 *  *ReverseEnd* is used with a reversed <wfSet> just as *End* is used with a <wfSet>.
	 *  If the return value of *ReverseEnd* is assigned to a *ConstReverseIterator*, then the
	 *  <wfSet> object cannot be modified.  If the return value of *ReverseEnd* is assigned to
	 *  a *ReverseIterator*, then the <wfSet> object can be modified.  The value returned by
	 *  *ReverseEnd* should not be dereferenced.  *ReverseEnd* can be used to test whether a
	 *  reverse iterator has reached the end of its <wfSet>.
	 */
	ReverseIterator      ReverseEnd()       { return ReverseIterator     (Begin()); }
	ConstReverseIterator ReverseEnd() const { return ConstReverseIterator(Begin()); }
	
	/*
	 * Function: Insert
	 *  Inserts an element into a <wfSet>
	 *
	 * Parameters:
	 *  data - The value of an element to be inserted into the set unless the
	 *         set already contain that element or, more generally, an element
	 *         whose key is equivalently ordered.
	 *
	 * Returns:
	 *   A reference to the element added, or element which already exists if
	 *   the set had already contained an element whose key value was equivalently
	 *   ordered.
	 */
	T& Insert(const T& data) {
		return Insert(m_root, data)->m_data;
	}
	
	/*
	 * Function: Clear
	 *  Erases all the elements of a <wfSet>.
	 */
	void Clear() {
		DestroyNode(m_root);
		m_nil->wfPrivate::wfSetNode<T>::~wfSetNode();
		g_miscHeap.Free(m_nil);
		
		m_root   = new (reinterpret_cast<wfPrivate::wfSetNode<T>*>(g_miscHeap.Alloc(sizeof(wfPrivate::wfSetNode<T>)))) wfPrivate::wfSetNode<T>;
		m_nil    = m_root;
		m_length = 0;
	}
	
	/*
	 * Function: Erase
	 *  Removes an element in a <wfSet> matching a specified key.
	 *
	 * Parameters:
	 *  key - The key of the element to be removed from the set.
	 */
	void Erase(const T& key) {
		wfPrivate::wfSetNode<T> *store = Erase(m_root, key);
        store->wfPrivate::wfSetNode<T>::~wfSetNode();
        g_miscHeap.Free(store);
	}
	
	/*
	 * Function: Find
	 *  Returns an iterator addressing the location of an element in a <wfSet> that has
	 *  a key equivalent to a specified key.
	 *
	 * Parameters:
	 *  key - The argument key to be matched by the sort key of an element from the
	 *        <wfSet> being searched.
	 *
	 * Returns:
	 *  An iterator or const iterator that addresses the location of an element equivalent
	 *  to a specified key or that addresses the location succeeding the last element in the
	 *  set if no match is found for the key.
	 *
	 * Remarks:
	 *  The member function returns an iterator that addresses an element in the set whose
	 *  sort key is equivalent to the argument key under a binary predicate that induces an
	 *  ordering based on a less-than comparability relation.  If the return value of *Find*
	 *  is assigned to a *ConstIterator*, the <wfSet> object cannot be modified.  If the
	 *  return value of *Find* is assigned to an *Iterator*, the <wfSet> object can be
	 *  modified.  There exists a const cv-qualified version of this function as well.
	 */
	Iterator      Find(const T& key)       { return Iterator     (Find(m_root, key)); }
	ConstIterator Find(const T& key) const { return ConstIterator(Find(m_root, key)); }
	
protected:
	void DestroyNode(wfPrivate::wfSetNode<T> *node) {
		if (node == m_nil)
			return;
			
		DestroyNode(node->m_left);
		DestroyNode(node->m_right);
		
		node->wfPrivate::wfSetNode<T>::~wfSetNode();
		g_miscHeap.Free(node);
	}
	
	void Skew(wfPrivate::wfSetNode<T> *&n1) {
		wfPrivate::wfSetNode<T> *n2 = wfNullPointer;
		
		if (n1->m_level && n1->m_level == n1->m_left->m_level) {
			n2                    = n1->m_left;
			n2->m_parent          = n1->m_parent;
			n1->m_left            = n2->m_right;
			n1->m_left->m_parent  = n1;
			n2->m_right           = n1;
			n2->m_right->m_parent = n2;
			n1                    = n2;
		}
	}
	
	void Split(wfPrivate::wfSetNode<T> *&n1) {
		wfPrivate::wfSetNode<T> *n2 = wfNullPointer;
		
		if (n1->m_level && n1->m_level == n1->m_right->m_right->m_level) {
			n2                    = n1->m_right;
			n2->m_parent          = n1->m_parent;
			n1->m_right           = n2->m_left;
			n1->m_right->m_parent = n1;
			n2->m_left            = n1;
			n2->m_left->m_parent  = n2;
			n1                    = n2;
			n1->m_level           = n1->m_level + 1;
		}
	}
	
	template <typename U>
	wfPrivate::wfSetNode<T> *Insert(wfPrivate::wfSetNode<T> *&node, const U& data, wfPrivate::wfSetNode<T> *prev = wfNullPointer) {
		wfPrivate::wfSetNode<T> *ret = wfNullPointer;
		
		if (node == m_nil) {
			if (!prev)
				 prev = m_root;
				 
			node = new (reinterpret_cast<wfPrivate::wfSetNode<T>*>(g_miscHeap.Alloc(sizeof(wfPrivate::wfSetNode<T>)))) wfPrivate::wfSetNode<T> (data, m_nil, prev);
			m_length ++;

            return node;
		}
		
		if (!wfFunctional::wfEqual<U, U>()(data, node->m_data)) {
			ret = Insert(((wfFunctional::wfGreater<U, U>()(data, node->m_data))
				? node->m_right
				: node->m_left
			), data, node);
			
			if (!ret)
				return wfNullPointer;
		}
		
		Skew (node);
		Split(node);
		
		return ret;
	}
	
	template <typename U>
	wfPrivate::wfSetNode<T> *Erase(wfPrivate::wfSetNode<T> *&node, const U& key) {
		if (node == m_nil)
			return wfNullPointer;
			
		if (wfFunctional::wfEqual<U, T>()(key, node->m_data)) {
			if (node->m_left != m_nil && node->m_right != m_nil) {
				wfPrivate::wfSetNode<T> *heir = node->m_left;
				while (heir->m_right != m_nil)
					heir = heir->m_right;
					
				node->m_data = heir->m_data;
				
				return Erase(node->m_left, node->m_data);
			} else {
				wfPrivate::wfSetNode<T> *ret = node;
				wfPrivate::wfSetNode<T> *par = node->m_parent;
				
				node = ((node->m_left == m_nil)
					 ? node->m_right
					 : node->m_left
				);
				
				node->m_parent = par;
				m_length --;
				
				return ret;
			}
		} else {
			return Erase(((wfFunctional::wfLess<U, T>()(key, node->m_data))
						? node->m_left
						: node->m_right
			), key);
		}
	}
	
	wfPrivate::wfSetNode<T> *Find(wfPrivate::wfSetNode<T> *node, const T& key, bool add=false) {
		if (node == m_nil)
			return (add) ? Insert(m_root, T()) : m_nil;
		
		if (!wfFunctional::wfEqual<T, T>()(key, node->m_data))
			return Find(((wfFunctional::wfLess<T, T>()(key, node->m_data))
					? node->m_left
					: node->m_right
			), key, add);
			
		return node;
	}
	
	
	wfPrivate::wfSetNode<T> *m_root;
	wfPrivate::wfSetNode<T> *m_nil;
	
private:
	size_t                   m_length;
};

#endif
