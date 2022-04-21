// bsl_cmath.h                                                        -*-C++-*-
#ifndef INCLUDED_BSL_CMATH
#define INCLUDED_BSL_CMATH

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide functionality of the corresponding C++ Standard header.
//
//@SEE_ALSO: package bos+stdhdrs in the bos package group
//
//@DESCRIPTION: Provide types, in the 'bsl' namespace, equivalent to those
// defined in the corresponding C++ standard header.  Include the native
// compiler-provided standard header, and also directly include Bloomberg's
// implementation of the C++ standard type (if one exists).  Finally, place the
// included symbols from the 'std' namespace (if any) into the 'bsl' namespace.

#include <bsls_libraryfeatures.h>
#include <bsls_nativestd.h>

#include <cmath>

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
#if !defined(BDE_DONT_ALLOW_TRANSITIVE_INCLUDES) && \
     defined(BDE_BUILD_TARGET_STLPORT)

// Code in Robo depends on <math.h> included transitively with <cmath> and it
// fails to build otherwise in the stlport4 mode on Sun.

#include <math.h>

#endif
#endif  // BDE_OMIT_INTERNAL_DEPRECATED

namespace bsl {
    // Import selected symbols into bsl namespace

    using native_std::abs;
    using native_std::acos;
    using native_std::asin;
    using native_std::atan;
    using native_std::atan2;
    using native_std::ceil;
    using native_std::cos;
    using native_std::cosh;
    using native_std::exp;
    using native_std::fabs;
    using native_std::floor;
    using native_std::fmod;
    using native_std::frexp;
    using native_std::ldexp;
    using native_std::log;
    using native_std::log10;
    using native_std::modf;
    using native_std::pow;
    using native_std::sin;
    using native_std::sinh;
    using native_std::sqrt;
    using native_std::tan;
    using native_std::tanh;

#ifdef BSLS_LIBRARYFEATURES_HAS_C99_FP_CLASSIFY
    using native_std::fpclassify;
    using native_std::isfinite;
    using native_std::isinf;
    using native_std::isnan;
    using native_std::isnormal;
    using native_std::signbit;
    using native_std::isgreater;
    using native_std::isgreaterequal;
    using native_std::isless;
    using native_std::islessequal;
    using native_std::islessgreater;
    using native_std::isunordered;
#endif

#ifdef BSLS_LIBRARYFEATURES_HAS_C99_LIBRARY
    using native_std::double_t;
    using native_std::float_t;
    using native_std::acosh;
    using native_std::asinh;
    using native_std::atanh;
    using native_std::cbrt;
    using native_std::copysign;
    using native_std::erf;
    using native_std::erfc;
    using native_std::exp2;
    using native_std::expm1;
    using native_std::fdim;
    using native_std::fma;
    using native_std::fmax;
    using native_std::fmin;
    using native_std::hypot;
    using native_std::ilogb;
    using native_std::lgamma;
    using native_std::llrint;
    using native_std::log1p;
    using native_std::log2;
    using native_std::logb;
    using native_std::lrint;
    using native_std::lround;
    using native_std::llround;
    using native_std::nan;
    using native_std::nanl;
    using native_std::nanf;
    using native_std::nearbyint;
    using native_std::nextafter;
    using native_std::nexttoward;
    using native_std::remainder;
    using native_std::remquo;
    using native_std::rint;
    using native_std::round;
    using native_std::scalbln;
    using native_std::scalbn;
    using native_std::tgamma;
    using native_std::trunc;
#endif  //  BSLS_LIBRARYFEATURES_HAS_C99_LIBRARY

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_SPECIAL_MATH_FUNCTIONS
    using native_std::assoc_laguerre;
    using native_std::assoc_laguerref;
    using native_std::assoc_laguerrel;
    using native_std::assoc_legendre;
    using native_std::assoc_legendref;
    using native_std::assoc_legendrel;
    using native_std::beta;
    using native_std::betaf;
    using native_std::betal;
    using native_std::comp_ellint_1;
    using native_std::comp_ellint_1f;
    using native_std::comp_ellint_1l;
    using native_std::comp_ellint_2;
    using native_std::comp_ellint_2f;
    using native_std::comp_ellint_2l;
    using native_std::comp_ellint_3;
    using native_std::comp_ellint_3f;
    using native_std::comp_ellint_3l;
    using native_std::cyl_bessel_i;
    using native_std::cyl_bessel_if;
    using native_std::cyl_bessel_il;
    using native_std::cyl_bessel_j;
    using native_std::cyl_bessel_jf;
    using native_std::cyl_bessel_jl;
    using native_std::cyl_bessel_k;
    using native_std::cyl_bessel_kf;
    using native_std::cyl_bessel_kl;
    using native_std::cyl_neumann;
    using native_std::cyl_neumannf;
    using native_std::cyl_neumannl;
    using native_std::ellint_1;
    using native_std::ellint_1f;
    using native_std::ellint_1l;
    using native_std::ellint_2;
    using native_std::ellint_2f;
    using native_std::ellint_2l;
    using native_std::ellint_3;
    using native_std::ellint_3f;
    using native_std::ellint_3l;
    using native_std::expint;
    using native_std::expintf;
    using native_std::expintl;
    using native_std::hermite;
    using native_std::hermitef;
    using native_std::hermitel;
    using native_std::legendre;
    using native_std::legendref;
    using native_std::legendrel;
    using native_std::laguerre;
    using native_std::laguerref;
    using native_std::laguerrel;
    using native_std::riemann_zeta;
    using native_std::riemann_zetaf;
    using native_std::riemann_zetal;
    using native_std::sph_bessel;
    using native_std::sph_besself;
    using native_std::sph_bessell;
    using native_std::sph_legendre;
    using native_std::sph_legendref;
    using native_std::sph_legendrel;
    using native_std::sph_neumann;
    using native_std::sph_neumannf;
    using native_std::sph_neumannl;
#endif  // BSLS_LIBRARYFEATURES_HAS_CPP17_SPECIAL_MATH_FUNCTIONS

}  // close package namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
