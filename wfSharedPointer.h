#ifndef WF_STDLIB_SHAREDPOINTER_HDR
#define WF_STDLIB_SHAREDPOINTER_HDR
#include "wfStandard.h"

/*
 * Class: wfSharedPointer
 * Wraps a reference-counted smart pointer around a dynamically allocated
 * object.
 *
 * Remarks:
 *  The template class describes an object that uses reference counting to manage
 *  resources. A <wfSharedPointer> object effectively holds a pointer to the resource
 *  that it owns or holds a <wfNullPointer>.  A resource can be owned by more than one
 *  <wfSharedPointer> object; when the last <wfSharedPointer> object that owns a
 *  paticular resource is destroyed, the resource is freed.
 */
template <typename T>
class wfSharedPointer;

template <typename T>
class wfSharedPointer : public wfPrivate::wfHeapContainerManager<T> {
public:

	wfSharedPointer(wfHeap *heap = wfMemory::s_miscHeap) :
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_pointer                           (wfNullPointer),
		m_refcount                          (CounterNull())
	{
		IncReferenceCount();
	}
	
	wfSharedPointer(const wfSharedPointer<T> &rhs, wfHeap *heap = wfMemory::s_miscHeap) :
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_pointer                           (rhs.m_pointer),
		m_refcount                          (rhs.m_refcount)
	{
		IncReferenceCount();
	}
	
	wfSharedPointer(T *pointer, wfHeap *heap = wfMemory::s_miscHeap) :
		wfPrivate::wfHeapContainerManager<T>(heap),
		m_pointer                           (pointer)
	{
		 m_refcount = this->thisAllocateMemory<size_t>(sizeof(size_t));
		*m_refcount = 1;
		IncReferenceCount();
	}
	
	~wfSharedPointer() {
		DecReferenceCount();
	}
	
	
	T*       Get()               { return  m_pointer; }
	T*       operator ->()       { return  m_pointer; }
	T&       operator * ()       { return *m_pointer; }
	const T& operator * () const { return *m_pointer; }
	const T* operator ->() const { return  m_pointer; }
	const T* Get()         const { return  m_pointer; }	
	
	bool operator == (const wfSharedPointer<T>& s) const { return m_pointer == s.m_pointer; }
	bool operator != (const wfSharedPointer<T>& s) const { return m_pointer != s.m_pointer; }
	bool operator <  (const wfSharedPointer<T>& s) const { return m_pointer <  s.m_pointer; }
	
	const size_t Count() const {
		return *m_refcount;
	}
	
private:
	T      *m_pointer;
	size_t *m_refcount;
	
	static size_t* CounterNull() {
		static size_t nil = 1;
		return &nil;
	}
	
	void DecReferenceCount() {
		if (--(*m_refcount) == 0) {
			wfHeap::Free(m_pointer);
			wfHeap::Free(m_refcount);
		}
	}
	
	void IncReferenceCount() {
		++(*m_refcount);
	}
};


#endif