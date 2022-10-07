// bdlb_guidutil.h                                                    -*-C++-*-
#ifndef INCLUDED_BDLB_GUIDUTIL
#define INCLUDED_BDLB_GUIDUTIL

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide functions that produce Globally Unique Identifiers.
//
//@CLASSES:
//  bdlb::GuidUtil: namespace for methods for creating GUIDs
//
//@SEE_ALSO: bdlb_guid
//
//@DESCRIPTION: This component provides a 'struct', 'bdlb::GuidUtil', that
// serves as a namespace for utility functions that create and work with
// Globally Unique Identifiers (GUIDs).
//
// Note that all the GUIDs generated by this component are actually Universally
// Unique Identifiers (UUIDs), which are a type of GUID.  The two terms will be
// used interchangeably in the documentation below.
//
///Grammar for GUIDs Used in 'guidFromString'
///------------------------------------------
// This conversion performed by 'guidFromString' is intended to be used for
// GUIDs generated by external sources that have a variety of formats.
//
///GUID String Format
///------------------
//..
// <SPEC>             ::=  <BRACED GUID>     |   <GUID>
//
// <BRACED GUID>      ::=  '[' <GUID> ']'    |   '[ ' <GUID> ' ]'
//                         '{' <GUID> '}'    |   '{ ' <GUID> ' }'
//
// <GUID>             ::=  <FORMATTED GUID>   |   <UNFORMATTED GUID>
//
// <FORMATTED GUID>    ::=  <X>{4} '-' <X>{2} '-' <X>{2} '-' <X>{2} '-' <X>{6}
//
// <UNFORMATTED GUID> ::=  <X>{16}
//
// <X>                ::=  [0123456789ABCDEFabcdef]{2}
//
//
// EXAMPLES:
// ---------
// { 87654321-AAAA-BBBB-CCCC-012345654321 }
// 00010203-0405-0607-0809-101112131415
// [00112233445566778899aAbBcCdDeEfF]
//..
//
///Cryptographic Security
///----------------------
// 'GuidUtil' provides three families of functions for generating GUIDs:
// 'generate', 'generateNonSecure', and 'generateFromName'.  The 'generate' and
// 'generateNonSecure' methods use random number generators, with the slower
// 'generate' methods aiming to produce cryptographically secure UUIDs by
// accessing underlying system resources to obtain truly random numbers, and
// the faster 'generateNonSecure' methods using a fast high-quality (but not
// strictly cryptographically secure) in-process random-number generator.
//
// The 'generateFromName' method does not use random numbers, but produces a
// UUID deterministically based on a given name and namespace.  The user should
// heed the following admonition in RFC 4122: "Do not assume that UUIDs are
// hard to guess; they should not be used as security capabilities (identifiers
// whose mere possession grants access), for example."  In addition,
// applications that generate name-based UUIDs from untrusted inputs must not
// assume that such UUIDs will be unique, since collision attacks are already
// known against the SHA-1 hash algorithm.
//
///Usage
///-----
// Suppose we are building a system for managing records for employees in a
// large international firm.  These records have no natural field which can be
// used as a unique ID, so a GUID must be created for each employee.
//
// First let us define a value-type for employees.
//..
//  class MyEmployee {
//      // This class provides a value-semantic type to represent an employee
//      // record.  These records are for internal use only.
//..
// For the sake of brevity, we provide a limited amount of data in each record.
// We additionally show a very limited scope of functionality.
//..
//      // DATA
//      bsl::string  d_name;    // name of the employee
//      double       d_salary;  // salary in some common currency
//      bdlb::Guid   d_guid;    // a GUID for the employee
//
//    public:
//      // CREATORS
//      MyEmployee(const string& name, double salary);
//          // Create an object with the specified 'name' and specified
//          //'salary', generating a new GUID to represent the employee.
//
//      // ...
//
//      // ACCESSORS
//      const bdlb::Guid& Guid() const;
//          // Return the 'guid' of this object.
//
//      const bsl::string& name() const;
//          // Return the 'name' of this object.
//
//      double salary() const;
//          // Return the 'salary' of this object.
//      // ...
//  };
//..
// Next, we create free functions 'operator<' and 'operator==' to allow
// comparison of 'MyEmployee' objects.  We take advantage of the monotonically
// increasing nature of sequential GUIDs to implement these methods.
//..
//
//  bool operator== (const MyEmployee& lhs, const MyEmployee& rhs);
//      // Return 'true' if the specified 'lhs' object has the same value as
//      // the specified 'rhs' object, and 'false' otherwise.  Note that two
//      // 'MyEmployee' objects have the same value if they have the same
//      // guid.
//
//  bool operator< (const MyEmployee& lhs, const MyEmployee& rhs);
//      // Return 'true' if the value of the specified 'lhs' MyEmployee object
//      // is less than the value of the specified 'rhs' MyEmployee object,
//      // and 'false' otherwise.  A MyEmployee object is less than another if
//      // the guid is less than the other.
// ...
//
//  // CREATORS
//  MyEmployee::MyEmployee(const string& name, double salary)
//  : d_name(name)
//  , d_salary(salary)
//  {
//      bdlb::GuidUtil::generate(&d_guid);
//  }
//
//  // ACCESSORS
//  const bdlb::Guid& MyEmployee::Guid() const
//  {
//      return d_guid;
//  }
//
//  const bsl::string& MyEmployee::name() const
//  {
//      return d_name;
//  }
//
//  double MyEmployee::salary() const
//  {
//      return d_salary;
//  }
//
//  // FREE FUNCTIONS
//  bool operator==(const MyEmployee& lhs, const MyEmployee& rhs)
//  {
//      return lhs.Guid() == rhs.Guid();
//  }
//
//  bool operator<(const MyEmployee& lhs, const MyEmployee& rhs)
//  {
//       return lhs.Guid() < rhs.Guid();
//  }
//..
// Next, we create some employees:
//..
//      MyEmployee e1("Foo Bar"    , 1011970);
//      MyEmployee e2("John Doe"   , 12345);
//      MyEmployee e3("Joe Six-pack", 1);
//..
// Finally, we verify that the generated GUIDs are unique.
//..
//      assert(e1 < e2 || e2 < e1);
//      assert(e2 < e3 || e3 < e2);
//      assert(e1 < e3 || e3 < e1);
//..

#include <bdlscm_version.h>

#include <bdlb_guid.h>
#include <bdlb_pcgrandomgenerator.h>

#include <bslmf_assert.h>

#include <bsls_assert.h>
#include <bsls_libraryfeatures.h>
#include <bsls_types.h>

#include <bsl_array.h>
#include <bsl_cstddef.h>
#include <bsl_cstdint.h>
#include <bsl_string.h>

#include <string>

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
#include <memory_resource>
#endif

namespace BloombergLP {
namespace bdlb {

                              // ===================
                              // class GuidState_Imp
                              // ===================

class GuidState_Imp {
    // This component-private 'class' describes holds the PCG generators and
    // generation functions for use by 'GuidUtil'.

  public:
    // PUBLIC CLASS CONSTANTS
    enum {
        k_GENERATOR_COUNT = 4
    };

  private:
    // DATA
    bsl::array<bdlb::PcgRandomGenerator, k_GENERATOR_COUNT> d_generators;

  public:

    // MANIPULATORS
    void generateRandomBits(
                       bsl::uint32_t (*out)[GuidState_Imp::k_GENERATOR_COUNT]);
        // Populate the specified 'out' with the results of calling 'generate'
        // on the internal random generators.

    void seed(const bsl::array<bsl::uint64_t, k_GENERATOR_COUNT>& state);
        // Seed the internal generators based on the specified 'state' values.
};

                              // ===============
                              // struct GuidUtil
                              // ===============

struct GuidUtil {
    // This 'struct' provides a namespace for functions that create Universally
    // Unique Identifiers per RFC 4122 (http://www.ietf.org/rfc/rfc4122.txt).

    // CLASS METHODS
    static void generate(Guid *result, bsl::size_t numGuids = 1);
        // Generate a sequence of GUIDs meeting the RFC 4122 version 4
        // specification, and load the resulting GUIDs into the array referred
        // to by the specified 'result'.  Optionally specify 'numGuids',
        // indicating the number of GUIDs to load into the 'result' array.  If
        // 'numGuids' is not supplied, a default of 1 is used.  An RFC 4122
        // version 4 GUID consists of 122 randomly generated bits, two
        // "variant" bits set to '10', and four "version" bits set to '0100'.
        // The behavior is undefined unless 'result' refers to a contiguous
        // sequence of at least 'numGuids' 'Guid' objects.

    static Guid generate();
        // Generate and return a single GUID meeting the RFC 4122 version 4
        // specification, consisting of 122 randomly generated bits, two
        // "variant" bits set to '10', and four "version" bits set to '0100'.

    static void generateNonSecure(Guid *result, bsl::size_t numGuids = 1);
        // Generate a sequence of GUIDs meeting the RFC 4122 version 4
        // specification, and load the resulting GUIDs into the array referred
        // to by the specified 'result'.  Optionally specify 'numGuids',
        // indicating the number of GUIDs to load into the 'result' array.  If
        // 'numGuids' is not supplied, a default of 1 is used.  An RFC 4122
        // version 4 GUID consists of 122 randomly generated bits, two
        // "variant" bits set to '10', and four "version" bits set to '0100'.
        // The behavior is undefined unless 'result' refers to a contiguous
        // sequence of at least 'numGuids' 'Guid' objects.  Note that this
        // function generates high quality, albeit not cryptographically
        // secure, random numbers for GUIDs.

    static Guid generateNonSecure();
        // Generate and return a single GUID meeting the RFC 4122 version 4
        // specification, consisting of 122 randomly generated bits, two
        // "variant" bits set to '10', and four "version" bits set to '0100'.
        // Note that this function generates high quality, albeit not
        // cryptographically secure, random numbers for GUIDs.

    static Guid generateFromName(const Guid&             namespaceId,
                                 const bsl::string_view& name);
        // Generate and return a single GUID meeting the RFC 4122 version 5
        // specification from the specified 'namespaceId' and 'name'.
        // 'namespaceId' may (but need not) be one of the pre-defined namespace
        // IDs.  Note that this method is a pure function of its arguments.

    static Guid dnsNamespace();
        // Return the pre-defined namespace ID for the DNS namespace from
        // Appendix C of RFC 4122, for use with the 'generateFromName' method
        // when the name string is a fully-qualified domain name.

    static Guid urlNamespace();
        // Return the pre-defined namespace ID for the URL namespace from
        // Appendix C of RFC 4122, for use with the 'generateFromName' method
        // when the name string is a URL.

    static Guid oidNamespace();
        // Return the pre-defined namespace ID for the OID namespace from
        // Appendix C of RFC 4122, for use with the 'generateFromName' method
        // when the name string is an ISO Object ID (OID).

    static Guid x500Namespace();
        // Return the pre-defined namespace ID for the X500 namespace from
        // Appendix C of RFC 4122, for use with the 'generateFromName' method
        // when the name string is an X.500 Distinguished Name.

    static int guidFromString(Guid                    *result,
                              const bsl::string_view&  guidString);
        // Parse the specified 'guidString' (in {GUID String Format}) and load
        // its value into the specified 'result'.  Return 0 if 'result'
        // successfully loaded, and a non-zero value otherwise.

    static Guid guidFromString(const bsl::string_view& guidString);
        // Parse the specified 'guidString' (in {GUID String Format}) and
        // return the converted GUID, or a default-constructed 'Guid' if the
        // string is improperly formatted.

    static void guidToString(bsl::string *result, const Guid& guid);
    static void guidToString(std::string *result, const Guid& guid);
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
    static void guidToString(std::pmr::string *result, const Guid& guid);
#endif
        // Serialize the specified 'guid' into the specified 'result'.  The
        // 'result' string will be in a format suitable for 'guidFromString'.

    static bsl::string guidToString(const Guid& guid);
        // Convert the specified 'guid' into a string suitable for
        // 'guidFromString', and return the string.

    static int getVersion(const bdlb::Guid& guid);
        // Return the version of the specified 'guid' object.  The behavior is
        // undefined unless the contents of the 'guid' object are compliant
        // with RFC 4122.

    static bsls::Types::Uint64 getMostSignificantBits(const Guid& guid);
        // Return the most significant 8 bytes of the specified 'guid'.

    static bsls::Types::Uint64 getLeastSignificantBits(const Guid& guid);
        // Return the least significant 8 bytes of the specified 'guid'.

    // DEPRECATED CLASS METHODS
    static void generate(unsigned char *result, bsl::size_t numGuids = 1);
        // !DEPRECATED!: Use 'generate(Guid *, size_t)' instead.
        //
        // Generate a sequence of GUIDs meeting the RFC 4122 version 4
        // specification, and load the bytes of the resulting GUIDs into the
        // array referred to by the specified 'result'.  Optionally specify
        // 'numGuids', indicating the number of GUIDs to load into the 'result'
        // array.  If 'numGuids' is not supplied, a default of 1 is used.  An
        // RFC 4122 version 4 GUID consists of 122 randomly generated bits, two
        // "variant" bits set to '10', and four "version" bits set to '0100'.
        // The behavior is undefined unless 'result' refers to a contiguous
        // sequence of at least '16 * numGuids' bytes.
};

// ============================================================================
//                      INLINE DEFINITIONS
// ============================================================================

                              // -------------------
                              // class GuidState_Imp
                              // -------------------


// MANIPULATORS
inline
void GuidState_Imp::generateRandomBits(
              bsl::uint32_t (*out)[GuidState_Imp::k_GENERATOR_COUNT])
{
    for (int i = 0;  i < GuidState_Imp::k_GENERATOR_COUNT; i++) {
        (*out)[i] = d_generators[i].generate();
    }
}

                              // ---------------
                              // struct GuidUtil
                              // ---------------
// CLASS METHODS
inline
void GuidUtil::generate(Guid *result, bsl::size_t numGuids)
{
    generate(reinterpret_cast<unsigned char *>(result), numGuids);
}

inline
int GuidUtil::getVersion(const Guid& guid)
{
    return (guid[6] & 0xF0) >> 4;
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
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
