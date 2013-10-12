#ifndef WF_STDLIB_LIST_HDR
#define WF_STDLIB_LIST_HDR
#include "wfStandard.h"
#include "wfAssert.h"
//#define WF_VALIDATE_LIST    // validate the list whenever it is modified

template<typename T>
class wfList
{
public:
    void Check()
    {
#ifdef WF_VALIDATE_LIST
        if( m_head == NULL )
        {
            wfAssert( GetSize() == 0 );
            wfAssert( m_tail == NULL );
        }
        else
        {
            u32 count = 1;
            wfNode* node = m_head;
            wfNode* prev = NULL;
            wfNode* last = node;
            while( node->m_next )
            {
                wfAssert( prev == node->m_prev );
                prev = node;
                node = node->m_next;
                if( node ) last = node;
                ++count;
            }
            wfAssert( last == m_tail );
            wfAssert( count == GetSize() );
        }
#endif
    }

    /*!
    \class wfList::wfNode

    Each list item should have a wfNode as a member.  At construction, the wfNode should be passed the pointer to the item.
    */
    class wfNode
    {
    public:

        // you could use if( m_prev == m_next && m_prev != NULL ) to flag something, at no extra memory cost

        inline wfNode() :
            m_item( NULL ),
            m_prev( NULL ),
            m_next( NULL ),
            m_list( NULL )
        {}

        inline void Init( T* item )
        {
            m_item = item;
        }

        inline wfNode( T* item ) :
            m_item( item ),
            m_prev( NULL ),
            m_next( NULL ),
            m_list( NULL )
        {}

        //! If a wfNode is a member of a list, it will remove itself upon destruction.
        inline ~wfNode()
        {
            if( m_list )
            {
                m_list->Remove( this );
            }
        }

        wfList< T >*       m_list;
        T*                 m_item;
        wfNode*            m_prev;
        wfNode*            m_next;
    };

    /*!
    \class wfList::wfIter
    */
    class wfIter
    {
        friend class wfList<T>;
    public:

        //! Creates an invalid wfIterator
        /*
        An iterator created with this constructor can only be made into a valid iterator through assignment to a valid iterator.
        */
        inline wfIter() :
            m_list( NULL ),
            m_current( NULL )
        {}

        //! Returns true if the wfIterator points to something
        inline bool IsValid() const
        {
            return m_current != NULL;
        }

        //! Moves the wfIterator to the next list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline wfIter& operator++() // prefix
        {
            m_current = m_current->m_next;
            return *this;
        }

        inline wfIter operator++( int ) // postfix
        {
            wfIter prevIter( *this );
            m_current = m_current->m_next;
            return prevIter;
        }

        //! Moves the wfIterator to the previous list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline wfIter& operator--() // prefix
        {
            m_current = m_current->m_prev;
            return *this;
        }

        inline wfIter operator--( int ) // postfix
        {
            wfIter prevIter( *this );
            m_current = m_current->m_prev;
            return prevIter;
        }

        //! Returns a pointer to the current list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline T* GetItem()
        {
            return m_current->m_item;
        }
        inline T* operator->()
        {
            return GetItem();
        }
        inline T* operator*()
        {
            return GetItem();
        }

        //! Returns the wfNode for the current list item
        /*!
        Direct access to the node should be rarely used, maybe for a situation such as deleting the node
        */
        inline wfNode* GetNode() { return m_current; }

        //! Returns a pointer to the list that this iterator points to (regardless of whether it points to a valid item inside of that list)
        inline wfList< T >* GetList() { return m_list; }
        inline const wfList< T >* GetList() const { return m_list; }

    //protected:
    //    template< class T > friend class wfList;

        //! Used internally by wfList::Begin/End
        inline wfIter( wfList< T >* list, wfNode* item ) :
            m_list( list ),
            m_current( item )
        {}

    private:
        wfList< T >*    m_list;
        wfNode*            m_current;
    };

    // Const interator
    class wfConstIter
    {
        friend class wfList<T>;
    public:

        //! Creates an invalid wfIterator
        /*
        An iterator created with this constructor can only be made into a valid iterator through assignment to a valid iterator.
        */
        inline wfConstIter() :
            m_list( NULL ),
            m_current( NULL )
        {}

        //! Returns true if the wfIterator points to something
        inline bool IsValid() const
        {
            return m_current != NULL;
        }

        //! Moves the wfIterator to the next list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline wfConstIter& operator++() // prefix
        {
            m_current = m_current->m_next;
            return *this;
        }

        inline wfConstIter operator++( int ) // postfix
        {
            wfConstIter prevIter( *this );
            m_current = m_current->m_next;
            return prevIter;
        }

        //! Moves the wfIterator to the previous list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline wfConstIter& operator--() // prefix
        {
            m_current = m_current->m_prev;
            return *this;
        }

        inline wfConstIter operator--( int ) // postfix
        {
            wfConstIter prevIter( *this );
            m_current = m_current->m_prev;
            return prevIter;
        }

        //! Returns a pointer to the current list item
        /*!
        Using this when !IsValid() has undefined behavior and may crash with invalid memory access.
        */
        inline const T* GetItem()
        {
            return m_current->m_item;
        }
        inline const T* operator->()
        {
            return GetItem();
        }
        inline const T* operator*()
        {
            return GetItem();
        }

        //! Returns the wfNode for the current list item
        /*!
        Direct access to the node should be rarely used, maybe for a situation such as deleting the node
        */
        inline const wfNode* GetNode() { return m_current; }

        //! Returns a pointer to the list that this iterator points to (regardless of whether it points to a valid item inside of that list)
        inline const wfList< T >* GetList() { return m_list; }

    //protected:
    //    template< class T > friend class wfList;

        //! Used internally by wfList::Begin/End
        inline wfConstIter( const wfList< T >* list, const wfNode* item ) :
            m_list( list ),
            m_current( item )
        {}

    private:
        const wfList< T >*    m_list;
        const wfNode*        m_current;
    };

    //! Initializes an empty list
    inline wfList() :
        m_size( 0 ),
        m_head( NULL ),
        m_tail( NULL )
    {}

    //! A single wfNode CANNOT BE IN TWO LISTS! If you want to move a wfNode from one list to another, call Remove() first!
    inline void Prepend( wfNode* node )
    {
        Check();
        if( m_head == NULL )
        {
            wfAssert( m_size == 0 );
            m_head = m_tail = node;
        }
        else
        {
            m_head->m_prev = node;
            node->m_next = m_head;
            m_head = node;
        }
        node->m_list = this;
        ++m_size;
        Check();
    }

    //! *Moves* items from one list to the end of another
    /*!
    This WILL modify 'list', removing all items from it!
    */
    inline void Prepend( wfList< T >* list )
    {
        Check();
        for( wfList< T >::wfIter iter = list->Begin(); iter.IsValid(); ++iter )
        {
            iter.m_current->m_list = this;
        }

        if( m_head == NULL )
        {
            m_head = list->m_head;
            m_tail = list->m_tail;
            m_size = list->m_size;
        }
        else
        {
            m_head->m_prev = list->m_tail;
            list->m_tail->m_next = m_head;
            m_head = list->m_head;
            m_size += list->m_size;
        }

        list->m_head = list->m_tail = NULL;
        list->m_size = 0;
        Check();
    }

    //! A single wfNode CANNOT BE IN TWO LISTS! If you want to move a wfNode from one list to another, call Remove() first!
    inline void Append( wfNode* node )
    {
        Check();
#ifdef WF_VALIDATE_LIST
        {
            bool found = false;
            for( wfList< T >::wfIter iter = Begin(); iter.IsValid(); ++iter )
            {
                if( iter.GetNode() == node )
                {
                    found = true;
                    break;
                }
            }
            wfAssert( !found );
        }
#endif
        if( m_head == NULL )
        {
            wfAssert( m_size == 0 );
            m_head = m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            node->m_prev = m_tail;
            m_tail = node;
        }
        node->m_list = this;
        ++m_size;
        Check();
    }

    //! *Moves* items from one list to the end of another
    /*!
    This WILL modify 'list', removing all items from it!
    */
    inline void Append( wfList< T >* list )
    {
        if( list->GetSize() == 0 ) return;
        Check();
        for( wfList< T >::wfIter iter = list->Begin(); iter.IsValid(); ++iter )
        {
            iter.m_current->m_list = this;
        }

        if( m_head == NULL )
        {
            m_head = list->m_head;
            m_tail = list->m_tail;
            m_size = list->m_size;
        }
        else
        {
            m_tail->m_next = list->m_head;
            list->m_head->m_prev = m_tail;
            m_tail = list->m_tail;
            m_size += list->m_size;
        }

        list->m_head = list->m_tail = NULL;
        list->m_size = 0;
        Check();
    }

    //! Removes a node from the list
    /*!
    This will NOT delete a node, only remove it from the list.
    */
    inline void Remove( wfNode* node )
    {
        Check();
        if( node->m_list == NULL )
        {
            return;
        }
#ifdef WF_VALIDATE_LIST
        {
            bool found = false;
            for( wfList< T >::wfIter iter = Begin(); iter.IsValid(); ++iter )
            {
                if( iter.GetNode() == node )
                {
                    found = true;
                    break;
                }
            }
            wfAssert( found );
        }
#endif
        if( node == m_head )
        {
            m_head = node->m_next;
        }
        else
        {
            node->m_prev->m_next = node->m_next;
        }
        if( node == m_tail )
        {
            m_tail = node->m_prev;
        }
        else
        {
            node->m_next->m_prev = node->m_prev;
        }

        node->m_next = node->m_prev = NULL;

        node->m_list = NULL;

        wfAssert( m_size > 0 );

/*
#ifdef WF_DEBUG
        if( m_size == 1 )
        {
            wfAssert( m_head == NULL && m_tail == NULL );
        }
        else
        {
            wfAssert( m_head != NULL && m_tail != NULL );
        }
#endif
*/

        --m_size;

        if( m_head == NULL )
        {
            wfAssert( m_size == 0 );
        }

        Check();
    }

    inline wfIter Remove( const wfIter& iterToRemove )
    {
        Check();
        wfIter nextIter = iterToRemove;
        ++nextIter; 

        Remove( iterToRemove.m_current );
        Check();
        return nextIter;
    }

    //! Returns a pointer to the first item in the list
    inline wfNode* GetFirstNode()
    {
        return m_head;
    }

    inline T* GetFirst()
    {
        if(m_head)
            return m_head->m_item;
        return NULL;
    }

    //! Returns a pointer to the first item in the list
    inline T* GetLast()
    {
        if(m_tail)
            return m_tail->m_item;
        return NULL;
    }

    //! Returns the number of items in the list
    inline u32 GetSize() const
    {
        return m_size;
    }

    //! Returns an iterator pointing to the first item in the list
    inline wfIter Begin()
    {
        return wfIter( this, m_head );
    }

    inline wfConstIter ConstBegin() const
    {
        return wfConstIter( this, m_head );
    }

    //! Returns an iterator pointing to the last item in the list
    /*!
    This DOES NOT behave like std::list::end()!  The returned iterator points to the last item, not beyond it.
    */
    inline wfIter End()
    {
        return wfIter( this, m_tail );//, m_size - 1 );
    }

    inline wfConstIter ConstEnd() const
    {
        return wfConstIter( this, m_tail );
    }

    //! Runs a common method on all list items
    // if you are getting compile errors on the line below, you are probably using wfList< T* > instead of wfList< T > (* is implied)
    typedef void ( T::*ForeachCallback )( void ); 
    inline void Foreach( ForeachCallback callback )
    {
        wfNode* entry = m_head;
        while( entry )
        {
            ( *( entry->m_item ).*( callback ) )();
            entry = entry->m_next;
        }
    }

    //! Inserts a new node before a node that is already in the list
    inline void InsertBefore( wfNode* insertBefore, wfNode* node )
    {
        Check();
        node->m_prev = insertBefore->m_prev;
        node->m_next = insertBefore;

        if( insertBefore == m_head )
        {
            m_head = node;
        }
        else
        {
            node->m_prev->m_next = node;
        }

        insertBefore->m_prev = node;

        node->m_list = this;

        ++m_size;
        Check();
    }

    //! Returns a pointer to the first item in the list and Remove()'s its node
    inline T* TakeFirst()
    {
        Check();
        if( m_head )
        {
            wfNode* first = m_head;
            Remove( first );
            Check();
            return first->m_item;
        }
        else
        {
            return NULL;
        }
    }

    //! Returns a pointer to the last item in the list and Remove()'s its node
    inline T* TakeLast()
    {
        Check();
        if( m_tail )
        {
            wfNode* last = m_tail;
            Remove( last );
            Check();
            return last->m_item;
        }
        else
        {
            return NULL;
        }
    }

    //! Returns true if the provided item is within this list.
    inline bool Contains( const T* pItem ) const
    {
        wfNode* pNode = m_head;
        while( pNode )
        {
            if(pNode->m_item == pItem)
                return true;
            pNode = pNode->m_next;
        }
        return false;
    }

    typedef bool (*ComparisonFunctionPtr)( const T* item1, const T* item2 );

    inline void InsertionSort( ComparisonFunctionPtr comparisonFunctionPtr )
    {
        wfNode* outer, *inner;
        outer = m_head;
        while (outer)
        {
            wfNode* tmp = outer->m_next;
            bool insert = false;
            inner = outer->m_prev;
            while (inner)
            {
                if (comparisonFunctionPtr( inner->m_item, outer->m_item )) //> 0
                {
                    if (!insert)
                    {
                        break;
                    }
                    else
                    {
                        if( outer == m_tail )
                        {
                            m_tail = outer->m_prev;
                        }

                        outer->m_prev->m_next = outer->m_next;
                        if( outer->m_next )
                        {
                            outer->m_next->m_prev = outer->m_prev;
                        }

                        outer->m_prev = inner;
                        outer->m_next = inner->m_next;

                        inner->m_next->m_prev = outer;
                        inner->m_next = outer;

                        insert = false;
                        break;
                    }
                }
                else
                {
                    insert = true;
                }
                inner = inner->m_prev;
            }
            if (insert)
            {
                if( outer == m_tail )
                {
                    m_tail = outer->m_prev;
                }

                outer->m_prev->m_next = outer->m_next;
                if( outer->m_next )
                {
                    outer->m_next->m_prev = outer->m_prev;
                }

                m_head->m_prev = outer;
                outer->m_next = m_head;
                outer->m_prev = NULL;
                m_head = outer;
            }
            outer = tmp;
        }
    }

    inline void MergeSort( ComparisonFunctionPtr comparisonFunctionPtr )
    {
        wfNode *p, *q, *e, *tail, *head;
        s32 insize, nmerges, psize, qsize, i;

        head = m_head;

        //if the head is empty, stop
        if (head == NULL)
        {
            return;
        }

        insize = 1;

        while (1) 
        {
            p = head;
            head = NULL;
            tail = NULL;

            //count the number of merges this pass
            nmerges = 0;

            while (p) 
            {
                nmerges++;  /* there exists a merge to be done */
                // step `insize' places along from p
                q = p;
                psize = 0;
                for (i = 0; i < insize; i++) 
                {
                    psize++;
                    
                    q = q->m_next;
                    if (q == NULL)
                    {
                        break;
                    }
                }

                // if q hasn't fallen off end, we have two lists to merge
                qsize = insize;
                
                //now we have two lists; merge them
                while (psize > 0 || (qsize > 0 && q)) 
                {
                    // decide whether next element of merge comes from p or q
                    if (psize == 0) 
                    {
                        // p is empty; e must come from q. 
                        e = q; 
                        q = q->m_next; 
                        qsize--;
                    } 
                    else if (qsize == 0 || q == NULL)
                    {
                        // q is empty; e must come from p.
                        e = p; 
                        p = p->m_next; 
                        psize--;
                    }
                    else if ( comparisonFunctionPtr( p->m_item, q->m_item ) )
                    {
                        // comparitor says p is lower; e must come from p
                        e = p; 
                        p = p->m_next; 
                        psize--;
                    } 
                    else
                    {
                        // First element of q is lower; e must come from q. 
                        e = q;
                        q = q->m_next; 
                        qsize--;
                    }

                    // add the next element to the merged list
                    if (tail) 
                    {
                        tail->m_next = e;
                    } 
                    else 
                    {
                        head = e;
                    }

                    // Maintain reverse pointers in a doubly linked list.
                    e->m_prev = tail;

                    tail = e;
                }

                // now p has stepped `insize' places along, and q has too
                p = q;
            }
        
           tail->m_next = NULL;

            // If we have done only one merge, we're finished.
            if (nmerges <= 1)   // allow for nmerges==0, the empty list case 
            {
                m_head = head;
                m_tail = tail;
                return;
            }

            /* Otherwise repeat, merging lists twice the size */
            insize *= 2;
        }
    }

    //! Removes all objects from the list.
    inline void Clear()
    {
        while(m_size)
        {
            TakeFirst();
        }
    }

    //! Deletes and removes all objects from the list.
    inline void ClearDelete()
    {
        T* pDel;
        while(m_size)
        {
            pDel = TakeFirst();
            delete pDel;
        }
    }

protected:
    wfNode*    m_head;
    wfNode*    m_tail;
    u32        m_size;
};

#endif // !WF_LIST_H
