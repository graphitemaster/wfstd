#ifndef WF_STDLIB_MATH_NEON_HDR
#define WF_STDLIB_MATH_NEON_HDR

#include <arm_neon.h>

namespace wfPrivate {
    typedef float32x4_t wfMathVec4;

    struct wfMathMat4 {
        wfMathVec4 x;
        wfMathVec4 y;
        wfMathVec4 z;
        wfMathVec4 w;
    };

    /* Align? Shouldn't need it */
    union wfMathVec4Union {
        wfMathVec4 s;
        float      f[4];
    };

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Create(float x, float y, float z, float w) {
        const float32_t  data[4] = { x, y, z, w };
        const wfMathVec4 load    = vld1q_f32(data);

        return load;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Zero() {
        return vdupq_n_f32(0.0f);
    }

    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad(const float *array);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<4>(const float *array) {
        const float32_t *data = (const float32_t*)array;
        return vld1q_f32(data);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<3>(const float *array) {
        return wfMathVec4Create(array[0], array[1], array[2], 0.0f);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<2>(const float *array) {
        /*
         * This is more optimal than calling wfMathVec4Create with
         * two arguments and nulling out the z and w components.
         */
        const float32_t *data = (const float32_t *)array;
        const float32_t  zero = 0.0f;
        float32x2_t      high = vld1_dup_f32(&zero);
        float32x2_t      low  = vld1_f32(data);

        return vcombine_f32(low, high);
    }


    template <size_t E>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore(const wfMathVec4& value, float *array);

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<4>(const wfMathVec4& value, float *array) {
        vst1q_f32((float32_t*)array, value);
    }

    /*
     * There must be a better way of doing this?  We'll find out some day
     * so far this is all I can think of.
     */   
    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<3>(const wfMathVec4& value, float *array) {
        wfMathVec4Union u = { value };
        array[0] = u.f[0];
        array[1] = u.f[1];
        array[2] = u.f[2];
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<2>(const wfMathVec4& value, float *array) {
        /*
         * Initially I had decided to use the wfMathVec4Union trick here
         * but later research lead me to find a more interesting solution
         * which is a little faster.
         */
        #if 0
            wfMathVec4Union u = { value };

            array[0] = u.f[0];
            array[1] = u.f[1];
        #else
            /* A little faster */
            vst1_f32((float32_t*)array, vget_low_f32(value));
        #endif
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Single(float x) {
        return vdupq_n_f32(x);
    };

    enum wfMathVec4Splat {
        VEC_SPLAT_X,
        VEC_SPLAT_Y,
        VEC_SPLAT_Z,
        VEC_SPLAT_W
    };


    template <wfMathVec4Splat E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat(const wfMathVec4& data);
    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_X>(const wfMathVec4& data) {
        return vdupq_lane_f32(vget_low_f32(data), 0);
    }
    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Y>(const wfMathVec4& data) {
        return vdupq_lane_f32(vget_low_f32(data), 1);
    }
    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Z>(const wfMathVec4& data) {
        return vdupq_lane_f32(vget_high_f32(data), 0);
    }
    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_W>(const wfMathVec4& data) {
        return vdupq_lane_f32(vget_high_f32(data), 1);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Reciprocal(const wfMathVec4& vec) {
        wfMathVec4 est;

        est = vrecpeq_f32(vec);
        est = vmulq_f32(vrecpsq_f32(est, vec), est);
        est = vmulq_f32(vrecpsq_f32(est, vec), est);

        return est;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Add(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vaddq_f32(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sub(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vsubq_f32(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Mul(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vmulq_f32(lhs, rhs);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Div(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return vmulq_f32(lhs, wfMathVec4Reciprocal(rhs));
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4MulAdd(const wfMathVec4& m1, const wfMathVec4& m2, const wfMathVec4& rhs) {
        return vmlaq_f32(rhs, m1, m2);
    }


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4RSqrt(const wfMathVec4& vec) {
        /*
         * It never gets easy does it?  I actually find this to be
         * less annoying than AltiVec but jesus christ there is no
         * excuse to _NOT_ have an intrinsic for this.
         */
        wfMathVec4 est1 = vrsqrteq_f32(vec);
        wfMathVec4 est2 = vmulq_f32(est1, vec);

        est1 = vmulq_f32(est1, vrsqrtsq_f32(est2, est1));
        est2 = vmulq_f32(est1, vec);
        est1 = vmulq_f32(est1, vrsqrtsq_f32(est2, est1));
        est2 = vmulq_f32(est1, vec);

        return vmulq_f32(est1, vrsqrtsq_f32(est2, est1));
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sqrt(const wfMathVec4& vec) {
        return wfMathVec4Reciprocal(wfMathVec4RSqrt(vec));
    }

    /* Getters */
    WF_MATH_VEC_INLINE float wfMathVec4GetX(const wfMathVec4 &vec) { return vgetq_lane_f32(vec, 0); }
    WF_MATH_VEC_INLINE float wfMathVec4GetY(const wfMathVec4 &vec) { return vgetq_lane_f32(vec, 1); }
    WF_MATH_VEC_INLINE float wfMathVec4GetZ(const wfMathVec4 &vec) { return vgetq_lane_f32(vec, 2); }
    WF_MATH_VEC_INLINE float wfMathVec4GetW(const wfMathVec4 &vec) { return vgetq_lane_f32(vec, 3); }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Cross3(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        /*
         * TODO: further research on how this is acomplished without creating
         * so many more vec4s and aliasing memory.
         */
        #define LOAD(VEC, A, B, C, D)   \
            wfMathVec4Create (          \
                wfMathVec4Get##A (VEC), \
                wfMathVec4Get##B (VEC), \
                wfMathVec4Get##C (VEC), \
                wfMathVec4Get##D (VEC)  \
            )

        const wfMathVec4 lyzx = LOAD(lhs, Y, Z, X, W);
        const wfMathVec4 lzxy = LOAD(lhs, Z, X, Y, W);
        const wfMathVec4 ryzx = LOAD(rhs, Y, Z, X, W);
        const wfMathVec4 rzxy = LOAD(rhs, Z, X, Y, W);

        #undef LOAD

        return vmlsq_f32(vmulq_f32(lyzx, rzxy), lzxy, ryzx);
    }

    /* Min/Max */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Min(const wfMathVec4& a, const wfMathVec4& b) {
        return vminq_f32(a, b);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Max(const wfMathVec4& a, const wfMathVec4& b) {
        return vmaxq_f32(a, b);
    }

    enum wfMathVec4Shuffle {
        VEC_SHUFFLE_WXYZ,
        VEC_SHUFFLE_ZWXY,
        VEC_SHUFFLE_YZWX
    };

    template <wfMathVec4Shuffle E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle(const wfMathVec4& vec);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(const wfMathVec4& vec) {
        wfMathVec4Union u = { vec };
        return wfMathVec4Create(u.f[3], u.f[0], u.f[1], u.f[2]);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(const wfMathVec4& vec) {
        wfMathVec4Union u = { vec };
        return wfMathVec4Create(u.f[2], u.f[3], u.f[0], u.f[1]);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(const wfMathVec4& vec) {
        wfMathVec4Union u = { vec };
        return wfMathVec4Create(u.f[1], u.f[2], u.f[3], u.f[0]);
    }

    enum wfMathFlipSign {
        VEC_SIGN_0101,
        VEC_SIGN_1010
    };

    template<wfMathFlipSign E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign(const wfMathVec4& vec);

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_0101>(const wfMathVec4& vec) {
        const unsigned int upnpn[4] = { 0x00000000, 0x80000000, 0x00000000, 0x80000000 };
        const uint32x4_t   pnpn     = vld1q_u32(upnpn);

        return vreinterpretq_f32_u32(
            veorq_u32(
                vreinterpretq_u32_f32(vec),
                pnpn
            )
        ); 
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_1010>(const wfMathVec4& vec) {
        const unsigned int upnpn[4] = { 0x80000000, 0x00000000, 0x80000000, 0x00000000 };
        const uint32x4_t   pnpn     = vld1q_u32(upnpn);

        return vreinterpretq_f32_u32(
            veorq_u32(
                vreinterpretq_u32_f32(vec),
                pnpn
            )
        ); 
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Merge(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        wfMathVec4Union merge[] = {
            { lhs },
            { rhs }
        };

        return wfMathVec4Create(
            merge[0].f[2],
            merge[0].f[3],
            merge[1].f[2],
            merge[1].f[3]
        );
    }

    /*
     * Matrix mainpulation is dead fucking easy in SSE.  It's why I love
     * SSE just so effn much.
     */
    WF_MATH_VEC_INLINE
    void wfMathMat4TransposeInplace(wfMathMat4 *mat) {
        const wfMathVec4Union x = { mat->x };
        const wfMathVec4Union y = { mat->y };
        const wfMathVec4Union z = { mat->z };
        const wfMathVec4Union w = { mat->w };

        mat->x = wfMathVec4Create(x.f[0], y.f[0], z.f[0], w.f[0]);
        mat->y = wfMathVec4Create(x.f[1], y.f[1], z.f[1], w.f[1]);
        mat->z = wfMathVec4Create(x.f[2], y.f[2], z.f[2], w.f[2]);
        mat->w = wfMathVec4Create(x.f[3], y.f[3], z.f[3], w.f[3]);
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Transpose(const wfMathMat4 *mat, wfMathMat4 *out) {
        *out = *mat;
        wfMathMat4TransposeInplace(out);
    }
}

#endif /*! WF_STDLIB_MATH_NEON_HDR */