// bslstl_stdexceptutil.t.cpp                                         -*-C++-*-
#include <bslstl_stdexceptutil.h>

#include <bsls_bsltestutil.h>
#include <bsls_exceptionutil.h>
#include <bsls_log.h>

#include <algorithm>
#include <stdexcept>    //  yes, we want the native std here

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace BloombergLP;
using namespace std;

//=============================================================================
//                             TEST PLAN
//-----------------------------------------------------------------------------
// [ 2] void logCheapStackTrace(const char *, const char *);
// [ 2] void setRuntimeErrorHook(   PreThrowHook);
// [ 2] void setLogicErrorHook(     PreThrowHook);
// [ 2] void setDomainErrorHook(    PreThrowHook);
// [ 2] void setInvalidArgumentHook(PreThrowHook);
// [ 2] void setLengthErrorHook(    PreThrowHook);
// [ 2] void setOutOfRangeHook(     PreThrowHook);
// [ 2] void setRangeErrorHook(     PreThrowHook);
// [ 2] void setOverflowErrorHook(  PreThrowHook);
// [ 2] void setUnderflowErrorHook( PreThrowHook);
//-----------------------------------------------------------------------------
// [ 1] BREATHING TEST
// [ 3] USAGE EXAMPLE
//-----------------------------------------------------------------------------

// ============================================================================
//                     STANDARD BSL ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", line, message);

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BSL TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLS_BSLTESTUTIL_ASSERT
#define ASSERTV      BSLS_BSLTESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLS_BSLTESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLS_BSLTESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLS_BSLTESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLS_BSLTESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLS_BSLTESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLS_BSLTESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLS_BSLTESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLS_BSLTESTUTIL_LOOP6_ASSERT

#define Q            BSLS_BSLTESTUTIL_Q   // Quote identifier literally.
#define P            BSLS_BSLTESTUTIL_P   // Print identifier and value.
#define P_           BSLS_BSLTESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLS_BSLTESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLS_BSLTESTUTIL_L_  // current Line number

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

enum { VERBOSE_ARG_NUM = 2, VERY_VERBOSE_ARG_NUM, VERY_VERY_VERBOSE_ARG_NUM };

//=============================================================================
//                  GLOBAL HELPER FUNCTIONS/VARIABLES FOR TESTING
//-----------------------------------------------------------------------------

int verbose;
int veryVerbose;

//=============================================================================
//                  CLASSES FOR TESTING USAGE EXAMPLES
//-----------------------------------------------------------------------------
#if defined(BDE_BUILD_TARGET_EXC)
///Usage
///-----
// First we declare a function template that wants to throw a standard
// exception.  Note that the 'stdexcept' header is not included at this
// point.
//..
  # include <bslstl_stdexceptutil.h>

  template<class T>
  void testFunction(int selector)
      //  Throw a standard exception according to the specified 'selector'.
  {
    switch(selector) {
      case 1: bslstl::StdExceptUtil::throwRuntimeError("sample message 1");
      case 2: bslstl::StdExceptUtil::throwLogicError("sample message 2");
      default : bslstl::StdExceptUtil::throwInvalidArgument("ERROR");
    }
  }
//..
// However, if client code wishes to catch the exception, the .cpp file
// must #include the appropriate header.
//..
  #include <stdexcept>

  void callTestFunction()
  {
    try {
      testFunction<int>(1);
      ASSERT(0 == "Should throw before reaching here");
    }
    catch(const runtime_error& ex) {
      ASSERT(0 == std::strcmp(ex.what(), "sample message 1"));
    }

    try {
      testFunction<double>(2);
      ASSERT(0 == "Should throw before reaching here");
    }
    catch(const logic_error& ex) {
      ASSERT(0 == std::strcmp(ex.what(), "sample message 2"));
    }
  }
//..
#endif // defined BDE_BUILD_TARGET_EXC
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

namespace BSLSTL_STDEXCEPTUTIL_TEST_CASE_2 {

namespace TC = BSLSTL_STDEXCEPTUTIL_TEST_CASE_2;

typedef bslstl::StdExceptUtil Util;

bsls::LogSeverity::Enum  severity;
char                     sourceFileNameBuf[8192];
int                      lineNumber;
char                     outBuf[8192];

bool                     caught;

void logMessageHandler(bsls::LogSeverity::Enum  severity,
                       const char              *file,
                       int                      line,
                       const char              *message)
{
    namespace TC = BSLSTL_STDEXCEPTUTIL_TEST_CASE_2;

    TC::severity = severity;
    strcpy(TC::sourceFileNameBuf, file);
    TC::lineNumber = line;
    strcpy(TC::outBuf, message);

    if (veryVerbose) {
        P_(severity);    P_(file);    P_(line);    P(message);
    }
}

void failAssert(const char *, const char *)
{
    BSLS_ASSERT_INVOKE_NORETURN("fail");
}

void verifyLogContent(int line, const char *exceptionName, const char *message)
{
    // Verify severity.

    ASSERTV(line, bsls::LogSeverity::e_WARN == TC::severity);

    // The file name should be the imp file of this component.  Verify that by
    // substituting '.t.cpp' for '.cpp' and then comparing to '__FILE__'.

    char *pc = strstr(TC::sourceFileNameBuf, ".cpp");
    ASSERTV(line, pc);
    strcpy(pc, ".t.cpp");
    ASSERTV(line, TC::sourceFileNameBuf, __FILE__,
                                     !strcmp(TC::sourceFileNameBuf, __FILE__));

    // Verify the line number was positive.

    ASSERTV(line, 0 < TC::lineNumber);

    // Verify outBuf contains "About to throw <exceptionName>"

    char aboutBuf[256];
    strcpy(aboutBuf, "About to throw ");
    strcat(aboutBuf, exceptionName);
    ASSERTV(line, TC::outBuf, strstr(TC::outBuf, aboutBuf));

    // Verify outBuf contains 'message'

    ASSERTV(line, strstr(TC::outBuf, message));

    // Verify outBuf

    ASSERTV(line, TC::outBuf, strstr(TC::outBuf, "/bb/bin/showfunc.tsk "));

    // After the 'showfunc.tsk' should be the cheap stack trace -- a series
    // of addresses of stack frames separated by space.  Confirm there are
    // at least 6 spaces.  For some reason using 'std::count' didn't compile on
    // Solaris.

    int count = 0;
    for (pc = strstr(outBuf, ".tsk ") + 4; *pc; ++pc) {
        count += ' ' == *pc;
    }
    ASSERTV(line, 6 <= count);
}

static const struct NotA5 {
    bool operator()(char c) const
    {
        static const char unset = static_cast<char>(0xa5);
        return unset != c;
    }
} notA5;

void verifyNotLogged(int line)
    // Verify that none of the logged state in this namespace has been
    // modified since it was cleared before the throw.
{
    char *pc  = reinterpret_cast<char *>(&TC::severity);
    char *end = pc + sizeof(TC::severity);
    ASSERTV(line, std::find_if(pc, end, TC::notA5) == end);

    pc  = TC::sourceFileNameBuf;
    end = pc + sizeof(TC::sourceFileNameBuf);
    ASSERTV(line, std::find_if(pc, end, TC::notA5) == end);

    ASSERTV(line, TC::lineNumber < 0);

    pc  = TC::outBuf;
    end = pc + sizeof(TC::outBuf);
    ASSERTV(line, std::find_if(pc, end, TC::notA5) == end);
}

template <class EXCEPTION>
void verifyExceptionType(int                    line,
                         const std::exception&  exc,
                         const char            *exceptionName)
    // Verify that the specified 'exc' is actually a reference to an
    // 'EXCEPTION' object, using the specified 'line' and 'exceptionName'
    // to report errors.
{
    ASSERTV(line, exceptionName, dynamic_cast<const EXCEPTION *>(&exc));
}


int throwAndCatchExceptionsAndCheckLogging(const char *message)
{
    typedef void (*SetHookFunc)(Util::PreThrowHook);
    typedef void (*ThrowFunc)(const char *);
    typedef void (*VerifyExceptionFunc)(
                                     int, const std::exception&, const char *);

#define U_TABLE_LINE(setHookFunc, throwFunc, exceptionName)                   \
        { L_, &Util::setHookFunc, &Util::throwFunc,                           \
                              &TC::verifyExceptionType<std::exceptionName>,   \
                                                       "std::" #exceptionName }

    static const struct Data {
        int                  d_line;
        SetHookFunc          d_setHookFunc;
        ThrowFunc            d_throwFunc;
        VerifyExceptionFunc  d_verifyExceptionFunc;
        const char          *d_exceptionName;
    } DATA[] = {
        U_TABLE_LINE(setRuntimeErrorHook,  throwRuntimeError, runtime_error),
        U_TABLE_LINE(setLogicErrorHook,    throwLogicError,   logic_error),
        U_TABLE_LINE(setDomainErrorHook,   throwDomainError,  domain_error),
        U_TABLE_LINE(setInvalidArgumentHook, throwInvalidArgument,
                                                             invalid_argument),
        U_TABLE_LINE(setLengthErrorHook,   throwLengthError,  length_error),
        U_TABLE_LINE(setOutOfRangeHook,    throwOutOfRange,   out_of_range),
        U_TABLE_LINE(setRangeErrorHook,    throwRangeError,   range_error),
        U_TABLE_LINE(setOverflowErrorHook, throwOverflowError,
                                                              overflow_error),
        U_TABLE_LINE(setUnderflowErrorHook, throwUnderflowError,
                                                              underflow_error)
    };
#undef U_TABLE_LINE
    enum { k_NUM_DATA = sizeof DATA / sizeof *DATA };

    for (int ti = 0; ti < k_NUM_DATA * 2; ++ti) {
        const int                  tii            = ti % k_NUM_DATA;
        const bool                 LOG            = ti < k_NUM_DATA;
        const Data&                data           = DATA[tii];
        const int                  LINE           = data.d_line;
        const SetHookFunc          SET_HOOK_FUNC  = data.d_setHookFunc;
        const ThrowFunc            THROW_FUNC     = data.d_throwFunc;
        const VerifyExceptionFunc  VERIFY_FUNC    = data.d_verifyExceptionFunc;
        const char                *EXCEPTION_NAME = data.d_exceptionName;

        // Set all the hooks to failing.

        for (int tj = 0; tj < k_NUM_DATA; ++tj) {
            (*DATA[tj].d_setHookFunc)(&TC::failAssert);
        }

        // Set all the test state in 'TC' to failing.

        memset(&TC::severity, 0xa5, sizeof(TC::severity));
        memset(TC::sourceFileNameBuf, 0xa5, sizeof(TC::sourceFileNameBuf));
        TC::lineNumber = -1;
        memset(TC::outBuf, 0xa5, sizeof(TC::outBuf));
        TC::caught = false;

        try {
            (*SET_HOOK_FUNC)(LOG ? &Util::logCheapStackTrace : 0);
            (*THROW_FUNC)(message);
            ASSERTV(LINE, EXCEPTION_NAME, 0 && "throw failed");
        }
        catch (const std::exception& exc) {
            ASSERTV(LINE, EXCEPTION_NAME, exc.what(), message,
                                                 !strcmp(exc.what(), message));
            (*VERIFY_FUNC)(LINE, exc, EXCEPTION_NAME);
            TC::caught = true;
        }

        ASSERTV(LINE, EXCEPTION_NAME, TC::caught);
        LOG ? TC::verifyLogContent(LINE, EXCEPTION_NAME, message)
            : TC::verifyNotLogged(LINE);
    }

    return 1;
}

template <class FUNC_PTR>
int recurser(int *depth, FUNC_PTR func, const char *message)
    // Recurse to the specified 'depth' and then call
    // 'throwAndCatchExceptionsAndCheckLogging'.
    //
    // We want to recurse several times here so that the stack trace will have
    // some depth to traverse.  This is non-trivial because optimizers are very
    // motivated to inline function calls and replace calls at the end of
    // functions with simple jumps.  Most of the manipulation and checking of
    // the 'depth' variable after the function calls are intended to force the
    // optimizer to do funciton calls and return from them rather than simply
    // chaining.
{
    const int depthIn = *depth;

    int rc = --*depth <= 0
           ? (*bsls::BslTestUtil::makeFunctionCallNonInline(func))(message)
           : (*bsls::BslTestUtil::makeFunctionCallNonInline(
                                   &recurser<FUNC_PTR>))(depth, func, message);
    *depth += rc;

    ASSERT(depthIn == *depth);

    return 1;
}

}  // close namespace BSLSTL_STDEXCEPTUTIL_TEST_CASE_2

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    verbose = argc > 2;
    veryVerbose = argc > 3;
    // int veryVeryVerbose = argc > 4;

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:  // Zero is always the leading case.
      case 3: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE TEST:
        //
        // Concerns:
        //   The usage example provided in the component header file must
        //   compile, link, and run on all platforms as shown.
        //
        // Plan:
        //   Simply invoke the functions 'addSecurity' and 'removeSecurity' to
        //   ensure the code compiles.
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------
        if (verbose) printf("\nUSAGE EXAMPLE"
                            "\n=============");

#if !defined(BDE_BUILD_TARGET_EXC)
        if (verbose) printf(
                "\nThis case is not run as it relies on exception support.\n");
#else
        callTestFunction();
#endif // defined BDE_BUILD_TARGET_EXC
     } break;
     case 2: {
        // --------------------------------------------------------------------
        // HOOK / STACKTRACE TEST
        //
        // Concerns:
        //: 1 That the 'set*Hook' functions work.
        //:
        //: 2 That 'logCheapStackTrace' works.
        //
        // Plan:
        //: 1 Set the 'bsls::Log' message handler to 'TC::logMessageHandler' to
        //:   capture 'bsls::Log' output in state in the 'TC' namespace.
        //:
        //: 2 Call 'TC::recurser' which will recurse a few times and then
        //:   call 'TC::throwAndCatchExceptionsAndCheckLogging'.
        //:
        //: 3 'TC::throwAndCatchExceptionsAndCheckLogging' does a table-driven
        //:   iteration through all the supported exception types, one at a
        //:   time, setting the pre throw hook for the exception type under
        //:   test (and only that type) to 'logCheapStackTrace', and then
        //:   throwing the exception, which is then caught as a reference to
        //:   base class 'std::excption', and then verified through dynamic
        //:   cast to be of the exception type expected.  The output of
        //:   'logCheapStackTrace' is then checked and verified to be correct
        //
        // Testing:
        //   void logCheapStackTrace(const char *, const char *);
        //   void setRuntimeErrorHook(   PreThrowHook);
        //   void setLogicErrorHook(     PreThrowHook);
        //   void setDomainErrorHook(    PreThrowHook);
        //   void setInvalidArgumentHook(PreThrowHook);
        //   void setLengthErrorHook(    PreThrowHook);
        //   void setOutOfRangeHook(     PreThrowHook);
        //   void setRangeErrorHook(     PreThrowHook);
        //   void setOverflowErrorHook(  PreThrowHook);
        //   void setUnderflowErrorHook( PreThrowHook);
        // --------------------------------------------------------------------

#if !defined(BDE_BUILD_TARGET_EXC)
        if (verbose) printf(
                "Test case 2 is not run as it relies on exception support.\n");
#else
        if (verbose) printf("HOOK / STACKTRACE TEST\n"
                            "======================\n");

        namespace TC = BSLSTL_STDEXCEPTUTIL_TEST_CASE_2;

        bsls::Log::setLogMessageHandler(TC::logMessageHandler);

        int depth = 5;
        TC::recurser(&depth,
                     &TC::throwAndCatchExceptionsAndCheckLogging,
                     "test case 2");
        ASSERT(5 == depth);
#endif
     } break;
     case 1: {
        // --------------------------------------------------------------------
        // BREATHING/USAGE TEST
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) printf("\nBREATHING TEST"
                            "\n==============");

#if !defined(BDE_BUILD_TARGET_EXC)
        if (verbose) printf(
                "\nThis case is not run as it relies on exception support.\n");
#else
    try {
            if(verbose) printf("\nThrowing a runtime_error exception");
            bslstl::StdExceptUtil::throwRuntimeError("one");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const runtime_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "one"));
        }
        try {
            if(verbose) printf("\nThrowing a logic_error exception");
            bslstl::StdExceptUtil::throwLogicError("two");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const logic_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "two"));
        }
        try {
            if(verbose) printf("\nThrowing a domain_error exception");
            bslstl::StdExceptUtil::throwDomainError("three");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const domain_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "three"));
        }
        try {
            if(verbose) printf("\nThrowing an invalid_argument exception");
            bslstl::StdExceptUtil::throwInvalidArgument("four");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const invalid_argument& ex) {
            ASSERT(0 == strcmp(ex.what(), "four"));
        }
        try {
            if(verbose) printf("\nThrowing a length_error exception");
            bslstl::StdExceptUtil::throwLengthError("five");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const length_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "five"));
        }
        try {
            if(verbose) printf("\nThrowing an out_of_range exception");
            bslstl::StdExceptUtil::throwOutOfRange("six");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const out_of_range& ex) {
            ASSERT(0 == strcmp(ex.what(), "six"));
        }
        try {
            if(verbose) printf("\nThrowing a range_error exception");
            bslstl::StdExceptUtil::throwRangeError("seven");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const range_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "seven"));
        }
        try {
            if(verbose) printf("\nThrowing an overflow_error exception");
            bslstl::StdExceptUtil::throwOverflowError("eight");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const overflow_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "eight"));
        }
        try {
            if(verbose) printf("\nThrowing an underflow_error exception");
            bslstl::StdExceptUtil::throwUnderflowError("nine");
            ASSERT( false ); // execution should jump to catch block
        }
        catch(const underflow_error& ex) {
            ASSERT(0 == strcmp(ex.what(), "nine"));
        }
#endif // defined BDE_BUILD_TARGET_EXC
      } break;
      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }

    return testStatus;
}

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
