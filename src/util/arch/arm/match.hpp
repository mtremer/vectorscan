/*
 * Copyright (c) 2015-2017, Intel Corporation
 * Copyright (c) 2020-2021, VectorCamp PC
 * Copyright (c) 2023, Arm Limited
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

template <>
really_really_inline
const u8 *first_non_zero_match<16>(const u8 *buf, SuperVector<16> mask, u16 const UNUSED len) {
    uint32x4_t m = mask.u.u32x4[0];
    uint64_t vmax = vgetq_lane_u64 (vreinterpretq_u64_u32 (vpmaxq_u32(m, m)), 0);
    if (vmax != 0) {
        typename SuperVector<16>::comparemask_type z = mask.comparemask();
        DEBUG_PRINTF("z %08llx\n", z);
        DEBUG_PRINTF("buf %p z %08llx \n", buf, z);
        u32 pos = ctz64(z) / SuperVector<16>::mask_width();
        DEBUG_PRINTF("match @ pos %u\n", pos);
        assert(pos < 16);
        DEBUG_PRINTF("buf + pos %p\n", buf + (pos));
        return buf + pos;
    } else {
        return NULL; // no match
    }
}

template <>
really_really_inline
const u8 *last_non_zero_match<16>(const u8 *buf, SuperVector<16> mask, u16 const UNUSED len) {
    uint32x4_t m = mask.u.u32x4[0];
    uint64_t vmax = vgetq_lane_u64 (vreinterpretq_u64_u32 (vpmaxq_u32(m, m)), 0);
    if (vmax != 0) {
        typename SuperVector<16>::comparemask_type z = mask.comparemask();
        DEBUG_PRINTF("buf %p z %08llx \n", buf, z);
        DEBUG_PRINTF("z %08llx\n", z);
        u32 pos = clz64(z) / SuperVector<16>::mask_width();
        DEBUG_PRINTF("match @ pos %u\n", pos);
        return buf + (15 - pos);
    } else {
        return NULL; // no match
    }
}

template <>
really_really_inline
const u8 *first_zero_match_inverted<16>(const u8 *buf, SuperVector<16> mask, u16 const UNUSED len) {
    uint32x4_t m = mask.u.u32x4[0];
    uint64_t vmax = vgetq_lane_u64 (vreinterpretq_u64_u32 (vpmaxq_u32(m, m)), 0);
    if (vmax != 0) {
        typename SuperVector<16>::comparemask_type z = mask.comparemask();
        DEBUG_PRINTF("z %08llx\n", z);
        DEBUG_PRINTF("buf %p z %08llx \n", buf, z);
        u32 pos = ctz64(z) / SuperVector<16>::mask_width();
        DEBUG_PRINTF("match @ pos %u\n", pos);
        assert(pos < 16);
        DEBUG_PRINTF("buf + pos %p\n", buf + pos);
        return buf + pos;
    } else {
        return NULL; // no match
    }
}

template <>
really_really_inline
const u8 *last_zero_match_inverted<16>(const u8 *buf, SuperVector<16> mask, u16 const UNUSED len) {
    uint32x4_t m = mask.u.u32x4[0];
    uint64_t vmax = vgetq_lane_u64 (vreinterpretq_u64_u32 (vpmaxq_u32(m, m)), 0);
    if (vmax != 0) {
        typename SuperVector<16>::comparemask_type z = mask.comparemask();
        DEBUG_PRINTF("buf %p z %08llx \n", buf, z);
        DEBUG_PRINTF("z %08llx\n", z);
        u32 pos = clz64(z) / SuperVector<16>::mask_width();
        DEBUG_PRINTF("match @ pos %u\n", pos);
        return buf + (15 - pos);
    } else {
        return NULL; // no match
    }
}

#ifdef HAVE_SVE

// Return the index of the first lane with an active predicate.
// Assumes pred is not svpfalse()
static really_inline
uint64_t index_first_predicate(const svbool_t pred) {

    const svuint8_t indices = svindex_u8(0, 1);
    const svbool_t single_lane_mask = svpnext_b8(pred, svpfalse());
    const uint64_t lane_index = svlastb(single_lane_mask, indices);

    return lane_index;
}

/*
 * It is assumed mask have the value 0 for all inactive lanes, if any.
 */
static really_inline
uint64_t last_non_zero(const size_t vector_size_int_8, svuint8_t mask) {
    const svbool_t non_zero = svcmpne(svptrue_b8(), mask, 0);

    if (svptest_any(svptrue_b8(), non_zero)) {
        return vector_size_int_8 - 1 - index_first_predicate(svrev_b8(non_zero));
    } else {
        return vector_size_int_8;
    }
}

/*
 * It is assumed mask have the value 0 for all inactive lanes, if any.
 */
static really_inline
uint64_t first_non_zero(const size_t vector_size_int_8, svuint8_t mask) {
    const svbool_t non_zero = svcmpne(svptrue_b8(), mask, 0);

    if (svptest_any(svptrue_b8(), non_zero)) {
        return index_first_predicate(non_zero);
    } else {
        return vector_size_int_8;
    }
}

#endif //HAVE_SVE