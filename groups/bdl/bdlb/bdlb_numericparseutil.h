// bdlb_numericparseutil.h                                            -*-C++-*-
#ifndef INCLUDED_BDLB_NUMERICPARSEUTIL
#define INCLUDED_BDLB_NUMERICPARSEUTIL

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide conversions from text into fundamental numeric types.
//
//@CLASSES:
//  bdlb::NumericParseUtil: namespace for parsing functions
//
//@DESCRIPTION: This component provides a namespace, 'bdlb::NumericParseUtil',
// containing utility functions for parsing ascii text representations of
// numeric values into the corresponding value of a fundamental C++ type (like
// 'int' or 'double').
//
// None of the parsing functions in this component consume leading whitespace.
// For parsing to succeed, the sought item must be found at the beginning of
// the input string.
//
// The following two subsections describe the grammar defining the parsing
// rules.
//
///DEFINITION OF SYMBOLS USED IN PRODUCTION RULES
///----------------------------------------------
//
// The following grammar is used to specify regular expressions:
//..
//   -     Within brackets the minus means through.  For example, [a-z] is
//         equivalent to [abcd...xyz].  The - can appear as itself only if used
//         as the first or last character.  For example, the character class
//         expression []-] matches the characters ] and -.
//
//   |     Logical OR between two expressions means one must be present.
//
//   ( ... ) Parentheses are used for grouping.  An operator, for example, *,
//         +, {}, can work on a single character or on a regular expression
//         enclosed in parentheses.  For example, (a*(cb+)*)$.
//..
///GRAMMAR PRODUCTION RULES
///------------------------
//..
// <NUMBER> ::= <OPTIONAL_SIGN><DIGIT>+
// <DECIMAL_NUMBER> ::= <OPTIONAL_SIGN><DECIMAL_DIGIT>+
// <POSITIVE_NUMBER> ::= <DIGIT>+
// <OPTIONAL_SIGN> ::= (+|-)?
// <DIGIT> ::= depending on base can include characters 0-9 and case-
//      insensitive letters.  For example, octal digit is in the range
//      [0 .. 7].
// <DECIMAL_DIGIT> ::= [0123456789]
// <OCTAL_DIGIT> ::= [01234567]
// <HEX_DIGIT> ::= [0123456789abcdefABCDEF]
// <SHORT> ::= <NUMBER>
//      <SHORT> must be in range [SHRT_MIN .. SHRT_MAX].
// <INT> ::= <NUMBER>
//      <INT> must be in range [INT_MIN .. INT_MAX].
// <INT64> ::= <NUMBER>
//      <INT64> must be in range
//                           [-0x8000000000000000uLL .. 0x7FFFFFFFFFFFFFFFuLL].
// <UNSIGNED> ::= <NUMBER>
//      <UNSIGED> must be in range [0 .. UINT_MAX].
// <UNSIGED64> ::= <NUMBER>
//      <UNSIGNED64> must be in range
//                           [0 .. 0xFFFFFFFFFFFFFFFFuLL].
// <REAL> ::= <OPTIONAL_SIGN>
//            (<DECIMAL_DIGIT>+ (. <DECIMAL_DIGIT>*)? | . <DECIMAL_DIGIT>+)
//            (e|E <DECIMAL_NUMBER>)
// <DOUBLE> ::= <REAL>
//      <DOUBLE> must be in range [DBL_MIN .. DBL_MAX].
// <EOS> ::= End of line, or ASCII 0.
//..
///ENDING POSITION (endPos)
///------------------------
// Each parsing function returns as its first, modifiable argument the
// character position in the input string immediately following the text of a
// successfully parsed token, or the position at which the parse failure was
// detected.  The value returned for '*endPos' will be the address of the
// character following the maximal prefix for a valid instance of the requested
// token.  If that prefix is itself valid, the parse function loads its result
// and returns 0; otherwise, it returns a non-zero value with no effect on the
// result.
//
///FLOATING POINT VALUES
///---------------------
// The conversion from text to values of type 'double' results in the closest
// representable value to the decimal text.  For example, the ASCII string
// "3.14159" is converted, on some platforms, to 3.1415899999999999.
//
// Note also that the 'NaN' class of 'double' values is not uniform across all
// platforms and is therefore not supported by these parsers.  Note also that
// the 'Inf' (infinity) 'double' values are not uniformly supported across all
// platforms and is therefore not supported by these parsers.  Note also that
// the hexadecimal 'double' values are not uniformly supported across all
// platforms and is therefore not supported by these parsers.
//
///Usage Example
///-------------
// In this section, we show the intended usage of this component.
//
///Example 1: Parsing an Integer Value from a 'StringRef'
/// - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose that we have a 'StringRef' that presumably contains a (not
// necessarily NUL terminated) string representing a 32-bit integer value and
// we want to convert that string into an 'int' (32-bit integer).
//
// First, we create the string:
//..
//  bslstl::StringRef input("20171023", 4);
//..
// Then we create the output variables for the parser:
//..
//  int                         result;
//  NumericParseUtil::size_type endPos;
//..
// Next we call the parser function:
//..
//  int rv = NumericParseUtil::parseInt(&endPos, &result, input);
//..
// Finally we verify the results:
//..
//  assert(0 == rv);
//  assert(4 == endPos);
//..


#ifndef INCLUDED_BDLSCM_VERSION
#include <bdlscm_version.h>
#endif

#ifndef INCLUDED_BSLS_TYPES
#include <bsls_types.h>
#endif

#ifndef INCLUDED_BSL_STRING
#include <bsl_string.h>
#endif

namespace BloombergLP {

namespace bdlb {
                          // =======================
                          // struct NumericParseUtil
                          // =======================

struct NumericParseUtil {
    // This 'struct' 'NumericParseUtil' provides a namespace for a suite of
    // stateless procedures that perform low-level parsing functionality.

  public:
    // TYPES
    typedef bslstl::StringRef::size_type size_type;
        // Shorter name for readability.

    // CLASS METHODS
    static int characterToDigit(char character, int base);
        // Determine whether the specified 'character' represents a digit in
        // the specified 'base'; return the numeric equivalent if so, and -1
        // otherwise.  The behavior is undefined if any argument is 0 and
        // unless '2 <= base' and 'base <= 36' (i.e., bases where digits are
        // representable by characters in the range ['0'-'9'], ['a'-'z'], or
        // ['A'-'Z']).

    static int parseDouble(size_type                *endPos,
                           double                   *result,
                           const bslstl::StringRef&  inputString);
        // Parse the specified 'inputString' for a sequence of characters
        // matching the production rule <DOUBLE> (see {GRAMMAR PRODUCTION
        // RULES}) and place into the specified 'result' the corresponding
        // value.  Store in the specified '*endPos' the position of the
        // character in 'inputString' immediately following the successfully
        // parsed text, or the position at which the parse failure was
        // detected.  Return zero on success, and a non-zero value otherwise.
        // The value of '*result' is unchanged if a parse failure occurs.  The
        // behavior is undefined if any argument is 0.  The behavior is also
        // undefined unless the current locale is the "C" locale, such that
        // 'setlocale(0, 0) == "C"'.

    static int parseInt(size_type                *endPos,
                        int                      *result,
                        const bslstl::StringRef&  inputString,
                        int                       base = 10);
        // Parse the specified 'inputString' for the maximal sequence of
        // characters forming an <INT> (see {GRAMMAR PRODUCTION RULES}) in the
        // optionally specified 'base' or in base 10 if 'base' is not
        // specified, and place into the specified 'result' the corresponding
        // value.  Store in the specified '*endPos' the position of the
        // character in 'inputString' immediately following the successfully
        // parsed text, or the position at which the parse failure was
        // detected.  If the parsed number is outside of the
        // '[INT_MIN .. INT_MAX]' range the 'result' will be the longest
        // number that does not over/underflow and 'endPos' will point to the
        // first digit that would make the number too large/small.  Return zero
        // on success, and a non-zero value otherwise.  The value of '*result'
        // is unchanged if a parse failure occurs.  The behavior is undefined
        // if any argument is 0 and unless '2 <= base' and 'base <= 36' (i.e.,
        // bases where digits are representable by characters in the range
        // ['0'-'9'], ['a'-'z'], or ['A'-'Z']).
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is empty.
        //   2. The first character of 'inputString' is not a valid digit in
        //      'base', or an optional sign followed by a valid digit.
        //..

    static int parseInt64(size_type                *endPos,
                          bsls::Types::Int64       *result,
                          const bslstl::StringRef&  inputString,
                          int                       base = 10);
        // Parse the specified 'inputString' for the maximal sequence of
        // characters forming a valid <INT64> (see {GRAMMAR PRODUCTION RULES})
        // in the optionally specified 'base' or in base 10 if 'base' is not
        // specified, and place into the specified 'result' the corresponding
        // value.  Store in the specified '*endPos' the position of the
        // character in 'inputString' immediately following the successfully
        // parsed text, or the position at which the parse failure was
        // detected.  If the parsed number is outside of the
        // '[-0x8000000000000000uLL .. 0x7FFFFFFFFFFFFFFFull]' range the
        // 'result' will be the longest number that does not over/underflow and
        // 'endPos' will point to the first digit that would make the number
        // too large/small.  Return zero on success, and a non-zero value
        // otherwise.  The value of '*result' is unchanged if a parse failure
        // occurs.  The behavior is undefined if any argument is 0 and unless
        // '2 <= base' and 'base <= 36' (i.e., bases where digits are
        // representable by characters in the range ['0'-'9'], ['a'-'z'], or
        // ['A'-'Z']).
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is empty.
        //   2. The first character of 'inputString' is not a valid digit in
        //      'base', or an optional sign followed by a valid digit.
        //..

    static int parseUint(size_type                *endPos,
                         unsigned int             *result,
                         const bslstl::StringRef&  inputString,
                         int                       base = 10);
        // Parse the specified 'inputString' for the maximal sequence of
        // characters forming an <UNSIGNED> (see {GRAMMAR PRODUCTION RULES}) in
        // the optionally specified 'base' or in base 10 if 'base' is not
        // specified, and place into the specified 'result' the corresponding
        // value.  Store in the specified '*endPos' the position of the
        // character in 'inputString' immediately following the successfully
        // parsed text, or the position at which the parse failure was
        // detected.  If the parsed number is outside of the '[0 .. UINT_MAX]'
        // range the 'result' will be the longest number that does not
        // overflow and 'endPos' will point to the first digit that would make
        // the number too large.  Return zero on success, and a non-zero value
        // otherwise.  The value of '*result' is unchanged if a parse failure
        // occurs.  The behavior is undefined if any argument is 0 and unless
        // '2 <= base' and 'base <= 36' (i.e., bases where digits are
        // representable by characters in the range ['0'-'9'], ['a'-'z'], or
        // ['A'-'Z']).
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is empty.
        //   2. The first character of 'inputString' is not a valid digit in
        //      'base', or an optional sign followed by a valid digit.
        //..

    static int parseUint64(size_type                *endPos,
                           bsls::Types::Uint64      *result,
                           const bslstl::StringRef&  inputString,
                           int                       base = 10);
        // Parse the specified 'inputString' for the maximal sequence of
        // characters forming a valid <UNSIGNED64> (see {GRAMMAR PRODUCTION
        // RULES}) in the optionally specified 'base' or in base 10 if 'base'
        // is not specified, and place into the specified 'result' the
        // corresponding value.  Store in the specified '*endPos' the position
        // of the character in 'inputString' immediately following the
        // successfully parsed text, or the position at which the parse failure
        // was detected.  If the parsed number is outside of the
        // '[0 .. 0XFFFFFFFFFFFFFFFF]' range the 'result' will be the longest
        // number that does not overflow and 'endPos' will point to the first
        // digit that would make the number too large.  Return zero on success,
        // and a non-zero value otherwise.  The value of '*result' is unchanged
        // if a parse failure occurs.  The behavior is undefined if any
        // argument is 0 and unless '2 <= base' and 'base <= 36' (i.e., bases
        // where digits are representable by characters in the range ['0'-'9'],
        // ['a'-'z'], or ['A'-'Z']).
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is empty.
        //   2. The first character of 'inputString' is not a valid digit in
        //      'base', or an optional sign followed by a valid digit.
        //..

    static int parseShort(size_type                *endPos,
                          short                    *result,
                          const bslstl::StringRef&  inputString,
                          int                       base = 10);
        // Parse the specified 'inputString' for the maximal sequence of
        // characters forming a valid <SHORT> (see {GRAMMAR PRODUCTION RULES})
        // in the optionally specified 'base' or in base 10 if 'base' is not
        // specified, and place into the specified 'result' the corresponding
        // value.  Store in the specified '*endPos' the position of the
        // character in 'inputString' immediately following the successfully
        // parsed text, or the position at which the parse failure was
        // detected.  If the parsed number is outside of the
        // '[SHRT_MIN .. SHRT_MAX]' range the 'result' will be the longest
        // number that does not over/underflow and 'endPos' will point to the
        // first digit that would make the number too large/small.  Return zero
        // on success, and a non-zero value otherwise.  Return zero on success,
        // and a non-zero value otherwise.  The value of '*result' is unchanged
        // if a parse failure occurs.  The behavior is undefined if any
        // argument is 0, and unless '2 <= base' and 'base <= 36' (i.e., bases
        // where digits are representable by characters in the range ['0'-'9'],
        // ['a'-'z'] or ['A'-'Z']).
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is empty.
        //   2. The first character of 'inputString' is not a valid digit in
        //      'base', or an optional sign followed by a valid digit.
        //..

    static int parseSignedInteger(size_type                *endPos,
                                  bsls::Types::Int64       *result,
                                  const bslstl::StringRef&  inputString,
                                  int                       base,
                                  const bsls::Types::Int64  minValue,
                                  const bsls::Types::Int64  maxValue);
        // Parse the specified 'inputString' for an optional sign followed by a
        // sequence of characters representing digits in the specified 'base',
        // consuming the maximum that will form a number whose value is less
        // than or equal to the specified 'maxValue' and greater than or equal
        // to the specified 'minValue'.  Place into the specified 'result' the
        // extracted value, and store in the specified '*endPos' the position
        // of the character in 'inputString' immediately following the
        // successfully parsed text, or the position at which the parse failure
        // was detected.  Return 0 on success, and a non-zero value otherwise.
        // The value of '*result' is unchanged if a parse failure occurs.  The
        // behavior is undefined unless 'maxValue' a positive integer, and
        // 'minValue' is negative (this is required to allow for efficient
        // implementation).  The behavior is also undefined if any argument is
        // 0, and unless '2 <= base' and 'base <= 36', (i.e., bases where
        // digits are representable by characters '[ 0 .. 9 ]', '[ a .. z ]' or
        // '[ A .. Z ]').
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The parsed string is not a '<NUMBER>', i.e., does not contain
        //      an optional sign followed by at least one digit.
        //   2. The first digit in 'inputString' is larger than 'maxValue' or
        //      smaller than 'minValue'.
        //   3. The first digit is not a valid number for the 'base'.
        //..

    static int parseUnsignedInteger(size_type                 *endPos,
                                    bsls::Types::Uint64       *result,
                                    const bslstl::StringRef&   inputString,
                                    int                        base,
                                    const bsls::Types::Uint64  maxValue);
    static int parseUnsignedInteger(size_type                 *endPos,
                                    bsls::Types::Uint64       *result,
                                    const bslstl::StringRef&   inputString,
                                    int                        base,
                                    const bsls::Types::Uint64  maxValue,
                                    int                        maxNumDigits);
        // Parse the specified 'inputString' for a sequence of characters
        // representing digits in the specified 'base', consuming the maximum
        // up to the optionally specified 'maxNumDigits' that form a number
        // whose value does not exceed the specified 'maxValue'.  Place into
        // the specified 'result' the extracted value, and store in the
        // specified '*endPos' the position of the character in 'inputString'
        // immediately following the successfully parsed text, or the position
        // at which the parse failure was detected.  Return 0 on success, and a
        // non-zero value otherwise.  The value of '*result' is unchanged if a
        // parse failure occurs.  If 'maxNumDigits' is not specified, it
        // defaults to a number larger than the number of possible digits in an
        // unsigned 64-bit integer.  The behavior is undefined if any argument
        // is 0, and unless '2 < = base' and  'base <= 36' (i.e., bases where
        // digits are representable by characters '[ 0 .. 9 ]', '[ a .. z ]' or
        // '[ A .. Z ]').
        //
        // A parse failure can occur for the following reasons:
        //..
        //   1. The 'inputString' is not a '<POSITIVE_NUMBER>', i.e., does
        //      not begin with a digit.
        //   2. The first digit in 'inputString' is larger than 'maxValue'.
        //   3. The first digit is not a valid number for the 'base'.
        //..
};

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2017 Bloomberg Finance L.P.
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
