#ifndef WF_STDLIB_MATH_SSE_HDR
#define WF_STDLIB_MATH_SSE_HDR

#include <xmmintrin.h>

namespace wfPrivate {
    typedef __m128 wfMathVec4;

    union wfMathVec4Union {
        wfMathVec4 v;
        float      f [4];
        unsigned   ui[4];
    };

    struct wfMathMat4 {
        wfMathVec4 x;
        wfMathVec4 y;
        wfMathVec4 z;
        wfMathVec4 w;
    };

    enum wfMathVec4Splat {
        VEC_SPLAT_X,
        VEC_SPLAT_Y,
        VEC_SPLAT_Z,
        VEC_SPLAT_W
    };


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Create(float x, float y, float z, float w) {
        wfMathVec4 data = { x, y, z, w };
        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Zero() {
        return _mm_setzero_ps();
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad(const float *array);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<4>(const float *array) {
        return  _mm_loadu_ps(array);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<3>(const float *array) {
        return wfMathVec4Create(array[0], array[1], array[2], 0.0f);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<2>(const float *array) {
        return wfMathVec4Create(array[0], array[1], 0.0f, 0.0f);
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore(const wfMathVec4& value, float *array);

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<4>(const wfMathVec4& value, float *array) {
        _mm_storeu_ps(array, value);
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<3>(const wfMathVec4& value, float *array) {
        memcpy(array, &value, sizeof(float) * 3);
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<2>(const wfMathVec4& value, float *array) {
        memcpy(array, &value, sizeof(float) * 2);
    }

    /* Utilities */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Single(float data) {
        return _mm_set1_ps(data);
    }

    template <wfMathVec4Splat E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat(const wfMathVec4& data) {
        /*
         * Let me explain how this works, since it looks sort of cryptic:
         * but it does make for some clean code ;)
         *
         * splat_x is equal to shuffle with (0, 0, 0, 0)
         * splat_y is equal to shuffle with (1, 1, 1, 1)
         * splat_z is equal to shuffle with (2, 2, 2, 2)
         * splat_w is equal to shuffle with (3, 3, 3, 3)
         *
         * The wfMathVec4Splat enumeration defines the correct order
         * for splat with X=0, Y=1, Z=2, W=3, which essentially make
         * using E the correct behaviour of the splat here.
         */     
        return _mm_shuffle_ps(data, data, _MM_SHUFFLE(E, E, E, E));
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Add(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return _mm_add_ps(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sub(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return _mm_sub_ps(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Mul(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return _mm_mul_ps(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Div(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return _mm_div_ps(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4MulAdd(const wfMathVec4& m1, const wfMathVec4& m2, const wfMathVec4& rhs) {
        return wfMathVec4Add(
            wfMathVec4Mul(m1, m2),
            rhs
        );
    }

    /* RCP/SQRT/RSQRT */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Reciprocal(const wfMathVec4& vec) {
        wfMathVec4              rcp = _mm_rcp_ps(vec);
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
        return _mm_sqrt_ps(vec);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4RSqrt(const wfMathVec4& vec) {
        wfMathVec4              rsqrt = _mm_rsqrt_ps(vec);
        static const wfMathVec4 half  = wfMathVec4Create(0.5f, 0.5f, 0.5f, 0.5f);
        static const wfMathVec4 three = wfMathVec4Create(3.0f, 3.0f, 3.0f, 3.0f);

        /* TODO: Optimize .. ME PLEASE! */
        return wfMathVec4Mul (
            wfMathVec4Mul (
                rsqrt,
                half
            ),
            wfMathVec4Sub (
                three,
                wfMathVec4Mul(
                    rsqrt,
                    wfMathVec4Mul(
                        vec,
                        rsqrt
                    )
                )
            )
        );
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Cross3(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        const wfMathVec4 lyzx = _mm_shuffle_ps(lhs, rhs, _MM_SHUFFLE(3,0,2,1));
        const wfMathVec4 lzxy = _mm_shuffle_ps(lhs, rhs, _MM_SHUFFLE(3,1,0,2));
        const wfMathVec4 ryzx = _mm_shuffle_ps(lhs, rhs, _MM_SHUFFLE(3,0,2,1));
        const wfMathVec4 rzxy = _mm_shuffle_ps(lhs, rhs, _MM_SHUFFLE(3,1,0,2));

        return _mm_sub_ps (
            _mm_mul_ps(
                lyzx,
                rzxy
            ),
            _mm_mul_ps(
                lzxy,
                ryzx
            )
        );
    }

    
    /* Getters */
    WF_MATH_VEC_INLINE float wfMathVec4GetX(const wfMathVec4 &vec) { wfMathVec4Union u={vec}; return u.f[0]; }
    WF_MATH_VEC_INLINE float wfMathVec4GetY(const wfMathVec4 &vec) { wfMathVec4Union u={vec}; return u.f[1]; }
    WF_MATH_VEC_INLINE float wfMathVec4GetZ(const wfMathVec4 &vec) { wfMathVec4Union u={vec}; return u.f[2]; }
    WF_MATH_VEC_INLINE float wfMathVec4GetW(const wfMathVec4 &vec) { wfMathVec4Union u={vec}; return u.f[3]; }

    /* Min/Max */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Min(const wfMathVec4& a, const wfMathVec4& b) {
        return _mm_min_ps(a, b);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Max(const wfMathVec4& a, const wfMathVec4& b) {
        return _mm_max_ps(a, b);
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
        return _mm_shuffle_ps(mat, mat, _MM_SHUFFLE(2, 1, 0, 3));
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(const wfMathVec4& mat) {
        return _mm_shuffle_ps(mat, mat, _MM_SHUFFLE(1, 0, 3, 2));
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(const wfMathVec4& mat) {
        return _mm_shuffle_ps(mat, mat, _MM_SHUFFLE(0, 3, 2, 1));
    }

    /*
     * Sign flipping done like a mad man on steroids .. I went and did
     * some magic here
     */   
    typedef WF_MATH_VEC_ALIGNED16 union {
        unsigned int ui[4];
        float         f[4];
    } wfMathVec4UnionSign;

    enum wfMathFlipSign {
        VEC_SIGN_0101 = 0x00000000,
        VEC_SIGN_1010 = 0x80000000
    };

    template<wfMathFlipSign E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign(const wfMathVec4& vec);

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_0101>(const wfMathVec4& mat) {
        const wfMathVec4UnionSign u = {{0x00000000, 0x80000000, 0x00000000, 0x80000000}};
        return _mm_xor_ps(mat, _mm_load_ps(u.f));
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_1010>(const wfMathVec4& mat) {
        const wfMathVec4UnionSign u = {{0x80000000, 0x00000000, 0x80000000, 0x00000000}};
        return _mm_xor_ps(mat, _mm_load_ps(u.f));
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Merge(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return _mm_movehl_ps(rhs, lhs);
    }

    /*
     * Matrix mainpulation is dead fucking easy in SSE.  It's why I love
     * SSE just so effn much.
     */
    WF_MATH_VEC_INLINE
    void wfMathMat4TransposeInplace(wfMathMat4 *mat) {
#   ifndef WF_STDLIB_MATH_EXPERIMENTAL_AVX
        _MM_TRANSPOSE4_PS(
            mat->x,
            mat->y,
            mat->z,
            mat->w
        );
#else 
        __m256 temp0 = _m256_unpacklo_ps(mat->x, mat->y);
        __m256 temp1 = _m256_unpacklo_ps(mat->x, mat->y);
        __m256 temp2 = _m256_unpackhi_ps(mat->z, mat->w);
        __m256 temp3 = _m256_unpackhi_ps(mat->z, mat->w);
        mat->x = _mm256_extractf128_ps(_mm256_insertf128_ps(temp0, _mm256_extractf128_ps(temp1, 0), 1), 0);
        mat->y = _mm256_extractf128_ps(_mm256_insertf128_ps(temp1, _mm256_extractf128_ps(temp0, 1), 0), 0);
        mat->z = _mm256_extractf128_ps(_mm256_insertf128_ps(temp2, _mm256_extractf128_ps(temp3, 0), 1), 0);
        mat->w = _mm256_extractf128_ps(_mm256_insertf128_ps(temp3, _mm256_extractf128_ps(temp2, 1), 0), 0);
#endif
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Transpose(const wfMathMat4 *mat, wfMathMat4 *out) {
        *out = *mat;
        wfMathMat4TransposeInplace(out);
    }
}

#endif /*! WF_STDLIB_MATH_SSE_HDR */