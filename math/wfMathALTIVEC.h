#ifndef WF_STDLIB_MATH_ALTIVEC_HDR
#define WF_SRDLIB_MATH_ALTIVEC_HDR

#include <altivec.h>

namespace wfPrivate {
    typedef vector float wfMathVec4;

    struct wfMathMat4 {
        wfMathVec4 x;
        wfMathVec4 y;
        wfMathVec4 z;
        wfMathVec4 w;
    };

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Create(float x, float y, float z, float w) {
        wfMathVec4 data = {x, y, z, w};
        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Zero() {
        wfMathVec4 data = {0.0f, 0.0f, 0.0f, 0.0f};
        return data;
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad(const float *array);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<4>(const float *array) {
        wfMathVec4 data = {array[0], array[1], array[2], array[3]};
        return data;
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<3>(const float *array) {
        wfMathVec4 data = {array[0], array[1], array[2], 0.0f};
        return data;
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<2>(const float *array) {
        wfMathVec4 data = {array[0], array[1], 0.0f, 0.0f};
        return data;
    }


    template <size_t E>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore(const wfMathVec4& value, float *array);

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<4>(const wfMathVec4& value, float *array) {
        vec_st(value, 0, array);
    }

    /*
     * There must be a better way of doing this?  We'll find out some day
     * so far this is all I can think of.
     */   
    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<3>(const wfMathVec4& value, float *array) {
        float WF_MATH_VEC_ALIGNED16 data[4];
        wfMathVec4UnalignedStore<4>(value, data);
        memcpy(array, data, sizeof(float) * 3);
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<2>(const wfMathVec4& value, float *array) {
        float WF_MATH_VEC_ALIGNED16 data[4];
        wfMathVec4UnalignedStore<4>(value, data);
        memcpy(array, data, sizeof(float) * 2);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Single(float x) {
        wfMathVec4 data = {x, x, x, x};
        return data;
    };

    enum wfMathVec4Splat {
        VEC_SPLAT_X,
        VEC_SPLAT_Y,
        VEC_SPLAT_Z,
        VEC_SPLAT_W
    };


    template <wfMathVec4Splat E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat(const wfMathVec4& data) {     
        return vec_splat(data, E);
    }


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Add(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vec_add(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sub(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vec_sub(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Mul(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vec_madd(lhs, rhs, wfMathVec4Zero());
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Div(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        /*
         * est is | (r-1/x) / (1/x) | ≤ 1/256, where x is the value of
         * each element of a and r is est
         */  
        const wfMathVec4 est    = vec_re(rhs);
        const wfMathVec4 newton =
        vec_madd(                           /* := ?                     * ?   + ?   */
            vec_nmsub(                      /* := -(?    * ?    - ?)    * ?   + ?   */
                est,                        /* := -(est  * ?    - ?)    * ?   + ?   */
                rhs,                        /* := -(est  * lhs  - ?)    * ?   + ?   */
                wfMathVec4Single(1.0f)),    /* := -(est  * lhs  - 1.0f) * ?   + ?   */
            est,                            /* := -(est  * lhs  - 1.0f) * est + ?   */
            est                             /* := -(est  * lhs  - 1.0f) * est + est */
        );

        return vec_madd(
            lhs,
            vec_sel(est, newton, vec_cmpeq(newton, newton)),
            wfMathVec4Zero()
        );
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4MulAdd(const wfMathVec4& m1, const wfMathVec4& m2, const wfMathVec4& rhs) {
        return vec_madd(m1, m2, rhs);
    }


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Reciprocal(const wfMathVec4& vec) {
        wfMathVec4              rcp = vec_re(vec);
        static const wfMathVec4 two = wfMathVec4Create(2.0f, 2.0f, 2.0f, 2.0f);

        /* TODO: Optimize .. ME PLEASE! */
        return wfMathVec4Mul(
            rcp,
            wfMathVec4Sub(
                two,
                wfMathVec4Mul(
                    vec,
                    rcp
                )
            )
        );
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sqrt(const wfMathVec4& vec) {
        /*
         * rsqrt is: | (r-1/√x) / 1/√x | ≤ 1/32 where x is a value of
         * each element of a, and r is the return value.
         */ 
        const wfMathVec4 rsqrt = vec_rsqrte(vec);
        return vec_madd(                        /* := ?   * ?                                                                           + ?    */
                vec,                            /* := vec * ?                                                                           + ?    */
                vec_madd(                       /* := vec * ( (?                                      * ?                    + ?) )     + ?    */
                    vec_nmsub(                  /* := vec * ( (-(?   * ?                      - ?)    * ?                    + ?) )     + ?    */
                        vec,                    /* := vec * ( (-(vec * ?                      - ?)    * ?                    + ?) )     + ?    */
                        vec_madd(               /* := vec * ( (-(vec * (?     * ?     + ?)    - ?)    * ?                    + ?) )     + ?    */
                            rsqrt,              /* := vec * ( (-(vec * (rsqrt * ?     + ?)    - ?)    * ?                    + ?) )     + ?    */
                            rsqrt,              /* := vec * ( (-(vec * (rsqrt * rsqrt + ?)    - ?)    * ?                    + ?) )     + ?    */
                            wfMathVec4Zero()),  /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - ?)    * ?                    + ?) )     + ?    */
                        wfMathVec4Single(1.0f)),/* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * ?                    + ?) )     + ?    */
                    vec_madd(                   /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (?     * ?    + ?)   + ?) )     + ?    */
                        rsqrt,                  /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (rsqrt * ?    + ?)   + ?) )     + ?    */
                        wfMathVec4Single(0.5f), /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (rsqrt * 0.5f + ?)   + ?) )     + ?    */
                        wfMathVec4Zero()),      /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (rsqrt * 0.5f + 0.0) + ?) )     + ?    */
                    rsqrt),                     /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (rsqrt * 0.5f + 0.0) + rsqrt) ) + ?    */
                wfMathVec4Zero()                /* := vec * ( (-(vec * (rsqrt * rsqrt + 0.0f) - 1.0f) * (rsqrt * 0.5f + 0.0) + rsqrt) ) + 0.0f */
        );

        /* same as saying this: vec*((-(vec*(rsqrt*rsqrt)-1.0f)*(rsqrt*0.5f)+rsqrt)) */
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4RSqrt(const wfMathVec4& vec) {
        const wfMathVec4 est = vec_rsqrte(vec);

        return vec_madd (
            vec_nmsub (
                vec,
                vec_madd (
                    est,
                    est,
                    wfMathVec4Zero()
                ),
                wfMathVec4Single(1.0f)
            ),
            vec_madd(
                est,
                wfMathVec4Single(0.5f),
                wfMathVec4Zero  ()
            ),
            est
        );
    }


    /* Reverse Operands to work like _MM_SHUFFLE */
    #define VEC_SHUFFLE(D, C, B, A)                 \
        (vector unsigned char){                     \
            (A)*4, (A)*4+1, (A)*4+2, (A)*4+3,       \
            (B)*4, (B)*4+1, (B)*4+2, (B)*4+3,       \
            (C)*4, (C)*4+1, (C)*4+2, (C)*4+3,       \
            (D)*4, (D)*4+1, (D)*4+2, (D)*4+3,       \
        }

    #define VEC_EXTRACT_LANE(VEC, LANE)             \
        do {                                        \
            float WF_MATH_VEC_ALIGNED16 data[4];    \
            wfMathVec4UnalignedStore<4>(VEC, data); \
            return data[LANE];                      \
        } while (0)

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Cross3(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        const wfMathVec4 lyzx = vec_perm(lhs, rhs, VEC_SHUFFLE(3,0,2,1));
        const wfMathVec4 lzxy = vec_perm(lhs, rhs, VEC_SHUFFLE(3,1,0,2));
        const wfMathVec4 ryzx = vec_perm(lhs, rhs, VEC_SHUFFLE(3,0,2,1));
        const wfMathVec4 rzxy = vec_perm(lhs, rhs, VEC_SHUFFLE(3,1,0,2));

        return vec_sub(
            vec_madd(
                lyzx,
                rzxy,
                wfMathVec4Zero()
            ),
            vec_madd(
                lzxy,
                ryzx,
                wfMathVec4Zero()
            )
        );
    }
    
    /* Getters */
    WF_MATH_VEC_INLINE float wfMathVec4GetX(const wfMathVec4 &vec) { VEC_EXTRACT_LANE(vec, 0); }
    WF_MATH_VEC_INLINE float wfMathVec4GetY(const wfMathVec4 &vec) { VEC_EXTRACT_LANE(vec, 1); }
    WF_MATH_VEC_INLINE float wfMathVec4GetZ(const wfMathVec4 &vec) { VEC_EXTRACT_LANE(vec, 2); }
    WF_MATH_VEC_INLINE float wfMathVec4GetW(const wfMathVec4 &vec) { VEC_EXTRACT_LANE(vec, 3); }

    /* Min/Max */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Min(const wfMathVec4& a, const wfMathVec4& b) {
        return vec_min(a, b);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Max(const wfMathVec4& a, const wfMathVec4& b) {
        return vec_max(a, b);
    }

    enum wfMathVec4Shuffle {
        VEC_SHUFFLE_WXYZ,
        VEC_SHUFFLE_ZWXY,
        VEC_SHUFFLE_YZWX
    };

    template <wfMathVec4Shuffle E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle(const wfMathVec4& mat);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(const wfMathVec4& mat) {
        return vec_perm(mat, mat, VEC_SHUFFLE(2, 1, 0, 3));
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(const wfMathVec4& mat) {
        return vec_perm(mat, mat, VEC_SHUFFLE(1, 0, 3, 2));
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(const wfMathVec4& mat) {
        return vec_perm(mat, mat, VEC_SHUFFLE(0, 3, 2, 1));
    }

    #undef VEC_SHUFFLE
    #undef VEC_EXTRACT_LANE

    typedef WF_MATH_VEC_ALIGNED16 union {
        unsigned int ui[4];
        float         f[4];
    } wfMathVec4UnionSign;

    enum wfMathFlipSign {
        VEC_SIGN_0101,
        VEC_SIGN_1010
    };

    template<wfMathFlipSign E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign(const wfMathVec4& vec);

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_0101>(const wfMathVec4& mat) {
        const wfMathVec4UnionSign u = {{0x00000000, 0x80000000, 0x00000000, 0x80000000}};
        return vec_xor(mat, wfMathVec4UnalignedLoad<4>(u.f));
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_1010>(const wfMathVec4& mat) {
        const wfMathVec4UnionSign u = {{0x80000000, 0x00000000, 0x80000000, 0x00000000}};
        return vec_xor(mat, wfMathVec4UnalignedLoad<4>(u.f));
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Merge(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        static const vector unsigned char build = (vector unsigned char){
            0,  1,  2,  3,
            4,  5,  6,  7,
            16, 17, 18, 18,
            20, 21, 22, 23
        };

        // try lhs,rhs incase
        return vec_perm(rhs, lhs, build);
    }

    /*
     * Matrix mainpulation is dead fucking easy in SSE.  It's why I love
     * SSE just so effn much.
     */
    WF_MATH_VEC_INLINE
    void wfMathMat4TransposeInplace(wfMathMat4 *mat) {
        wfMathVec4 tmp3 = vec_mergel(mat->y, mat->w);
        wfMathVec4 tmp2 = vec_mergeh(mat->y, mat->w);
        wfMathVec4 tmp1 = vec_mergel(mat->x, mat->z);
        wfMathVec4 tmp0 = vec_mergeh(mat->x, mat->z);
        mat->x          = vec_mergeh(tmp0, tmp2);
        mat->y          = vec_mergel(tmp0, tmp2);
        mat->z          = vec_mergeh(tmp1, tmp3);
        mat->w          = vec_mergel(tmp1, tmp3);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Transpose(const wfMathMat4 *mat, wfMathMat4 *out) {
        *out = *mat;
        wfMathMat4TransposeInplace(out);
    }
}

#endif /*! WF_STDLIB_MATH_ALTIVEC_HDR */
