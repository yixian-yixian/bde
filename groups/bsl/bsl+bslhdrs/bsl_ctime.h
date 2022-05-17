// bsl_ctime.h                                                        -*-C++-*-
#ifndef INCLUDED_BSL_CTIME
#define INCLUDED_BSL_CTIME

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

#include <bsls_nativestd.h>

#include <ctime>

namespace bsl {
    // Import selected symbols into bsl namespace

    using native_std::asctime;
    using native_std::clock;
    using native_std::clock_t;
    using native_std::ctime;
    using native_std::difftime;
    using native_std::gmtime;
    using native_std::localtime;
    using native_std::mktime;
    using native_std::size_t;
    using native_std::strftime;
    using native_std::time;
    using native_std::time_t;
    using native_std::tm;

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_TIMESPEC_GET
    using native_std::timespec_get;
    using native_std::timespec;
#endif
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
