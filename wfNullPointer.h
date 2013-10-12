#ifndef WF_STDLIB_NULLPOINTER_HDR
#define WF_STDLIB_NULLPOINTER_HDR
#include "wfStandard.h"

#ifndef WF_STDLIB_CPP11

/*
 * File: wfNullPointer
 * >#include "wfNullPointer.h"
 *  
 * >typedef -implementation-defined- wfNullPointerType;
 */  

/*
 * Type: wfNullPointerType
 *  <wfNullPointerType> is the type of the null pointer literal,
 *  <wfNullPointer>. 
 * 
 * Example:
 *  If two or more overloads accept different pointer types, an overload
 *  for wfNullPointerType is necessary to accept a null pointer argument.
 *  (start code)
 *  void foo(int*)              { }
 *  void foo(double*)           { }
 *  void foo(wfNullPointerType) { }
 *
 *  int main() {
 *      int    *bar;
 *      double *baz;
 *
 *      foo(bar);
 *      foo(baz);
 *      foo(wfNullPointer); // would be ambigious without void foo(wfNullPointerType)
 *      foo(NULL);          // ambigious overload: all three functions are candidates.
 *  (end code)       
 */
class wfNullPointerType {
public:
    template <typename T>
    //
    // Makes it convertible to any type of null non-member
    // pointer.
    //
    inline operator T*() const { return 0; }

    //
    // Also anytype of null for member pointers.
    //
    template <typename C, typename T>
    inline operator T C::*() const { return 0; }

private:
    //
    // Prevent taking the address of this const objet
    // (nullptr_t cannot be taken the address of either)
    //
    void operator&() const;
};

/*
 * Object: wfNullPointer
 * A null pointer literal.  This is a unspecified prvalue of
 * type <wfNullPointerType>. There exists implicit conversions
 * from wfNullPointer to null pointer value of any pointer type
 * and any pointer to member type. Similar conversions exist for
 * any value of type <wfNullPointerType> as well as for the macro
 * *NULL*, the null pointer constant.
 */ 
const wfNullPointerType wfNullPointer = {};
#else // !WF_STDLIB_CPP11
typedef decltype(nullptr) wfNullPointerType;
const   decltype(nullptr) wfNullPointer = {};
#endif
#endif
