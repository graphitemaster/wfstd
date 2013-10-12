#ifndef WF_STDLIB_MATH_AUTOVECTORIZE_HDR
#define WF_STDLIB_MATH_AUTOVECTORIZE_HDR

namespace wfPrivate {
    
    typedef float wfMathVec4 __attribute__((vector_size (16)));
    
    typedef union {
        wfMathVec4 s;
        float      f[4];
    } wfMathVec4Union;
    
    WF_MATH_VEC_INLINE
    float wfMathVec4GetX(const wfMathVec4& vec) {
        wfMathVec4Union u={vec};
        return u.f[0];
    }
    
    WF_MATH_VEC_INLINE
    float wfMathVec4GetY(const wfMathVec4& vec) {
        wfMathVec4Union u={vec};
        return u.f[1];
    }
    
    WF_MATH_VEC_INLINE
    float wfMathVec4GetZ(const wfMathVec4& vec) {
        wfMathVec4Union u={vec};
        return u.f[2];
    }
    
    WF_MATH_VEC_INLINE
    float wfMathVec4GetW(const wfMathVec4& vec) {
        wfMathVec4Union u={vec};
        return u.f[3];
    }
    

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Create(float x, float y, float z, float w) {
        wfMathVec4 vec = {x, y, z, w};
        return vec;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Zero() {
        return wfMathVec4Create(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    
    template<size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad(const float *array);
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<4>(const float *array) {
        wfMathVec4 vec = { array[0], array[1], array[2], array[3] };
        return vec;
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<3>(const float *array) {
        wfMathVec4 vec = { array[0], array[1], array[2], 0.0f };
        return vec;
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<2>(const float *array) {
        wfMathVec4 vec = { array[0], array[1], 0.0f, 0.0f};
        return vec;
    }
    
    template<size_t E>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore(const wfMathVec4& val, float *array) {
        memcpy(array, &val, sizeof(float) * E);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Single(float single) {
        wfMathVec4 vec = { single, single, single, single };
        return vec;
    }
    
    enum wfMathVec4Splat {
        VEC_SPLAT_X,
        VEC_SPLAT_Y,
        VEC_SPLAT_Z,
        VEC_SPLAT_W
    };
    
    template<wfMathVec4Splat E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat(const wfMathVec4& vec);
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_X>(const wfMathVec4& vec) {
        float      get = wfMathVec4GetX(vec);
        wfMathVec4 set = { get, get, get, get };
        return set;
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Y>(const wfMathVec4& vec) {
        float      get = wfMathVec4GetY(vec);
        wfMathVec4 set = { get, get, get, get };
        return set;
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Z>(const wfMathVec4& vec) {
        float      get = wfMathVec4GetZ(vec);
        wfMathVec4 set = { get, get, get, get };
        return set;
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_W>(const wfMathVec4& vec) {
        float      get = wfMathVec4GetW(vec);
        wfMathVec4 set = { get, get, get, get };
        return set;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Reciprocal(const wfMathVec4& vec) {
        return wfMathVec4Single(1.0f) / vec;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sqrt(const wfMathVec4& vec) {
        wfMathVec4 ret = {
               sqrtf(wfMathVec4GetX(vec)),
               sqrtf(wfMathVec4GetY(vec)),
               sqrtf(wfMathVec4GetZ(vec)),
               sqrtf(wfMathVec4GetW(vec))
        };
        
        return ret;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4RSqrt(const wfMathVec4& vec) {
        return wfMathVec4Single(1.0f) / wfMathVec4Sqrt(vec);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Add(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return lhs + rhs;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sub(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return lhs - rhs;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Mul(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return lhs * rhs;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Div(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return lhs / rhs;
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4MulAdd(const wfMathVec4 &a, const wfMathVec4 &b, const wfMathVec4 &c) {
        return wfMathVec4Add(wfMathVec4Mul(a, b), c);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Cross3(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        wfMathVec4Union l = {lhs};
        wfMathVec4Union r = {rhs};
        
        return wfMathVec4Create(l.f[1] * r.f[2] - l.f[2] * r.f[1],
                                l.f[2] * r.f[0] - l.f[0] * r.f[2],
                                l.f[0] * r.f[1] - l.f[1] * r.f[0], 0);
    }
    
    enum wfMathVec4Shuffle {
        VEC_SHUFFLE_WXYZ,
        VEC_SHUFFLE_ZWXY,
        VEC_SHUFFLE_YZWX
    };
    
    template<wfMathVec4Shuffle E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle(const wfMathVec4 &vec);
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(const wfMathVec4 &vec) {
        wfMathVec4Union u={vec};
        return wfMathVec4Create(u.f[3], u.f[0], u.f[1], u.f[2]);
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(const wfMathVec4 &vec) {
        wfMathVec4Union u={vec};
        return wfMathVec4Create(u.f[2], u.f[3], u.f[0], u.f[1]);
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(const wfMathVec4 &vec) {
        wfMathVec4Union u={vec};
        return wfMathVec4Create(u.f[1], u.f[2], u.f[3], u.f[0]);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Merge(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        wfMathVec4Union a={lhs};
        wfMathVec4Union b={rhs};
        
        return wfMathVec4Create(a.f[2], a.f[3], b.f[2], b.f[3]);
    }
    
    enum wfMathFlipSign {
        VEC_SIGN_0101 = 0x00000000,
        VEC_SIGN_1010 = 0x80000000
    };
    
    template<wfMathFlipSign E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign(const wfMathVec4 &vec);
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_0101>(const wfMathVec4 &vec) {
        wfMathVec4Union u={vec};
        return wfMathVec4Create(u.f[0], -u.f[1], u.f[2], -u.f[3]);
    }
    
    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_1010>(const wfMathVec4 &vec){
        wfMathVec4Union u={vec};
        return wfMathVec4Create(-u.f[0], u.f[1], -u.f[2], u.f[3]);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Min(const wfMathVec4 &a, const wfMathVec4 &b) {
        wfMathVec4Union ua = {a};
        wfMathVec4Union ub = {b};
        return wfMathVec4Create(ua.f[0] < ub.f[0] ? ua.f[0] : ub.f[0],
                                ua.f[1] < ub.f[1] ? ua.f[1] : ub.f[1],
                                ua.f[2] < ub.f[2] ? ua.f[2] : ub.f[2],
                                ua.f[3] < ub.f[3] ? ua.f[3] : ub.f[3]);
    }
    
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Max(const wfMathVec4 &a, const wfMathVec4 &b) {
        wfMathVec4Union ua = {a};
        wfMathVec4Union ub = {b};
        return wfMathVec4Create(ua.f[0] > ub.f[0] ? ua.f[0] : ub.f[0],
                                ua.f[1] > ub.f[1] ? ua.f[1] : ub.f[1],
                                ua.f[2] > ub.f[2] ? ua.f[2] : ub.f[2],
                                ua.f[3] > ub.f[3] ? ua.f[3] : ub.f[3]);
    }
    
    struct wfMathMat4 {
        wfMathVec4 x;
        wfMathVec4 y;
        wfMathVec4 z;
        wfMathVec4 w;
    };
    
    WF_MATH_VEC_INLINE
    void wfMathMat4TransposeInplace(wfMathMat4 *mat) {
        const wfMathVec4Union unload[4] = {
            { mat->x },
            { mat->y },
            { mat->z },
            { mat->w }
        };
        
        const wfMathVec4 reload[4] = {
            { unload[0].f[0], unload[1].f[0], unload[2].f[0], unload[3].f[0] },
            { unload[0].f[1], unload[1].f[1], unload[2].f[1], unload[3].f[1] },
            { unload[0].f[2], unload[1].f[2], unload[2].f[2], unload[3].f[2] },
            { unload[0].f[3], unload[1].f[3], unload[2].f[3], unload[3].f[3] },
        };
        
        mat->x = reload[0];
        mat->y = reload[1];
        mat->z = reload[2];
        mat->w = reload[3];
    }
    
    WF_MATH_VEC_INLINE
    void wfMathMat4Transpose(const wfMathMat4 *in, wfMathMat4 *out) {
        *out = *in;
        wfMathMat4TransposeInplace(out);
    }
}

#endif /*! WF_STDLIB_MATH_AUTOVECTORIZE_HDR */