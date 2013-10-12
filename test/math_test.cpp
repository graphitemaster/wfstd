#include "math/wfVector4f.h"
#include "math/wfVector3f.h"
#include "math/wfVector2f.h"
#include "math/wfMat4f.h"

#include <stdio.h>
#include <float.h>

/*
 * The cross-compiler I was using to test uses uClibc which for some reason
 * implements bool incorrectly.  I just override it and provide my own.
 */
#ifdef bool  
#   undef bool
#endif /*! bool */
#ifdef true
#   undef true
#endif /*! true */
#ifdef false
#   undef false
#endif /*! false */

/* Probably no the safest? */
#define bool  int
#define true  static_cast<bool>(1)
#define false static_cast<bool>(!true)

#define WF_MATH_TEST_ALLOC malloc
#define WF_MATH_TEST_FREE  free

#define WF_STDLIB_MAKE_STANDALONE
#include "wfSystemInfo.h"

struct wfMathTest;
static wfMathTest *wfMathTests = NULL;

struct wfMathTest {
    const char *name;
    bool      (*func)(wfMathTest *);
    wfMathTest *next;
    const char *error;
};


static void wfMathTestsAdd(const char *name, bool (*func)(wfMathTest *)) {
    wfMathTest *find;

    if (!wfMathTests) {
        wfMathTests = (wfMathTest*)WF_MATH_TEST_ALLOC(sizeof(wfMathTest));
        wfMathTests->name  = name;
        wfMathTests->func  = func;
        wfMathTests->error = NULL;
        wfMathTests->next  = NULL;
        return;
    }

    for (find = wfMathTests; find->next; find = find->next);

    find->next        = (wfMathTest*)WF_MATH_TEST_ALLOC(sizeof(wfMathTest));
    find->next->name  = name;
    find->next->func  = func;
    find->next->error = NULL;
    find->next->next  = NULL;
}

static void wfMathTestsCleanup() {
    wfMathTest *node = wfMathTests;
    wfMathTest *temp = NULL;
    while (node) {
        temp = node;
        node = node->next;
        WF_MATH_TEST_FREE(temp);
    }
    wfMathTests = NULL;
}

static bool wfMathTestsRun() {
    wfMathTest *node;
    for (node = wfMathTests; node; node = node->next) {
        if (!node->func(node)) {
            printf("Failure: %s\n"
            "    Condition:  `%s` failed\n"
            "    Evaluation: `%s` should be true\n", node->name, node->error, node->error);

            return false;
        } else {
            printf("Success: %s\n", node->name);
        }
    }
    return true;
}

#define WF_MATH_FAIL(COND)        \
    do {                          \
        if (!(COND)) {            \
            store->error = #COND; \
            return false;         \
        }                         \
    } while (0)


/*
 * Printing utility functions to make my life so much less painful
 * as you'd expect.
 */
void wfMathTestPrintVec4(const wfPrivate::wfMathVec4 &vec) {
    fprintf(stderr, "[%f, %f, %f, %f]\n",
        wfPrivate::wfMathVec4GetX(vec),
        wfPrivate::wfMathVec4GetY(vec),
        wfPrivate::wfMathVec4GetZ(vec),
        wfPrivate::wfMathVec4GetW(vec)
    );
}

/*
 * A little bit about how this works:
 *  Generally speaking an epsilon is the smallest value that could be
 *  added to 1.0f and still change its actual representable value. The
 *  value 1.0f is actually very crucial to how floating point arithmetic
 *  works, large numbers don't actually change when added to an epsilon.
 *  By scaling this value to the numbers we're trying to compare, you
 *  can tell if they're different or not.  The problem is that there is
 *  no gurantee SIMD will be as sufficent as IEEE 754 in terms of actual
 *  accuracy.  So to avoid noise in the comparision, we could chop off
 *  a few of the least significant bits.
 */          
static bool wfMathTestSafeCompare(float a, float b) {
    return (fabs(a - b) <= 16 * FLT_EPSILON * fmax(fabs(a), fabs(b)));
}

/*
 * We compare every component individually with wfMathTestSafeCompare
 * for proper comparision between two wfMathVec4s
 */  
static bool wfMathTestSafeCompareMathVec4(
    const wfPrivate::wfMathVec4& lhs,
    const wfPrivate::wfMathVec4& rhs
) {
    return wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(lhs), wfPrivate::wfMathVec4GetX(rhs)) &&
           wfMathTestSafeCompare(wfPrivate::wfMathVec4GetY(lhs), wfPrivate::wfMathVec4GetY(rhs)) &&
           wfMathTestSafeCompare(wfPrivate::wfMathVec4GetZ(lhs), wfPrivate::wfMathVec4GetZ(rhs)) &&
           wfMathTestSafeCompare(wfPrivate::wfMathVec4GetW(lhs), wfPrivate::wfMathVec4GetW(rhs));
}

static bool wfMathTestSafeCompareMathMat4(
    const wfPrivate::wfMathMat4& lhs,
    const wfPrivate::wfMathMat4& rhs
) {
    return wfMathTestSafeCompareMathVec4(lhs.x, rhs.x) &&
           wfMathTestSafeCompareMathVec4(lhs.y, rhs.y) &&
           wfMathTestSafeCompareMathVec4(lhs.z, rhs.z) &&
           wfMathTestSafeCompareMathVec4(lhs.w, rhs.w);
}

static bool wfMathTestVec4_Construction(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(a);

    WF_MATH_FAIL(a.x() == 1);
    WF_MATH_FAIL(a.y() == 2);
    WF_MATH_FAIL(a.z() == 3);
    WF_MATH_FAIL(a.w() == 4);

    WF_MATH_FAIL(b.x() == 1);
    WF_MATH_FAIL(b.y() == 2);
    WF_MATH_FAIL(b.z() == 3);
    WF_MATH_FAIL(b.w() == 4);

    return true;
}

static bool wfMathTestVec4_AdditionVector(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(a);
    wfVector4f c = a + b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 6);
    WF_MATH_FAIL(c.w() == 8);

    return true;
}

static bool wfMathTestVec4_SubtractionVector(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(2, 4, 6, 8);
    wfVector4f c = b - a;

    WF_MATH_FAIL(c.x() == a.x());
    WF_MATH_FAIL(c.y() == a.y());
    WF_MATH_FAIL(c.z() == a.z());
    WF_MATH_FAIL(c.w() == a.w());

    return true;
}


static bool wfMathTestVec4_MultiplicationVector(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(2, 4, 6, 8);
    wfVector4f c = b * a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 8);
    WF_MATH_FAIL(c.z() == 18);
    WF_MATH_FAIL(c.w() == 32);

    return true;
}

static bool wfMathTestVec4_DivisionVector(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(2, 4, 6, 8);
    wfVector4f c = b / a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 2);
    WF_MATH_FAIL(c.z() == 2);
    WF_MATH_FAIL(c.w() == 2);

    return true;
}

static bool wfMathTestVec4_AdditionFloat(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    float      b = 2;
    wfVector4f c = a + b;

    WF_MATH_FAIL(c.x() == 3);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 5);
    WF_MATH_FAIL(c.w() == 6);

    return true;
}

static bool wfMathTestVec4_SubtractionFloat(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    float      b = 1;
    wfVector4f c = a - b;

    WF_MATH_FAIL(c.x() == 0);
    WF_MATH_FAIL(c.y() == 1);
    WF_MATH_FAIL(c.z() == 2);
    WF_MATH_FAIL(c.w() == 3);

    return true;
}


static bool wfMathTestVec4_MultiplicationFloat(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    float      b = 2;
    wfVector4f c = a * b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 6);
    WF_MATH_FAIL(c.w() == 8);

    return true;
}

static bool wfMathTestVec4_DotProduct(wfMathTest *store) {
    wfVector4f a(1, 2, 3, 4);
    wfVector4f b(4, 3, 2, 1);
    float      c = a.Dot(a);
    float      d = b.Dot(b);
    float      e = a.Dot(b);
    float      f = b.Dot(a);
    

    WF_MATH_FAIL(c == 30);
    WF_MATH_FAIL(d == 30);
    WF_MATH_FAIL(e == 20);
    WF_MATH_FAIL(f == 20);
    
    return true;
}



/////
static bool wfMathTestVec3_Construction(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(a);

    WF_MATH_FAIL(a.x() == 1);
    WF_MATH_FAIL(a.y() == 2);
    WF_MATH_FAIL(a.z() == 3);

    WF_MATH_FAIL(b.x() == 1);
    WF_MATH_FAIL(b.y() == 2);
    WF_MATH_FAIL(b.z() == 3);

    return true;
}

static bool wfMathTestVec3_AdditionVector(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(a);
    wfVector3f c = a + b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 6);

    return true;
}

static bool wfMathTestVec3_SubtractionVector(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(2, 4, 6);
    wfVector3f c = b - a;

    WF_MATH_FAIL(c.x() == a.x());
    WF_MATH_FAIL(c.y() == a.y());
    WF_MATH_FAIL(c.z() == a.z());

    return true;
}


static bool wfMathTestVec3_MultiplicationVector(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(2, 4, 6);
    wfVector3f c = b * a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 8);
    WF_MATH_FAIL(c.z() == 18);

    return true;
}

static bool wfMathTestVec3_DivisionVector(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(2, 4, 6);
    wfVector3f c = b / a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 2);
    WF_MATH_FAIL(c.z() == 2);

    return true;
}

static bool wfMathTestVec3_AdditionFloat(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    float      b = 2;
    wfVector3f c = a + b;

    WF_MATH_FAIL(c.x() == 3);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 5);

    return true;
}

static bool wfMathTestVec3_SubtractionFloat(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    float      b = 1;
    wfVector3f c = a - b;

    WF_MATH_FAIL(c.x() == 0);
    WF_MATH_FAIL(c.y() == 1);
    WF_MATH_FAIL(c.z() == 2);

    return true;
}


static bool wfMathTestVec3_MultiplicationFloat(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    float      b = 2;
    wfVector3f c = a * b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);
    WF_MATH_FAIL(c.z() == 6);

    return true;
}

static bool wfMathTestVec3_DotProduct(wfMathTest *store) {
    wfVector3f a(1, 2, 3);
    wfVector3f b(4, 3, 2);
    float      c = a.Dot(a);
    float      d = b.Dot(b);
    float      e = a.Dot(b);
    float      f = b.Dot(a);
    

    WF_MATH_FAIL(c == 14);
    WF_MATH_FAIL(d == 29);
    WF_MATH_FAIL(e == 16);
    WF_MATH_FAIL(f == 16);
    
    return true;
}
/////

static bool wfMathTestVec2_Construction(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(a);

    WF_MATH_FAIL(a.x() == 1);
    WF_MATH_FAIL(a.y() == 2);

    WF_MATH_FAIL(b.x() == 1);
    WF_MATH_FAIL(b.y() == 2);

    return true;
}

static bool wfMathTestVec2_AdditionVector(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(a);
    wfVector2f c = a + b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);

    return true;
}

static bool wfMathTestVec2_SubtractionVector(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(2, 4);
    wfVector2f c = b - a;

    WF_MATH_FAIL(c.x() == a.x());
    WF_MATH_FAIL(c.y() == a.y());

    return true;
}


static bool wfMathTestVec2_MultiplicationVector(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(2, 4);
    wfVector2f c = b * a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 8);

    return true;
}

static bool wfMathTestVec2_DivisionVector(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(2, 4);
    wfVector2f c = b / a;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 2);

    return true;
}

static bool wfMathTestVec2_AdditionFloat(wfMathTest *store) {
    wfVector2f a(1, 2);
    float      b = 2;
    wfVector2f c = a + b;

    WF_MATH_FAIL(c.x() == 3);
    WF_MATH_FAIL(c.y() == 4);

    return true;
}

static bool wfMathTestVec2_SubtractionFloat(wfMathTest *store) {
    wfVector2f a(1, 2);
    float      b = 1;
    wfVector2f c = a - b;

    WF_MATH_FAIL(c.x() == 0);
    WF_MATH_FAIL(c.y() == 1);

    return true;
}


static bool wfMathTestVec2_MultiplicationFloat(wfMathTest *store) {
    wfVector2f a(1, 2);
    float      b = 2;
    wfVector2f c = a * b;

    WF_MATH_FAIL(c.x() == 2);
    WF_MATH_FAIL(c.y() == 4);

    return true;
}

static bool wfMathTestVec2_DotProduct(wfMathTest *store) {
    wfVector2f a(1, 2);
    wfVector2f b(4, 3);
    float      c = a.Dot(a);
    float      d = b.Dot(b);
    float      e = a.Dot(b);
    float      f = b.Dot(a);
    

    WF_MATH_FAIL(c == 5);
    WF_MATH_FAIL(d == 25);
    WF_MATH_FAIL(e == 10);
    WF_MATH_FAIL(f == 10);
    
    return true;
}

/* Backend testing */
static bool wfMathTestPrivate_wfMathVec4_FlipSign(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec   = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 flip1 = wfPrivate::wfMathVec4FlipSign<wfPrivate::VEC_SIGN_0101>(vec);
    wfPrivate::wfMathVec4 flip2 = wfPrivate::wfMathVec4FlipSign<wfPrivate::VEC_SIGN_1010>(vec);

    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(flip1) ==  1.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetY(flip1) == -2.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetZ(flip1) ==  3.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetW(flip1) == -4.0f);

    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(flip2) == -1.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetY(flip2) ==  2.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetZ(flip2) == -3.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetW(flip2) ==  4.0f);

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_Shuffle(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec  = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 wxyz = wfPrivate::wfMathVec4Shuffle<wfPrivate::VEC_SHUFFLE_WXYZ>(vec);
    wfPrivate::wfMathVec4 zwxy = wfPrivate::wfMathVec4Shuffle<wfPrivate::VEC_SHUFFLE_ZWXY>(vec);
    wfPrivate::wfMathVec4 yzwx = wfPrivate::wfMathVec4Shuffle<wfPrivate::VEC_SHUFFLE_YZWX>(vec);

    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(wxyz) == 4.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetY(wxyz) == 1.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetZ(wxyz) == 2.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetW(wxyz) == 3.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(zwxy) == 3.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetY(zwxy) == 4.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetZ(zwxy) == 1.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetW(zwxy) == 2.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(yzwx) == 2.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetY(yzwx) == 3.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetZ(yzwx) == 4.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetW(yzwx) == 1.0f);

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_Sum(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 sum = wfPrivate::wfMathVec4Sum(vec);

    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(sum) == 10.0f);

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_Dot(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec  = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 dot4 = wfPrivate::wfMathVec4Dot<4>(vec, vec);
    wfPrivate::wfMathVec4 dot3 = wfPrivate::wfMathVec4Dot<3>(vec, vec);
    wfPrivate::wfMathVec4 dot2 = wfPrivate::wfMathVec4Dot<2>(vec, vec);

    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(dot4) == 30.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(dot3) == 14.0f);
    WF_MATH_FAIL(wfPrivate::wfMathVec4GetX(dot2) == 5.0f);

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_Length(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec  = wfPrivate::wfMathVec4Create(1.0f, 1.02f, 3.04f, 4.0f);
    wfPrivate::wfMathVec4 len4 = wfPrivate::wfMathVec4Length<4>(vec);
    wfPrivate::wfMathVec4 len3 = wfPrivate::wfMathVec4Length<3>(vec);
    wfPrivate::wfMathVec4 len2 = wfPrivate::wfMathVec4Length<2>(vec);

    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len4), 5.223217f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len3), 3.358869f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len2), 1.428426f));

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_LengthSquared(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec  = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 len4 = wfPrivate::wfMathVec4LengthSquared<4>(vec);
    wfPrivate::wfMathVec4 len3 = wfPrivate::wfMathVec4LengthSquared<3>(vec);
    wfPrivate::wfMathVec4 len2 = wfPrivate::wfMathVec4LengthSquared<2>(vec);

    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len4), 0.182574f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len3), 0.267261f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(len2), 0.447214f));

    return true;
}

static bool wfMathTestPrivate_wfMathVec4_Normalize(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec  = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathVec4 nrm4 = wfPrivate::wfMathVec4Normalize<4>(vec);
    wfPrivate::wfMathVec4 nrm3 = wfPrivate::wfMathVec4Normalize<3>(vec);
    wfPrivate::wfMathVec4 nrm2 = wfPrivate::wfMathVec4Normalize<2>(vec);

    
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(nrm4), 0.182574f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetY(nrm4), 0.365148f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetZ(nrm4), 0.547723f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetW(nrm4), 0.730297f));

    /*
     * spherical coords:
     *
     * radius:          1
     * polar angle:     sin-1(sqrt(5/14))
     * azimuthal angle: tan-1(2)
     */ 
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(nrm3), 0.267261f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetY(nrm3), 0.534522f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetZ(nrm3), 0.801784f));

    /*
     * angles to cord axes:
     *  
     * horizontal: cos-1(1/sqrt(5))
     * vertical:   cos-1(2/sqrt(5))
     */ 
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetX(nrm2), 0.447214f));
    WF_MATH_FAIL(wfMathTestSafeCompare(wfPrivate::wfMathVec4GetY(nrm2), 0.894427f));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Create(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);

    WF_MATH_FAIL(wfMathTestSafeCompareMathVec4(vec, mat.x));
    WF_MATH_FAIL(wfMathTestSafeCompareMathVec4(vec, mat.y));
    WF_MATH_FAIL(wfMathTestSafeCompareMathVec4(vec, mat.z));
    WF_MATH_FAIL(wfMathTestSafeCompareMathVec4(vec, mat.w));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Identity(wfMathTest *store) {
    wfPrivate::wfMathMat4 mat;
    wfPrivate::wfMathMat4Identity(&mat);

    WF_MATH_FAIL(wfMathTestSafeCompare(1.0f, wfPrivate::wfMathVec4GetX(mat.x)));
    WF_MATH_FAIL(wfMathTestSafeCompare(1.0f, wfPrivate::wfMathVec4GetY(mat.y)));
    WF_MATH_FAIL(wfMathTestSafeCompare(1.0f, wfPrivate::wfMathVec4GetZ(mat.z)));
    WF_MATH_FAIL(wfMathTestSafeCompare(1.0f, wfPrivate::wfMathVec4GetW(mat.w)));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_UnalignedLoad(wfMathTest *store) {
    wfPrivate::wfMathMat4 mat;
    wfPrivate::wfMathMat4 cmp;

    float data[] = {
        100.0f, 200.0f, 300.0f, 400.0f,
        400.0f, 300.0f, 200.0f, 100.0f,
        100.0f, 200.0f, 300.0f, 400.0f,
        400.0f, 300.0f, 200.0f, 100.0f
    };

    cmp = wfPrivate::wfMathMat4Create (
        wfPrivate::wfMathVec4Create(100.0f, 200.0f, 300.0f, 400.0f),
        wfPrivate::wfMathVec4Create(400.0f, 300.0f, 200.0f, 100.0f),
        wfPrivate::wfMathVec4Create(100.0f, 200.0f, 300.0f, 400.0f),
        wfPrivate::wfMathVec4Create(400.0f, 300.0f, 200.0f, 100.0f)
    );


    wfPrivate::wfMathMat4UnalignedLoad(&mat, data);

    WF_MATH_FAIL(wfMathTestSafeCompareMathMat4(mat, cmp));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Sum(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);
    wfPrivate::wfMathMat4Sum(&mat, &vec);

    WF_MATH_FAIL(wfMathTestSafeCompare(4.0f,  wfPrivate::wfMathVec4GetX(vec)));
    WF_MATH_FAIL(wfMathTestSafeCompare(8.0f,  wfPrivate::wfMathVec4GetY(vec)));
    WF_MATH_FAIL(wfMathTestSafeCompare(12.0f, wfPrivate::wfMathVec4GetZ(vec)));
    WF_MATH_FAIL(wfMathTestSafeCompare(16.0f, wfPrivate::wfMathVec4GetW(vec)));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Vec4Mul(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);
    wfPrivate::wfMathVec4 out;

    wfPrivate::wfMathMat4Vec4Mul(&mat, &vec, &out);

    WF_MATH_FAIL(wfMathTestSafeCompare(10.0f, wfPrivate::wfMathVec4GetX(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(20.0f, wfPrivate::wfMathVec4GetY(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(30.0f, wfPrivate::wfMathVec4GetZ(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(40.0f, wfPrivate::wfMathVec4GetW(out)));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Vec3Mul(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);
    wfPrivate::wfMathVec4 out;

    wfPrivate::wfMathMat4Vec3Mul(&mat, &vec, &out);

    WF_MATH_FAIL(wfMathTestSafeCompare(6.0f,  wfPrivate::wfMathVec4GetX(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(12.0f, wfPrivate::wfMathVec4GetY(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(18.0f, wfPrivate::wfMathVec4GetZ(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(24.0f, wfPrivate::wfMathVec4GetW(out)));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_Point3Mul(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);
    wfPrivate::wfMathVec4 out;

    wfPrivate::wfMathMat4Point3Mul(&mat, &vec, &out);

    WF_MATH_FAIL(wfMathTestSafeCompare(7.0f,  wfPrivate::wfMathVec4GetX(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(14.0f, wfPrivate::wfMathVec4GetY(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(21.0f, wfPrivate::wfMathVec4GetZ(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(28.0f, wfPrivate::wfMathVec4GetW(out)));

    return true;
}

static bool wfMathTestPrivate_wfMathMat4_InverseOrthographicPoint3Mul(wfMathTest *store) {
    wfPrivate::wfMathVec4 vec = wfPrivate::wfMathVec4Create(1.0f, 2.0f, 3.0f, 4.0f);
    wfPrivate::wfMathMat4 mat = wfPrivate::wfMathMat4Create(vec, vec, vec, vec);
    wfPrivate::wfMathVec4 out;

    wfPrivate::wfMathMat4Point3Mul(&mat, &vec, &out);

    WF_MATH_FAIL(wfMathTestSafeCompare(7.0f,  wfPrivate::wfMathVec4GetX(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(14.0f, wfPrivate::wfMathVec4GetY(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(21.0f, wfPrivate::wfMathVec4GetZ(out)));
    WF_MATH_FAIL(wfMathTestSafeCompare(28.0f, wfPrivate::wfMathVec4GetW(out)));

    return true;
}

#undef WF_MATH_FAIL

int main() {
    int ret = EXIT_FAILURE;

    fprintf(stderr, "  Using: %s Path\n\nCPU FEATURES(Host: %s):\n", WF_STDLIB_MATH_PATH_TYPE, EMUTYPE);
	#define B2STR(X) ((X) ? "supported" : "unsupported")
	
	fprintf(stderr, "    RDTSC:   %s\n", B2STR(wfCPUHasRDTSC()));
	fprintf(stderr, "    Altivec: %s\n", B2STR(wfCPUHasALTIVEC()));
	fprintf(stderr, "    MMX:     %s\n", B2STR(wfCPUHasMMX()));
	fprintf(stderr, "    3DNow:   %s\n", B2STR(wfCPUHas3DNOW()));
	fprintf(stderr, "    SSE:     %s\n", B2STR(wfCPUHasSSE()));
	fprintf(stderr, "    SSE2:    %s\n", B2STR(wfCPUHasSSE2()));
	fprintf(stderr, "    SSE3:    %s\n", B2STR(wfCPUHasSSE3()));
	fprintf(stderr, "    SSE4.1:  %s\n", B2STR(wfCPUHasSSE41()));
	fprintf(stderr, "    SSE4.2:  %s\n", B2STR(wfCPUHasSSE42()));
	fprintf(stderr, "    NEON:    %s\n", B2STR(wfCPUHasNEON()));
	
	#undef B2STR
	
	/* Backend */
    wfMathTestsAdd("wfMathVec4: Flip Sign",                         &wfMathTestPrivate_wfMathVec4_FlipSign);
    wfMathTestsAdd("wfMathVec4: Shuffle",                           &wfMathTestPrivate_wfMathVec4_Shuffle);
    wfMathTestsAdd("wfMathVec4: Sum",                               &wfMathTestPrivate_wfMathVec4_Sum);
    wfMathTestsAdd("wfMathVec4: Dot Product",                       &wfMathTestPrivate_wfMathVec4_Dot);
    wfMathTestsAdd("wfMathVec4: Length Of Vector",                  &wfMathTestPrivate_wfMathVec4_Length);
    wfMathTestsAdd("wfMathVec4: Length Of Vector Squared",          &wfMathTestPrivate_wfMathVec4_LengthSquared);
    wfMathTestsAdd("wfMathVec4: Normalize",                         &wfMathTestPrivate_wfMathVec4_Normalize);

    wfMathTestsAdd("wfMathMat4: Create",                            &wfMathTestPrivate_wfMathMat4_Create);
    wfMathTestsAdd("wfMathMat4: Identity",                          &wfMathTestPrivate_wfMathMat4_Identity);
    wfMathTestsAdd("wfMathMat4: UnalignedLoad",                     &wfMathTestPrivate_wfMathMat4_UnalignedLoad);
    wfMathTestsAdd("wfMathMat4: Sum",                               &wfMathTestPrivate_wfMathMat4_Sum);
    wfMathTestsAdd("wfMathMat4: Vec4Mul",                           &wfMathTestPrivate_wfMathMat4_Vec4Mul);
    wfMathTestsAdd("wfMathMat4: Vec3Mul",                           &wfMathTestPrivate_wfMathMat4_Vec3Mul);
    wfMathTestsAdd("wfMathMat4: Point3Mul",                         &wfMathTestPrivate_wfMathMat4_Point3Mul);
    wfMathTestsAdd("wfMathMat4: Inverse Orthographic Point 3 Mul",  &wfMathTestPrivate_wfMathMat4_InverseOrthographicPoint3Mul);

    /* wfVector4f.h */
    wfMathTestsAdd("wfVector4f: Construction",                      &wfMathTestVec4_Construction);
    wfMathTestsAdd("wfVector4f: Addition Of Vector",                &wfMathTestVec4_AdditionVector);
    wfMathTestsAdd("wfVector4f: Subtraction Of Vector",             &wfMathTestVec4_SubtractionVector);
    wfMathTestsAdd("wfVector4f: Multiplication Of Vector",          &wfMathTestVec4_MultiplicationVector);
    wfMathTestsAdd("wfVector4f: Division Of Vector",                &wfMathTestVec4_DivisionVector);
    wfMathTestsAdd("wfVector4f: Addition Of Float",                 &wfMathTestVec4_AdditionFloat);
    wfMathTestsAdd("wfVector4f: Subtraction Of Float",              &wfMathTestVec4_SubtractionFloat);
    wfMathTestsAdd("wfVector4f: Multiplication Of Float",           &wfMathTestVec4_MultiplicationFloat);
    wfMathTestsAdd("wfVector4f: Dot Product",                       &wfMathTestVec4_DotProduct);
	
	/* wfVector3f.h */
    wfMathTestsAdd("wfVector3f: Construction",                      &wfMathTestVec3_Construction);
    wfMathTestsAdd("wfVector3f: Addition Of Vector",                &wfMathTestVec3_AdditionVector);
    wfMathTestsAdd("wfVector3f: Subtraction Of Vector",             &wfMathTestVec3_SubtractionVector);
    wfMathTestsAdd("wfVector3f: Multiplication Of Vector",          &wfMathTestVec3_MultiplicationVector);
    wfMathTestsAdd("wfVector3f: Division Of Vector",                &wfMathTestVec3_DivisionVector);
    wfMathTestsAdd("wfVector3f: Addition Of Float",                 &wfMathTestVec3_AdditionFloat);
    wfMathTestsAdd("wfVector3f: Subtraction Of Float",              &wfMathTestVec3_SubtractionFloat);
    wfMathTestsAdd("wfVector3f: Multiplication Of Float",           &wfMathTestVec3_MultiplicationFloat);
    wfMathTestsAdd("wfVector3f: Dot Product",                       &wfMathTestVec3_DotProduct);
	
	/* wfVector2f.h */
    wfMathTestsAdd("wfVector2f: Construction",                      &wfMathTestVec2_Construction);
    wfMathTestsAdd("wfVector2f: Addition Of Vector",                &wfMathTestVec2_AdditionVector);
    wfMathTestsAdd("wfVector2f: Subtraction Of Vector",             &wfMathTestVec2_SubtractionVector);
    wfMathTestsAdd("wfVector2f: Multiplication Of Vector",          &wfMathTestVec2_MultiplicationVector);
    wfMathTestsAdd("wfVector2f: Division Of Vector",                &wfMathTestVec2_DivisionVector);
    wfMathTestsAdd("wfVector2f: Addition Of Float",                 &wfMathTestVec2_AdditionFloat);
    wfMathTestsAdd("wfVector2f: Subtraction Of Float",              &wfMathTestVec2_SubtractionFloat);
    wfMathTestsAdd("wfVector2f: Multiplication Of Float",           &wfMathTestVec2_MultiplicationFloat);
    wfMathTestsAdd("wfVector2f: Dot Product",                       &wfMathTestVec2_DotProduct);

    if (wfMathTestsRun())
        ret = EXIT_SUCCESS;

    wfMathTestsCleanup();
	
	fprintf(stderr, "\n");
    return ret;
}
