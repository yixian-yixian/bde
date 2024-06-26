// bdljsn_jsontype.cpp                                                -*-C++-*-
#include <bdljsn_jsontype.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdljsn_jsontype_cpp, "$Id$ $CSID$")

#include <bdlb_print.h>

#include <bsl_ostream.h>

namespace BloombergLP {
namespace bdljsn {

                               // --------------
                               // class JsonType
                               // --------------

// CLASS METHODS
bsl::ostream& JsonType::print(bsl::ostream&  stream,
                              JsonType::Enum value,
                              int            level,
                              int            spacesPerLevel)
{
    bdlb::Print::indent(stream, level, spacesPerLevel);

    stream << JsonType::toAscii(value);

    if (spacesPerLevel >= 0) {
        stream << '\n';
    }

    return stream;
}

const char *JsonType::toAscii(JsonType::Enum value)
{
#define CASE(X) case(e_ ## X): return #X;

    switch (value) {
      CASE(OBJECT)
      CASE(ARRAY)
      CASE(STRING)
      CASE(NUMBER)
      CASE(BOOLEAN)
      CASE(NULL)

      default: return "(* UNKNOWN *)";
    }

#undef CASE
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2022 Bloomberg Finance L.P.
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
