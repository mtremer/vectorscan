/*
 * Copyright (c) 2015, Intel Corporation
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

/** \file
 * \brief Truffle: fully general character class acceleration.
 *
 * Utilises the SSSE3 pshufb or AVX2 vpshufb shuffle instructions
 */

#ifndef TRUFFLE_H
#define TRUFFLE_H

#include "util/simd_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CAN_USE_WIDE_TRUFFLE
const u8 *truffleExecWide(m256 mask, const u8 *buf,
                      const u8 *buf_end);

const u8 *rtruffleExecWide(m256 mask, const u8 *buf,
                       const u8 *buf_end);
#endif

const u8 *truffleExec(m128 shuf_mask_lo_highclear, m128 shuf_mask_lo_highset,
                      const u8 *buf, const u8 *buf_end);

const u8 *rtruffleExec(m128 shuf_mask_lo_highclear, m128 shuf_mask_lo_highset,
                       const u8 *buf, const u8 *buf_end);

#ifdef __cplusplus
}
#endif


#endif /* TRUFFLE_H */

