#ifndef WFSTDLIB_MATH_HDR
#define WFSTDLIB_MATH_HDR

#if defined(__GNUC__) && !defined(__arm__) && defined(WF_STDLIB_MATH_AUTOVECTORIZE)
#   define WF_STDLIB_MATH_PATH_AUTOVECTORIZATION
#   define WF_STDLIB_MATH_PATH_TYPE "Auto-Vectorization"
#elif defined(__SSE__) || (_M_IX86_FP > 0)
#   define WF_STDLIB_MATH_PATH_SSE

    /*
     * Supporting any other varation of SSE is a waste of effort.  It's
     * irrelevant since we're not operating on data that will fill the
     * SIMD units enough to take advantage of SSE3+ operations.
     */
#   define WF_STDLIB_MATH_PATH_TYPE "SSE2"
#elif defined(__ARM_NEON__)
#   define WF_STDLIB_MATH_PATH_NEON
#   define WF_STDLIB_MATH_PATH_TYPE "NEON"

/*
 * Interestingly enough GCC is the only compiler that provided builtins
 * that can do _proper_ portable (within confining yourself to GCC)
 * vectorization.
 *
 * Sadly the supoprt on arm for this is timid at best.  There have been
 * noumours reports of bugs.
 */
#elif defined(__ppc__) || defined(__PPC__)
#   define WF_STDLIB_MATH_PATH_ALTIVEC
#   define WF_STDLIB_MATH_PATH_TYPE "ALTIVEC"
#else
#   define WF_STDLIB_MATH_PATH_SCALAR
#   define WF_STDLIB_MATH_PATH_TYPE "SCALAR"
#endif


/*
 * Handle all the annoying alignment macros and "restrict" varations that
 * ensure restrict qualification on types.
 */
#if defined(__GNUC__)
#   if defined(__cplusplus)
#       define WF_MATH_VEC_RESTRICT __restrict
#   endif
#   define WF_MATH_VEC_ALIGNED16    __attribute__((__aligned__(16)))
#elif defined (_WIN32)
#   define WF_MATH_VEC_RESTRICT
#   define WF_MATH_VEC_ALIGNED16    __declspec(aligned(16))
#else
#   define WF_MATH_VEC_RESTRICT      restrict
#   define WF_MATH_VEC_ALIGNED16
#endif

/*
 * Pure hinting for compiler convincing (useless on anything other than
 * GCC-like compilers).
 */
#if defined(__GNUC__)
#   define WF_MATH_VEC_HINT_PURE   __attribute__((__pure__))
#else
#   define WF_MATH_VEC_HINT_PURE
#endif

/*
 * Some constants for tasty pi..
 */

#define WF_MATH_VEC_CONST_PI       3.14159265f
#define WF_MATH_VEC_CONST_HALFPI   1.57079633f

#define WF_MATH_VEC_INLINE         inline

#include <cstring>
#include <cstdlib>
#include <cmath>

#ifdef WF_STDLIB_MATH_PATH_SCALAR
#   include "math/wfMathSCALAR.h"
#elif defined(WF_STDLIB_MATH_PATH_SSE)
#   include "math/wfMathSSE.h"
#elif defined(WF_STDLIB_MATH_PATH_NEON)
#   include "math/wfMathNEON.h"
#elif defined(WF_STDLIB_MATH_PATH_ALTIVEC)
#   include "math/wfMathALTIVEC.h"
#elif defined(WF_STDLIB_MATH_PATH_AUTOVECTORIZATION)
#   include "math/wfMathAUTOVEC.h"
#endif

#ifdef WF_STDLIB_MATH_NEON
#   define WF_SELECTIVE_NEON_WORKAROUND(SANE, WORKAROUND) \
        do {                                              \
            WORKAROUND;                                   \
        } while (0)
#else
#   define WF_SELECTIVE_NEON_WORKAROUND(SANE, WORKAROUND) \
        do {                                              \
          SANE;                                           \
        } while (0)
#endif /*! WF_STDLIB_MATH_NEON */

/*
 * These are common operations that can be performed without the help of
 * SIMD operations.
 */
namespace wfPrivate {
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sum(const wfMathVec4& vec) {
        return wfMathVec4Add(
            wfMathVec4Add(
                wfMathVec4Add(
                    wfMathVec4Splat<VEC_SPLAT_X>(vec),
                    wfMathVec4Splat<VEC_SPLAT_Y>(vec)
                ),
                wfMathVec4Splat<VEC_SPLAT_Z>(vec)
            ),
            wfMathVec4Splat<VEC_SPLAT_W>(vec)
        );
    }

    /* Dot products */
    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Dot(const wfMathVec4& lhs, const wfMathVec4& rhs);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Dot<4>(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return wfMathVec4Sum(
            wfMathVec4Mul(lhs, rhs)
        );
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Dot<3>(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        /* store for less computations */
        const wfMathVec4 mult = wfMathVec4Mul(lhs, rhs);

        return wfMathVec4Add(
            wfMathVec4Add(
                wfMathVec4Splat<VEC_SPLAT_X>(mult),
                wfMathVec4Splat<VEC_SPLAT_Y>(mult)
            ),
            wfMathVec4Splat<VEC_SPLAT_Z>(mult)
        );
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Dot<2>(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        /* store for less computations */
        const wfMathVec4 mult = wfMathVec4Mul(lhs, rhs);

        return wfMathVec4Add (
            wfMathVec4Splat<VEC_SPLAT_X>(mult),
            wfMathVec4Splat<VEC_SPLAT_Y>(mult)
        );
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Length(const wfMathVec4& vec) {
        return wfMathVec4Sqrt(wfMathVec4Dot<E>(vec, vec));
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4LengthSquared(const wfMathVec4& vec) {
        return wfMathVec4RSqrt(wfMathVec4Dot<E>(vec, vec));
    }


    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Normalize(const wfMathVec4& vec) {
        return wfMathVec4Mul(
            vec,
            wfMathVec4RSqrt(wfMathVec4Dot<E>(vec, vec))
        );
    }

    /*
     * Common 4x4 Matrices functions and what not implemented from the
     * SIMD stuff included earlier.
     */
    WF_MATH_VEC_INLINE
    wfMathMat4 wfMathMat4Create(
        const wfMathVec4& x,
        const wfMathVec4& y,
        const wfMathVec4& z,
        const wfMathVec4& w
    ) {
        wfMathMat4 data = { x, y, z, w };
        return data;
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Identity(wfMathMat4 *mat) {
        /*
         * Store static constant copy to prevent extra overhead of
         * creating an identity matrix each call to this function.
         */
        static const wfMathVec4 identity[] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        /* TODO: profile if it's cheaper to memcpy into */
        *mat = wfMathMat4Create(
                   identity[0],
                   identity[1],
                   identity[2],
                   identity[3]
        );
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4UnalignedLoad(wfMathMat4 *mat, const float *data) {
        mat->x = wfMathVec4UnalignedLoad<4>(data + 0);
        mat->y = wfMathVec4UnalignedLoad<4>(data + 4);
        mat->z = wfMathVec4UnalignedLoad<4>(data + 8);
        mat->w = wfMathVec4UnalignedLoad<4>(data + 12);
    }


    /*
     * All the functionality below is "generic" for 4x4 matrices.  There
     * is no requirement to further optimize this with SIMD since the
     * inline behaviour of the generic functions allows for clean, efficent
     * pipelinable SIMD code.
     */
    WF_MATH_VEC_INLINE
    wfMathMat4 wfMathMat4Sum(const wfMathMat4* lhs, wfMathVec4* out) {
        /* *out := x + y + z + w */
        *out = wfMathVec4Add(
                    wfMathVec4Add(
                        wfMathVec4Add(
                            lhs->x,
                            lhs->y
                        ),
                        lhs->z
                    ),
                lhs->w
            );

        /* return; */
    }

    /*
     * Matrix multiplication is associative, in the sense that:
     * cik = aij bjk,
     * where j is summed over for all possible elemental values of i and k
     * (the above notation is Einstien summation convention).
     *
     * Hence This must satisfiy (n * m)(m * p) = (n * p)
     */
    WF_MATH_VEC_INLINE
    void wfMathMat4Vec4Mul(const wfMathMat4* mat, const wfMathVec4* vec, wfMathVec4 *out) {
        /*
         * Okay this was such a piss off, I tried really hard getting this in as
         * few LOADs as possible.  It seems that GCC and MSVC enjoy fucking around
         * with their CSE in ways that just KILLS.  So mind the agressive loads
         * I had no choice.
         */
        const wfMathVec4 load_x  = mat->x;
        const wfMathVec4 load_y  = mat->y;
        const wfMathVec4 load_z  = mat->z;
        const wfMathVec4 load_w  = mat->w;

        const wfMathVec4 load    = *vec;

        /*
         * Splatter calculations across the whole vec4 spectrum.  It's
         * amazing how clean and efficent this is.
         */
        const wfMathVec4 splat_x = wfMathVec4Splat<VEC_SPLAT_X>(load);
        const wfMathVec4 splat_y = wfMathVec4Splat<VEC_SPLAT_Y>(load);
        const wfMathVec4 splat_z = wfMathVec4Splat<VEC_SPLAT_Z>(load);
        const wfMathVec4 splat_w = wfMathVec4Splat<VEC_SPLAT_W>(load);

        WF_SELECTIVE_NEON_WORKAROUND(
            *out =  wfMathVec4MulAdd(load_x, splat_x,
                    wfMathVec4MulAdd(load_y, splat_y,
                    wfMathVec4MulAdd(load_z, splat_z,
                    wfMathVec4Mul   (load_w, splat_w)))
            ),
            *out =  wfMathVec4Add(wfMathVec4Mul(load_x, splat_x),
                    wfMathVec4Add(wfMathVec4Mul(load_y, splat_y),
                    wfMathVec4Add(wfMathVec4Mul(load_z, splat_z),
                                  wfMathVec4Mul(load_w, splat_w)))
            )
        );

        WF_SELECTIVE_NEON_WORKAROUND(
            *out =  wfMathVec4MulAdd(load_x, splat_x,
                    wfMathVec4MulAdd(load_y, splat_y,
                    wfMathVec4MulAdd(load_z, splat_z,
                    wfMathVec4Mul   (load_w, splat_w)))
            ),
            *out =  wfMathVec4Add(wfMathVec4Mul(load_x, splat_x),
                    wfMathVec4Add(wfMathVec4Mul(load_y, splat_y),
                    wfMathVec4Add(wfMathVec4Mul(load_z, splat_z),
                                  wfMathVec4Mul(load_w, splat_w)))
            )
        );
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Vec3Mul(const wfMathMat4* mat, const wfMathVec4* vec, wfMathVec4 *out) {
        WF_SELECTIVE_NEON_WORKAROUND(
            *out = wfMathVec4MulAdd(mat->x, wfMathVec4Splat<VEC_SPLAT_X>(*vec),
                   wfMathVec4MulAdd(mat->y, wfMathVec4Splat<VEC_SPLAT_Y>(*vec),
                   wfMathVec4Mul   (mat->z, wfMathVec4Splat<VEC_SPLAT_Z>(*vec)))
            ),
            *out = wfMathVec4Add (wfMathVec4Mul(mat->x, wfMathVec4Splat<VEC_SPLAT_X>(*vec)),
                   wfMathVec4Add (wfMathVec4Mul(mat->y, wfMathVec4Splat<VEC_SPLAT_Y>(*vec)),
                   wfMathVec4Mul               (mat->z, wfMathVec4Splat<VEC_SPLAT_Z>(*vec)))
            )
        );
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Point3Mul(const wfMathMat4* mat, const wfMathVec4* vec, wfMathVec4 *out) {
        WF_SELECTIVE_NEON_WORKAROUND(
            *out =  wfMathVec4MulAdd(mat->x, wfMathVec4Splat<VEC_SPLAT_X>(*vec),
                    wfMathVec4MulAdd(mat->y, wfMathVec4Splat<VEC_SPLAT_Y>(*vec),
                    wfMathVec4MulAdd(mat->z, wfMathVec4Splat<VEC_SPLAT_Z>(*vec),
                    mat->w))
            ),
            *out =  wfMathVec4Add(wfMathVec4Mul(mat->x, wfMathVec4Splat<VEC_SPLAT_X>(*vec)),
                    wfMathVec4Add(wfMathVec4Mul(mat->y, wfMathVec4Splat<VEC_SPLAT_Y>(*vec)),
                    wfMathVec4Add(wfMathVec4Mul(mat->z, wfMathVec4Splat<VEC_SPLAT_Z>(*vec)),
                    mat->w))
            )
        );
    }

    /*
     * All the important stuff like inverse orthgraphic projection and
     * perspective stuff
     */
    WF_MATH_VEC_INLINE
    void wfMathMat4InverseOrthographicPoint3Mul(const wfMathMat4* mat, const wfMathVec4 *vec, wfMathVec4 *out) {
        wfMathVec4 translation   = wfMathVec4Sub(*vec, mat->w);
        wfMathMat4 transposition = *mat;
        transposition.w          = wfMathVec4Create(0.0f, 0.0f, 0.0f, 0.0f);


        wfMathMat4TransposeInplace(&transposition);
        wfMathMat4Point3Mul       (&transposition, &translation, out);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4InverseOrthographicVec3Mul(const wfMathMat4* mat, const wfMathVec4 *vec, wfMathVec4 *out) {
        wfMathVec4 translation   = *vec;
        wfMathMat4 transposition = *mat;
        transposition.w          = wfMathVec4Create(0.0f, 0.0f, 0.0f, 0.0f);


        wfMathMat4TransposeInplace(&transposition);
        wfMathMat4Vec3Mul         (&transposition, &translation, out);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Mul(const wfMathMat4 *m1, const wfMathMat4 *m2, wfMathMat4 *out) {
        wfMathMat4Vec4Mul(m1, &m2->x, &out->x);
        wfMathMat4Vec4Mul(m1, &m2->y, &out->y);
        wfMathMat4Vec4Mul(m1, &m2->z, &out->z);
        wfMathMat4Vec4Mul(m1, &m2->w, &out->w);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Perspective(wfMathMat4 *mat, float rad, float asp, float near, float far) {
        float delta = far - near;
        float cotan = tanf(WF_MATH_VEC_CONST_HALFPI - rad * 0.5f);
        float x     = cotan / asp;
        float y     = cotan;
        float z     = -(far + near) / delta;
        float w     = -2 * near * far / delta;
        mat->x      = wfMathVec4Create(x,    0.0f, 0.0f, 0.0f);
        mat->y      = wfMathVec4Create(0.0f, y,    0.0f, 0.0f);
        mat->z      = wfMathVec4Create(0.0f, 0.0f, z,   -1.0f);
        mat->w      = wfMathVec4Create(0.0f, 0.0f, w,    0.0f);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Orthographic(wfMathMat4 *mat, float l, float r, float b, float t, float n, float f) {
        float deltax = r-l;
        float deltay = t-b;
        float deltaz = f-n;
        float x      = 2.0f   / deltax;
        float w0     = -(r+l) / deltax;
        float y      = 2.0f   / deltay;
        float w1     = -(t+b) / deltay;
        float z      = 2.0f   / deltaz;
        float w2     = -(f+n) / deltax;
        mat->x       = wfMathVec4Create(x,    0.0f, 0.0f, 0.0f);
        mat->y       = wfMathVec4Create(0.0f, y,    0.0f, 0.0f);
        mat->z       = wfMathVec4Create(0.0f, 0.0f, z,    0.0f);
        mat->w       = wfMathVec4Create(w0,   w1,   w2,   1.0f);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Look(wfMathMat4 *mat, const wfMathVec4& eye, const wfMathVec4& center, const wfMathVec4& up) {
        wfMathVec4 az = wfMathVec4Normalize<3>(wfMathVec4Sub(center, eye));
        wfMathVec4 ax = wfMathVec4Normalize<3>(wfMathVec4Cross3(az, up));
        wfMathVec4 ay = wfMathVec4Cross3      (ax, az);
        float      x;
        float      y;
        float      z;

        az =  wfMathVec4Sub (wfMathVec4Zero(), az);
        x  = -wfMathVec4GetX(wfMathVec4Dot<3>(ax, eye));
        y  = -wfMathVec4GetX(wfMathVec4Dot<3>(ay, eye));
        z  = -wfMathVec4GetX(wfMathVec4Dot<3>(az, eye));

        mat->x = ax;
        mat->y = ay;
        mat->z = az;

        static const wfMathVec4 transpose = wfMathVec4Create(0, 0, 0, 1);

        /* Transposition */
        mat->w = transpose;
            wfMathMat4TransposeInplace(mat);
        mat->w = wfMathVec4Create(x,    y,    z,    1.0f);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Translation(wfMathMat4 *mat, float x, float y, float z) {
        /*
         * Save some extra possible extra constructions.  In reality the
         * calls to wfMathMat4Create with constant arguments should be
         * inlined, but we cannot trust it.
         */
        static const wfMathVec4 translation[] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
        };

        *mat = wfMathMat4Create (
            translation[0],
            translation[1],
            translation[2],
            wfMathVec4Create(x, y, z, 1.0f)
        );
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4AxisRotation(wfMathMat4 *mat, float rad, wfMathVec4 axis) {

        rad  = -rad;                          /* inverse sign */
        axis = wfMathVec4Normalize<3>(axis);  /* normalize    */

        const float sine   = sinf(rad);
        const float cosine = cosf(rad);
        const float loadx  = wfMathVec4GetX(axis);
        const float loady  = wfMathVec4GetY(axis);
        const float loadz  = wfMathVec4GetZ(axis);
        const float loada  = loadx * loady * (1 - cosine);
        const float loadb  = loady * loadz * (1 - cosine);
        const float loadc  = loadz * loadx * (1 - cosine);
        const float sqx    = loadx * loadx;
        const float sqy    = loady * loady;
        const float sqz    = loadz * loadz;

        const wfMathVec4 storei = wfMathVec4Create(
            sqx   + cosine * (1 - sqx), /* x */
            loada - loadz  * sine,      /* y */
            loadc + loady  * sine,      /* z */
            0
        );

        const wfMathVec4 storej = wfMathVec4Create(
            loada + loadz  * sine,      /* x */
            sqy   + cosine * (1 - sqy), /* y */
            loadc - loadx  * sine,      /* z */
            0
        );

        const wfMathVec4 storek = wfMathVec4Create(
            loadc - loady  * sine,      /* x */
            loadb + loadx  * sine,      /* y */
            sqz   + cosine * (1 - sqz), /* z */
            0
        );

        *mat = wfMathMat4Create(
            storei,
            storej,
            storek,
            wfMathVec4Create(0.0f, 0.0f, 0.0f, 1.0f)
        );
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Add(wfMathMat4 *a, wfMathMat4 *b, wfMathMat4 *out) {
        out->x = wfMathVec4Add(a->x, b->x);
        out->y = wfMathVec4Add(a->y, b->y);
        out->z = wfMathVec4Add(a->z, b->z);
        out->w = wfMathVec4Add(a->w, b->w);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Sub(wfMathMat4 *a, wfMathMat4 *b, wfMathMat4 *out) {
        out->x = wfMathVec4Sub(a->x, b->x);
        out->y = wfMathVec4Sub(a->y, b->y);
        out->z = wfMathVec4Sub(a->z, b->z);
        out->w = wfMathVec4Sub(a->w, b->w);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Mul(wfMathMat4 *a, wfMathMat4 *b, wfMathMat4 *out) {
        out->x = wfMathVec4Mul(a->x, b->x);
        out->y = wfMathVec4Mul(a->y, b->y);
        out->z = wfMathVec4Mul(a->z, b->z);
        out->w = wfMathVec4Mul(a->w, b->w);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Div(wfMathMat4 *a, wfMathMat4 *b, wfMathMat4 *out) {
        out->x = wfMathVec4Div(a->x, b->x);
        out->y = wfMathVec4Div(a->y, b->y);
        out->z = wfMathVec4Div(a->z, b->z);
        out->w = wfMathVec4Div(a->w, b->w);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Inverse(const wfMathMat4 *mat, wfMathMat4 *out) {
        const wfMathVec4 load0 = mat->x;
        const wfMathVec4 load1 = mat->y;
        const wfMathVec4 load2 = mat->z;
        const wfMathVec4 load3 = mat->w;
        const wfMathVec4 wxyz0 = wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(load0);
        const wfMathVec4 zwxy0 = wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(load0);
        const wfMathVec4 yzwx0 = wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(load0);
        const wfMathVec4 wxyz1 = wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(load1);
        const wfMathVec4 zwxy1 = wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(load1);
        const wfMathVec4 yzwx1 = wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(load1);
        const wfMathVec4 wxyz2 = wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(load2);
        const wfMathVec4 zwxy2 = wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(load2);
        const wfMathVec4 yzwx2 = wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(load2);
        const wfMathVec4 wxyz3 = wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(load3);
        const wfMathVec4 zwxy3 = wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(load3);
        const wfMathVec4 yzwx3 = wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(load3);
        const wfMathVec4 comp1 = wfMathVec4Mul(wxyz0, load1);
        const wfMathVec4 comp2 = wfMathVec4Mul(wxyz0, yzwx1);
        const wfMathVec4 comp3 = wfMathVec4Mul(wxyz0, zwxy1);
        const wfMathVec4 comp4 = wfMathVec4Mul(wxyz2, load3);
        const wfMathVec4 comp5 = wfMathVec4Mul(wxyz2, yzwx3);
        const wfMathVec4 comp6 = wfMathVec4Mul(wxyz2, zwxy3);
        const wfMathVec4 ar1   = wfMathVec4Sub(wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(comp6), wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(comp4));
        const wfMathVec4 ar2   = wfMathVec4Sub(wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(comp5), comp5);
        const wfMathVec4 ar3   = wfMathVec4Sub(comp6,                                      wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(comp4));
        const wfMathVec4 br1   = wfMathVec4Sub(wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(comp3), wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(comp1));
        const wfMathVec4 br2   = wfMathVec4Sub(wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(comp2), comp2);
        const wfMathVec4 br3   = wfMathVec4Sub(comp3,                                      wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(comp1));
        const wfMathVec4 sum0  = wfMathVec4MulAdd(yzwx0, ar3, wfMathVec4MulAdd(zwxy0, ar2, wfMathVec4Mul(wxyz0, ar1)));
        const wfMathVec4 sum1  = wfMathVec4MulAdd(wxyz1, ar1, wfMathVec4MulAdd(zwxy1, ar2, wfMathVec4Mul(yzwx1, ar3)));
        const wfMathVec4 sum2  = wfMathVec4MulAdd(yzwx2, br3, wfMathVec4MulAdd(zwxy2, br2, wfMathVec4Mul(wxyz2, br1)));
        const wfMathVec4 sum3  = wfMathVec4MulAdd(wxyz3, br1, wfMathVec4MulAdd(zwxy3, br2, wfMathVec4Mul(yzwx3, br3)));
        const wfMathVec4 d0    = wfMathVec4Mul(sum1, load0);
        const wfMathVec4 d1    = wfMathVec4Add(d0, wfMathVec4Merge(d0, d0));
        const wfMathVec4 d3    = wfMathVec4Sub(d1, wfMathVec4Splat<VEC_SPLAT_Y>(d1));
        const wfMathVec4 inv   = wfMathVec4Splat<VEC_SPLAT_X>(wfMathVec4Div(wfMathVec4Single(1.0f), d3));
        const wfMathVec4 out0  = wfMathVec4Mul(wfMathVec4FlipSign<VEC_SIGN_0101>(sum1), inv);
        const wfMathVec4 out1  = wfMathVec4Mul(wfMathVec4FlipSign<VEC_SIGN_1010>(sum0), inv);
        const wfMathVec4 out2  = wfMathVec4Mul(wfMathVec4FlipSign<VEC_SIGN_0101>(sum3), inv);
        const wfMathVec4 out3  = wfMathVec4Mul(wfMathVec4FlipSign<VEC_SIGN_1010>(sum2), inv);
        const wfMathMat4 omat  = wfMathMat4Create(out0, out1, out2, out3);

        wfMathMat4Transpose(&omat, out);
    }
}
#endif
