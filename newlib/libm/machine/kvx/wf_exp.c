/**
 * generated using metalibm 1.1
 * sha1 git: b'"a068f1660bc5f925ea7bf7c4659ac6d5d079a675"'(dirty)
 *
 * WARNING: git status was not clean when file was generated !
 *
 * command used for generation:
 *   metalibm/metalibm_functions/ml_exp.py --target kv3 \
 *     --target-exec-options platform:jtag --verbose Info --passes \
 *     beforecodegen:fuse_fma --accuracy faithful --fname kv3_ml_exp --output \
 *     kv3_ml_exp.c
 *
**/
#include <stdint.h>
#include "ml_support_lib.h"
#include <math.h>

float expf(float);
float expf(float x){
    int nan_or_inf;

    nan_or_inf = ml_is_nan_or_inff(x);

    if (__builtin_expect(nan_or_inf, 0)) {
        int is_nan_test;

        is_nan_test = ml_is_nanf(x);

        if (is_nan_test) {
            int is_signaling_nan;

            is_signaling_nan = ml_is_signaling_nanf(x);

            if (is_signaling_nan) {

                return __builtin_nanf("");
            } else {

                return __builtin_nanf("");
            }
        } else {
            int inf_sign;

            inf_sign = x >= 0.0f;

            if (inf_sign) {

                return INFINITY;
            } else {

                return 0.f;
            }
        }
    } else {
        int tmp;

        tmp = x > 0x1.64p6f;

        if (__builtin_expect(tmp, 0)) {

            return INFINITY;
        } else {
            int tmp0;

            tmp0 = x < -0x1.ap6f;

            if (__builtin_expect(tmp0, 0)) {

                return 0.f;
            } else {
                float unround_k;
                int32_t ik;
                int late_overflow_test;
                int32_t carg;
                float k;
                int late_underflow_test;
                int tmp1;
                int tmp2;

                unround_k = x * 0x1.715476p0f;
                ik = __builtin_kvx_fixedw(unround_k, 0, ".rn");
                late_overflow_test = ik > INT32_C(127);
                carg = __builtin_kvx_fixedw(unround_k, 0, ".rn");
                k = __builtin_kvx_floatw(carg, 0, ".rn");
                late_underflow_test = k <= -126.0f;
                tmp1 = late_overflow_test || late_underflow_test;
                tmp2 =  ! tmp1;

                if (__builtin_expect(tmp2, 1)) {
                    float exact_hi;
                    float tmp3;
                    float exact_lo;
                    float exact_hi1;
                    float r;
                    float r2_;
                    float tmp4;
                    float tmp5;
                    float tmp6;
                    float tmp7;
                    float tmp8;
                    float tmp9;
                    float tmp10;
                    float tmp11;
                    float r3_;
                    float pre_sub_poly;
                    float tmp12;
                    float tmp13;
                    float poly;
                    float exp_ik;
                    float std_result;

                    exact_hi = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                    tmp3 =  - k;
                    exact_lo = tmp3 * 0x1.0bfbe8p-15f;
                    exact_hi1 = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                    r = exact_hi1 + exact_lo;
                    r2_ = r * r;
                    tmp4 = 0x1.55548cp-3f * r2_;
                    tmp5 = r * tmp4;
                    tmp6 = __builtin_kvx_ffmaw(0x1p-1f, r2_, tmp5, "");
                    tmp7 = 0x1.5554cep-5f * r2_;
                    tmp8 = __builtin_kvx_ffmaw(r2_, tmp7, tmp6, "");
                    tmp9 = 0x1.6d6748p-10f * r2_;
                    tmp10 = r * tmp9;
                    tmp11 = __builtin_kvx_ffmaw(0x1.123d02p-7f, r2_, tmp10, "");
                    r3_ = r * r2_;
                    pre_sub_poly = __builtin_kvx_ffmaw(r3_, tmp11, tmp8, "");
                    tmp12 = exact_lo + pre_sub_poly;
                    tmp13 = exact_hi + tmp12;
                    poly = 1.0f + tmp13;
                    exp_ik = ml_exp_insertion_fp32(ik);
                    std_result = exp_ik * poly;
                    return std_result;
                } else {


                    if (__builtin_expect(late_overflow_test, 0)) {
                        float exact_hi;
                        float tmp3;
                        float exact_lo;
                        float exact_hi1;
                        float r;
                        float r2_;
                        float tmp4;
                        float tmp5;
                        float tmp6;
                        float tmp7;
                        float tmp8;
                        float tmp9;
                        float tmp10;
                        float tmp11;
                        float r3_;
                        float pre_sub_poly;
                        float tmp12;
                        float tmp13;
                        float poly;
                        int32_t diff_k;
                        float tmp14;
                        float tmp15;
                        float tmp16;
                        float late_overflow_result;
                        int tmp17;

                        exact_hi = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                        tmp3 =  - k;
                        exact_lo = tmp3 * 0x1.0bfbe8p-15f;
                        exact_hi1 = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                        r = exact_hi1 + exact_lo;
                        r2_ = r * r;
                        tmp4 = 0x1.55548cp-3f * r2_;
                        tmp5 = r * tmp4;
                        tmp6 = __builtin_kvx_ffmaw(0x1p-1f, r2_, tmp5, "");
                        tmp7 = 0x1.5554cep-5f * r2_;
                        tmp8 = __builtin_kvx_ffmaw(r2_, tmp7, tmp6, "");
                        tmp9 = 0x1.6d6748p-10f * r2_;
                        tmp10 = r * tmp9;
                        tmp11 = __builtin_kvx_ffmaw(0x1.123d02p-7f, r2_, tmp10, "");
                        r3_ = r * r2_;
                        pre_sub_poly = __builtin_kvx_ffmaw(r3_, tmp11, tmp8, "");
                        tmp12 = exact_lo + pre_sub_poly;
                        tmp13 = exact_hi + tmp12;
                        poly = 1.0f + tmp13;
                        diff_k = ik - INT32_C(115);
                        tmp14 = ml_exp_insertion_fp32(diff_k);
                        tmp15 = tmp14 * poly;
                        tmp16 = ml_exp_insertion_fp32(INT32_C(115));
                        late_overflow_result = tmp15 * tmp16;
                        tmp17 = ml_is_inff(late_overflow_result);

                        if (__builtin_expect(tmp17, 0)) {

                            return INFINITY;
                        } else {

                            return late_overflow_result;
                        }
                    } else {
                        float exact_hi;
                        float tmp3;
                        float exact_lo;
                        float exact_hi1;
                        float r;
                        float r2_;
                        float tmp4;
                        float tmp5;
                        float tmp6;
                        float tmp7;
                        float tmp8;
                        float tmp9;
                        float tmp10;
                        float tmp11;
                        float r3_;
                        float pre_sub_poly;
                        float tmp12;
                        float tmp13;
                        float poly;
                        int32_t corrected_exp;
                        float tmp14;
                        float tmp15;
                        float tmp16;
                        float late_underflow_result;
                        int tmp17;

                        exact_hi = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                        tmp3 =  - k;
                        exact_lo = tmp3 * 0x1.0bfbe8p-15f;
                        exact_hi1 = __builtin_kvx_ffmsw(k, 0x1.62ep-1f, x, "");
                        r = exact_hi1 + exact_lo;
                        r2_ = r * r;
                        tmp4 = 0x1.55548cp-3f * r2_;
                        tmp5 = r * tmp4;
                        tmp6 = __builtin_kvx_ffmaw(0x1p-1f, r2_, tmp5, "");
                        tmp7 = 0x1.5554cep-5f * r2_;
                        tmp8 = __builtin_kvx_ffmaw(r2_, tmp7, tmp6, "");
                        tmp9 = 0x1.6d6748p-10f * r2_;
                        tmp10 = r * tmp9;
                        tmp11 = __builtin_kvx_ffmaw(0x1.123d02p-7f, r2_, tmp10, "");
                        r3_ = r * r2_;
                        pre_sub_poly = __builtin_kvx_ffmaw(r3_, tmp11, tmp8, "");
                        tmp12 = exact_lo + pre_sub_poly;
                        tmp13 = exact_hi + tmp12;
                        poly = 1.0f + tmp13;
                        corrected_exp = ik + INT32_C(46);
                        tmp14 = ml_exp_insertion_fp32(corrected_exp);
                        tmp15 = tmp14 * poly;
                        tmp16 = ml_exp_insertion_fp32(INT32_C(-46));
                        late_underflow_result = tmp15 * tmp16;
                        tmp17 = ml_is_subnormalf(late_underflow_result);

                        if (tmp17) {

                            return late_underflow_result;
                        }
                        return late_underflow_result;
                    }
                }
            }
        }
    }
}
