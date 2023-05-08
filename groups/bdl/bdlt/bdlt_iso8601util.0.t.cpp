// bdlt_iso8601util.t.cpp                                             -*-C++-*-
#include <bdlt_iso8601util.h>

#include <bdlt_date.h>
#include <bdlt_datetime.h>
#include <bdlt_datetimetz.h>
#include <bdlt_datetz.h>
#include <bdlt_time.h>
#include <bdlt_timetz.h>

#include <bdlb_chartype.h>

#include <bslim_testutil.h>

#include <bsls_asserttest.h>
#include <bsls_review.h>

#include <bsl_climits.h>
#include <bsl_cstdlib.h>
#include <bsl_cstring.h>
#include <bsl_iostream.h>
#include <bsl_sstream.h>
#include <bsl_string.h>

#undef SEC

using namespace BloombergLP;
using bsl::cout;
using bsl::cerr;
using bsl::endl;
using bsl::ends;
using bsl::flush;
using bsl::ptrdiff_t;

//=============================================================================
//                             TEST PLAN
//-----------------------------------------------------------------------------
//                              Overview
//                              --------
// The component under test consists of a suite of static member functions
// (pure functions) that perform conversions between the values of several
// 'bdlt' vocabulary types and corresponding string representations, where the
// latter are defined by the ISO 8601 standard.  The general plan is that each
// function is to be independently tested using the table-driven technique.  A
// set of test vectors is defined globally for use in testing all functions.
// This global data is sufficient for thoroughly testing the string generating
// functions, but additional test vectors are required to address concerns
// specific to the string parsing functions.  Hence, additional test data is
// defined locally to the test cases that verify parsing.
//
// Global Concerns:
//: o No memory is ever allocated from the global allocator.
//: o Precondition violations are detected in appropriate build modes.
//-----------------------------------------------------------------------------
// CLASS METHODS
// [ 1] int generate(char *, int, const TimeInterval&);
// [ 1] int generate(char *, int, const TimeInterval&, const Config&);
// [ 2] int generate(char *, int, const Date&);
// [ 2] int generate(char *, int, const Date&, const Config&);
// [ 3] int generate(char *, int, const Time&);
// [ 3] int generate(char *, int, const Time&, const Config&);
// [ 4] int generate(char *, int, const Datetime&);
// [ 4] int generate(char *, int, const Datetime&, const Config&);
// [ 5] int generate(char *, int, const DateTz&);
// [ 5] int generate(char *, int, const DateTz&, const Config&);
// [ 6] int generate(char *, int, const TimeTz&);
// [ 6] int generate(char *, int, const TimeTz&, const Config&);
// [ 7] int generate(char *, int, const DatetimeTz&);
// [ 7] int generate(char *, int, const DatetimeTz&, const Config&);
// [12] int generate(char *, int, const DateOrDateTz&);
// [12] int generate(char *, int, const DateOrDateTz&, const Config&);
// [13] int generate(char *, int, const TimeOrTimeTz&);
// [13] int generate(char *, int, const TimeOrTimeTz&, const Config&);
// [14] int generate(char *, int, const DatetimeOrDatetimeTz&);
// [14] int generate(char*,int,const DatetimeOrDatetimeTz&,const Config&);
// [ 1] int generate(string *, const TimeInterval&);
// [ 1] int generate(string *, const TimeInterval&, const Config&);
// [ 2] int generate(string *, const Date&);
// [ 2] int generate(string *, const Date&, const Config&);
// [ 3] int generate(string *, const Time&);
// [ 3] int generate(string *, const Time&, const Config&);
// [ 4] int generate(string *, const Datetime&);
// [ 4] int generate(string *, const Datetime&, const Config&);
// [ 5] int generate(string *, const DateTz&);
// [ 5] int generate(string *, const DateTz&, const Config&);
// [ 6] int generate(string *, const TimeTz&);
// [ 6] int generate(string *, const TimeTz&, const Config&);
// [ 7] int generate(string *, const DatetimeTz&);
// [ 7] int generate(string *, const DatetimeTz&, const Config&);
// [12] int generate(string *, const DateOrDateTz&);
// [12] int generate(string *, const DateOrDateTz&, const Config&);
// [13] int generate(string *, const TimeOrTimeTz&);
// [13] int generate(string *, const TimeOrTimeTz&, const Config&);
// [14] int generate(string *, const DatetimeOrDatetimeTz&);
// [14] int generate(string*, const DatetimeOrDatetimeTz&, const Config&);
// [ 1] ostream generate(ostream&, const TimeInterval&);
// [ 1] ostream generate(ostream&, const TimeInterval&, const Config&);
// [ 2] ostream generate(ostream&, const Date&);
// [ 2] ostream generate(ostream&, const Date&, const Config&);
// [ 3] ostream generate(ostream&, const Time&);
// [ 3] ostream generate(ostream&, const Time&, const Config&);
// [ 4] ostream generate(ostream&, const Datetime&);
// [ 4] ostream generate(ostream&, const Datetime&, const Config&);
// [ 5] ostream generate(ostream&, const DateTz&);
// [ 5] ostream generate(ostream&, const DateTz&, const Config&);
// [ 6] ostream generate(ostream&, const TimeTz&);
// [ 6] ostream generate(ostream&, const TimeTz&, const Config&);
// [ 7] ostream generate(ostream&, const DatetimeTz&);
// [ 7] ostream generate(ostream&, const DatetimeTz&, const Config&);
// [12] ostream generate(ostream&, const DateOrDateTz&);
// [12] ostream generate(ostream&, const DateOrDateTz&, const Config&);
// [13] ostream generate(ostream&, const TimeOrTimeTz&);
// [13] ostream generate(ostream&, const TimeOrTimeTz&, const Config&);
// [14] ostream generate(ostream&, const DatetimeOrDatetimeTz&);
// [14] ostream generate(ostream&,const DatetimeOrDatetimeTz&,const Con&);
// [ 1] int generateRaw(char *, const TimeInterval&);
// [ 1] int generateRaw(char *, const TimeInterval&, const Config&);
// [ 2] int generateRaw(char *, const Date&);
// [ 2] int generateRaw(char *, const Date&, const Config&);
// [ 3] int generateRaw(char *, const Time&);
// [ 3] int generateRaw(char *, const Time&, const Config&);
// [ 4] int generateRaw(char *, const Datetime&);
// [ 4] int generateRaw(char *, const Datetime&, const Config&);
// [ 5] int generateRaw(char *, const DateTz&);
// [ 5] int generateRaw(char *, const DateTz&, const Config&);
// [ 6] int generateRaw(char *, const TimeTz&);
// [ 6] int generateRaw(char *, const TimeTz&, const Config&);
// [ 7] int generateRaw(char *, const DatetimeTz&);
// [ 7] int generateRaw(char *, const DatetimeTz&, const Config&);
// [12] int generateRaw(char *, const DateOrDateTz&);
// [12] int generateRaw(char *, const DateOrDateTz&, const Config&);
// [13] int generateRaw(char *, const TimeOrTimeTz&);
// [13] int generateRaw(char *, const TimeOrTimeTz&, const Config&);
// [14] int generateRaw(char *, const DatetimeOrDatetimeTz&);
// [14] int generateRaw(char*,const DatetimeOrDatetimeTz&, const Config&);
// [ 8] int parse(TimeInterval *, const char *, int);
// [ 9] int parse(Date *, const char *, int);
// [10] int parse(Time *, const char *, int);
// [ 9] int parse(DateTz *, const char *, int);
// [10] int parse(TimeTz *, const char *, int);
// [15] int parse(DateOrDateTz *, const char *, int);
// [16] int parse(TimeOrTimeTz *, const char *, int);
// [ 8] int parse(TimeInterval *result, const StringRef& string);
// [ 9] int parse(Date *result, const StringRef& string);
// [10] int parse(Time *result, const StringRef& string);
// [ 9] int parse(DateTz *result, const StringRef& string);
// [10] int parse(TimeTz *result, const StringRef& string);
// [15] int parse(DateOrDateTz *result, const StringRef& string);
// [15] int parse(TimeOrTimeTz *result, const StringRef& string);
// [17] int parse(DatetimeOrDatetimeTz *, const char *, int);
// [17] int parse(DatetimeOrDatetimeTz *result, const StringRef& string);
// [17] int parseRelaxed(DatetimeOrDatetimeTz *, const char *, int);
// [17] int parseRelaxed(DatetimeOrDatetimeTz *, const bsl::string_view&);
// [11] int parse(Datetime *, const char *, int);
// [11] int parse(DatetimeTz *, const char *, int);
// [11] int parse(Datetime *result, const StringRef& string);
// [11] int parse(DatetimeTz *result, const StringRef& string);
// [11] int parseRelaxed(Datetime *, const char *, int);
// [11] int parseRelaxed(DatetimeTz *, const char *, int);
// [11] int parseRelaxed(Datetime *, const bsl::string_view&);
// [11] int parseRelaxed(DatetimeTz *, const bsl::string_view&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
// [ 2] int generate(char *, const Date&, int);
// [ 3] int generate(char *, const Time&, int);
// [ 4] int generate(char *, const Datetime&, int);
// [ 5] int generate(char *, const DateTz&, int);
// [ 6] int generate(char *, const TimeTz&, int);
// [ 7] int generate(char *, const DatetimeTz&, int);
// [ 5] int generate(char *, const DateTz&, int, bool useZ);
// [ 6] int generate(char *, const TimeTz&, int, bool useZ);
// [ 7] int generate(char *, const DatetimeTz&, int, bool useZ);
// [ 5] ostream generate(ostream&, const DateTz&, bool useZ);
// [ 6] ostream generate(ostream&, const TimeTz&, bool useZ);
// [ 7] ostream generate(ostream&, const DatetimeTz&, bool useZ);
// [ 5] int generateRaw(char *, const DateTz&, bool useZ);
// [ 6] int generateRaw(char *, const TimeTz&, bool useZ);
// [ 7] int generateRaw(char *, const DatetimeTz&, bool useZ);
#endif // BDE_OMIT_INTERNAL_DEPRECATED
//-----------------------------------------------------------------------------
// [18] USAGE EXAMPLE
//-----------------------------------------------------------------------------

// ============================================================================
//                     STANDARD BDE ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        cout << "Error " __FILE__ "(" << line << "): " << message
             << "    (failed)" << endl;

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLIM_TESTUTIL_ASSERT
#define ASSERTV      BSLIM_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLIM_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLIM_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLIM_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLIM_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLIM_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLIM_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLIM_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLIM_TESTUTIL_LOOP6_ASSERT

#define Q            BSLIM_TESTUTIL_Q   // Quote identifier literally.
#define P            BSLIM_TESTUTIL_P   // Print identifier and value.
#define P_           BSLIM_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLIM_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLIM_TESTUTIL_L_  // current Line number

// ============================================================================
//                  NEGATIVE-TEST MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT_SAFE_PASS(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_PASS(EXPR)
#define ASSERT_SAFE_FAIL(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL(EXPR)
#define ASSERT_PASS(EXPR)      BSLS_ASSERTTEST_ASSERT_PASS(EXPR)
#define ASSERT_FAIL(EXPR)      BSLS_ASSERTTEST_ASSERT_FAIL(EXPR)
#define ASSERT_OPT_PASS(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_PASS(EXPR)
#define ASSERT_OPT_FAIL(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_FAIL(EXPR)

//=============================================================================
//                  GLOBALS, TYPEDEFS, CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

typedef bdlt::Iso8601Util              Util;
typedef bdlt::Iso8601UtilConfiguration Config;
typedef bsl::string_view               StrView;

const int k_INTERVAL_MAX_PRECISION   = 9;
const int k_DATE_MAX_PRECISION       = 3;
const int k_DATETZ_MAX_PRECISION     = 3;
const int k_DATETIME_MAX_PRECISION   = 6;
const int k_DATETIMETZ_MAX_PRECISION = 6;
const int k_TIME_MAX_PRECISION       = 6;
const int k_TIMETZ_MAX_PRECISION     = 6;

// ============================================================================
//                             GLOBAL TEST DATA
// ----------------------------------------------------------------------------

// Define DEFAULT DATA generally usable across 'generate' and 'parse' test
// cases.

// *** 'TimeInterval' Data ***

struct DefaultIntervalDataRow {
    int                 d_line;         // source line number
    bsls::Types::Int64  d_sec;          // seconds
    int                 d_usec;         // nanoseconds
    const char         *d_iso8601;      // ISO 8601 string
    bool                d_canonical;    // There are many strings which can be
                                        // parsed into the same TimeInterval,
                                        // but for each TimeInterval there is
                                        // only one canonical string
                                        // representation (the string returned
                                        // by the generate functions).

};

static const DefaultIntervalDataRow DEFAULT_INTERVAL_DATA[] = {
    //LINE             SECONDS             NANOSECONDS
    //---- ------------------------------  -----------
    //                                       ISO8601                CANONICAL
    //                        ------------------------------------  ---------
    { L_,                               0,         0,
                              "P0W",                                   false },
    { L_,                               0,         0,
                              "P0D",                                   false },
    { L_,                               0,         0,
                              "PT0H",                                  false },
    { L_,                               0,         0,
                              "PT0M",                                  false },
    { L_,                               0,         0,
                              "PT0S",                                  false },
    { L_,                               0,         0,
                              "PT0.000000000S",                         true },
    { L_,                               0,         0,
                              "P0DT0M",                                false },
    { L_,                               0,         0,
                              "P0DT0S",                                false },
    { L_,                               0,         0,
                              "P0W0DT0H0M0S",                          false },
    { L_,                               0,         0,
                              "P0W0DT0H0M0.0S",                        false },
    { L_,                               0,         0,
                              "P0W0DT0H0M0.000000000S",                false },
    { L_,                               0,         0,
                              "P0W0DT0H0M0.0000000000S",               false },
    { L_,                          604800,         0,
                              "P1W",                                   false },
    { L_,                          604800,         0,
                              "P1WT0.000000000S",                       true },
    { L_,                           86400,         0,
                              "P1D",                                   false },
    { L_,                           86400,         0,
                              "P1DT0.000000000S",                       true },
    { L_,                            3600,         0,
                              "PT1H",                                  false },
    { L_,                            3600,         0,
                              "PT1H0.000000000S",                       true },
    { L_,                              60,         0,
                              "PT1M",                                  false },
    { L_,                              60,         0,
                              "PT1M0.000000000S",                       true },
    { L_,                               1,         0,
                              "PT1S",                                  false },
    { L_,                               1,         0,
                              "PT1.000000000S",                         true },
    { L_,                           93600,         0,
                              "P1DT2H0.000000000S",                     true },
    { L_,                           93600,         0,
                              "PT26H",                                 false },
    { L_,                    172800 + 300,         0,
                              "P2DT5M0.000000000S",                     true },
    { L_,                    172800 + 300,         0,
                              "P2DT5M",                                false },
    { L_,                      259200 + 7,         0,
                              "P3DT7.000000000S",                       true },
    { L_, 604800 + 86400 + 7200 + 240 + 5,      5497,
                              "P1W1DT2H4M5.000005497S",                 true },
    { L_,                               0,   3000000,
                              "PT0.003000000S",                         true },
    { L_,                               0,   3000000,
                              "PT0.0030S",                             false },
    { L_,                               0,   3000000,
                              "P0W0DT0H0M0.003S",                      false },
    { L_,                               0,   3000000,
                              "P0W0DT0H0M0.0030S",                     false },
    { L_,                               0,         5,
                              "PT0.0000000045S",                       false },
    { L_,                               0,         4,
                              "PT0.00000000449S",                      false },
    { L_,           9223372036854775807LL, 999999999,
                              "PT9223372036854775807.999999999S",      false },
    { L_,           9223372036854775807LL, 999999999,
                              "P15250284452471W3DT15H30M7.999999999S",  true },
};
const int NUM_DEFAULT_INTERVAL_DATA =
static_cast<int>(sizeof DEFAULT_INTERVAL_DATA / sizeof *DEFAULT_INTERVAL_DATA);


// *** 'Date' Data ***

struct DefaultDateDataRow {
    int         d_line;     // source line number
    int         d_year;     // year (of calendar date)
    int         d_month;    // month
    int         d_day;      // day
    const char *d_iso8601;  // ISO 8601 string
};

static
const DefaultDateDataRow DEFAULT_DATE_DATA[] =
{
    //LINE   YEAR   MONTH   DAY      ISO8601
    //----   ----   -----   ---    ------------
    { L_,       1,      1,    1,   "0001-01-01" },
    { L_,       9,      9,    9,   "0009-09-09" },
    { L_,      30,     10,   20,   "0030-10-20" },
    { L_,     842,     12,   19,   "0842-12-19" },
    { L_,    1847,      5,   19,   "1847-05-19" },
    { L_,    2000,      2,   29,   "2000-02-29" },
    { L_,    9999,     12,   31,   "9999-12-31" },
};
const int NUM_DEFAULT_DATE_DATA =
        static_cast<int>(sizeof DEFAULT_DATE_DATA / sizeof *DEFAULT_DATE_DATA);

// *** 'Time' Data ***

struct DefaultTimeDataRow {
    int         d_line;     // source line number
    int         d_hour;     // hour (of day)
    int         d_min;      // minute
    int         d_sec;      // second
    int         d_msec;     // millisecond
    int         d_usec;     // microsecond
    const char *d_iso8601;  // ISO 8601 string
};

static
const DefaultTimeDataRow DEFAULT_TIME_DATA[] =
{
    //LINE   HOUR   MIN   SEC   MSEC   USEC         ISO8601
    //----   ----   ---   ---   ----   ----    -----------------
    { L_,       0,    0,    0,     0,     0,   "00:00:00.000000" },
    { L_,       1,    2,    3,     4,     5,   "01:02:03.004005" },
    { L_,      10,   20,   30,    40,    50,   "10:20:30.040050" },
    { L_,      19,   43,   27,   805,   107,   "19:43:27.805107" },
    { L_,      23,   59,   59,   999,   999,   "23:59:59.999999" },
    { L_,      24,    0,    0,     0,     0,   "24:00:00.000000" },
};
const int NUM_DEFAULT_TIME_DATA =
        static_cast<int>(sizeof DEFAULT_TIME_DATA / sizeof *DEFAULT_TIME_DATA);

// *** Zone Data ***

struct DefaultZoneDataRow {
    int         d_line;     // source line number
    int         d_offset;   // offset (in minutes) from UTC
    const char *d_iso8601;  // ISO 8601 string
};

static
const DefaultZoneDataRow DEFAULT_ZONE_DATA[] =
{
    //LINE   OFFSET   ISO8601
    //----   ------   --------
    { L_,     -1439,  "-23:59" },
    { L_,      -120,  "-02:00" },
    { L_,       -30,  "-00:30" },
    { L_,         0,  "+00:00" },
    { L_,        90,  "+01:30" },
    { L_,       240,  "+04:00" },
    { L_,      1439,  "+23:59" },
};
const int NUM_DEFAULT_ZONE_DATA =
        static_cast<int>(sizeof DEFAULT_ZONE_DATA / sizeof *DEFAULT_ZONE_DATA);

// *** Configuration Data ***

struct DefaultCnfgDataRow {
    int  d_line;       // source line number
    bool d_omitColon;  // 'omitColonInZoneDesignator' attribute
    int  d_precision;  // 'precision'                     "
    bool d_useComma;   // 'useCommaForDecimalSign'        "
    bool d_useZ;       // 'useZAbbreviationForUtc'        "
};

static
const DefaultCnfgDataRow DEFAULT_CNFG_DATA[] =
{
    //LINE   omit ':'   precision   use ','   use 'Z'
    //----   --------   ---------   -------   -------
    { L_,      false,          3,   false,    false  },
    { L_,      false,          3,   false,     true  },
    { L_,      false,          3,    true,    false  },
    { L_,      false,          3,    true,     true  },
    { L_,      false,          6,   false,    false  },
    { L_,      false,          6,   false,     true  },
    { L_,      false,          6,    true,    false  },
    { L_,      false,          6,    true,     true  },
    { L_,       true,          3,   false,    false  },
    { L_,       true,          3,   false,     true  },
    { L_,       true,          3,    true,    false  },
    { L_,       true,          3,    true,     true  },
    { L_,       true,          6,   false,    false  },
    { L_,       true,          6,   false,     true  },
    { L_,       true,          6,    true,    false  },
    { L_,       true,          6,    true,     true  },

    // additional configurations

    { L_,      false,          0,   false,    false  },
    { L_,      false,          1,   false,    false  },
    { L_,      false,          2,   false,    false  },
    { L_,      false,          4,   false,    false  },
    { L_,      false,          5,   false,    false  }
};
const int NUM_DEFAULT_CNFG_DATA =
        static_cast<int>(sizeof DEFAULT_CNFG_DATA / sizeof *DEFAULT_CNFG_DATA);

// Define BAD (invalid) DATA generally usable across 'parse' test cases.

// *** Bad 'Date' Data ***

struct BadDateDataRow {
    int         d_line;     // source line number
    const char *d_invalid;  // test string
};

static
const BadDateDataRow BAD_DATE_DATA[] =
{
    //LINE  INPUT STRING
    //----  -------------------------
    { L_,   ""                      },  // length = 0

    { L_,   "0"                     },  // length = 1
    { L_,   "-"                     },
    { L_,   "+"                     },
    { L_,   "T"                     },
    { L_,   "Z"                     },
    { L_,   ":"                     },
    { L_,   " "                     },

    { L_,   "12"                    },  // length = 2
    { L_,   "3T"                    },
    { L_,   "4-"                    },
    { L_,   "x1"                    },
    { L_,   "T:"                    },
    { L_,   "+:"                    },

    { L_,   "999"                   },  // length = 3

    { L_,   "9999"                  },  // length = 4
    { L_,   "1-9-"                  },

    { L_,   "4-5-6"                 },  // length = 5
    { L_,   "+0130"                 },

    { L_,   "1-01-1"                },  // length = 6
    { L_,   "01-1-1"                },
    { L_,   "1-1-01"                },

    { L_,   "02-02-2"               },  // length = 7
    { L_,   "03-3-03"               },
    { L_,   "4-04-04"               },

    { L_,   "05-05-05"              },  // length = 8
    { L_,   "005-05-5"              },
    { L_,   "006-6-06"              },
    { L_,   "0006-6-6"              },

    { L_,   "0007-07-7"             },  // length = 9
    { L_,   "0008-8-08"             },
    { L_,   "009-09-09"             },

    { L_,   "0001 01-01"            },  // length = 10
    { L_,   "0001-01:01"            },
    { L_,   "0000-01-01"            },
    { L_,   "0001-00-01"            },
    { L_,   "0001-13-01"            },
    { L_,   "0001-01-00"            },
    { L_,   "0001-01-32"            },
    { L_,   "0001-04-31"            },
    { L_,   "1900-02-29"            },
    { L_,   "2000-02-30"            },

    { L_,   "0001-01-010"           },  // length = 11
    { L_,   "1970-12-310"           },
};
const int NUM_BAD_DATE_DATA =
                static_cast<int>(sizeof BAD_DATE_DATA / sizeof *BAD_DATE_DATA);


static
const BadDateDataRow BASIC_BAD_DATE_DATA[] =
{
    //LINE  INPUT STRING
    //----  -------------------------
    { L_,   ""                      },  // length = 0

    { L_,   "0"                     },  // length = 1
    { L_,   "-"                     },
    { L_,   "+"                     },
    { L_,   "T"                     },
    { L_,   "Z"                     },
    { L_,   ":"                     },
    { L_,   " "                     },

    { L_,   "12"                    },  // length = 2
    { L_,   "3T"                    },
    { L_,   "4-"                    },
    { L_,   "x1"                    },
    { L_,   "T:"                    },
    { L_,   "+:"                    },

    { L_,   "456"                   },  // length = 5
    { L_,   "999"                   },  // length = 3

    { L_,   "9999"                  },  // length = 4
    { L_,   "1011"                  },
    { L_,   "0111"                  },
    { L_,   "1101"                  },

    { L_,   "02022"                 },  // length = 5
    { L_,   "03303"                 },
    { L_,   "40404"                 },
    { L_,   "+0130"                 },

    { L_,   "050505"                },  // length = 6
    { L_,   "005055"                },
    { L_,   "006606"                },
    { L_,   "000666"                },

    { L_,   "0007077"               },  // length = 7
    { L_,   "0008808"               },
    { L_,   "0090909"               },

    { L_,   "000101010"             },  // length = 9
    { L_,   "197012310"             },
};
const int NUM_BASIC_BAD_DATE_DATA =
    static_cast<int>(sizeof BASIC_BAD_DATE_DATA / sizeof *BASIC_BAD_DATE_DATA);

// *** Bad 'Time' Data ***

struct BadTimeDataRow {
    int         d_line;     // source line number
    const char *d_invalid;  // test string
};

static
const BadTimeDataRow BAD_TIME_DATA[] =
{
    //LINE  INPUT STRING
    //----  -------------------------
    { L_,   ""                       },  // length = 0

    { L_,   "0"                      },  // length = 1
    { L_,   "-"                      },
    { L_,   "+"                      },
    { L_,   "T"                      },
    { L_,   "Z"                      },
    { L_,   ":"                      },
    { L_,   "."                      },
    { L_,   ","                      },
    { L_,   " "                      },

    { L_,   "12"                     },  // length = 2
    { L_,   "3T"                     },
    { L_,   "4-"                     },
    { L_,   "x1"                     },
    { L_,   "T:"                     },
    { L_,   "+:"                     },

    { L_,   "222"                    },  // length = 3
    { L_,   "000"                    },
    { L_,   "1:2"                    },

    { L_,   "1234"                   },  // length = 4
    { L_,   "1:19"                   },
    { L_,   "11:9"                   },

    { L_,   "12:60"                  },  // length = 5
    { L_,   "2:001"                  },
    { L_,   "24:01"                  },
    { L_,   "25:00"                  },
    { L_,   "99:00"                  },

    { L_,   "1:2:30"                 },  // length = 6
    { L_,   "1:20:3"                 },
    { L_,   "10:2:3"                 },
    { L_,   "1:2:3."                 },
    { L_,   "12:100"                 },
    { L_,   ":12:12"                 },

    { L_,   "12:00:1"                },  // length = 7
    { L_,   "12:0:01"                },
    { L_,   "2:10:01"                },
    { L_,   "24:00.1"                },

    { L_,   "12:2:001"               },  // length = 8
    { L_,   "3:02:001"               },
    { L_,   "3:2:0001"               },
    { L_,   "20:20:61"               },
    { L_,   "24:00:01"               },

    { L_,   "04:05:06."              },  // length = 9
    { L_,   "04:05:006"              },
    { L_,   "12:59:100"              },

    { L_,   "03:02:001."             },  // length = 10
    { L_,   "03:02:001,"             },
    { L_,   "03:2:001.1"             },
    { L_,   "24:00:00.1"             },

    { L_,   "24:00:00.01"            },  // length = 11

    { L_,   "24:00:00.001"           },  // length = 12
    { L_,   "24:00:00.999"           },
    { L_,   "25:00:00.000"           },

    { L_,   "24:00:00.000001"        },  // length = 15
};
const int NUM_BAD_TIME_DATA =
                static_cast<int>(sizeof BAD_TIME_DATA / sizeof *BAD_TIME_DATA);


static
const BadTimeDataRow BASIC_BAD_TIME_DATA[] =
{
    //LINE  INPUT STRING
    //----  -------------------------
    { L_,   ""                       },  // length = 0

    { L_,   "0"                      },  // length = 1
    { L_,   "-"                      },
    { L_,   "+"                      },
    { L_,   "T"                      },
    { L_,   "Z"                      },
    { L_,   ":"                      },
    { L_,   "."                      },
    { L_,   ","                      },
    { L_,   " "                      },

    { L_,   "12"                     },  // length = 2
    { L_,   "3T"                     },
    { L_,   "4-"                     },
    { L_,   "x1"                     },
    { L_,   "T:"                     },
    { L_,   "+:"                     },

    { L_,   "222"                    },  // length = 3
    { L_,   "000"                    },
    { L_,   "119"                    },

    { L_,   "1234"                   },  // length = 4
    { L_,   "123."                   },
    { L_,   "1260"                   },
    { L_,   "2001"                   },
    { L_,   "2401"                   },
    { L_,   "2500"                   },
    { L_,   "9900"                   },
    { L_,   "1230"                   },
    { L_,   "1203"                   },
    { L_,   "1023"                   },
    { L_,   "1212"                   },

    { L_,   "12100"                  },  // length = 5
    { L_,   "12001"                  },
    { L_,   "12001"                  },
    { L_,   "21001"                  },

    { L_,   "2400.1"                 },  // length = 6

    { L_,   "040506."                },  // length = 7
    { L_,   "0405006"                },
    { L_,   "1259100"                },

    { L_,   "0302001."               },  // length = 8
    { L_,   "0302001,"               },
    { L_,   "240000.1"               },

    { L_,   "240000.01"              },  // length = 9

    { L_,   "240000.001"             },  // length = 10
    { L_,   "240000.999"             },
    { L_,   "250000.000"             },

    { L_,   "240000.0001"            },  // length = 11

    { L_,   "240000.00001"           },  // length = 12

    { L_,   "240000.000001"          },  // length = 13
};
const int NUM_BASIC_BAD_TIME_DATA =
    static_cast<int>(sizeof BASIC_BAD_TIME_DATA / sizeof *BASIC_BAD_TIME_DATA);

// *** Bad Zone Data ***

struct BadZoneDataRow {
    int         d_line;     // source line number
    const char *d_invalid;  // test string
};

static
const BadZoneDataRow BAD_ZONE_DATA[] =
{
    //LINE  INPUT STRING
    //----  -------------------------
    { L_,   "0"                      },  // length = 1
    { L_,   "+"                      },
    { L_,   "-"                      },
    { L_,   "T"                      },

    { L_,   "+0"                     },  // length = 2
    { L_,   "-0"                     },
    { L_,   "Z0"                     },

    { L_,   "+01"                    },  // length = 3
    { L_,   "-01"                    },

    { L_,   "+10:"                   },  // length = 4
    { L_,   "-10:"                   },
    { L_,   "+120"                   },
    { L_,   "-030"                   },

    { L_,   "+01:1"                  },  // length = 5
    { L_,   "-01:1"                  },
    { L_,   "+1:12"                  },
    { L_,   "+12:1"                  },
    { L_,   "+2360"                  },
    { L_,   "-2360"                  },
    { L_,   "+2400"                  },
    { L_,   "-2400"                  },

    { L_,   "+12:1x"                 },  // length = 6
    { L_,   "+12:1 "                 },
    { L_,   "+1200x"                 },
    { L_,   "+23:60"                 },
    { L_,   "-23:60"                 },
    { L_,   "+24:00"                 },
    { L_,   "-24:00"                 },

    { L_,   "+123:23"                },  // length = 7
    { L_,   "+12:123"                },
    { L_,   "+011:23"                },
    { L_,   "+12:011"                },

    { L_,   "+123:123"               },  // length = 8
};
const int NUM_BAD_ZONE_DATA =
                static_cast<int>(sizeof BAD_ZONE_DATA / sizeof *BAD_ZONE_DATA);

//=============================================================================
//                  GLOBAL HELPER FUNCTIONS FOR TESTING
//-----------------------------------------------------------------------------

static
Config& gg(Config *object,
           int     fractionalSecondPrecision,
           bool    omitColonInZoneDesignatorFlag,
           bool    useCommaForDecimalSignFlag,
           bool    useZAbbreviationForUtcFlag)
    // Return, by reference, the specified '*object' with its value adjusted
    // according to the specified 'omitColonInZoneDesignatorFlag',
    // 'useCommaForDecimalSignFlag', and 'useZAbbreviationForUtcFlag'.
{
    if (fractionalSecondPrecision > 6) {
        fractionalSecondPrecision = 6;
    }

    object->setFractionalSecondPrecision(fractionalSecondPrecision);
    object->setOmitColonInZoneDesignator(omitColonInZoneDesignatorFlag);
    object->setUseCommaForDecimalSign(useCommaForDecimalSignFlag);
    object->setUseZAbbreviationForUtc(useZAbbreviationForUtcFlag);

    return *object;
}

static
void updateExpectedPerConfig(bsl::string   *expected,
                             const Config&  configuration,
                             int            maxPrecision)
    // Update the specified 'expected' ISO 8601 string as if it were generated
    // using the specified 'configuration'.  The behavior is undefined unless
    // the zone designator within 'expected' (if any) is of the form
    // "(+|-)dd:dd".
{
    ASSERT(expected);

    const bsl::string::size_type index = expected->find('.');

    if (configuration.useCommaForDecimalSign()) {
        if (index != bsl::string::npos) {
            (*expected)[index] = ',';
        }
    }

    if (index != bsl::string::npos) {
        ptrdiff_t length = 0;
        while (bdlb::CharType::isDigit((*expected)[index + length + 1])) {
            ++length;
        }

        int precision = configuration.fractionalSecondPrecision();

        if (precision > maxPrecision) {
            precision = maxPrecision;
        }

        if (0 == precision) {
            expected->erase(index, length + 1);
        }
        else if (precision < length) {
            expected->erase(index + precision + 1,
                            length - precision);
        }
    }

    // If there aren't enough characters in 'expected', don't bother with the
    // other configuration options.

    const ptrdiff_t ZONELEN = sizeof "+dd:dd" - 1;

    if (expected->length() < ZONELEN
     || (!configuration.useZAbbreviationForUtc()
      && !configuration.omitColonInZoneDesignator())) {
        return;                                                       // RETURN
    }

    // See if the tail of 'expected' has the pattern of a zone designator.

    const bsl::string::size_type zdx = expected->length() - ZONELEN;

    if (('+' != (*expected)[zdx] && '-' != (*expected)[zdx])
      || !bdlb::CharType::isDigit((*expected)[zdx + 1])
      || !bdlb::CharType::isDigit((*expected)[zdx + 2])
      || ':' !=   (*expected)[zdx + 3]
      || !bdlb::CharType::isDigit((*expected)[zdx + 4])
      || !bdlb::CharType::isDigit((*expected)[zdx + 5])) {
        return;                                                       // RETURN
    }

    if (configuration.useZAbbreviationForUtc()) {
        const bsl::string zone = expected->substr(
                                                 expected->length() - ZONELEN);

        if (0 == zone.compare("+00:00")) {
            expected->erase(expected->length() - ZONELEN);
            expected->push_back('Z');

            return;                                                   // RETURN
        }
    }

    if (configuration.omitColonInZoneDesignator()) {
        const bsl::string::size_type index = expected->find_last_of(':');

        if (index != bsl::string::npos) {
            expected->erase(index, 1);
        }
    }
}

static
bool containsOnlyDigits(const char *string)
    // Return 'true' if the specified 'string' contains nothing but digits, and
    // 'false' otherwise.
{
    while (*string) {
        if (!bdlb::CharType::isDigit(*string)) {
            return false;                                             // RETURN
        }

        ++string;
    }

    return true;
}

namespace {
namespace u {

const Util::ParseConfiguration& BASIC = Util::ParseConfiguration().setBasic();
const Util::ParseConfiguration& RELAXED_BASIC =
                            Util::ParseConfiguration().setBasic().setRelaxed();

void removeCharFromString(bsl::string *str, char chr, int limit = INT_MAX)
{
    for (int ii = 0; ii < limit; ++ii) {
        bsl::size_t offset = str->find(chr);
        if (bsl::string::npos == offset) {
            break;
        }
        str->erase(offset, 1);
    }
}

}  // close namespace u
}  // close unnamed namespace

//=============================================================================
//                           OUT-OF-LINE TEST CASES
//-----------------------------------------------------------------------------
// This section includes test cases that have been moved out of the 'switch'
// block in 'main' in order to alleviate compiler crash and/or performance
// issues.


//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const int                 test = argc > 1 ? atoi(argv[1]) : 0;
    const bool             verbose = argc > 2;
    const bool         veryVerbose = argc > 3;
    const bool     veryVeryVerbose = argc > 4;
    const bool veryVeryVeryVerbose = argc > 5;

    (void)veryVeryVerbose;  // eliminate unused variable warning
    (void)veryVeryVeryVerbose;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    // CONCERN: 'BSLS_REVIEW' failures should lead to test failures.
    bsls::ReviewFailureHandlerGuard reviewGuard(&bsls::Review::failByAbort);

    switch (test) { case 0:  // Zero is always the leading case.
      case 10: {
        // --------------------------------------------------------------------
        // PARSE: TIME & TIMETZ
        //
        // Concerns:
        //: 1 All ISO 8601 string representations supported by this component
        //:   (as documented in the header file) for 'Time' and 'TimeTz' values
        //:   are parsed successfully.
        //:
        //: 2 If parsing succeeds, the result 'Time' or 'TimeTz' object has the
        //:   expected value.
        //:
        //: 3 If the optional zone designator is present in the input string
        //:   when parsing into a 'Time' object, the resulting value is
        //:   converted to the equivalent UTC time.
        //:
        //: 4 If the optional zone designator is *not* present in the input
        //:   string when parsing into a 'TimeTz' object, it is assumed to be
        //:   UTC.
        //:
        //: 5 If parsing succeeds, 0 is returned.
        //:
        //: 6 All strings that are not ISO 8601 representations supported by
        //:   this component for 'Time' and 'TimeTz' values are rejected (i.e.,
        //:   parsing fails).
        //:
        //: 7 If parsing fails, the result object is unaffected and a non-zero
        //:   value is returned.
        //:
        //: 8 The entire extent of the input string is parsed.
        //:
        //: 9 Leap seconds and fractional seconds containing more than three
        //:   digits are handled correctly.
        //:
        //:10 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Time' values ('T'), zone designators ('Z'), and configurations
        //:   ('C').
        //:
        //: 2 Apply the (fully-tested) 'generateRaw' functions to each element
        //:   in the cross product, 'T x Z x C', of the test data from P-1.
        //:
        //: 3 Invoke the 'parse' functions on the strings generated in P-2 and
        //:   verify that parsing succeeds, i.e., that 0 is returned and the
        //:   result objects have the expected values.  (C-1..5)
        //:
        //: 4 Using the table-driven technique, specify a set of distinct
        //:   strings that are not ISO 8601 representations supported by this
        //:   component for 'Time' and 'TimeTz' values.
        //:
        //: 5 Invoke the 'parse' functions on the strings from P-4 and verify
        //:   that parsing fails, i.e., that a non-zero value is returned and
        //:   the result objects are unchanged.  (C-6..8)
        //:
        //: 6 Using the table-driven technique, specify a set of distinct
        //:   ISO 8601 strings that specifically cover cases involving leap
        //:   seconds and fractional seconds containing more than three digits.
        //:
        //: 7 Invoke the 'parse' functions on the strings from P-6 and verify
        //:   the results are as expected.  (C-9)
        //:
        //: 8 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-10)
        //
        // Testing:
        //   int parse(Time *, const char *, int);
        //   int parse(TimeTz *, const char *, int);
        //   int parse(Time *result, const StringRef& string);
        //   int parse(TimeTz *result, const StringRef& string);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "PARSE: TIME & TIMETZ" << endl
                          << "====================" << endl;

        char buffer[Util::k_MAX_STRLEN];

        const bdlt::Time   XX(2, 4, 6, 8);  // 'XX' and 'ZZ' are controls,
        const bdlt::TimeTz ZZ(XX, -7);      // distinct from any test data

        const int                  NUM_TIME_DATA =       NUM_DEFAULT_TIME_DATA;
        const DefaultTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                             DEFAULT_TIME_DATA;

        const int                  NUM_ZONE_DATA =       NUM_DEFAULT_ZONE_DATA;
        const DefaultZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] =
                                                             DEFAULT_ZONE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        if (verbose) cout << "\nValid ISO 8601 strings." << endl;

        for (int ti = 0; ti < NUM_TIME_DATA; ++ti) {
            const int   ILINE     = TIME_DATA[ti].d_line;
            const int   HOUR      = TIME_DATA[ti].d_hour;
            const int   MIN       = TIME_DATA[ti].d_min;
            const int   SEC       = TIME_DATA[ti].d_sec;
            const int   MSEC      = TIME_DATA[ti].d_msec;
            const int   USEC      = TIME_DATA[ti].d_usec;

            for (int tj = 0; tj < NUM_ZONE_DATA; ++tj) {
                const int   JLINE     = ZONE_DATA[tj].d_line;
                const int   OFFSET    = ZONE_DATA[tj].d_offset;

                if (   bdlt::Time(HOUR, MIN, SEC, MSEC, USEC) == bdlt::Time()
                    && OFFSET != 0) {
                    continue;  // skip invalid compositions
                }

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    int expMsec = MSEC;
                    int expUsec = USEC;
                    {
                        // adjust the expected milliseconds to account for
                        // PRECISION truncating the value generated

                        int precision = (PRECISION < 3 ? PRECISION : 3);

                        for (int i = 3; i > precision; --i) {
                            expMsec /= 10;
                        }

                        for (int i = 3; i > precision; --i) {
                            expMsec *= 10;
                        }

                        // adjust the expected microseconds to account for
                        // PRECISION truncating the value generated

                        precision = (PRECISION > 3 ? PRECISION - 3: 0);

                        for (int i = 3; i > precision; --i) {
                            expUsec /= 10;
                        }

                        for (int i = 3; i > precision; --i) {
                            expUsec *= 10;
                        }
                    }

                    const bdlt::Time TIME(HOUR, MIN, SEC, expMsec, expUsec);

                    const bdlt::TimeTz TIMETZ(TIME, OFFSET);

                    if (veryVerbose) {
                        if (0 == tc) {
                            T_ P_(ILINE) P_(JLINE) P_(TIME) P(TIMETZ);
                        }
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                          P_(USECOMMA) P(USEZ);
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // without zone designator in parsed string

                    if (0 == tj) {
                        const int LENGTH = Util::generateRaw(buffer, TIME, C);
                        const bsl::string str(buffer, LENGTH);

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, TIME, X, TIME == X);

                        bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());
                    }

                    // without zone designator in parsed string - basic

                    if (0 == tj) {
                        const int LEN = Util::generateRaw(buffer, TIME, C);
                        buffer[LEN] = 0;
                        bsl::string str(buffer, LEN);
                        u::removeCharFromString(&str, ':');
                        const ptrdiff_t LENGTH = str.length();

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
                        bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mX,
                                                 str.c_str(),
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, str, TIME, X, TIME == X);

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mZ,
                                                 str.c_str(),
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, str,
                                                        TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE, str,
                                                              0 == Z.offset());

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mX, StrView(str), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mX, str.c_str(), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIME == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mZ, StrView(str), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, str,
                                                        TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE, str, 0 == Z.offset());

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE, str,
                                0 == Util::parse(&mZ, str.c_str(), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, str,
                                                        TIME == Z.localTime());
                        ASSERTV(ILINE, JLINE, CLINE, str, 0 == Z.offset());
                    }

                    // with zone designator in parsed string

                    {
                        const int LENGTH = Util::generateRaw(buffer,
                                                             TIMETZ,
                                                             C);
                        const bsl::string str(buffer, LENGTH);

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
                        bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);
                    }

                    // with zone designator in parsed string - basic

                    {
                        int LENGTH = Util::generateRaw(buffer,
                                                       TIMETZ,
                                                       C);
                        bsl::string str(buffer, LENGTH);
                        u::removeCharFromString(&str, ':');
                        LENGTH = static_cast<int>(str.length());

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
                        bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 str.c_str(),
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 str.c_str(),
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 StrView(str),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 str.c_str(),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ.utcTime() == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 StrView(str),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 str.c_str(),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, TIMETZ           == Z);
                    }
                }  // loop over 'CNFG_DATA'
            }  // loop over 'ZONE_DATA'
        }  // loop over 'TIME_DATA'

        {
            // verify 'z' is accepted

            bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
            bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

            ASSERT(0 == Util::parse(&mX, "01:02:03z", 9));
            ASSERT(X == bdlt::Time(1, 2, 3));

            ASSERT(0 == Util::parse(&mZ, "01:02:03z", 9));
            ASSERT(Z == bdlt::TimeTz(bdlt::Time(1, 2, 3), 0));
        }

        {
            // verify 'z' is accepted - basic

            bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
            bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

            ASSERT(0 == Util::parse(&mX, "010203z", 7, u::BASIC));
            ASSERT(X == bdlt::Time(1, 2, 3));

            ASSERT(0 == Util::parse(&mZ, "010203z", 7, u::BASIC));
            ASSERT(Z == bdlt::TimeTz(bdlt::Time(1, 2, 3), 0));
        }

        if (verbose) cout << "\nInvalid strings." << endl;
        {
            bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
            bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

            const int              NUM_TIME_DATA =         NUM_BAD_TIME_DATA;
            const BadTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] = BAD_TIME_DATA;

            for (int ti = 0; ti < NUM_TIME_DATA; ++ti) {
                const int   LINE   = TIME_DATA[ti].d_line;
                const char *STRING = TIME_DATA[ti].d_invalid;
                const int   LENGTH = static_cast<int>(bsl::strlen(STRING));

                const bsl::string str(STRING, LENGTH);

                if (veryVerbose) { T_ P_(LINE) P(STRING) }


                ASSERTV(LINE, STRING,  0 != Util::parse(&mX, STRING, LENGTH));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ, STRING, LENGTH));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, str.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, str.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }

            const int              NUM_ZONE_DATA =         NUM_BAD_ZONE_DATA;
            const BadZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] = BAD_ZONE_DATA;

            for (int ti = 0; ti < NUM_ZONE_DATA; ++ti) {
                const int LINE = ZONE_DATA[ti].d_line;

                // Initialize with a *valid* time string, then append an
                // invalid zone designator.

                bsl::string bad("12:26:52.726");

                // Ensure that 'bad' is initially valid.

                static bool firstFlag = true;
                if (firstFlag) {
                    firstFlag = false;

                    const char      *STRING = bad.data();
                    const ptrdiff_t  LENGTH = bad.length();

                    bdlt::Time mT(XX);  const bdlt::Time& T = mT;

                    ASSERT( 0 == Util::parse(&mT, STRING, LENGTH));
                    ASSERT(XX != T);

                    mT = XX;

                    ASSERT( 0 == Util::parse(&mT, StrView(STRING, LENGTH)));
                    ASSERT(XX != T);
                }

                // If 'ZONE_DATA[ti].d_invalid' contains nothing but digits,
                // appending it to 'bad' simply extends the fractional second
                // (so 'bad' remains valid).

                if (containsOnlyDigits(ZONE_DATA[ti].d_invalid)) {
                    continue;
                }

                bad.append(ZONE_DATA[ti].d_invalid);

                const char      *STRING = bad.c_str();
                const ptrdiff_t  LENGTH = bad.length();

                if (veryVerbose) { T_ P_(LINE) P(STRING) }

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX, STRING, LENGTH));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ, STRING, LENGTH));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, bad.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, bad.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }
        }

        if (verbose) cout << "\nInvalid strings -- basic." << endl;
        {
            bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
            bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

            const int              NUM_TIME_DATA =    NUM_BASIC_BAD_TIME_DATA;
            const BadTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                           BASIC_BAD_TIME_DATA;

            for (int ti = 0; ti < NUM_TIME_DATA; ++ti) {
                const int   LINE   = TIME_DATA[ti].d_line;
                const char *STRING = TIME_DATA[ti].d_invalid;

                bsl::string str(STRING);
                u::removeCharFromString(&str, ':');
                STRING = str.c_str();
                const ptrdiff_t LENGTH = str.length();

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(str)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, str.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(str)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, str.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }

            const int              NUM_ZONE_DATA =         NUM_BAD_ZONE_DATA;
            const BadZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] = BAD_ZONE_DATA;

            for (int ti = 0; ti < NUM_ZONE_DATA; ++ti) {
                const int LINE = ZONE_DATA[ti].d_line;

                // Initialize with a *valid* time string, then append an
                // invalid zone designator.

                bsl::string bad("12:26:52.726");

                // Ensure that 'bad' is initially valid.

                static bool firstFlag = true;
                if (firstFlag) {
                    firstFlag = false;

                    const char      *STRING = bad.data();
                    const ptrdiff_t  LENGTH = bad.length();

                    bdlt::Time mT(XX);  const bdlt::Time& T = mT;

                    ASSERT( 0 == Util::parse(&mT, STRING, LENGTH));
                    ASSERT(XX != T);

                    mT = XX;

                    ASSERT( 0 == Util::parse(&mT, StrView(bad)));
                    ASSERT(XX != T);
                }

                bsl::string invalid(ZONE_DATA[ti].d_invalid);
                u::removeCharFromString(&invalid, ':');

                // If 'ZONE_DATA[ti].d_invalid' contains nothing but digits,
                // appending it to 'bad' simply extends the fractional second
                // (so 'bad' remains valid).

                if (containsOnlyDigits(invalid.c_str())) {
                    continue;
                }

                bad += invalid;

                const char      *STRING = bad.c_str();
                const ptrdiff_t  LENGTH = bad.length();

                if (veryVerbose) { T_ P_(LINE) P(STRING) }

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(bad), u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, bad.c_str(), u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(bad), u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, bad.c_str(), u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);
            }
        }

        if (verbose) cout << "\nTesting leap seconds and fractional seconds."
                          << endl;
        {
            const struct {
                int         d_line;
                const char *d_input;
                int         d_hour;
                int         d_min;
                int         d_sec;
                int         d_msec;
                int         d_usec;
                int         d_offset;
            } DATA[] = {
                // leap seconds
                { L_, "00:00:60.000",    00, 01, 00, 000, 000,   0 },
                { L_, "22:59:60.999",    23, 00, 00, 999, 000,   0 },
                { L_, "23:59:60.999",    00, 00, 00, 999, 000,   0 },

                // fractional seconds
                { L_, "00:00:00.0001",      00, 00, 00, 000, 100,   0 },
                { L_, "00:00:00.0009",      00, 00, 00, 000, 900,   0 },
                { L_, "00:00:00.00001",     00, 00, 00, 000,  10,   0 },
                { L_, "00:00:00.00049",     00, 00, 00, 000, 490,   0 },
                { L_, "00:00:00.00050",     00, 00, 00, 000, 500,   0 },
                { L_, "00:00:00.00099",     00, 00, 00, 000, 990,   0 },
                { L_, "00:00:00.0000001",   00, 00, 00, 000, 000,   0 },
                { L_, "00:00:00.0000009",   00, 00, 00, 000, 001,   0 },
                { L_, "00:00:00.00000001",  00, 00, 00, 000, 000,   0 },
                { L_, "00:00:00.00000049",  00, 00, 00, 000, 000,   0 },
                { L_, "00:00:00.00000050",  00, 00, 00, 000, 001,   0 },
                { L_, "00:00:00.00000099",  00, 00, 00, 000, 001,   0 },
                { L_, "00:00:00.9994",      00, 00, 00, 999, 400,   0 },
                { L_, "00:00:00.9995",      00, 00, 00, 999, 500,   0 },
                { L_, "00:00:00.9999",      00, 00, 00, 999, 900,   0 },
                { L_, "00:00:59.9999",      00, 00, 59, 999, 900,   0 },
                { L_, "23:59:59.9999",      23, 59, 59, 999, 900,   0 },
                { L_, "00:00:00.9999994",   00, 00, 00, 999, 999,   0 },
                { L_, "00:00:00.9999995",   00, 00,  1, 000, 000,   0 },
                { L_, "00:00:00.9999999",   00, 00,  1, 000, 000,   0 },
                { L_, "00:00:59.9999999",   00,  1, 00, 000, 000,   0 },
                { L_, "23:59:59.9999999",   00, 00, 00, 000, 000,   0 },

                // omit fractional seconds
                { L_, "12:34:45",        12, 34, 45, 000, 000,   0 },
                { L_, "12:34:45Z",       12, 34, 45, 000, 000,   0 },
                { L_, "12:34:45+00:30",  12, 34, 45, 000, 000,  30 },
                { L_, "00:00:00+00:30",  00, 00, 00, 000, 000,  30 },
                { L_, "12:34:45-01:30",  12, 34, 45, 000, 000, -90 },
                { L_, "23:59:59-01:30",  23, 59, 59, 000, 000, -90 },
            };
            const int NUM_DATA = static_cast<int>(sizeof DATA / sizeof *DATA);

            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int   LINE   = DATA[ti].d_line;
                const char *INPUT  = DATA[ti].d_input;
                const int   LENGTH = static_cast<int>(bsl::strlen(INPUT));
                const int   HOUR   = DATA[ti].d_hour;
                const int   MIN    = DATA[ti].d_min;
                const int   SEC    = DATA[ti].d_sec;
                const int   MSEC   = DATA[ti].d_msec;
                const int   USEC   = DATA[ti].d_usec;
                const int   OFFSET = DATA[ti].d_offset;

                if (veryVerbose) { T_ P_(LINE) P(INPUT) }

                // Test 'parse'

                {
                    bsl::string str(INPUT, LENGTH);

                    bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;

                    bdlt::TimeTz EXPECTED(bdlt::Time(HOUR, MIN, SEC, MSEC,
                                                                USEC), OFFSET);

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mX, INPUT, LENGTH));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mZ, INPUT, LENGTH));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);

                    mX = XX;

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mX, StrView(INPUT, LENGTH)));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    mX = XX;

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mX, str.c_str()));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    mZ = ZZ;

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mZ, StrView(INPUT, LENGTH)));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);

                    mZ = ZZ;

                    ASSERTV(LINE, INPUT, LENGTH,
                            0 == Util::parse(&mZ, str.c_str()));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);
                }

                // Test 'parseBasic'

                {
                    bdlt::Time   mX(XX);  const bdlt::Time&   X = mX;
                    bdlt::TimeTz mZ(ZZ);  const bdlt::TimeTz& Z = mZ;

                    bdlt::TimeTz EXPECTED(bdlt::Time(HOUR, MIN, SEC, MSEC,
                                                                USEC), OFFSET);

                    bsl::string str(INPUT, LENGTH);
                    u::removeCharFromString(&str, ':');
                    INPUT = str.c_str();
                    const ptrdiff_t LEN = str.length();

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mX, INPUT, LEN, u::BASIC));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mZ, INPUT, LEN, u::BASIC));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);

                    mX = XX;

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mX, StrView(str), u::BASIC));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    mX = XX;

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mX, str.c_str(), u::BASIC));
                    ASSERTV(LINE, EXPECTED, X, EXPECTED.utcTime() == X);

                    mZ = ZZ;

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mZ, StrView(str), u::BASIC));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);

                    mZ = ZZ;

                    ASSERTV(LINE, INPUT, LEN,
                            0 == Util::parse(&mZ, str.c_str(), u::BASIC));
                    ASSERTV(LINE, EXPECTED, Z, EXPECTED == Z);
                }
            }
        }

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const char      *INPUT  = "01:23:45";
            const ptrdiff_t  LENGTH = bsl::strlen(INPUT);

            const StrView stringRef(INPUT, LENGTH);
            const StrView nullRef;

            bdlt::Time   result;
            bdlt::TimeTz resultTz;

            if (veryVerbose) cout << "\t'Invalid result'" << endl;
            {
                bdlt::Time   *bad   = 0;
                bdlt::TimeTz *badTz = 0;

                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(      bad, INPUT, LENGTH));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(    badTz, INPUT, LENGTH));

                ASSERT_PASS(Util::parse(  &result, stringRef));
                ASSERT_FAIL(Util::parse(      bad, stringRef));

                ASSERT_PASS(Util::parse(&resultTz, stringRef));
                ASSERT_FAIL(Util::parse(    badTz, stringRef));
            }

            if (veryVerbose) cout << "\t'Invalid input'" << endl;
            {
                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(  &result,     0, LENGTH));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(&resultTz,     0, LENGTH));

                ASSERT_PASS(Util::parse(  &result, stringRef));
                ASSERT_FAIL(Util::parse(  &result, nullRef));

                ASSERT_PASS(Util::parse(&resultTz, stringRef));
                ASSERT_FAIL(Util::parse(&resultTz, nullRef));
            }

            if (veryVerbose) cout << "\t'Invalid length'" << endl;
            {
                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_PASS(Util::parse(  &result, INPUT,      0));
                ASSERT_FAIL(Util::parse(  &result, INPUT,     -1));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_PASS(Util::parse(&resultTz, INPUT,      0));
                ASSERT_FAIL(Util::parse(&resultTz, INPUT,     -1));
            }
        }
      } break;
      case 9: {
        // --------------------------------------------------------------------
        // PARSE: bsls::TimeInterval
        //
        // Concerns:
        //: 1 All ISO 8601 string representations supported by this component
        //:   (as documented in the header file) for 'TimeInterval' values are
        //:   parsed successfully.
        //:
        //: 2 If parsing succeeds, the result 'TimeInterval' object has the
        //:   expected value.
        //:
        //: 3 If parsing succeeds, 0 is returned.
        //:
        //: 4 All strings that are not ISO 8601 representations supported by
        //:   this component for 'TimeInterval' values are rejected (i.e.,
        //:   parsing fails).
        //:
        //: 5 If parsing fails, the result object is uneffected and a non-zero
        //:   value is returned.
        //:
        //: 6 The entire extent of the input string is parsed.
        //:
        //: 7 Fractional seconds containing more than nine digits are handled
        //:   correctly.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'TimeInterval' values.
        //:
        //: 2 Apply the (fully-tested) 'generateRaw' functions to each element
        //:   in the cross product, 'T x Z x C', of the test data from P-1.
        //:
        //: 3 Invoke the 'parse' functions on the strings generated in P-2 and
        //:   verify that parsing succeeds, i.e., that 0 is returned and the
        //:   result objects have the expected values.  (C-1..5)
        //:
        //: 4 Using the table-driven technique, specify a set of distinct
        //:   strings that are not ISO 8601 representations supported by this
        //:   component for 'Time' and 'TimeTz' values.
        //:
        //: 5 Invoke the 'parse' functions on the strings from P-4 and verify
        //:   that parsing fails, i.e., that a non-zero value is returned and
        //:   the result objects are unchanged.  (C-6..8)
        //:
        //: 6 Using the table-driven technique, specify a set of distinct
        //:   ISO 8601 strings that specifically cover cases involving leap
        //:   seconds and fractional seconds containing more than three digits.
        //:
        //: 7 Invoke the 'parse' functions on the strings from P-6 and verify
        //:   the results are as expected.  (C-9)
        //
        // Testing:
        //   int parse(Time *, const char *, int);
        //   int parse(TimeTz *, const char *, int);
        //   int parse(Time *result, const StringRef& string);
        //   int parse(TimeTz *result, const StringRef& string);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "PARSE: bsls::TimeInterval" << endl
                          << "=========================" << endl;

        const bsls::TimeInterval XX(2, 4);  // A control, distinct from any
                                            // test data.

        if (verbose) cout << "\nTesting bsls::TimeInterval." << endl;
        {
            const int NUM_INTERVAL_DATA = NUM_DEFAULT_INTERVAL_DATA;
            const DefaultIntervalDataRow (&INTERVAL_DATA)[NUM_INTERVAL_DATA] =
                                                         DEFAULT_INTERVAL_DATA;

            for (int ti = 0; ti < NUM_INTERVAL_DATA; ++ti) {
                const int                 LINE   = INTERVAL_DATA[ti].d_line;
                const char               *INPUT  = INTERVAL_DATA[ti].d_iso8601;
                const ptrdiff_t           LENGTH = bsl::strlen(INPUT);
                const bsls::Types::Int64  SEC    = INTERVAL_DATA[ti].d_sec;
                const int                 USEC   = INTERVAL_DATA[ti].d_usec;

                if (veryVerbose) { T_ P_(LINE) P(INPUT) }

                bsls::TimeInterval mX(XX);  const bsls::TimeInterval&   X = mX;
                bsls::TimeInterval EXPECTED(SEC, USEC);

                ASSERTV(LINE, INPUT, LENGTH,
                        0 == Util::parse(&mX, INPUT, LENGTH));
                ASSERTV(LINE, EXPECTED, X, EXPECTED == X);

                mX = XX;

                ASSERTV(LINE, INPUT, LENGTH,
                        0 == Util::parse(&mX, StrView(INPUT, LENGTH)));
                ASSERTV(LINE, EXPECTED, X, EXPECTED == X);
            }
        }

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const struct {
                int         d_line;
                const char *d_input;
            } DATA[] = {
                { L_, "1D"         },
                { L_, "P"          },
                { L_, "P0"         },
                { L_, "P1"         },
                { L_, "PW"         },
                { L_, "PW1D"       },
                { L_, "P1D1W"      },
                { L_, "P1D1D"      },
                { L_, "P1H"        },
                { L_, "PT"         },
                { L_, "PT1"        },
                { L_, "P1.0W1D"    },
                { L_, "PT1.1H1M"   },
                { L_, "PT1.0H1.0S" },
            };

            const int NUM_DATA = static_cast<int>(sizeof DATA / sizeof *DATA);

            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int   LINE = DATA[ti].d_line;
                const char *INPUT = DATA[ti].d_input;
                const int   LENGTH = static_cast<int>(bsl::strlen(INPUT));

                if (veryVerbose) { T_ P_(LINE) P(INPUT) }

                bsls::TimeInterval mX(XX);  const bsls::TimeInterval&   X = mX;

                ASSERTV(LINE, INPUT, LENGTH,
                    0 != Util::parse(&mX, INPUT, LENGTH));
                ASSERTV(LINE, XX, X, XX == X);

                ASSERTV(LINE, INPUT, LENGTH,
                    0 != Util::parse(&mX, StrView(INPUT, LENGTH)));
                ASSERTV(LINE, XX, X, XX == X);
            }
        }

      } break;
      case 8: {
        // --------------------------------------------------------------------
        // PARSE: DATE & DATETZ
        //
        // Concerns:
        //: 1 All ISO 8601 string representations supported by this component
        //:   (as documented in the header file) for 'Date' and 'DateTz' values
        //:   are parsed successfully.
        //:
        //: 2 If parsing succeeds, the result 'Date' or 'DateTz' object has the
        //:   expected value.
        //:
        //: 3 If the optional zone designator is present in the input string
        //:   when parsing into a 'Date' object, it is parsed for validity but
        //:   is otherwise ignored.
        //:
        //: 4 If the optional zone designator is *not* present in the input
        //:   string when parsing into a 'DateTz' object, it is assumed to be
        //:   UTC.
        //:
        //: 5 If parsing succeeds, 0 is returned.
        //:
        //: 6 All strings that are not ISO 8601 representations supported by
        //:   this component for 'Date' and 'DateTz' values are rejected (i.e.,
        //:   parsing fails).
        //:
        //: 7 If parsing fails, the result object is unaffected and a non-zero
        //:   value is returned.
        //:
        //: 8 The entire extent of the input string is parsed.
        //:
        //: 9 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Date' values ('D'), zone designators ('Z'), and configurations
        //:   ('C').
        //:
        //: 2 Apply the (fully-tested) 'generateRaw' functions to each element
        //:   in the cross product, 'D x Z x C', of the test data from P-1.
        //:
        //: 3 Invoke the 'parse' functions on the strings generated in P-2 and
        //:   verify that parsing succeeds, i.e., that 0 is returned and the
        //:   result objects have the expected values.  (C-1..5)
        //:
        //: 4 Using the table-driven technique, specify a set of distinct
        //:   strings that are not ISO 8601 representations supported by this
        //:   component for 'Date' and 'DateTz' values.
        //:
        //: 5 Invoke the 'parse' functions on the strings from P-4 and verify
        //:   that parsing fails, i.e., that a non-zero value is returned and
        //:   the result objects are unchanged.  (C-6..8)
        //:
        //: 6 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-9)
        //
        // Testing:
        //   int parse(Date *, const char *, int);
        //   int parse(DateTz *, const char *, int);
        //   int parse(Date *result, const StringRef& string);
        //   int parse(DateTz *result, const StringRef& string);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "PARSE: DATE & DATETZ" << endl
                          << "====================" << endl;

        char buffer[Util::k_MAX_STRLEN];

        const bdlt::Date   XX(246, 8, 10);  // 'XX' and 'ZZ' are controls,
        const bdlt::DateTz ZZ(XX, -7);      // distinct from any test data

        const int                  NUM_DATE_DATA =       NUM_DEFAULT_DATE_DATA;
        const DefaultDateDataRow (&DATE_DATA)[NUM_DATE_DATA] =
                                                             DEFAULT_DATE_DATA;

        const int                  NUM_ZONE_DATA =       NUM_DEFAULT_ZONE_DATA;
        const DefaultZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] =
                                                             DEFAULT_ZONE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        if (verbose) cout << "\nValid ISO 8601 strings." << endl;

        for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
            const int   ILINE    = DATE_DATA[ti].d_line;
            const int   YEAR     = DATE_DATA[ti].d_year;
            const int   MONTH    = DATE_DATA[ti].d_month;
            const int   DAY      = DATE_DATA[ti].d_day;
            const char *DATE_ISO = DATE_DATA[ti].d_iso8601;

            const bdlt::Date DATE(YEAR, MONTH, DAY);

            for (int tj = 0; tj < NUM_ZONE_DATA; ++tj) {
                const int   JLINE    = ZONE_DATA[tj].d_line;
                const int   OFFSET   = ZONE_DATA[tj].d_offset;
                const char *ZONE_ISO = ZONE_DATA[tj].d_iso8601;

                const bdlt::DateTz DATETZ(DATE, OFFSET);

                if (veryVerbose) { T_ P_(ILINE) P_(JLINE) P_(DATE) P(DATETZ) }

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // without zone designator in parsed string
                    {
                        const int LENGTH = Util::generateRaw(buffer, DATE, C);
                        const bsl::string str(buffer, LENGTH);

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
                        bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());
                    }

                    // without zone designator in parsed string -- basic
                    {
                        bsl::string str(DATE_ISO);
                        u::removeCharFromString(&str, '-');
                        char *buffer = str.data();
                        const ptrdiff_t LENGTH = str.length();

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
                        bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 buffer,
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 buffer,
                                                 LENGTH,
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 StrView(str),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX,
                                                 str.c_str(),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 StrView(str),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ,
                                                 str.c_str(),
                                                 u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE == Z.localDate());
                        ASSERTV(ILINE, JLINE, CLINE,    0 == Z.offset());
                    }

                    // with zone designator in parsed string
                    {
                        const int LENGTH = Util::generateRaw(buffer,
                                                             DATETZ,
                                                             C);
                        const bsl::string str(buffer, LENGTH);

                        if (veryVerbose) {
                            T_ T_ P(str)
                        }

                        bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
                        bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mX, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, buffer, LENGTH));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mX, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, StrView(buffer, LENGTH)));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                               0 == Util::parse(&mZ, str.c_str()));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);
                    }

                    // with zone designator in parsed string -- basic
                    {
                        if (tc != 0) {
                            continue;
                        }

                        bsl::string str(DATE_ISO);
                        bsl::string tzStr(ZONE_ISO);

                        u::removeCharFromString(&str, '-');
                        u::removeCharFromString(&tzStr, ':');

                        str += tzStr;
                        char            *buffer = str.data();
                        const ptrdiff_t  LENGTH = str.length();

                        if (veryVerbose) {
                            const bsl::string STRING(buffer, LENGTH);
                            T_ T_ P(STRING)
                        }

                        bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
                        bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

                        ASSERTV(ILINE, JLINE, CLINE, 0 == Util::parse(
                                               &mX, buffer, LENGTH, u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        ASSERTV(ILINE, JLINE, CLINE, 0 == Util::parse(
                                               &mZ, buffer, LENGTH, u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE, 0 == Util::parse(
                                                 &mX, StrView(str), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        mX = XX;

                        ASSERTV(ILINE, JLINE, CLINE, 0 == Util::parse(
                                                 &mX, str.c_str(), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATE   == X);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, StrView(str), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);

                        mZ = ZZ;

                        ASSERTV(ILINE, JLINE, CLINE,
                                0 == Util::parse(&mZ, str.c_str(), u::BASIC));
                        ASSERTV(ILINE, JLINE, CLINE, DATETZ == Z);
                    }
                }  // loop over 'CNFG_DATA'
            }  // loop over 'ZONE_DATA'
        }  // loop over 'DATE_DATA'

        {
            // verify 'z' is accepted

            bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
            bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

            ASSERT(0 == Util::parse(&mX, "0001-02-03z", 11));
            ASSERT(X == bdlt::Date(1, 2, 3));

            ASSERT(0 == Util::parse(&mZ, "0001-02-03z", 11));
            ASSERT(Z == bdlt::DateTz(bdlt::Date(1, 2, 3), 0));
        }

        if (verbose) cout << "\nInvalid strings." << endl;
        {
            bdlt::Date   mX(XX);  const bdlt::Date&   X = mX;
            bdlt::DateTz mZ(ZZ);  const bdlt::DateTz& Z = mZ;

            const int              NUM_DATE_DATA =         NUM_BAD_DATE_DATA;
            const BadDateDataRow (&DATE_DATA)[NUM_DATE_DATA] = BAD_DATE_DATA;

            for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
                const int   LINE   = DATE_DATA[ti].d_line;
                const char *STRING = DATE_DATA[ti].d_invalid;
                const int   LENGTH = static_cast<int>(bsl::strlen(STRING));

                const bsl::string str(STRING, LENGTH);

                if (veryVerbose) { T_ P_(LINE) P(STRING) }


                ASSERTV(LINE, STRING,  0 != Util::parse(&mX, STRING, LENGTH));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ, STRING, LENGTH));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, str.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, str.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }

            for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
                const int   LINE   = DATE_DATA[ti].d_line;

                bsl::string str(DATE_DATA[ti].d_invalid);
                u::removeCharFromString(&str, '-');

                const char      *STRING = str.c_str();
                const ptrdiff_t  LENGTH = bsl::strlen(STRING);


                if (veryVerbose) { T_ P_(LINE) P(str) }

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(str)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, str.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(str)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, str.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }

            const int              NUM_ZONE_DATA =         NUM_BAD_ZONE_DATA;
            const BadZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] = BAD_ZONE_DATA;

            for (int ti = 0; ti < NUM_ZONE_DATA; ++ti) {
                const int LINE = ZONE_DATA[ti].d_line;

                // Initialize with a *valid* date string, then append an
                // invalid zone designator.

                bsl::string bad("2010-08-17");

                // Ensure that 'bad' is initially valid.

                static bool firstFlag = true;
                if (firstFlag) {
                    firstFlag = false;

                    const char      *STRING = bad.data();
                    const ptrdiff_t  LENGTH = bad.length();

                    bdlt::Date mD(XX);  const bdlt::Date& D = mD;

                    ASSERT( 0 == Util::parse(&mD, STRING, LENGTH));
                    ASSERT(XX != D);

                    mD = XX;

                    ASSERT( 0 == Util::parse(&mD, StrView(STRING, LENGTH)));
                    ASSERT(XX != D);
                }

                bad.append(ZONE_DATA[ti].d_invalid);

                const char      *STRING = bad.c_str();
                const ptrdiff_t  LENGTH = bad.length();

                if (veryVerbose) { T_ P_(LINE) P(STRING) }

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX, STRING, LENGTH));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ, STRING, LENGTH));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mX, bad.c_str()));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, StrView(STRING, LENGTH)));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,
                        0 != Util::parse(&mZ, bad.c_str()));
                ASSERTV(LINE, STRING, ZZ == Z);
            }

            // repeat above loop, only 'basic'

            for (int ti = 0; ti < NUM_ZONE_DATA; ++ti) {
                const int LINE = ZONE_DATA[ti].d_line;

                // Initialize with a *valid* date string, then append an
                // invalid zone designator.

                bsl::string bad("20100817");

                // Ensure that 'bad' is initially valid.

                static bool firstFlag = true;
                if (firstFlag) {
                    firstFlag = false;

                    const char      *STRING = bad.data();
                    const ptrdiff_t  LENGTH = bad.length();

                    bdlt::Date mD(XX);  const bdlt::Date& D = mD;

                    ASSERT( 0 == Util::parse(&mD, STRING, LENGTH, u::BASIC));
                    ASSERT(XX != D);

                    mD = XX;

                    ASSERT( 0 == Util::parse(&mD,
                                             StrView(STRING, LENGTH),
                                             u::BASIC));
                    ASSERT(XX != D);

                    mD = XX;

                    ASSERT( 0 == Util::parse(&mD,
                                             bad.c_str(),
                                             u::BASIC));
                    ASSERT(XX != D);
                }

                bsl::string tz(ZONE_DATA[ti].d_invalid);
                u::removeCharFromString(&tz, ':');

                bad += tz;

                const char      *STRING = bad.c_str();
                const ptrdiff_t  LENGTH = bad.length();

                if (veryVerbose) { T_ P_(LINE) P(STRING) }

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        STRING,
                                                        LENGTH,
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        StrView(bad),
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mX,
                                                        bad.c_str(),
                                                        u::BASIC));
                ASSERTV(LINE, STRING, XX == X);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        StrView(bad),
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);

                ASSERTV(LINE, STRING,  0 != Util::parse(&mZ,
                                                        bad.c_str(),
                                                        u::BASIC));
                ASSERTV(LINE, STRING, ZZ == Z);
            }
        }

        const char      *INPUT   = "2013-10-23";
        const ptrdiff_t  LENGTH  = bsl::strlen(INPUT);

        const StrView stringRef(INPUT, LENGTH);
        const StrView nullRef;

        bdlt::Date   result;
        bdlt::DateTz resultTz;

        if (verbose) cout << "Length too short." << endl;
        {
            ASSERT(0 != Util::parse(&result, INPUT, LENGTH-1));
            ASSERT(0 != Util::parse(&result, StrView(INPUT, LENGTH-1)));
        }

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            if (veryVerbose) cout << "\t'Invalid result'" << endl;
            {
                bdlt::Date   *bad   = 0;
                bdlt::DateTz *badTz = 0;

                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(      bad, INPUT, LENGTH));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(    badTz, INPUT, LENGTH));

                ASSERT_PASS(Util::parse(  &result, stringRef));
                ASSERT_FAIL(Util::parse(      bad, stringRef));

                ASSERT_PASS(Util::parse(&resultTz, stringRef));
                ASSERT_FAIL(Util::parse(    badTz, stringRef));
            }

            if (veryVerbose) cout << "\t'Invalid input'" << endl;
            {
                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(  &result,     0, LENGTH));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_FAIL(Util::parse(&resultTz,     0, LENGTH));

                ASSERT_PASS(Util::parse(  &result, stringRef));
                ASSERT_FAIL(Util::parse(  &result, nullRef));

                ASSERT_PASS(Util::parse(&resultTz, stringRef));
                ASSERT_FAIL(Util::parse(&resultTz, nullRef));
            }

            if (veryVerbose) cout << "\t'Invalid length'" << endl;
            {
                ASSERT_PASS(Util::parse(  &result, INPUT, LENGTH));
                ASSERT_PASS(Util::parse(  &result, INPUT,      0));
                ASSERT_FAIL(Util::parse(  &result, INPUT,     -1));

                ASSERT_PASS(Util::parse(&resultTz, INPUT, LENGTH));
                ASSERT_PASS(Util::parse(&resultTz, INPUT,      0));
                ASSERT_FAIL(Util::parse(&resultTz, INPUT,     -1));
            }
        }
      } break;
      case 7: {
        // --------------------------------------------------------------------
        // GENERATE 'DatetimeTz'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Date' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 In a second table, specify a set of distinct 'Time' values (one
        //:   per row) and their corresponding ISO 8601 string representations.
        //:
        //: 3 In a third table, specify a set of distinct timezone values (one
        //:   per row) and their corresponding ISO 8601 string representations.
        //:
        //: 4 For each element 'R' in the cross product of the tables from P-1,
        //:   P-2, and P-3:  (C-1..5)
        //:
        //:   1 Create a 'const' 'DatetimeTz' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const DatetimeTz&);
        //   int generate(char *, int, const DatetimeTz&, const Config&);
        //   int generate(string *, const DatetimeTz&);
        //   int generate(string *, const DatetimeTz&, const Config&);
        //   ostream generate(ostream&, const DatetimeTz&);
        //   ostream generate(ostream&, const DatetimeTz&, const Config&);
        //   int generateRaw(char *, const DatetimeTz&);
        //   int generateRaw(char *, const DatetimeTz&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const DatetimeTz&, int);
        //   int generate(char *, const DatetimeTz&, int, bool useZ);
        //   ostream generate(ostream&, const DatetimeTz&, bool useZ);
        //   int generateRaw(char *, const DatetimeTz&, bool useZ);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'DatetimeTz'" << endl
                          << "=====================" << endl;

        typedef bdlt::DatetimeTz TYPE;

        const int OBJLEN = Util::k_DATETIMETZ_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_DATE_DATA =       NUM_DEFAULT_DATE_DATA;
        const DefaultDateDataRow (&DATE_DATA)[NUM_DATE_DATA] =
                                                             DEFAULT_DATE_DATA;

        const int                  NUM_TIME_DATA =       NUM_DEFAULT_TIME_DATA;
        const DefaultTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                             DEFAULT_TIME_DATA;

        const int                  NUM_ZONE_DATA =       NUM_DEFAULT_ZONE_DATA;
        const DefaultZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] =
                                                             DEFAULT_ZONE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
            const int   ILINE   = DATE_DATA[ti].d_line;
            const int   YEAR    = DATE_DATA[ti].d_year;
            const int   MONTH   = DATE_DATA[ti].d_month;
            const int   DAY     = DATE_DATA[ti].d_day;
            const char *ISO8601 = DATE_DATA[ti].d_iso8601;

            const bdlt::Date  DATE(YEAR, MONTH, DAY);
            const bsl::string EXPECTED_DATE(ISO8601);

            for (int tj = 0; tj < NUM_TIME_DATA; ++tj) {
                const int   JLINE   = TIME_DATA[tj].d_line;
                const int   HOUR    = TIME_DATA[tj].d_hour;
                const int   MIN     = TIME_DATA[tj].d_min;
                const int   SEC     = TIME_DATA[tj].d_sec;
                const int   MSEC    = TIME_DATA[tj].d_msec;
                const int   USEC    = TIME_DATA[tj].d_usec;
                const char *ISO8601 = TIME_DATA[tj].d_iso8601;

                const bdlt::Time  TIME(HOUR, MIN, SEC, MSEC);
                const bsl::string EXPECTED_TIME(ISO8601);

                for (int tk = 0; tk < NUM_ZONE_DATA; ++tk) {
                    const int   KLINE   = ZONE_DATA[tk].d_line;
                    const int   OFFSET  = ZONE_DATA[tk].d_offset;
                    const char *ISO8601 = ZONE_DATA[tk].d_iso8601;

                    const bsl::string EXPECTED_ZONE(ISO8601);

                    if (TIME == bdlt::Time() && OFFSET != 0) {
                        continue;  // skip invalid compositions
                    }

                    const TYPE        X(bdlt::Datetime(YEAR,
                                                       MONTH,
                                                       DAY,
                                                       HOUR,
                                                       MIN,
                                                       SEC,
                                                       MSEC,
                                                       USEC),
                                        OFFSET);
                    const bsl::string BASE_EXPECTED(
                          EXPECTED_DATE + 'T' + EXPECTED_TIME + EXPECTED_ZONE);

                    if (veryVerbose) {
                        T_ P_(ILINE) P_(JLINE) P_(KLINE) P_(X) P(BASE_EXPECTED)
                    }

                    for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                        const int  CLINE     = CNFG_DATA[tc].d_line;
                        const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                        const int  PRECISION = CNFG_DATA[tc].d_precision;
                        const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                        const bool USEZ      = CNFG_DATA[tc].d_useZ;

                        if (veryVerbose) {
                            T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                        }

                        Config mC;  const Config& C = mC;
                        gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                        Config::setDefaultConfiguration(C);

                        bsl::string EXPECTED(BASE_EXPECTED);
                        updateExpectedPerConfig(&EXPECTED,
                                                C,
                                                k_DATETIMETZ_MAX_PRECISION);

                        const int OUTLEN = static_cast<int>(EXPECTED.length());

                        // 'generate' taking 'bufferLength'

                        for (int k = 0; k < BUFLEN; ++k) {
                            bsl::memset(buffer, '?', BUFLEN);

                            if (veryVeryVerbose) {
                                T_ T_ cout << "Length: "; P(k)
                            }

                            ASSERTV(ILINE, JLINE, KLINE, k, OUTLEN,
                                    OUTLEN == Util::generate(buffer, k, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, JLINE, KLINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, JLINE, KLINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, JLINE, KLINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                            // Swap order of 'k' and 'X' in call to 'generate'.
                            {
                                bsl::memset(buffer, '?', BUFLEN);

                                ASSERTV(ILINE, k, OUTLEN,
                                       OUTLEN == Util::generate(buffer, X, k));

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(
                                                     EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                                if (k <= OUTLEN) {
                                    ASSERTV(ILINE, EXPECTED, buffer,
                                            0 == bsl::memcmp(chaste,
                                                             buffer + k,
                                                             BUFLEN - k));
                                }
                                else {
                                    ASSERTV(ILINE, k, OUTLEN,
                                            '\0' == buffer[OUTLEN]);

                                    ASSERTV(ILINE, EXPECTED, buffer,
                                            0 == bsl::memcmp(chaste,
                                                             buffer + k + 1,
                                                             BUFLEN - k - 1));
                                }
                            }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                        }

                        // 'generate' to a 'bsl::string'
                        {
                            bsl::string mS("qwerty");

                            ASSERTV(ILINE, JLINE, KLINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, mS,
                                    EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }

                        // 'generate' to an 'std::string'
                        {
                            std::string mS("qwerty");

                            ASSERTV(ILINE, JLINE, KLINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, mS,
                                    EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                        // 'generate' to an 'std::pmr::string'
                        {
                            std::pmr::string mS("qwerty");

                            ASSERTV(ILINE, JLINE, KLINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, mS,
                                    EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }
#endif

                        // 'generate' to an 'ostream'
                        {
                            bsl::ostringstream os;

                            ASSERTV(ILINE, JLINE, KLINE,
                                    &os == &Util::generate(os, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, os.str(),
                                    EXPECTED == os.str());

                            if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                        }

                        // 'generateRaw'
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, JLINE, KLINE, OUTLEN,
                                    OUTLEN == Util::generateRaw(buffer, X));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     OUTLEN));

                            ASSERTV(ILINE, JLINE, KLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + OUTLEN,
                                                     BUFLEN - OUTLEN));
                        }
                    }  // loop over 'CNFG_DATA'

                    for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                        const int  CLINE     = CNFG_DATA[tc].d_line;
                        const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                        const int  PRECISION = CNFG_DATA[tc].d_precision;
                        const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                        const bool USEZ      = CNFG_DATA[tc].d_useZ;

                        if (veryVerbose) {
                            T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                        }

                        Config mC;  const Config& C = mC;
                        gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                        // Set the default configuration to the complement of
                        // 'C'.

                        Config mDFLT;  const Config& DFLT = mDFLT;
                        gg(&mDFLT,
                           9 - PRECISION,
                           !OMITCOLON,
                           !USECOMMA,
                           !USEZ);
                        Config::setDefaultConfiguration(DFLT);

                        bsl::string EXPECTED(BASE_EXPECTED);
                        updateExpectedPerConfig(&EXPECTED,
                                                C,
                                                k_DATETIMETZ_MAX_PRECISION);

                        const ptrdiff_t OUTLEN = EXPECTED.length();

                        // 'generate' taking 'bufferLength'

                        for (int k = 0; k < BUFLEN; ++k) {
                            bsl::memset(buffer, '?', BUFLEN);

                            if (veryVeryVerbose) {
                                T_ T_ cout << "Length: "; P(k)
                            }

                            ASSERTV(ILINE, k, OUTLEN,
                                    OUTLEN == Util::generate(buffer, k, X, C));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
                        }

                        // 'generate' to a 'bsl::string'
                        {
                            bsl::string mS("qwerty");

                            ASSERTV(ILINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X, C));

                            ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }

                        // 'generate' to an 'std::string'
                        {
                            std::string mS("qwerty");

                            ASSERTV(ILINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X, C));

                            ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                        // 'generate' to an 'std::pmr::string'
                        {
                            std::pmr::string mS("qwerty");

                            ASSERTV(ILINE, OUTLEN,
                                    OUTLEN == Util::generate(&mS, X, C));

                            ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                            if (veryVerbose) { P_(EXPECTED) P(mS); }
                        }
#endif

                        // 'generate' to an 'ostream'
                        {
                            bsl::ostringstream os;

                            ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                            ASSERTV(ILINE, EXPECTED, os.str(),
                                    EXPECTED == os.str());

                            if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                        }

                        // 'generateRaw'
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, OUTLEN,
                                    OUTLEN == Util::generateRaw(buffer, X, C));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     OUTLEN));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + OUTLEN,
                                                     BUFLEN - OUTLEN));
                        }
                    }  // loop over 'CNFG_DATA'

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                    // Test methods taking (legacy)
                    // 'bool useZAbbreviationForUtc'.

                    const bool USEZ_CNFG_DATA[] = { false, true };

                    for (int tc = 0; tc < 2; ++tc) {
                        const bool OMITCOLON = true;
                        const int  PRECISION = 3;
                        const bool USECOMMA  = true;
                        const bool USEZ      = USEZ_CNFG_DATA[tc];

                        if (veryVerbose) {
                            T_ P_(OMITCOLON) P_(PRECISION) P_(USECOMMA) P(USEZ)
                        }

                        Config mC;  const Config& C = mC;
                        gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                        // Set the default configuration to use the complement
                        // of 'USEZ'.

                        Config mDFLT;  const Config& DFLT = mDFLT;
                        gg(&mDFLT, PRECISION, OMITCOLON, USECOMMA, !USEZ);
                        Config::setDefaultConfiguration(DFLT);

                        bsl::string EXPECTED(BASE_EXPECTED);
                        updateExpectedPerConfig(&EXPECTED,
                                                C,
                                                k_DATETIMETZ_MAX_PRECISION);

                        const ptrdiff_t OUTLEN = EXPECTED.length();

                        // 'generate' taking 'bufferLength'

                        for (int k = 0; k < BUFLEN; ++k) {
                            bsl::memset(buffer, '?', BUFLEN);

                            if (veryVeryVerbose) {
                                T_ T_ cout << "Length: "; P(k)
                            }

                            ASSERTV(ILINE, k, OUTLEN,
                                 OUTLEN == Util::generate(buffer, X, k, USEZ));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
                        }

                        // 'generate' to an 'ostream'
                        {
                            bsl::ostringstream os;

                            ASSERTV(ILINE,
                                    &os == &Util::generate(os, X, USEZ));

                            ASSERTV(ILINE, EXPECTED, os.str(),
                                    EXPECTED == os.str());

                            if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                        }

                        // 'generateRaw'
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, OUTLEN,
                                 OUTLEN == Util::generateRaw(buffer, X, USEZ));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     OUTLEN));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + OUTLEN,
                                                     BUFLEN - OUTLEN));
                        }
                    }  // loop over 'USEZ_CNFG_DATA'
#endif  // BDE_OMIT_INTERNAL_DEPRECATED

                }  // loop over 'ZONE_DATA'
            }  // loop over 'TIME_DATA'
        }  // loop over 'DATE_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(     pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string  mSB("qwerty");
                bsl::string *pb = 0;

                ASSERT_PASS(Util::generate(&mSB, X));
                ASSERT_FAIL(Util::generate(  pb, X));

                ASSERT_PASS(Util::generate(&mSB, X, C));
                ASSERT_FAIL(Util::generate(  pb, X, C));

                std::string mSS("qwerty");
                std::string *ps = 0;

                ASSERT_PASS(Util::generate(&mSS, X));
                ASSERT_FAIL(Util::generate(  ps, X));

                ASSERT_PASS(Util::generate(&mSS, X, C));
                ASSERT_FAIL(Util::generate(  ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mSP("qwerty");
                std::pmr::string *pp = 0;

                ASSERT_PASS(Util::generate(&mSP, X));
                ASSERT_FAIL(Util::generate(  pp, X));

                ASSERT_PASS(Util::generate(&mSP, X, C));
                ASSERT_FAIL(Util::generate(  pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN];

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(     0, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(     0, X, C));
            }
        }

      } break;
      case 6: {
        // --------------------------------------------------------------------
        // GENERATE 'TimeTz'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Time' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 In a second table, specify a set of distinct timezone values (one
        //:   per row) and their corresponding ISO 8601 string representations.
        //:
        //: 3 For each element 'R' in the cross product of the tables from P-1
        //:   and P-2:  (C-1..5)
        //:
        //:   1 Create a 'const' 'TimeTz' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const TimeTz&);
        //   int generate(char *, int, const TimeTz&, const Config&);
        //   int generate(string *, const TimeTz&);
        //   int generate(string *, const TimeTz&, const Config&);
        //   ostream generate(ostream&, const TimeTz&);
        //   ostream generate(ostream&, const TimeTz&, const Config&);
        //   int generateRaw(char *, const TimeTz&);
        //   int generateRaw(char *, const TimeTz&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const TimeTz&, int);
        //   int generate(char *, const TimeTz&, int, bool useZ);
        //   ostream generate(ostream&, const TimeTz&, bool useZ);
        //   int generateRaw(char *, const TimeTz&, bool useZ);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'TimeTz'" << endl
                          << "=================" << endl;

        typedef bdlt::TimeTz TYPE;

        const int OBJLEN = Util::k_TIMETZ_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_TIME_DATA =       NUM_DEFAULT_TIME_DATA;
        const DefaultTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                             DEFAULT_TIME_DATA;

        const int                  NUM_ZONE_DATA =       NUM_DEFAULT_ZONE_DATA;
        const DefaultZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] =
                                                             DEFAULT_ZONE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_TIME_DATA; ++ti) {
            const int   ILINE   = TIME_DATA[ti].d_line;
            const int   HOUR    = TIME_DATA[ti].d_hour;
            const int   MIN     = TIME_DATA[ti].d_min;
            const int   SEC     = TIME_DATA[ti].d_sec;
            const int   MSEC    = TIME_DATA[ti].d_msec;
            const int   USEC    = TIME_DATA[ti].d_usec;
            const char *ISO8601 = TIME_DATA[ti].d_iso8601;

            const bdlt::Time  TIME(HOUR, MIN, SEC, MSEC, USEC);
            const bsl::string EXPECTED_TIME(ISO8601);

            for (int tj = 0; tj < NUM_ZONE_DATA; ++tj) {
                const int   JLINE   = ZONE_DATA[tj].d_line;
                const int   OFFSET  = ZONE_DATA[tj].d_offset;
                const char *ISO8601 = ZONE_DATA[tj].d_iso8601;

                const bsl::string EXPECTED_ZONE(ISO8601);

                if (TIME == bdlt::Time() && OFFSET != 0) {
                    continue;  // skip invalid compositions
                }

                const TYPE        X(TIME, OFFSET);
                const bsl::string BASE_EXPECTED(EXPECTED_TIME + EXPECTED_ZONE);

                if (veryVerbose) {
                    T_ P_(ILINE) P_(JLINE) P_(X) P(BASE_EXPECTED)
                }

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    Config::setDefaultConfiguration(C);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_TIMETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, JLINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, JLINE, k, OUTLEN,
                                    '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                        // Swap order of 'k' and 'X' in call to 'generate'.
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, k, OUTLEN,
                                    OUTLEN == Util::generate(buffer, X, k));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
                        }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                        std::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, JLINE, &os == &Util::generate(os, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // Set the default configuration to the complement of 'C'.

                    Config mDFLT;  const Config& DFLT = mDFLT;
                    gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                    Config::setDefaultConfiguration(DFLT);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_TIMETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                       std::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                        ASSERTV(ILINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                // Test methods taking (legacy) 'bool useZAbbreviationForUtc'.

                const bool USEZ_CNFG_DATA[] = { false, true };

                for (int tc = 0; tc < 2; ++tc) {
                    const bool OMITCOLON = true;
                    const int  PRECISION = 3;
                    const bool USECOMMA  = true;
                    const bool USEZ      = USEZ_CNFG_DATA[tc];

                    if (veryVerbose) {
                        T_ P_(OMITCOLON) P_(PRECISION) P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // Set the default configuration to use the complement of
                    // 'USEZ'.

                    Config mDFLT;  const Config& DFLT = mDFLT;
                    gg(&mDFLT, PRECISION, OMITCOLON, USECOMMA, !USEZ);
                    Config::setDefaultConfiguration(DFLT);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_TIMETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, X, k, USEZ));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, &os == &Util::generate(os, X, USEZ));

                        ASSERTV(ILINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X, USEZ));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'USEZ_CNFG_DATA'
#endif  // BDE_OMIT_INTERNAL_DEPRECATED

            }  // loop over 'ZONE_DATA'
        }  // loop over 'TIME_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(    pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                std::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN];

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(     0, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(     0, X, C));
            }
        }

      } break;
      case 5: {
        // --------------------------------------------------------------------
        // GENERATE 'DateTz'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Date' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 In a second table, specify a set of distinct timezone values (one
        //:   per row) and their corresponding ISO 8601 string representations.
        //:
        //: 3 For each element 'R' in the cross product of the tables from P-1
        //:   and P-2:  (C-1..5)
        //:
        //:   1 Create a 'const' 'DateTz' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const DateTz&);
        //   int generate(char *, int, const DateTz&, const Config&);
        //   int generate(string *, const DateTz&);
        //   int generate(string *, const DateTz&, const Config&);
        //   ostream generate(ostream&, const DateTz&);
        //   ostream generate(ostream&, const DateTz&, const Config&);
        //   int generateRaw(char *, const DateTz&);
        //   int generateRaw(char *, const DateTz&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const DateTz&, int);
        //   int generate(char *, const DateTz&, int, bool useZ);
        //   ostream generate(ostream&, const DateTz&, bool useZ);
        //   int generateRaw(char *, const DateTz&, bool useZ);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'DateTz'" << endl
                          << "=================" << endl;

        typedef bdlt::DateTz TYPE;

        const int OBJLEN = Util::k_DATETZ_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_DATE_DATA =       NUM_DEFAULT_DATE_DATA;
        const DefaultDateDataRow (&DATE_DATA)[NUM_DATE_DATA] =
                                                             DEFAULT_DATE_DATA;

        const int                  NUM_ZONE_DATA =       NUM_DEFAULT_ZONE_DATA;
        const DefaultZoneDataRow (&ZONE_DATA)[NUM_ZONE_DATA] =
                                                             DEFAULT_ZONE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
            const int   ILINE   = DATE_DATA[ti].d_line;
            const int   YEAR    = DATE_DATA[ti].d_year;
            const int   MONTH   = DATE_DATA[ti].d_month;
            const int   DAY     = DATE_DATA[ti].d_day;
            const char *ISO8601 = DATE_DATA[ti].d_iso8601;

            const bdlt::Date  DATE(YEAR, MONTH, DAY);
            const bsl::string EXPECTED_DATE(ISO8601);

            for (int tj = 0; tj < NUM_ZONE_DATA; ++tj) {
                const int   JLINE   = ZONE_DATA[tj].d_line;
                const int   OFFSET  = ZONE_DATA[tj].d_offset;
                const char *ISO8601 = ZONE_DATA[tj].d_iso8601;

                const bsl::string EXPECTED_ZONE(ISO8601);

                const TYPE        X(DATE, OFFSET);
                const bsl::string BASE_EXPECTED(EXPECTED_DATE + EXPECTED_ZONE);

                if (veryVerbose) {
                    T_ P_(ILINE) P_(JLINE) P_(X) P(BASE_EXPECTED)
                }

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    Config::setDefaultConfiguration(C);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_DATETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, JLINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, JLINE, k, OUTLEN,
                                    '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                        // Swap order of 'k' and 'X' in call to 'generate'.
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, k, OUTLEN,
                                    OUTLEN == Util::generate(buffer, X, k));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
                        }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                        std::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, JLINE, &os == &Util::generate(os, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // Set the default configuration to the complement of 'C'.

                    Config mDFLT;  const Config& DFLT = mDFLT;
                    gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                    Config::setDefaultConfiguration(DFLT);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_DATETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                        std::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                        ASSERTV(ILINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                // Test methods taking (legacy) 'bool useZAbbreviationForUtc'.

                const bool USEZ_CNFG_DATA[] = { false, true };

                for (int tc = 0; tc < 2; ++tc) {
                    const bool OMITCOLON = true;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = true;
                    const bool USEZ      = USEZ_CNFG_DATA[tc];

                    if (veryVerbose) {
                        T_ P_(OMITCOLON) P_(PRECISION) P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // Set the default configuration to use the complement of
                    // 'USEZ'.

                    Config mDFLT;  const Config& DFLT = mDFLT;
                    gg(&mDFLT, PRECISION, OMITCOLON, USECOMMA, !USEZ);
                    Config::setDefaultConfiguration(DFLT);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_DATETZ_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, X, k, USEZ));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, &os == &Util::generate(os, X, USEZ));

                        ASSERTV(ILINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X, USEZ));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'USEZ_CNFG_DATA'
#endif  // BDE_OMIT_INTERNAL_DEPRECATED

            }  // loop over 'ZONE_DATA'
        }  // loop over 'DATE_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(    pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                std::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(    pc, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(    pc, X, C));
            }
        }

      } break;
      case 4: {
        // --------------------------------------------------------------------
        // GENERATE 'Datetime'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Date' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 In a second table, specify a set of distinct 'Time' values (one
        //:   per row) and their corresponding ISO 8601 string representations.
        //:
        //: 3 For each element 'R' in the cross product of the tables from P-1
        //:   and P-2:  (C-1..5)
        //:
        //:   1 Create a 'const' 'Datetime' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const Datetime&);
        //   int generate(char *, int, const Datetime&, const Config&);
        //   int generate(string *, const Datetime&);
        //   int generate(string *, const Datetime&, const Config&);
        //   ostream generate(ostream&, const Datetime&);
        //   ostream generate(ostream&, const Datetime&, const Config&);
        //   int generateRaw(char *, const Datetime&);
        //   int generateRaw(char *, const Datetime&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const Datetime&, int);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'Datetime'" << endl
                          << "===================" << endl;

        typedef bdlt::Datetime TYPE;

        const int OBJLEN = Util::k_DATETIME_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_DATE_DATA =       NUM_DEFAULT_DATE_DATA;
        const DefaultDateDataRow (&DATE_DATA)[NUM_DATE_DATA] =
                                                             DEFAULT_DATE_DATA;

        const int                  NUM_TIME_DATA =       NUM_DEFAULT_TIME_DATA;
        const DefaultTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                             DEFAULT_TIME_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
            const int   ILINE   = DATE_DATA[ti].d_line;
            const int   YEAR    = DATE_DATA[ti].d_year;
            const int   MONTH   = DATE_DATA[ti].d_month;
            const int   DAY     = DATE_DATA[ti].d_day;
            const char *ISO8601 = DATE_DATA[ti].d_iso8601;

            const bdlt::Date  DATE(YEAR, MONTH, DAY);
            const bsl::string EXPECTED_DATE(ISO8601);

            for (int tj = 0; tj < NUM_TIME_DATA; ++tj) {
                const int   JLINE   = TIME_DATA[tj].d_line;
                const int   HOUR    = TIME_DATA[tj].d_hour;
                const int   MIN     = TIME_DATA[tj].d_min;
                const int   SEC     = TIME_DATA[tj].d_sec;
                const int   MSEC    = TIME_DATA[tj].d_msec;
                const int   USEC    = TIME_DATA[tj].d_usec;
                const char *ISO8601 = TIME_DATA[tj].d_iso8601;

                const bsl::string EXPECTED_TIME(ISO8601);

                const TYPE        X(YEAR,
                                    MONTH,
                                    DAY,
                                    HOUR,
                                    MIN,
                                    SEC,
                                    MSEC,
                                    USEC);
                const bsl::string BASE_EXPECTED(
                                          EXPECTED_DATE + 'T' + EXPECTED_TIME);

                if (veryVerbose) {
                    T_ P_(ILINE) P_(JLINE) P_(X) P(BASE_EXPECTED)
                }

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    Config::setDefaultConfiguration(C);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_DATETIME_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, JLINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, JLINE, k, OUTLEN,
                                    '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                        // Swap order of 'k' and 'X' in call to 'generate'.
                        {
                            bsl::memset(buffer, '?', BUFLEN);

                            ASSERTV(ILINE, k, OUTLEN,
                                    OUTLEN == Util::generate(buffer, X, k));

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(EXPECTED.c_str(),
                                                     buffer,
                                                     k < OUTLEN ? k : OUTLEN));

                            if (k <= OUTLEN) {
                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k,
                                                         BUFLEN - k));
                            }
                            else {
                                ASSERTV(ILINE, k, OUTLEN,
                                        '\0' == buffer[OUTLEN]);

                                ASSERTV(ILINE, EXPECTED, buffer,
                                        0 == bsl::memcmp(chaste,
                                                         buffer + k + 1,
                                                         BUFLEN - k - 1));
                            }
                        }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                        std::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, JLINE, &os == &Util::generate(os, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, JLINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, JLINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'

                for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                    const int  CLINE     = CNFG_DATA[tc].d_line;
                    const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                    const int  PRECISION = CNFG_DATA[tc].d_precision;
                    const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                    const bool USEZ      = CNFG_DATA[tc].d_useZ;

                    if (veryVerbose) {
                        T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                    }

                    Config mC;  const Config& C = mC;
                    gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                    // Set the default configuration to the complement of 'C'.

                    Config mDFLT;  const Config& DFLT = mDFLT;
                    gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                    Config::setDefaultConfiguration(DFLT);

                    bsl::string EXPECTED(BASE_EXPECTED);
                    updateExpectedPerConfig(&EXPECTED,
                                            C,
                                            k_DATETIME_MAX_PRECISION);

                    const ptrdiff_t OUTLEN = EXPECTED.length();

                    // 'generate' taking 'bufferLength'

                    for (int k = 0; k < BUFLEN; ++k) {
                        bsl::memset(buffer, '?', BUFLEN);

                        if (veryVeryVerbose) {
                            T_ T_ cout << "Length: "; P(k)
                        }

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, k, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }

                    // 'generate' to a 'bsl::string'
                    {
                        bsl::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

                    // 'generate' to an 'std::string'
                    {
                        std::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                    // 'generate' to an 'std::pmr::string'
                    {
                        std::pmr::string mS("qwerty");

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generate(&mS, X, C));

                        ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                        if (veryVerbose) { P_(EXPECTED) P(mS); }
                    }
#endif

                    // 'generate' to an 'ostream'
                    {
                        bsl::ostringstream os;

                        ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                        ASSERTV(ILINE, EXPECTED, os.str(),
                                EXPECTED == os.str());

                        if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                    }

                    // 'generateRaw'
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, OUTLEN,
                                OUTLEN == Util::generateRaw(buffer, X, C));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 OUTLEN));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + OUTLEN,
                                                 BUFLEN - OUTLEN));
                    }
                }  // loop over 'CNFG_DATA'
            }  // loop over 'TIME_DATA'
        }  // loop over 'DATE_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(    pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                std::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN];

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(     0, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(     0, X, C));
            }
        }

      } break;
      case 3: {
        // --------------------------------------------------------------------
        // GENERATE 'Time'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Time' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 For each row 'R' in the table from P-1:  (C-1..5)
        //:
        //:   1 Create a 'const' 'Time' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const Time&);
        //   int generate(char *, int, const Time&, const Config&);
        //   int generate(string *, const Time&);
        //   int generate(string *, const Time&, const Config&);
        //   ostream generate(ostream&, const Time&);
        //   ostream generate(ostream&, const Time&, const Config&);
        //   int generateRaw(char *, const Time&);
        //   int generateRaw(char *, const Time&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const Time&, int);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'Time'" << endl
                          << "===============" << endl;

        typedef bdlt::Time TYPE;

        const int OBJLEN = Util::k_TIME_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_TIME_DATA =       NUM_DEFAULT_TIME_DATA;
        const DefaultTimeDataRow (&TIME_DATA)[NUM_TIME_DATA] =
                                                             DEFAULT_TIME_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_TIME_DATA; ++ti) {
            const int   ILINE   = TIME_DATA[ti].d_line;
            const int   HOUR    = TIME_DATA[ti].d_hour;
            const int   MIN     = TIME_DATA[ti].d_min;
            const int   SEC     = TIME_DATA[ti].d_sec;
            const int   MSEC    = TIME_DATA[ti].d_msec;
            const int   USEC    = TIME_DATA[ti].d_usec;
            const char *ISO8601 = TIME_DATA[ti].d_iso8601;

            const TYPE        X(HOUR, MIN, SEC, MSEC, USEC);
            const bsl::string BASE_EXPECTED(ISO8601);

            if (veryVerbose) { T_ P_(ILINE) P_(X) P(BASE_EXPECTED) }

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                Config::setDefaultConfiguration(C);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_TIME_MAX_PRECISION);

                const ptrdiff_t OUTLEN = EXPECTED.length();

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                    // Swap order of 'k' and 'X' in call to 'generate'.
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, X, k));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                // Set the default configuration to the complement of 'C'.

                Config mDFLT;  const Config& DFLT = mDFLT;
                gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                Config::setDefaultConfiguration(DFLT);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_TIME_MAX_PRECISION);

                const ptrdiff_t OUTLEN = EXPECTED.length();

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'
        }  // loop over 'TIME_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(    pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                std::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(    pc, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(    pc, X, C));
            }
        }

      } break;
      case 2: {
        // --------------------------------------------------------------------
        // GENERATE 'Date'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'Date' values (one per row) and their corresponding ISO 8601
        //:   string representations.
        //:
        //: 2 For each row 'R' in the table from P-1:  (C-1..5)
        //:
        //:   1 Create a 'const' 'Date' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const Date&);
        //   int generate(char *, int, const Date&, const Config&);
        //   int generate(string *, const Date&);
        //   int generate(string *, const Date&, const Config&);
        //   ostream generate(ostream&, const Date&);
        //   ostream generate(ostream&, const Date&, const Config&);
        //   int generateRaw(char *, const Date&);
        //   int generateRaw(char *, const Date&, const Config&);
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
        //   int generate(char *, const Date&, int);
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'Date'" << endl
                          << "===============" << endl;

        typedef bdlt::Date TYPE;

        const int OBJLEN = Util::k_DATE_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_DATE_DATA =       NUM_DEFAULT_DATE_DATA;
        const DefaultDateDataRow (&DATE_DATA)[NUM_DATE_DATA] =
                                                             DEFAULT_DATE_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_DATE_DATA; ++ti) {
            const int   ILINE   = DATE_DATA[ti].d_line;
            const int   YEAR    = DATE_DATA[ti].d_year;
            const int   MONTH   = DATE_DATA[ti].d_month;
            const int   DAY     = DATE_DATA[ti].d_day;
            const char *ISO8601 = DATE_DATA[ti].d_iso8601;

            const TYPE        X(YEAR, MONTH, DAY);
            const bsl::string BASE_EXPECTED(ISO8601);

            if (veryVerbose) { T_ P_(ILINE) P_(X) P(BASE_EXPECTED) }

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                Config::setDefaultConfiguration(C);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_DATE_MAX_PRECISION);

                const ptrdiff_t OUTLEN = EXPECTED.length();

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
                    // Swap order of 'k' and 'X' in call to 'generate'.
                    {
                        bsl::memset(buffer, '?', BUFLEN);

                        ASSERTV(ILINE, k, OUTLEN,
                                OUTLEN == Util::generate(buffer, X, k));

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(EXPECTED.c_str(),
                                                 buffer,
                                                 k < OUTLEN ? k : OUTLEN));

                        if (k <= OUTLEN) {
                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k,
                                                     BUFLEN - k));
                        }
                        else {
                            ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                            ASSERTV(ILINE, EXPECTED, buffer,
                                    0 == bsl::memcmp(chaste,
                                                     buffer + k + 1,
                                                     BUFLEN - k - 1));
                        }
                    }
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                // Set the default configuration to the complement of 'C'.

                Config mDFLT;  const Config& DFLT = mDFLT;
                gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                Config::setDefaultConfiguration(DFLT);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_DATE_MAX_PRECISION);

                const int OUTLEN = static_cast<int>(EXPECTED.length());

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'
        }  // loop over 'DATE_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(    pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                bsl::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN];

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(     0, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(     0, X, C));
            }
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // GENERATE 'TimeInterval'
        //
        // Concerns:
        //: 1 The output generated by each method has the expected format and
        //:   contents.
        //:
        //: 2 When sufficient capacity is indicated, the method taking
        //:   'bufferLength' generates a null terminator.
        //:
        //: 3 Each method returns the expected value (the correct character
        //:   count or the supplied 'ostream', depending on the return type).
        //:
        //: 4 The value of the supplied object is unchanged.
        //:
        //: 5 The configuration that is in effect, whether user-supplied or the
        //:   process-wide default, has the desired effect on the output.
        //:
        //: 6 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of distinct
        //:   'TimeInterval' values (one per row) and their corresponding ISO
        //:   8601 string representations.
        //:
        //: 2 For each row 'R' in the table from P-1:  (C-1..5)
        //:
        //:   1 Create a 'const' 'TimeInterval' object, 'X', from 'R'.
        //:
        //:   2 Invoke the six methods under test on 'X' for all possible
        //:     configurations.  Also exercise the method taking 'bufferLength'
        //:     for all buffer lengths in the range '[0 .. L]', where 'L'
        //:     provides sufficient capacity for a null terminator and a few
        //:     extra characters.  For each call, verify that the generated
        //:     output matches the string from 'R' (taking the effect of the
        //:     configuration into account), a null terminator is appended when
        //:     expected, and the return value is correct.  (C-1..5)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid arguments, but not triggered for adjacent
        //:   valid ones (using the 'BSLS_ASSERTTEST_*' macros).  (C-6)
        //
        // Testing:
        //   int generate(char *, int, const TimeInterval&);
        //   int generate(char *, int, const TimeInterval&, const Config&);
        //   int generate(string *, const TimeInterval&);
        //   int generate(string *, const TimeInterval&, const Config&);
        //   ostream generate(ostream&, const TimeInterval&);
        //   ostream generate(ostream&, const TimeInterval&, const Config&);
        //   int generateRaw(char *, const TimeInterval&);
        //   int generateRaw(char *, const TimeInterval&, const Config&);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "GENERATE 'TimeInterval'" << endl
                          << "=======================" << endl;

        ASSERT(u::BASIC.basic());
        ASSERT(! u::BASIC.relaxed());

        ASSERT(u::RELAXED_BASIC.basic());
        ASSERT(u::RELAXED_BASIC.relaxed());

        typedef bsls::TimeInterval TYPE;

        const int OBJLEN = Util::k_TIMEINTERVAL_STRLEN;
        const int BUFLEN = OBJLEN + 4;

        char buffer[BUFLEN];
        char chaste[BUFLEN];  bsl::memset(chaste, '?', BUFLEN);

        const int                  NUM_INTERVAL_DATA =
                                                     NUM_DEFAULT_INTERVAL_DATA;
        const DefaultIntervalDataRow (&INTERVAL_DATA)[NUM_INTERVAL_DATA] =
                                                         DEFAULT_INTERVAL_DATA;

        const int                  NUM_CNFG_DATA =       NUM_DEFAULT_CNFG_DATA;
        const DefaultCnfgDataRow (&CNFG_DATA)[NUM_CNFG_DATA] =
                                                             DEFAULT_CNFG_DATA;

        for (int ti = 0; ti < NUM_INTERVAL_DATA; ++ti) {
            if (!INTERVAL_DATA[ti].d_canonical) {
                continue;
            }
            const int                 ILINE   = INTERVAL_DATA[ti].d_line;
            const bsls::Types::Int64  SEC     = INTERVAL_DATA[ti].d_sec;
            const int                 USEC    = INTERVAL_DATA[ti].d_usec;
            const char               *ISO8601 = INTERVAL_DATA[ti].d_iso8601;

            const TYPE        X(SEC, USEC);
            const bsl::string BASE_EXPECTED(ISO8601);

            if (veryVerbose) { T_ P_(ILINE) P_(X) P(BASE_EXPECTED) }

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                Config::setDefaultConfiguration(C);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_INTERVAL_MAX_PRECISION);

                const ptrdiff_t OUTLEN = EXPECTED.length();

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN, OUTLEN == Util::generate(&mS, X));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'

            for (int tc = 0; tc < NUM_CNFG_DATA; ++tc) {
                const int  CLINE     = CNFG_DATA[tc].d_line;
                const bool OMITCOLON = CNFG_DATA[tc].d_omitColon;
                const int  PRECISION = CNFG_DATA[tc].d_precision;
                const bool USECOMMA  = CNFG_DATA[tc].d_useComma;
                const bool USEZ      = CNFG_DATA[tc].d_useZ;

                if (veryVerbose) {
                    T_ P_(CLINE) P_(OMITCOLON) P_(PRECISION)
                                                           P_(USECOMMA) P(USEZ)
                }

                Config mC;  const Config& C = mC;
                gg(&mC, PRECISION, OMITCOLON, USECOMMA, USEZ);

                // Set the default configuration to the complement of 'C'.

                Config mDFLT;  const Config& DFLT = mDFLT;
                gg(&mDFLT, 9 - PRECISION, !OMITCOLON, !USECOMMA, !USEZ);
                Config::setDefaultConfiguration(DFLT);

                bsl::string EXPECTED(BASE_EXPECTED);
                updateExpectedPerConfig(&EXPECTED,
                                        C,
                                        k_TIME_MAX_PRECISION);

                const int OUTLEN = static_cast<int>(EXPECTED.length());

                // 'generate' taking 'bufferLength'

                for (int k = 0; k < BUFLEN; ++k) {
                    bsl::memset(buffer, '?', BUFLEN);

                    if (veryVeryVerbose) {
                        T_ T_ cout << "Length: "; P(k)
                    }

                    ASSERTV(ILINE, k, OUTLEN,
                            OUTLEN == Util::generate(buffer, k, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             k < OUTLEN ? k : OUTLEN));

                    if (k <= OUTLEN) {
                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k,
                                                 BUFLEN - k));
                    }
                    else {
                        ASSERTV(ILINE, k, OUTLEN, '\0' == buffer[OUTLEN]);

                        ASSERTV(ILINE, EXPECTED, buffer,
                                0 == bsl::memcmp(chaste,
                                                 buffer + k + 1,
                                                 BUFLEN - k - 1));
                    }
                }

                // 'generate' to a 'bsl::string'
                {
                    bsl::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

                // 'generate' to an 'std::string'
                {
                    std::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                // 'generate' to an 'std::pmr::string'
                {
                    std::pmr::string mS("qwerty");

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generate(&mS, X, C));

                    ASSERTV(ILINE, EXPECTED, mS, EXPECTED == mS);

                    if (veryVerbose) { P_(EXPECTED) P(mS); }
                }
#endif

                // 'generate' to an 'ostream'
                {
                    bsl::ostringstream os;

                    ASSERTV(ILINE, &os == &Util::generate(os, X, C));

                    ASSERTV(ILINE, EXPECTED, os.str(), EXPECTED == os.str());

                    if (veryVerbose) { P_(EXPECTED) P(os.str()); }
                }

                // 'generateRaw'
                {
                    bsl::memset(buffer, '?', BUFLEN);

                    ASSERTV(ILINE, OUTLEN,
                            OUTLEN == Util::generateRaw(buffer, X, C));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(EXPECTED.c_str(),
                                             buffer,
                                             OUTLEN));

                    ASSERTV(ILINE, EXPECTED, buffer,
                            0 == bsl::memcmp(chaste,
                                             buffer + OUTLEN,
                                             BUFLEN - OUTLEN));
                }
            }  // loop over 'CNFG_DATA'
        }  // loop over 'TIME_DATA'

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls::AssertTestHandlerGuard hG;

            const Config C;

            if (verbose) cout << "\t'generate'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X));
                ASSERT_SAFE_FAIL(Util::generate(    pc, OBJLEN, X));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X));
                ASSERT_SAFE_FAIL(Util::generate(buffer,     -1, X));

                ASSERT_SAFE_PASS(Util::generate(buffer, OBJLEN, X, C));
                ASSERT_FAIL(Util::generate(     pc, OBJLEN, X, C));

                ASSERT_SAFE_PASS(Util::generate(buffer,      0, X, C));
                ASSERT_FAIL(Util::generate(buffer,     -1, X, C));

                bsl::string mB("qwerty"), *pb = 0;

                ASSERT_PASS(Util::generate(&mB, X));
                ASSERT_FAIL(Util::generate( pb, X));

                ASSERT_PASS(Util::generate(&mB, X, C));
                ASSERT_FAIL(Util::generate( pb, X, C));

                std::string mS("qwerty"), *ps = 0;

                ASSERT_PASS(Util::generate(&mS, X));
                ASSERT_FAIL(Util::generate( ps, X));

                ASSERT_PASS(Util::generate(&mS, X, C));
                ASSERT_FAIL(Util::generate( ps, X, C));

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
                std::pmr::string mP("qwerty"), *pp = 0;

                ASSERT_PASS(Util::generate(&mP, X));
                ASSERT_FAIL(Util::generate( pp, X));

                ASSERT_PASS(Util::generate(&mP, X, C));
                ASSERT_FAIL(Util::generate( pp, X, C));
#endif
            }

            if (verbose) cout << "\t'generateRaw'" << endl;
            {
                const TYPE X;
                char       buffer[OBJLEN], *pc = 0;

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X));
                ASSERT_SAFE_FAIL(Util::generateRaw(    pc, X));

                ASSERT_SAFE_PASS(Util::generateRaw(buffer, X, C));
                ASSERT_FAIL(Util::generateRaw(    pc, X, C));
            }
        }

      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "." << endl;
    }
    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2016 Bloomberg Finance L.P.
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
