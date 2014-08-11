// bslmf_isvoid.h                                                     -*-C++-*-
#ifndef INCLUDED_BSLMF_ISVOID
#define INCLUDED_BSLMF_ISVOID

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a compile-time check for 'void' types.
//
//@CLASSES:
//  bsl::is_void: standard meta-function for determining 'void' types
//  bslmf::IsVoid: meta-function for determining 'void' types
//
//@SEE_ALSO: bslmf_integralconstant
//
//@AUTHOR: Alisdair Meredith (ameredit)
//
//@DESCRIPTION: This component defines two meta-functions, 'bsl::is_void' and
// 'BloombergLP::bslmf::IsVoid', both of which may be used to query whether a
// type is the (possibly cv-qualified) 'void' type.
//
// 'bsl::is_void' meets the requirements of the 'is_void' template defined in
// the C++11 standard [meta.unary.cat], while 'bslmf::IsVoid' was devised
// before 'is_void' was standardized.
//
// The two meta-functions are functionally equivalent.  The major difference
// between them is that the result for 'bsl::is_void' is indicated by the
// class member 'value', while the result for 'bslmf::IsVoid' is indicated
// by the class member 'VALUE'.
//
// Note that 'bsl::is_void' should be preferred over 'bslmf::IsVoid', and in
// general, should be used by new components.
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Determine Whether a Type is the 'void' Type
/// - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose that we want to assert whether a particular type is the 'void' type.
//
// First, we create two 'typedef's -- the 'void' type and another type:
//..
//  typedef int  MyType;
//  typedef void MyVoidType;
//..
// Now, we instantiate the 'bsl::is_void' template for each of the 'typedef's
// and assert the 'value' static data member of each instantiation:
//..
//  assert(false == bsl::is_void<MyType>::value);
//  assert(true  == bsl::is_void<MyVoidType>::value);
//..

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLMF_INTEGRALCONSTANT
#include <bslmf_integralconstant.h>
#endif

#ifndef INCLUDED_BSLMF_METAINT
#include <bslmf_metaint.h>
#endif

#ifndef INCLUDED_BSLMF_REMOVECV
#include <bslmf_removecv.h>
#endif

namespace BloombergLP {
namespace bslmf {

                         // =================
                         // struct IsVoid_Imp
                         // =================

template <class TYPE>
struct IsVoid_Imp : bsl::false_type {
    // This 'struct' template implements a meta-function to determine whether
    // the (template parameter) 'TYPE' is the (non-cv-qualified) 'void' type.
    // This generic default template derives from 'bsl::false_type'.  A
    // template specialization is provided (below) that derives from
    // 'bsl::true_type'.
};

                         // =========================
                         // struct IsVoid_Imp<TYPE *>
                         // =========================

template <>
struct IsVoid_Imp<void> : bsl::true_type {
     // This partial specialization of 'IsVoid_Imp' derives from
     // 'bsl::true_type' for when the (template parameter) 'TYPE' is the 'void'
     // type.
};

}  // close package namespace
}  // close enterprise namespace

namespace bsl {

                         // ==============
                         // struct is_void
                         // ==============

template <class TYPE>
struct is_void : BloombergLP::bslmf::IsVoid_Imp<
                                        typename remove_cv<TYPE>::type>::type {
    // This 'struct' template implements the 'is_void' meta-function defined in
    // the C++11 standard [meta.unary.cat] to determine if the (template
    // parameter) 'TYPE' is the (possibly cv-qualified) 'void' type.  This
    // 'struct' derives from 'bsl::true_type' if 'TYPE' is the 'void' type, and
    // 'bsl::false_type' otherwise.
};

}  // close namespace bsl

namespace BloombergLP {
namespace bslmf {

                        // ===================
                        // struct bslmf_IsVoid
                        // ===================

template<class TYPE>
struct IsVoid : bsl::is_void<TYPE>::type {
    // This 'struct' template implements a meta-function to determine if the
    // (template parameter) 'TYPE' is the (possibly cv-qualified) 'void' type.
    // This 'struct' derives from 'bslmf::MetaInt<1>' if 'TYPE' is the 'void'
    // type, and 'bslmf::MetaInt<0>' otherwise.
    //
    // Note that although this 'struct' is functionally equivalent to
    // 'bsl::is_void', and the use of 'bsl::is_void' should be preferred.
};

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg Finance L.P.
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
