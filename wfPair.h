#ifndef WF_STDLIB_PAIR_HDR
#define WF_STDLIB_PAIR_HDR
#include "wfFunctional.h"
#include "wfTypeTraits.h"

/*
 * Class: wfPair
 *  Provides for the ability to treat two objects as a single object.
 *
 * Parameters:
 *  T - Type for *First* element of <wfPair>.
 *  U - Type for *Second* element of <wfPair>.
 */
template <typename T1, typename T2>
struct wfPair {
    /* Stay consistent with C++ */
    typedef T1 FirstType;
    typedef T2 SecondType;

    T1 first;
    T2 second;

    wfPair() :
        first(),
        second()
    { }

    wfPair(const T1& x, const T2 &y) :
        first(x),
        second(y)
    { }

    template <typename U1, typename U2>
    wfPair(
        const wfPair<U1, U2>& p,
        typename wfEnableIf<wfIsConvertible<const U1&, T1>::value &&
                            wfIsConvertible<const U2&, T2>::value>::type* = wfNullPointer
    ) :
        first(p.first),
        second(p.second)
    { }

    wfPair(const 
};
#endif