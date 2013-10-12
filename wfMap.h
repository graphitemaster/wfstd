#ifndef WF_STDLIB_MAP_HDR
#define WF_STDLIB_MAP_HDR

#include "wfSet.h"
#include "wfPair.h"
#include "wfFunctional.h"

template <typename T, typename U>
struct wfMap : wfSet<wfPair<T, U> > {
private:
	typedef wfSet<wfPair<T, U> >                Base;
	typedef wfPrivate::wfSetNode<wfPair<T, U> > Node;
public:

    /*
     * Type: Iterator
     *  A type that provides a bidirectional iterator that can read or
     *  modify any type in a wfMap.
     *
     * Remarks:
     *  The *Iterator* defined by *wfMap* points to elements that are
     *  objects of *wfPair<T, U>*, whos *First* member is the key to the
     *  element and whose *Second* member is the mapped datum held by the
     *  element.
     *
     *  To dereference an *Iterator* pointing to an element in a *wfMap*,
     *  use the *->* operator.
     *
     *  To access the value of the key for the element use *Iter->First*,
     *  to access the value of the mapped datum for the element use
     *  *Iter->Second*.
     */   
	typedef wfSetIterator<wfPair<T, U> >        Iterator;

    /*
     * Type: ConstIterator
     *  A type that provides a bidirectional iterator that can a *const*
     *  element in the *wfMap*.
     *
     * Remarks:
     *  A type *ConstIterator* cannot be used to modify the value of an
     *  element.
     *  
     *  The *ConstIterator* defined by *wfMap* points to elements that are
     *  objects of *wfPair<T, U>*, whos *First* member is the key to the
     *  element and whose *Second* member is the mapped datum held by the
     *  element.
     *
     *  To dereference an *ConstIterator* pointing to an element in a *wfMap*,
     *  use the *->* operator.
     *
     *  To access the value of the key for the element use *Iter->First*,
     *  to access the value of the mapped datum for the element use
     *  *Iter->Second*.
     */  
	typedef wfSetConstIterator<wfPair<T, U> >   ConstIterator;

    /*
     * Type: ReverseIterator
     *  A type that provides a bidirectional iterator that can read
     *  or modify an element in a reversed map.
     */    
	typedef wfReverseIterator<Iterator>         ReverseIterator;
	typedef wfReverseIterator<ConstIterator>    ConstReverseIterator;
	
	U& Insert(const T& key, const U& data) {
		return Base::Insert(
			Base::m_root,
			wfPair<T, U>(key, data)
		)->m_data.Second;
	}
	
	U& Insert(const wfPair<T, U>& data) {
		return Base::Insert(Base::m_root, data)->m_data.Second;
	}

    /*
     * Function: Find
     *  Returns an iterator addressing the location of an element in a
     *  map that has a key equivalent to a specified key.
     *
     * Parameters:
     *  key - The value to be matched by the sort key of an element from
     *  the map being searched.
     *
     * Returns:
     *  An iterator that addresses the location of an element with a
     *  specified key, or the location succeeding the last element in
     *  the map if no match is found for the key.
     *
     * Remarks:
     *  There exists a const cv-qualified version of this function as
     *  well; If the return value of *Find* is assigned to a
     *  *ConstIterator*, the map object cannot be modified, likewise if
     *  the return value is assigned to a *Iterator*, the map object
     *  can be modified.
     *
     * Complexity:
     *   wfMap is implemented with a wfSet, which in turn utilizes
     *   an AA-tree, as such the time complexity for *Find* is:
     * 
     *   Average Case - O(log n)
     *   Worst Cast   - O(n) 
     */       
	Iterator      Find(const T& key)       { return Iterator     (m_find(Base::m_root, key)); }
	ConstIterator Find(const T& key) const { return ConstIterator(m_find(Base::m_root, key)); }

    void Erase(const T& key) {

		Iterator findResultIter	= Find( key );

		if( findResultIter == Base::End() )
		{
			return;
		}		 

		wfPair<T, U>  pairToErase(findResultIter->First, findResultIter->Second);

		Base::Erase( pairToErase );
    }
    
	U& operator[](const T& key) {
		return m_find(Base::m_root, key, true)->m_data.Second;
	}
protected:
	Node *m_find(Node *node, const T& key, bool insert=false) {
        if (node == Base::m_nil) {
            if (insert)
                return Base::Insert(Base::m_root, wfPair<T, U>(key, U()));
            else
                return Base::m_nil;
        }

        if (!wfFunctional::wfEqual<T, T>()(key, node->m_data.First)) {
            return m_find((
                (wfFunctional::wfLess<T, T>()(key, node->m_data.First))
                    ? node->m_left
                    : node->m_right
                ), key, insert);
        }
        return node;
	}
};
#endif
