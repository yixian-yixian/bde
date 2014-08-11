// bslma_bufferallocator.cpp                                          -*-C++-*-
#ifndef BDE_OMIT_TRANSITIONAL // DEPRECATED

#include <bslma_bufferallocator.h>

#include <bsls_ident.h>
BSLS_IDENT("$Id$ $CSID$")

#include <bslmf_assert.h>
#include <bsls_alignment.h>
#include <bsls_alignmentutil.h>
#include <bsls_assert.h>
#include <bsls_performancehint.h>
#include <bsls_bslexceptionutil.h>

#include <climits>  // INT_MAX
#include <cstdio>

namespace BloombergLP {

// STATIC HELPER FUNCTIONS
static
bool isPowerOfTwo(int alignment)
    // Return 'true' if the specified 'alignment' is a power of 2 no greater
    // than 256, and 'false' otherwise.  Note that this implementation is
    // limited to small powers of 2 as its purpose is to detect valid memory
    // alignment values.
{
    static const int VALUES[]   = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };
           const int NUM_VALUES = sizeof VALUES / sizeof *VALUES;

    BSLMF_ASSERT((256 >= bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT));

    for (int i = 0; i != NUM_VALUES; ++i) {
        if (VALUES[i] == alignment) {
            return true;
        }
    }
    return false;
}

static
void *allocateFromBufferImp(int                               *cursor,
                            char                              *buffer,
                            bslma::BufferAllocator::size_type  bufSize,
                            bslma::BufferAllocator::size_type  size,
                            int                                alignment)
    // Allocate a memory block of the specified 'size' from the specified
    // 'buffer' at the specified 'cursor' position, aligned at the specified
    // 'alignment' boundary.  Return the address of the allocated memory block
    // if 'buffer' contains enough available memory, and 'null' otherwise.  The
    // 'cursor' is set to the position of remaining free buffer space after the
    // allocation.  The behavior is undefined unless
    // '0 < alignment <= bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT' and alignment
    // is an integral power of 2.

{
    BSLS_ASSERT(cursor);
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 < alignment);
    BSLS_ASSERT(alignment <= bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
    BSLS_ASSERT_SAFE(isPowerOfTwo(alignment));

    int offset = bsls::AlignmentUtil::calculateAlignmentOffset(
                                                              buffer + *cursor,
                                                              alignment);

    if (*cursor + offset + size > bufSize) { // insufficient space
        return static_cast<void *>(0);
    }

#if 0
    // Consider adding the following:

    if (*cursor + offset + size > 
                     static_cast<bslma::BufferAllocator::size_type>(INT_MAX)) {
        // not representable

        return static_cast<void *>(0);
    }
#endif

    void *result = &buffer[*cursor + offset];
    *cursor += static_cast<int>(offset + size);

    return result;
}

namespace bslma {

                        // ---------------------
                        // class BufferAllocator
                        // ---------------------

// CLASS METHODS
void *BufferAllocator::allocateFromBuffer(int               *cursor,
                                          char              *buffer,
                                          size_type          bufSize,
                                          size_type          size,
                                          AlignmentStrategy  strategy)
{
    BSLS_ASSERT(cursor);
    BSLS_ASSERT(buffer);

    return 0 >= size
           ? static_cast<void *>(0)
           : allocateFromBufferImp(
                             cursor,
                             buffer,
                             bufSize,
                             size,
                             strategy == NATURAL_ALIGNMENT
                             ? bsls::AlignmentUtil::calculateAlignmentFromSize(
                                                                          size)
                             : bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
}

void *BufferAllocator::allocateFromBuffer(int       *cursor,
                                          char      *buffer,
                                          size_type  bufSize,
                                          size_type  size,
                                          int        alignment)
{
    BSLS_ASSERT(cursor);
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 < alignment);
    BSLS_ASSERT(alignment <= bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
    BSLS_ASSERT_SAFE(isPowerOfTwo(alignment));

    return 0 >= size
           ? static_cast<void *>(0)
           : allocateFromBufferImp(cursor,
                                   buffer,
                                   bufSize,
                                   size,
                                   alignment);
}

// CREATORS
BufferAllocator::~BufferAllocator()
{
}

// MANIPULATORS
void *BufferAllocator::allocate(size_type size)
{
    if (BSLS_PERFORMANCEHINT_PREDICT_UNLIKELY(0 == size)) {
        return static_cast<void *>(0);
    }

    void *result;
    if (d_strategy == NATURAL_ALIGNMENT) {
        result = allocateFromBufferImp(
                           &d_cursor,
                           d_buffer_p,
                           d_bufferSize,
                           size,
                           bsls::AlignmentUtil::calculateAlignmentFromSize(
                                                                        size));
    } else { // default is MAXIMUM_ALIGNMENT
        result = allocateFromBufferImp(
                                      &d_cursor,
                                      d_buffer_p,
                                      d_bufferSize,
                                      size,
                                      bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
    }
    if (BSLS_PERFORMANCEHINT_PREDICT_LIKELY(result)) {
        return result;
    }
    if (BSLS_PERFORMANCEHINT_PREDICT_LIKELY(d_allocCallback)) {
        return (*d_allocCallback)(static_cast<int>(size));
    }

    // Throw 'std::bad_alloc' if cannot satisfy request.
    bsls::BslExceptionUtil::throwBadAlloc();

    return result;
}

// ACCESSORS
void BufferAllocator::print() const
{
    union {
        // Quell diagnostics that occur whenever a function ptr is cast to
        // 'void *' by going through this union.

        AllocCallback  d_acb;
        void          *d_p;
    } u;
    u.d_acb = d_allocCallback;

    std::printf("buffer address      = %p\n"
                "buffer size         = %d\n"
                "cursor position     = %d\n"
                "allocation function = %p\n"
                "alignment strategy  = %s\n",
                static_cast<void *>(d_buffer_p),
                d_bufferSize,
                d_cursor,
                u.d_p,
                BufferAllocator::MAXIMUM_ALIGNMENT == d_strategy
                ? "MAXIMUM_ALIGNMENT"
                : "NATURAL_ALIGNMENT");

    std::fflush(stdout);
}

}  // close package namespace

}  // close enterprise namespace

#endif // BDE_OMIT_TRANSITIONAL -- DEPRECATED

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
