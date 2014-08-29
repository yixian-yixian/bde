// bsls_log.t.cpp                                                     -*-C++-*-
#include <bsls_log.h>

#include <bsls_assert.h>
#include <bsls_bsltestutil.h>
#include <bsls_platform.h>


#include <fcntl.h>
#include <limits.h>    // PATH_MAX on linux, INT_MAX
#include <stdint.h>    // SIZE_MAX
#include <stdio.h>     // 'snprintf', '_snprintf'
#include <stdlib.h>    // abort
#include <string.h>    // strlen
#include <sys/types.h> // struct stat: required on Sun and Windows only
#include <sys/stat.h>  // struct stat: required on Sun and Windows only

#if defined(BSLS_PLATFORM_OS_WINDOWS)
# include <windows.h>  // MAX_PATH
# include <io.h>       // _dup2, _dup, _close
#else
# include <unistd.h>
#endif

using namespace BloombergLP;

// ============================================================================
//                             TEST PLAN
// ----------------------------------------------------------------------------
//                              Overview
//                              --------
// The component under test provides a global facility for low-level code to
// write log messages.
//
// Global Concerns:
//: o The 'bsls::Log' class has no instance state.
//: o Exceptions thrown in a log message handler are successfully propagated.
//: o Setting and invoking the log message handler is thread-safe.
//: o By default, the 'platformDefaultMessageHandler' is used.
//: o TBD
//
// Global Assumptions:
//: o TBD
// ----------------------------------------------------------------------------
// MACROS
// [ 5] BSLS_LOG(format, ...)
// [  ] BSLS_LOG_SIMPLE(message)
//
// TYPES
// [ 4] typedef void (*LogMessageHandler)(file, line, message);
//
// CLASS METHODS
// [ 4] static bsls::Log::LogMessageHandler logMessageHandler();
// [ 4] static void setLogMessageHandler(bsls::Log::LogMessageHandler);
// [  ] static void logFormatted(file, line, format, ...);
// [ 5] static void logMessage(file, line, message);
// [  ] static void platformDefaultMessageHandler(file, line, message);
// [ 3] static void stdoutMessageHandler(file, line, message);
// [ 3] static void stderrMessageHandler(file, line, message);
// ----------------------------------------------------------------------------
// [  ] BREATHING TEST
//
// [ 2] TEST-DRIVER LOG MESSAGE HANDLER APPARATUS
// [ 1] STREAM REDIRECTION APPARATUS
//
// [11] COMPONENT-LEVEL DOCUMENTATION 1
// [10] COMPONENT-LEVEL DOCUMENTATION 2
// [ 9] COMPONENT-LEVEL DOCUMENTATION 3
// [ 8] USAGE EXAMPLE 1
// [ 7] USAGE EXAMPLE 2
// [ 6] USAGE EXAMPLE 3
//
// [  ] CONCERN: By default, the 'platformDefaultMessageHandler' is used


//=============================================================================
//                       STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------
static int testStatus = 0;

static void aSsErT(bool b, const char *s, int i)
{
    if (b) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", i, s);
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

// ============================================================================
//                      STANDARD BDE TEST DRIVER MACROS
// ----------------------------------------------------------------------------

#define ASSERT       BSLS_BSLTESTUTIL_ASSERT
#define LOOP_ASSERT  BSLS_BSLTESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLS_BSLTESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLS_BSLTESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLS_BSLTESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLS_BSLTESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLS_BSLTESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLS_BSLTESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLS_BSLTESTUTIL_LOOP6_ASSERT
#define ASSERTV      BSLS_BSLTESTUTIL_ASSERTV

#define Q   BSLS_BSLTESTUTIL_Q   // Quote identifier literally.
#define P   BSLS_BSLTESTUTIL_P   // Print identifier and value.
#define P_  BSLS_BSLTESTUTIL_P_  // P(X) without '\n'.
#define T_  BSLS_BSLTESTUTIL_T_  // Print a tab (w/o newline).
#define L_  BSLS_BSLTESTUTIL_L_  // current Line number


// ============================================================================
//                     GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
// ----------------------------------------------------------------------------

#ifndef SIZE_MAX
#define SIZE_MAX (static_cast<size_t>(-1))
#endif

static const size_t OUTPUT_REDIRECTOR_BUFFER_SIZE  = 4096;
static const size_t LOG_MESSAGE_SINK_BUFFER_SIZE   = 4096;

#ifdef BSLS_PLATFORM_OS_WINDOWS
static const size_t PATH_BUFFER_SIZE = MAX_PATH + 1;
#else
static const size_t PATH_BUFFER_SIZE = PATH_MAX + 1;
#endif

// Keep this in sync with 'bsls_log.cpp'
const size_t LOG_FORMATTED_STACK_BUFFER_SIZE = 1024;
const size_t WINDOWS_DEBUG_STACK_BUFFER_SIZE = 1024;

static bool             verbose;
static bool         veryVerbose;
static bool     veryVeryVerbose;
static bool veryVeryVeryVerbose;



// ============================================================================
//                             GLOBAL TEST DATA
// ----------------------------------------------------------------------------

struct DefaultDataRow {
    int         d_sourceLine;
    const char *d_file;
    int         d_line;
    const char *d_message;
    const char *d_expected;
};

static
const DefaultDataRow DEFAULT_DATA[] = {
    // This is a set of good, useful data that is the cross product of the sets
    // of situations in which the file name string is empty or non-empty, the
    // line is 0 or non-zero, and the message is empty or non-empty.  The
    // file name and message strings may also have 'printf'-style format
    // specifiers to ensure that no unwanted formatting is being done.
    //
    // All normal values:
    {L_,
     "Very VaLidF1%d%dLeN@me",
     408743,
     "Valid \n %fMessage String",
     "Very VaLidF1%d%dLeN@me:408743 Valid \n %fMessage String\n"},

    // Empty message string:
    {L_,
     "Good File Name!%s.cpp",
     2147483000,
     "",
     "Good File Name!%s.cpp:2147483000 \n"},

    // Zero line number:
    {L_,
     "%filename.cpp",
     0,
     "Message String!",
     "%filename.cpp:0 Message String!\n"},

    // Zero line number, empty message:
    {L_,
     "fi%ldename.cpp",
     0,
     "",
     "fi%ldename.cpp:0 \n"},

    // Empty file name:
    {L_,
     "",
     83274892,
     "Good MESSAGE STRING %x ~~~",
     ":83274892 Good MESSAGE STRING %x ~~~\n"},

    // Empty file name, empty message string:
    {L_,
     "",
     93874829,
     "",
     ":93874829 \n"},

    // Empty file name, zero line number:
    {L_,
     "",
     0,
     "Another Message :)",
     ":0 Another Message :)\n"},

    // Empty file name, zero line number, empty message:
    {L_,
     "",
     0,
     "",
     ":0 \n"},
};
const size_t NUM_DEFAULT_DATA = sizeof(DEFAULT_DATA) / sizeof(DEFAULT_DATA[0]);


// ============================================================================
//                       GLOBAL HELPER CLASSES FOR TESTING
// ----------------------------------------------------------------------------

                         // ====================
                         // class LogMessageSink
                         // ====================

struct LogMessageSink {
    // This struct provides a namespace for a utility function,
    // 'testMessageHandler', which is a valid log message handler
    // ('bsls::Log::LogMessageHandler') that will simply copy all arguments
    // into a set of 'public' 'static' data members.
  public:
    // PUBLIC CLASS DATA
    static bool s_hasBeenCalled;                         // Have we been called
                                                         // since the last
                                                         // reset?

    static char s_file[LOG_MESSAGE_SINK_BUFFER_SIZE];    // File name buffer

    static int  s_line;                                  // Line number

    static char s_message[LOG_MESSAGE_SINK_BUFFER_SIZE]; // Message buffer

    // CLASS METHODS
    static void reset();
        // Set 's_hasBeenCalled' to 'false', write a null byte to the beginning
        // of 's_file', set 's_line' to 0, and write a null byte to the
        // beginning of 's_message'.

    static void testMessageHandler(const char *file,
                                   const int   line,
                                   const char *message);
        // Copy the specified 'file' string into 's_file'.  Write the specified
        // 'line' to 's_line'. Copy the specified 'message' into 's_message'.
        // The behavior is undefined unless 'f' is a null-terminated string,
        // 'line' is not negative, and 'msg' is a null-terminated string.
};

// PUBLIC CLASS DATA
bool LogMessageSink::s_hasBeenCalled                         = false;
char LogMessageSink::s_file[LOG_MESSAGE_SINK_BUFFER_SIZE]    = {'\0'};
int  LogMessageSink::s_line                                  = 0;
char LogMessageSink::s_message[LOG_MESSAGE_SINK_BUFFER_SIZE] = {'\0'};

// CLASS METHODS
void LogMessageSink::reset()
{
    s_hasBeenCalled = false;
    s_file[0]       = '\0';
    s_line          = 0;
    s_message[0]    = '\0';
}

void LogMessageSink::testMessageHandler(const char *file,
                                        int         line,
                                        const char *message)
{
    ASSERT(file);
    ASSERT(line >= 0);
    ASSERT(message);

    s_hasBeenCalled = true;

    strncpy(s_file, file, LOG_MESSAGE_SINK_BUFFER_SIZE);
    s_line = line;
    strncpy(s_message, message, LOG_MESSAGE_SINK_BUFFER_SIZE);


    // Just to be safe.
    s_file   [LOG_MESSAGE_SINK_BUFFER_SIZE - 1] = '\0';
    s_message[LOG_MESSAGE_SINK_BUFFER_SIZE - 1] = '\0';
}


                         // ======================
                         // class OutputRedirector
                         // ======================

// Temp file creation and output redirection re-purposed from the pre-existing
// module in 'bsls_bsltestutil.t.cpp'.

class OutputRedirector {
    // This class provides a facility for redirecting 'stdout' and 'stderr' to
    // temporary files, retrieving output from the respective temporary file
    // and comparing the output to user-supplied character buffers.  An
    // 'OutputRedirector' object can be in an un-redirected state or a
    // redirected state.  If it is a redirected state, it will redirect either
    // 'stdout' or 'stderr', but not both simultaneously.  An
    // 'OutputRedirector' object has the concept of a scratch buffer, where
    // output captured from the process' 'stdout' or 'stderr' stream is stored
    // when the 'OutputRedirector' object is in the redirected state.
    // Throughout this class, the term "captured output" refers to data that
    // has been written to the 'stdout' or 'stderr' stream and is waiting to be
    // loaded into the scratch buffer.  Each time the 'load' method is called,
    // the scratch buffer is truncated, and the captured output is moved into
    // the scratch buffer.  When this is done, there is no longer any captured
    // output.
  public:
    // TYPES
    enum Stream {
        // The 'enum' 'Stream' represents the specific stream which our object
        // is responsible for redirecting.
        STDOUT_STREAM,
        STDERR_STREAM
    };

  private:
    // DATA
    char d_fileName[PATH_BUFFER_SIZE];        // name of temporary capture file

    char d_outputBuffer[OUTPUT_REDIRECTOR_BUFFER_SIZE];
    // 'd_outputBuffer' is the buffer that will hold the captured output.

    const Stream     d_stream;                // the stream for which this
                                              // object is responsible

    bool             d_isRedirectingFlag;     // Is this object currently
                                              // redirecting?

    bool             d_isFileCreatedFlag;     // has a temp file been created?

    bool             d_isOutputReadyFlag;     // has output been read from temp
                                              // file?

    size_t           d_outputSize;            // size of output loaded into
                                              // 'd_outputBuffer'

    struct stat      d_originalStat;          // status information for
                                              // 'stdout' or 'stderr' just
                                              // before redirection

    int              d_duplicatedOriginalFd;  // a file descriptor that is
                                              // associated with a duplicate of
                                              // the original target of the
                                              // redirected stream.  This is
                                              // made by calling 'dup' on the
                                              // original stream before any
                                              // redirection happens

    // PRIVATE MANIPULATORS
    void cleanup();
        // If the redirector is in a redirected state, restore the original
        // target of the redirected stream.  If the temporary file has been
        // created, delete it.

    void cleanupFiles();
        // Delete the temporary file, if it has been created.

    bool generateTempFileName();
        // Load into 'd_fileName' a file name string corresponding to the name
        // of a valid temp file on the system.  Return 'true' if the name was
        // successfully loaded, or 'false' otherwise.

  private:
    // NOT IMPLEMENTED
    OutputRedirector(const OutputRedirector&);                     // = delete;

    OutputRedirector& operator=(const OutputRedirector&);          // = delete;

  public:
    // CREATORS
    explicit OutputRedirector(Stream which);
        // Create an 'OutputRedirector' in an un-redirected state, and with an
        // empty scratch buffer.  Upon a call to 'enable', this redirector will
        // be responsible for redirecting the stream associated with the
        // specified 'which' to a temporary file.  The behavior is undefined
        // unless 'which' is equal to 'OutputRedirector::STDOUT_STREAM' or
        // 'OutputRedirector::STDERR_STREAM'.

    ~OutputRedirector();
        // Destroy this 'OutputRedirector' object.  If the object is in a
        // redirected state, the original stream will be restored to its
        // initial target and the temporary file to which the stream was
        // redirected will be deleted.

    // MANIPULATORS
    void disable();
        // If the redirector is in a redirected state, restore the original
        // target of the redirected stream and close the temporary buffer.  If
        // the redirector is not in a redirected state, this method is a no-op.
        // Calling this method invalidates all output in the temporary file, so
        // a call to 'load' after the next successful 'enable' call will not
        // load any output that was previously written to the file.  This
        // method does not clear the scratch buffer, so one may call 'load'
        // before calling 'disable', and the contents will be available after
        // 'disable' is called. If 'disable' fails to disable the redirection,
        // it will end the program by calling 'std::abort'.

    void enable();
        // If the 'Stream' specified at construction was 'STDOUT_STREAM',
        // redirect 'stdout' to a temporary file.  If the 'Stream' specified at
        // construction was 'STDERR_STREAM', redirect 'stderr' to a temporary
        // file. The temporary file to which the stream is redirected will be
        // created the first time 'enable' is called, and will be deleted when
        // this object is destroyed.  If 'enable' fails to redirect either
        // 'stdout' or 'stderr' it will end the program by calling
        // 'std::abort'.

    bool load();
        // Read captured output into the scratch buffer.  Return 'true' if all
        // captured output was successfully loaded, and 'false' otherwise.
        // Note that captured output is allowed to have zero length.  The
        // behavior is undefined unless 'enable' has been previously called
        // successfully (after the latest call to 'disable', if 'disable' has
        // been called successfully).

    void clear();
        // Reset the scratch buffer to empty.  The behavior is undefined unless
        // 'enable' has been previously called successfully (after the latest
        // call to 'disable' if 'disable' has been called successfully).

    // ACCESSORS
    int compare(const char *expected, size_t expectedLength) const;
        // Compare the character buffer pointed to by the specified pointer
        // 'expected' with any output that has been loaded into the scratch
        // buffer.  The length of the 'expected' buffer is supplied in the
        // specified 'expectedLength'.  Return 0 if the 'expected' buffer has
        // the same length and contents as the scratch buffer, and non-zero
        // otherwise.  Note that the 'expected' buffer is allowed to contain
        // embedded nulls.  The behavior is undefined unless 'enable' has
        // previously been called successfully.

    int compare(const char *expected) const;
        // Compare the character buffer pointed to by the specified pointer
        // 'expected' with any output that has been loaded into the scratch
        // buffer.  The 'expected' buffer is assumed to be a NTBS, and its
        // length is taken to be the string length of the NTBS.  Return 0 if
        // the 'expected' buffer has the same length and contents as the
        // scratch buffer, and non-zero otherwise.  The behavior is undefined
        // unless 'enable' has previously been called successfully.

    const char *getOutput() const;
        // Return the address of the scratch buffer.

    bool isOutputReady() const;
        // Return 'true' if the captured output has been loaded into the
        // scratch buffer, and 'false' otherwise.

    bool isRedirecting() const;
        // Return 'true' if 'stdout' or 'stderr' has been successfully
        // redirected, and 'false' otherwise.

    FILE *nonRedirectedStream() const;
        // Return the value of the global 'stdout' or 'stderr' corresponding to
        // the stream that is not intended to be redirected by this object.

    const struct stat& originalStat() const;
        // Return a reference to the status information for 'stdout' collected
        // just before redirection.  This method is used only to test the
        // correctness of 'OutputRedirector'.

    size_t outputSize() const;
        // Return the number of bytes currently loaded into the scratch buffer.

    FILE *redirectedStream() const;
        // Return the value of the global 'stdout' or 'stderr' corresponding to
        // the stream that is intended to be redirected by this object.

    OutputRedirector::Stream redirectedStreamId() const;
        // Return 'OutputRedirector::STDOUT_STREAM' if this object is
        // responsible for redirecting 'stdout', and
        // 'OutputRedirector::STDERR_STREAM' if this object is responsible for
        // redirecting 'stderr'.
};

// PRIVATE MANIPULATORS
void OutputRedirector::cleanup()
{
    disable();
    cleanupFiles();
}

void OutputRedirector::cleanupFiles()
{
#ifdef BSLS_PLATFORM_OS_WINDOWS
    _close(d_duplicatedOriginalFd);
#else
    close(d_duplicatedOriginalFd);
#endif

    d_duplicatedOriginalFd = -1;

    if (d_isFileCreatedFlag) {
        unlink(d_fileName);
        d_isFileCreatedFlag = false;
    }
}

bool OutputRedirector::generateTempFileName()
{
#ifdef BSLS_PLATFORM_OS_WINDOWS
    char tmpPathBuf[PATH_BUFFER_SIZE];
    if (! GetTempPathA(PATH_BUFFER_SIZE, tmpPathBuf) ||
        ! GetTempFileNameA(tmpPathBuf, "bsls", 0, d_fileName)) {
        return false;                                                 // RETURN
    }
#else
    char *fn = tempnam(0, "bsls");
    if (fn) {
        strncpy(d_fileName, fn, PATH_BUFFER_SIZE);
        free(fn);
        if(d_fileName[PATH_BUFFER_SIZE - 1] != '\0') {
            // Uh-oh! 'strncpy' didn't pad with zeroes.  Just fail here.
            return false;                                             // RETURN
        }
    } else {
        return false;                                                 // RETURN
    }
#endif
    if (veryVerbose) {
        fprintf(nonRedirectedStream(),
                "\tUsing '%s' as a base filename.\n",
                d_fileName);
    }
    return '\0' != d_fileName[0]; // Ensure that 'd_fileName' is not empty
}

// CREATORS
OutputRedirector::OutputRedirector(Stream which)
: d_stream(which)
, d_isRedirectingFlag(false)
, d_isFileCreatedFlag(false)
, d_isOutputReadyFlag(false)
, d_outputSize(0)
, d_duplicatedOriginalFd(-1)
{
    BSLS_ASSERT(which == STDOUT_STREAM || which == STDERR_STREAM);
    d_fileName[0] = '\0';
    memset(&d_originalStat, 0, sizeof(struct stat));
}

OutputRedirector::~OutputRedirector()
{
    cleanup();
}

// MANIPULATORS
void OutputRedirector::disable()
{
    if(!d_isRedirectingFlag) {
        return;                                                       // RETURN
    }

    d_isRedirectingFlag = false;

    fflush(redirectedStream());
    //fclose(redirectedStream());

#ifdef BSLS_PLATFORM_OS_WINDOWS
    const int status = _dup2(d_duplicatedOriginalFd,
                             _fileno(redirectedStream()));
#else
    const int status = dup2(d_duplicatedOriginalFd,
                            fileno(redirectedStream()));
#endif

    if(status < 0) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Error " __FILE__ "(%d): Bad 'dup2' status.\n",
                    __LINE__);
        }
        cleanupFiles();
        abort();
    }
}

void OutputRedirector::enable()
{
    if(d_isRedirectingFlag) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Warning " __FILE__ "(%d): Output already redirected\n",
                    __LINE__);
        }

        return;                                                       // RETURN
    }

    // Retain information about original file descriptor for use in later
    // tests.

    const int originalFD = fileno(redirectedStream());
    // These are meant to be the test driver 'ASSERT', not 'BSLS_ASSERT'
    ASSERT(-1 != originalFD);
    ASSERT(0 == fstat(originalFD, &d_originalStat));

    if(d_duplicatedOriginalFd == -1) {
#ifdef BSLS_PLATFORM_OS_WINDOWS
        d_duplicatedOriginalFd = _dup(originalFD);
#else
        d_duplicatedOriginalFd = dup(originalFD);
#endif
    }

    if(d_duplicatedOriginalFd < 0) {
        if (veryVerbose) {
                fprintf(nonRedirectedStream(),
                        "Error " __FILE__ "(%d): Bad 'dup' value.\n",
                        __LINE__);
            }
        cleanup();
        abort();
    }

    if(!d_isFileCreatedFlag) {
        if (!generateTempFileName()) {

            // Get temp file name

            if (veryVerbose) {
                fprintf(nonRedirectedStream(),
                        "Error "
                        __FILE__
                        "(%d): Failed to get temp file name for capture\n",
                        __LINE__);
            }
            cleanup();
            abort();
        }

        d_isFileCreatedFlag = true;
    }

    if (! freopen(d_fileName, "w+", redirectedStream())) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Error " __FILE__ "(%d): Failed to redirect stdout to"
                    " temp file '%s'\n",
                    __LINE__, d_fileName);
        }
        cleanup();
        abort();
    }

    if (EOF == fflush(redirectedStream())) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Warning " __FILE__ "(%d): Error flushing stream\n",
                    __LINE__);
        }
    }

    d_isRedirectingFlag = true;
}

bool OutputRedirector::load()
{
    BSLS_ASSERT(d_isRedirectingFlag);

    if(ferror(redirectedStream()) != 0) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Error " __FILE__ "(%d): Stream has an error\n",
                    __LINE__);
        }
        return false;                                                 // RETURN
    }

    const long tempOutputSize = ftell(redirectedStream());
    const long incremented    = tempOutputSize + 1;

    if(tempOutputSize < 0 || incremented < tempOutputSize) {
        // Protect against overflow or negative value
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Bad value from 'ftell': %ld",
                    incremented);
        }
        return false;                                                 // RETURN
    }

    // Conversion to 'unsigned long' is safe because 'incremented' > 0
    if(static_cast<unsigned long>(incremented) > SIZE_MAX) {
        // Our 'incremented' will not fit in a size_t, so it is too big for our
        // buffer.
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Bad value from 'ftell': %ld",
                    incremented);
        }
        return false;                                                 // RETURN
    }

    // Conversion to 'size_t' is safe because 'tempOutputSize' is nonnegative
    // and 'tempOutputSize'+1 is no larger than 'SIZE_MAX'.
    d_outputSize = static_cast<size_t>(tempOutputSize);

    if (static_cast<size_t>(incremented) > OUTPUT_REDIRECTOR_BUFFER_SIZE) {
        // Refuse to load output if it will not all fit in the scratch buffer,
        // INCLUDING the final null byte.
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Error "
                        __FILE__
                        "(%d): Captured output exceeds read buffer size\n",
                    __LINE__);
        }
        d_outputSize = 0u;
        return false;                                                 // RETURN
    }

    rewind(redirectedStream());

    const long charsRead = fread(d_outputBuffer,
                                 sizeof(char),
                                 d_outputSize,
                                 redirectedStream());

    if (charsRead < 0 || static_cast<unsigned long>(charsRead)!=d_outputSize) {
        // We failed to read all output from the capture file.
        if (veryVerbose) {
            if (ferror(redirectedStream())) {
                // We encountered a file error, not 'EOF'.
                fprintf(nonRedirectedStream(),
                        "Error "
                            __FILE__
                            "(%d): Non-EOF error with file\n",
                        __LINE__);
            }
            fprintf(nonRedirectedStream(),
                    "Error "
                        __FILE__
                        "(%d): Could not read all captured output\n",
                    __LINE__);
        }

        if(charsRead < 0) {
            d_outputBuffer[0] = '\0';
        } else if(static_cast<unsigned long>(charsRead) >= d_outputSize) {
            // This case should never happen.  This assignment is safe because
            // the total buffer size is enough to hold 'd_outputSize' + 1.
            d_outputBuffer[d_outputSize] = '\0';
        } else {
            d_outputBuffer[charsRead] = '\0';
        }
            // ...to ensure that direct inspection of buffer does not overflow

        return false;                                                 // RETURN

    } else {
        // We have read all output from the capture file.
        d_outputBuffer[d_outputSize] = '\0';
    }

    d_isOutputReadyFlag = true;

    return true;
}

void OutputRedirector::clear()
{
    BSLS_ASSERT(d_isRedirectingFlag);

    d_outputSize = 0u;
    d_isOutputReadyFlag = false;
    d_outputBuffer[0] = '\0';
    rewind(redirectedStream());
}

// ACCESSORS
int OutputRedirector::compare(const char *expected) const
{
    BSLS_ASSERT(expected);
    return compare(expected, strlen(expected));
}

int
OutputRedirector::compare(const char *expected, size_t expectedLength) const
{
    BSLS_ASSERT(expected || ! expectedLength);

    if (!d_isOutputReadyFlag) {
        if (veryVerbose) {
            fprintf(nonRedirectedStream(),
                    "Error " __FILE__ "(%d): No captured output available\n",
                    __LINE__);
        }
        return -1;                                                    // RETURN
    }

    // Use 'memcmp' instead of 'strncmp' to compare 'd_outputBuffer' to
    // 'expected', because 'expected' is allowed to contain embedded nulls.

    return static_cast<size_t>(d_outputSize) != expectedLength ||
           memcmp(d_outputBuffer, expected, expectedLength);
}

const char *OutputRedirector::getOutput() const
{
    return d_outputBuffer;
}

bool OutputRedirector::isOutputReady() const
{
    return d_isOutputReadyFlag;
}

bool OutputRedirector::isRedirecting() const
{
    return d_isRedirectingFlag;
}

FILE *OutputRedirector::nonRedirectedStream() const
{
    // This should return the opposite values
    if(d_stream == STDOUT_STREAM) {
        return stderr;                                                // RETURN
    } else {
        return stdout;                                                // RETURN
    }
}

const struct stat& OutputRedirector::originalStat() const
{
    return d_originalStat;
}

size_t OutputRedirector::outputSize() const
{
    return d_outputSize;
}

FILE *OutputRedirector::redirectedStream() const
{
    if(d_stream == STDOUT_STREAM) {
        return stdout;                                                // RETURN
    } else {
        return stderr;                                                // RETURN
    }
}

OutputRedirector::Stream OutputRedirector::redirectedStreamId() const
{
    return d_stream;
}

//=============================================================================
//                             USAGE EXAMPLES
//-----------------------------------------------------------------------------
namespace UsageExamples {

///Motivation
///----------
// This component provides a mechanism for installing and invoking a global log
// message handler function through a unified interface.  This interface is
// intended as a replacement for instances in low-level code in which a log
// message is written directly to an output stream such as 'stderr'.
//
// Outputting directly to a hard-coded stream is problematic in many ways.
// First, hard-coding the stream does not give users any freedom to control log
// messages.  A user may want all logs to be automatically redirected to a file
// or may want to add some custom formatting or handling.  These freedoms are
// not available if the output stream is predetermined.  Second, Windows
// applications running in non-console mode do not have a concept of 'stdout'
// or 'stderr'.  Writing directly to either of these streams is known to hang
// the process on certain Windows systems when no console is attached.  Third,
// a better overall logging system would encourage a greater number of useful
// logs to be written where they were originally omitted, possibly revealing
// previously unknown issues in code.
//
///Functionality
///-------------
// This section describes the full functionality provided by this component.
//
///Log Message Handlers
/// - - - - - - - - - -
// The class 'bsls::Log' provides a mechanism for installing and invoking a
// global log message handler function through a simple interface.
//
// Log message handler functions must have a signature that conforms to the
// 'bsls::Log::LogMessageHandler' 'typedef', which corresponds to a
// 'void'-valued function taking as input three parameters: the string name of
// the file from which the log was written, the line number at which the log
// was written, and the log message string.
//
///Invocation of the Installed Log Message Handler
///- - - - - - - - - - - - - - - - - - - - - - - -
// The dispatch method 'bsls::Log::logMessage' is provided as the most direct,
// safe, and efficient means of invoking the currently installed log message
// handler with the provided file name, line number, and message string.  It
// can be used as in the following expression:
//..
//  bsls::Log::logMessage(__FILE__, __LINE__, "Function 'f' is deprecated.");
//..
//
// The method 'bsls::Log::logFormatted' takes a format string and a set of
// variadic arguments instead of a raw message string.  This method will format
// the format string with the variadic argument substitutions according to the
// 'printf'-style rules and will subsequently invoke the currently installed
// log message handler with the provided file name and line number, along with
// the newly formatted string.  *Note:* Passing an arbitrary string as the
// format string is dangerous and can lead to undefined behavior.  The method
// may be used as in the following example:
//..
//  int x = 3;
//  const char *s = "Hello World";
//  bsls::Log::logFormatted(__FILE__, __LINE__, "Invalid String %d: %s", x, s);
//..
//
// The macro 'BSLS_LOG_SIMPLE' is provided.  This macro behaves exactly like
// 'bsls::Log::logMessage' except that the file name and line number parameters
// are automatically filled in with '__FILE__' and '__LINE__', respectively.
// Therefore, the 'BSLS_LOG_SIMPLE' macro takes a single parameter, which is
// the expected log message string:
//..
//  BSLS_LOG_SIMPLE("Error: Double initialization of singleton.");
//..
//
// Finally, the macro 'BSLS_LOG' is provided.  This macro behaves exactly like
// 'bsls::Log::logFormatted' except that the file name and line number
// parameters are automatically filled in with '__FILE__' and '__LINE__',
// respectively.  The 'BSLS_LOG' macro takes the format string as the first
// parameter, with the expected variadic substitutions as the subsequent
// parameters.  The 'BSLS_LOG' macro may be used as in the following example:
//..
//  int user = 5;
//  int admin = 7;
//  BSLS_LOG("User level %d does not have admin rights (%d).", user, admin);
//..
//
///Installation of Log Message Handlers
/// - - - - - - - - - - - - - - - - - -
// The currently installed log message handler function may be replaced by
// using the 'static' method 'bsls::Log::setLogMessageHandler' which takes, as
// a parameter, a single function pointer of type
// 'bsls::Log::LogMessageHandler'.
//
// Installing a log message handler is usually not necessary.  The default
// handler that is installed is usually the best handler available in a given
// moment.  Additionally, this component is designed to allow higher-level
// logging facilities to install their own low-level log message handlers that
// redirect all low-level logs to the higher-level facility.  Installing one's
// own handler may interfere with this behavior.
//
// As an example of a situation in which a user might want to install a log
// message handler function manually, suppose that the user wants to completely
// silence all low-level log messages.  This can be done by defining a log
// message handler that ignores all input messages, and later installing it:
//..
void ignoreMessage(const char *file, int line, const char *message)
    // Ignore the specified 'file', the specified 'line', and the specified
    // 'message'.  The behavior is undefined unless 'file' is a null-terminated
    // string, 'line' is not negative, and 'message' is a null-terminated
    // string.
{
    BSLS_ASSERT(file);
    BSLS_ASSERT(line >= 0);
    BSLS_ASSERT(message);
    return;
}
//..
//
// We may then install the handler:
//..
//  bsls::Log::setLogMessageHandler(&ignoreMessage);
//..
//
// If we attempt to write a low-level log, it will now be ignored:
//..
//  BSLS_LOG_SIMPLE("This message will be ignored");
//..
//
// Note that log message handlers may be invoked from different threads at the
// same time.  Ensure that handlers themselves are thread-safe if the program
// may use multiple threads.
//
///Standard Log Message Handlers
///- - - - - - - - - - - - - - -
// The 'static' methods 'bsls::Log::stdoutMessageHandler' and
// 'bsls::Log::stderrMessageHandler' are provided as a set of simple standard
// log message handlers.  'bsls::Log::stdoutMessageHandler' writes, in the
// following sequence, the file name, a colon character, the line number, a
// space character, the log message string, and a newline to the 'stdout'
// output stream.  'bsls::stderrMessageHandler' writes a string to the 'stderr'
// output stream with the same format as the string written by
// 'stdoutMessageHandler'.
//
// Suppose that the user wants to redirect all low-level logs to 'stdout'.
// This can be done by installing the standard handler
// 'bsls::Log::stdoutMessageHandler' as the log message handler:
//..
//  bsls::Log::setLogMessageHandler(&bsls::Log::stdoutMessageHandler);
//..
//
// Now, writing a low-level log message will result in a log printed to
// 'stdout':
//..
//  int x = 3;
//  BSLS_LOG("This message will be printed to stdout (x=%d)", x);
//..
//
// If the above logging call were to have occurred on line 7 of a file called
// 'myFile.cpp' , the following line would be printed to 'stdout', including
// a single newline:
//..
//  myfile.cpp:7 This message will be printed to stdout (x=3)
//..
//
// In addition to the simple handlers, the 'static' method
// 'bsls::Log::platformDefaultMessageHandler' is provided.  This method has
// platform-specific behavior.  Under non-Windows systems, this handler will
// simply ensure that all log messages are output to 'stderr', in the same
// format as the standard handler 'bsls::Log::stderrMessageHandler'.  On
// Windows, the method will check if the current process is running in console
// mode or non-console mode.  If the process is running in console mode, the
// method will behave exactly as in the non-Windows case: logs will be output
// to 'stderr', followed by a new line.  If the current process is running in
// non-console mode, messages will be written to the Windows debugger, in an
// format equivalent to the string written by the standard handler
// 'bsls::Log::stderrMessageHandler'.  In table form, this is described as
// follows:
//..
//  ========================================================================
//                                               Destination of
//    Platform      Console Mode    bsls::Log::platformDefaultMessageHandler
//  ------------    ------------    ----------------------------------------
//  Non-Windows     Undefined                       'stderr'
//  Windows         Console                         'stderr'
//  Windows         Non-Console                 Windows Debugger
//  ========================================================================
//..
//
///Default Log Message Handler
///- - - - - - - - - - - - - -
// The user does not need to do any set-up to write logs.  By default, the log
// message handler 'bsls::Log::platformDefaultMessageHandler' will be
// immediately available as the global log message handler.
//
///Usage
///-----
// This section illustrates the intended use of this component.
//
///Example 1: Logging Formatted Messages
///- - - - - - - - - - - - - - - - - - -
// Suppose that we are writing a function with certain input requirements.  If
// these requirements are not met, we may want to write a log alerting the
// client to the invalid input.
//
// First, we define a function, 'add', which will return the sum of two
// positive integer values:
//..
// myapp.cpp

unsigned int add(int a, int b)
    // Return the sum of the specified 'a' and the specified 'b'.  The behavior
    // is undefined unless 'a' and 'b' are not negative.
{
    unsigned int res;
    if(a >= 0 && b >= 0) {
        res = static_cast<unsigned int>(a) + static_cast<unsigned int>(b);
    } else {
//..
//
// If any of the input is invalid, we will want to alert the user to the error.
// We will use the macro 'BSLS_LOG' and will format the erroneous input values
// into the log message:
//..
        BSLS_LOG("Error: Invalid input combination (%d, %d).", a, b);
        res = 0;
    }

    return res;
}
//..
// The user might then call the 'add' function as follows:
//..
//  unsigned int x = add(3, -100);
//..
//
// Assuming the default log message handler is the currently installed handler,
// the following line would be printed to 'stderr' or to the Windows debugger:
//..
//  myapp.cpp:8 Error: Invalid input combination (3, -100).
//..
//
// !Note:! Never pass an arbitrary string as the format string to 'BSLS_LOG'.
// If the string happens to contain 'printf'-style format specifiers but the
// expected substitutions are not present, it will lead to undefined behavior.
//
///Example 2: Logging Formatless Messages
/// - - - - - - - - - - - - - - - - - - -
// Suppose we want to log a string that is not meant to be a 'printf'-style
// format string.
//
// First, we define a global association of error codes with error strings:
//..
// myapp.cpp

static const char *errorStrings[4] = {
    "Invalid input value 'a'.",
    "Invalid input value 'b'.",
    "Percentages add up to more than 100%.",
    "Please use '%2f' for a slash character in a URI."
};
//..
// Notice that the fourth string has a sequence that could be misinterpreted as
// a 'printf'-style format specifier.
//
// Next, we define a function that handles error codes and logs an error based
// on the error code:
//..
void handleError(size_t code)
    // Log the error message associated with the specified 'code'. The behavior
    // is undefined unless 'code' is in the range [0 .. 3].
{
    BSLS_ASSERT(code < sizeof(errorStrings)/sizeof(errorStrings[0]));
//..
//
// In the case that we receive a valid error code, we would want to log the
// string associated with this code.  We use the macro 'BSLS_LOG_SIMPLE' to
// ensure that the true strings are logged and are not interpreted as format
// strings:
//..
    BSLS_LOG_SIMPLE(errorStrings[code]);
}
//..
//
// A user may attempt to use error code '3':
//..
//  handleError(3);
//..
//
// Assuming the default log message handler is the currently installed handler,
// the following line would be printed to 'stderr' or to the Windows debugger:
//..
//  myapp.cpp:12 Please use '%2f' for a slash character in a URI.
//..
//
///Example 3: Using a Different File Name or Line Number
///- - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose that we want to define an error handling function that will use the
// logger to print an error message, except that we want to use the file name
// and line number of the code that *called* the error handling function
// instead of the file name and line number of the code *in* the error handling
// function.
//
// First, we will mimic Example 2 by defining a global array of error strings:
//..
// mylib.cpp

static const char *errorStringsNew[4] = {
    "Bad input attempt.",
    "Invalid types.",
    "Invalid username.",
    "Invalid password."
};
//..
//
// Next, we will define a function that takes in a file name and line number
// along with the error code:
//..
void handleErrorFlexible(const char *file, int line, size_t code)
    // Log the error message associated with the specified 'code', using the
    // specified 'file' and the specified 'line' as the source location for the
    // error.  The behavior is undefined unless 'file' is a null-terminated
    // string, 'line' is not negative, and 'code' is in the range [0 .. 3].
{
    BSLS_ASSERT(file);
    BSLS_ASSERT(line >= 0);
    BSLS_ASSERT(code < sizeof(errorStringsNew)/sizeof(errorStringsNew[0]));
//..
//
// We can bypass the macros by calling the function 'bsls::Log::logMessage'
// directly, allowing us to pass in the given file name and line number:
//..
    bsls::Log::logMessage(file, line, errorStringsNew[code]);
}
//..
//
// A user in a different file may now specify the original source of an error:
//..
//  handleErrorFlexible(__FILE__, __LINE__, 2);
//..
//
// If this line of code were placed on line 5 of the file 'otherapp.cpp', the
// following line would be printed to 'stderr' or to the Windows debugger:
//..
//  otherapp.cpp:5 Invalid username.
//..
//
// Users may wrap their error function in a macro to automatically fill in the
// file name and line number parameters, as done by the 'BSLS_LOG*' macros.

}  // close UsageExamples namespace


//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    int         test = argc > 1 ? atoi(argv[1]) : 0;
                verbose = argc > 2;
            veryVerbose = argc > 3;
        veryVeryVerbose = argc > 4;
    veryVeryVeryVerbose = argc > 5;

    printf("TEST %s CASE %d\n", __FILE__, test);

    switch(test) { case 0: // zero is always the leading case
      case 11: {
        // --------------------------------------------------------------------
        // COMPONENT-LEVEL DOCUMENTATION 1
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   COMPONENT-LEVEL DOCUMENTATION 1
        // --------------------------------------------------------------------
        if (verbose) puts("\nCOMPONENT-LEVEL DOCUMENTATION 1"
                          "\n===============================");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        bsls::Log::logMessage(__FILE__,
                              __LINE__,
                              "Function 'f' is deprecated.");
        int x = 3;
        const char *s = "Hello World";
        bsls::Log::logFormatted(__FILE__,
                                __LINE__,
                                "Invalid String %d: %s",
                                x,
                                s);

        BSLS_LOG_SIMPLE("Error: Double initialization of singleton.");
        int user = 5;
        int admin = 7;
        BSLS_LOG("User level %d does not have admin rights (%d).",
                 user,
                 admin);
      } break;
      case 10: {
        // --------------------------------------------------------------------
        // COMPONENT-LEVEL DOCUMENTATION 2
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   COMPONENT-LEVEL DOCUMENTATION 2
        // --------------------------------------------------------------------
        if (verbose) puts("\nCOMPONENT-LEVEL DOCUMENTATION 2"
                          "\n===============================");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        bsls::Log::setLogMessageHandler(&ignoreMessage);
        BSLS_LOG_SIMPLE("This message will be ignored");
      } break;
      case 9: {
        // --------------------------------------------------------------------
        // COMPONENT-LEVEL DOCUMENTATION 3
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   COMPONENT-LEVEL DOCUMENTATION 3
        // --------------------------------------------------------------------
        if (verbose) puts("\nCOMPONENT-LEVEL DOCUMENTATION 3"
                          "\n===============================");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        bsls::Log::setLogMessageHandler(&bsls::Log::stdoutMessageHandler);
        int x = 3;
        BSLS_LOG("This message will be printed to stdout (x=%d)", x);
      } break;
      case 8: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE 1
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   USAGE EXAMPLE 1
        // --------------------------------------------------------------------
        if (verbose) puts("\nUSAGE EXAMPLE 1"
                          "\n===============");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        unsigned int x = add(3, -100);
      } break;
      case 7: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE 2
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   USAGE EXAMPLE 2
        // --------------------------------------------------------------------
        if (verbose) puts("\nUSAGE EXAMPLE 2"
                          "\n===============");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        handleError(3);
      } break;
      case 6: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE 3
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Copy entire component documentation header into header of test
        //    driver, within the namespace 'UsageExamples'.  Uncomment any
        //    usage example lines that do not need to execute any code and can
        //    exist outside of the 'main' function.  For code that must be
        //    executed in main, copy it into a case of the test driver, after a
        //    declaration of 'using namespace UsageExamples;'.  White-space may
        //    be changed so that the code may fit in an indented block.  Code
        //    may be prepended or appended to allow output to be suppressed.
        //
        // Testing:
        //   USAGE EXAMPLE 3
        // --------------------------------------------------------------------
        if (verbose) puts("\nUSAGE EXAMPLE 3"
                          "\n===============");

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);
        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);

        stderrRedirector.enable();
        if(!verbose) {
            stdoutRedirector.enable();
        }

        using namespace UsageExamples;

        handleErrorFlexible(__FILE__, __LINE__, 2);
      } break;
      case 5: {
        // --------------------------------------------------------------------
        // CLASS METHOD 'logMessage', macro 'BSLS_LOG_SIMPLE'
        //
        // Concerns:
        //: 1 'logMessage' properly calls the currently installed handler
        //:
        //: 2 The macro 'BSLS_LOG_SIMPLE' properly calls 'logMessage'
        //:
        //: 3 The macro 'BSLS_LOG_SIMPLE' does not interpret the message as a
        //:   'printf'-style format string
        //
        // Plan:
        //: 1 Set the currently installed log message handler to
        //:   'LogMessageSink::testMessageHandler'. (C-1)
        //:
        //: 2 Call 'bsls::Log::logMessage' with simple values. (C-1)
        //:
        //: 3 Confirm that the registered handler was called with the proper
        //:   parameters. (C-1)
        //:
        //: 4 Reset the sink.  Call 'BSLS_LOG_SIMPLE' with a simple message
        //:   that includes 'printf'-style formats that should not be formatted
        //    (C-2) (C-3)
        //:
        //: 5 Confirm that the registered handler was again called, with the
        //:   current file name, the proper line number, and the simple
        //:   message. (C-2) (C-3)
        //
        // Testing:
        //   static void logMessage(file, line, message);
        //   BSLS_LOG_SIMPLE(message)
        // --------------------------------------------------------------------
        if (verbose) {
            puts("\nLOGGING A SIMPLE MESSAGE"
                 "\n------------------------\n");
        }

        bsls::Log::setLogMessageHandler(&LogMessageSink::testMessageHandler);
        ASSERT(bsls::Log::logMessageHandler()
               == &LogMessageSink::testMessageHandler);

        {
            if (verbose) {
                puts("\nCALLING logMessage"
                     "\n------------------\n");
            }
            LogMessageSink::reset();

            const char * const testFile    = "helloworld.cpp";
            const int          testLine    = 18437;
            const char * const testMessage = "Hello World!";

            bsls::Log::logMessage(testFile, testLine, testMessage);

            ASSERT(LogMessageSink::s_hasBeenCalled);

            LOOP2_ASSERT(testFile,
                         LogMessageSink::s_file,
                         strcmp(testFile, LogMessageSink::s_file) == 0);

            LOOP2_ASSERT(testLine,
                         LogMessageSink::s_line,
                         testLine == LogMessageSink::s_line);

            LOOP2_ASSERT(testMessage,
                         LogMessageSink::s_message,
                         strcmp(testMessage, LogMessageSink::s_message) == 0);

        }

        {
            if (verbose) {
                puts("\nCALLING BSLS_LOG_SIMPLE"
                     "\n-----------------------\n");
            }

            LogMessageSink::reset();

            const char * const testFile    = __FILE__;
            const char * const testMessage = "Hello%d Wor%ld%s!";

            // We are expanding the '__LINE__' macro on the same line as the
            // call to 'BSLS_LOG_SIMPLE' to ensure that the true line numbers
            // match.
            const int testLine =__LINE__; BSLS_LOG_SIMPLE(testMessage);

            ASSERT(LogMessageSink::s_hasBeenCalled);

            LOOP2_ASSERT(testFile,
                         LogMessageSink::s_file,
                         strcmp(testFile, LogMessageSink::s_file) == 0);

            LOOP2_ASSERT(testLine,
                         LogMessageSink::s_line,
                         testLine == LogMessageSink::s_line);

            LOOP2_ASSERT(testMessage,
                         LogMessageSink::s_message,
                         strcmp(testMessage, LogMessageSink::s_message) == 0);

            }

      } break;
      case 4: {
        // --------------------------------------------------------------------
        // CLASS METHODS 'setLogMessageHandler', 'logMessageHandler'
        //   Ensure that setting and retrieving a specific handler works as
        //   expected.
        //
        // Concerns:
        //: 1 The standard handlers ('stdoutMessageHandler',
        //:   'stderrMessageHandler', 'platformDefaultMessageHandler') can be
        //:   set and retrieved as the handler.
        //:
        //: 2 The handler defined in this test driver,
        //:   'LogMessageSink::testMessageHandler' can be set and retrieved as
        //:   the handler.
        //
        // Plan:
        //: 1 Call 'setLogMessageHandler' with 'stdoutMessageHandler' as a
        //:   parameter, and confirm that 'logMessageHandler()' returns
        //:   'stdoutMessageHandler'. (C-1)
        //:
        //: 2 Call 'setLogMessageHandler' with 'stderrMessageHandler' as a
        //:   parameter, and confirm that 'logMessageHandler()' returns
        //:   'stderrMessageHandler'. (C-1)
        //:
        //: 3 Call 'setLogMessageHandler' with 'platformDefaultMessageHandler'
        //:   as a parameter, and confirm that 'logMessageHandler()' returns
        //:   'platformDefaultMessageHandler'. (C-1)
        //:
        //: 4 Call 'setLogMessageHandler' with 'testMessageHandler' as a
        //:   parameter, and confirm that 'logMessageHandler()' returns
        //:   'testMessageHandler'. (C-1)
        //
        // Testing:
        //   static bsls::Log::LogMessageHandler logMessageHandler();
        //   static void setLogMessageHandler(bsls::Log::LogMessageHandler);
        // --------------------------------------------------------------------
        if (verbose) {
            puts("\nSETTING AND RETRIEVING THE LOG MESSAGE HANDLER"
                 "\n----------------------------------------------\n");
        }

        bsls::Log::setLogMessageHandler(&bsls::Log::stdoutMessageHandler);
        ASSERT(bsls::Log::logMessageHandler()
               == &bsls::Log::stdoutMessageHandler);

        bsls::Log::setLogMessageHandler(&bsls::Log::stderrMessageHandler);
        ASSERT(bsls::Log::logMessageHandler()
               == &bsls::Log::stderrMessageHandler);

        bsls::Log::setLogMessageHandler(
                                    &bsls::Log::platformDefaultMessageHandler);
        ASSERT(bsls::Log::logMessageHandler()
               == &bsls::Log::platformDefaultMessageHandler);

        bsls::Log::setLogMessageHandler(&LogMessageSink::testMessageHandler);
        ASSERT(bsls::Log::logMessageHandler()
               == &LogMessageSink::testMessageHandler);

      } break;
      case 3: {
        // --------------------------------------------------------------------
        // CLASS METHODS 'stdoutMessageHandler', 'stderrMessageHandler'
        //   Ensure that the methods correctly output a properly formatted
        //   string to their respective streams.
        //
        // Concerns:
        //: 1 'stdoutMessageHandler' writes to 'stdout', and
        //:   'stderrMessageHandler' writes to 'stderr'
        //:
        //: 2 Both methods format the output string with the same expected
        //:   format
        //:
        //: 3 Both methods properly format strings with all combinations of:
        //:   {empty file name, non-empty file name} X {zero line number,
        //:   other line number} X {empty message string, non-empty message
        //:   string}
        //:
        //: 4 Both methods properly handle the extreme value 'INT_MAX' for the
        //:   line number
        //
        // Plan:
        //: 1 For each handler, write a simple string and capture its output.
        //:   Ensure that the captured string matches what is expected. (C-1),
        //:   (C-2)
        //:
        //: 2 For each handler, apply the more extensive format inputs and
        //:   confirm that the output is formatted just like a predefined
        //:   expected format string. (C-3)
        //:
        //: 3 For each handler, send it a normal file name and message, but
        //:   send a line number of INT_MAX, and ensure that the captured
        //:   output is as expected. (C-4)
        //:
        //
        // Testing:
        //   typedef void (*LogMessageHandler)(file, line, message);
        //   static void stdoutMessageHandler(file, line, message);
        //   static void stderrMessageHandler(file, line, message);
        // --------------------------------------------------------------------
        if (verbose) {
            fputs("\nBASIC STREAM HANDLERS"
                  "\n---------------------\n",
                  stdout);
        }
        static const struct {
            bsls::Log::LogMessageHandler d_handler;
            OutputRedirector::Stream d_stream;
        } HANDLERS[] = {
            {&bsls::Log::stdoutMessageHandler,OutputRedirector::STDOUT_STREAM},
            {&bsls::Log::stderrMessageHandler,OutputRedirector::STDERR_STREAM}
        };
        const size_t NUM_HANDLERS = sizeof(HANDLERS)/sizeof(HANDLERS[0]);

        // If more handlers are added, extra code changes will be required.
        ASSERT(NUM_HANDLERS == 2);

        for(size_t i=0; i < NUM_HANDLERS; i++) {
            const bsls::Log::LogMessageHandler HANDLER = HANDLERS[i].d_handler;
            const OutputRedirector::Stream     STREAM  = HANDLERS[i].d_stream;

            if(verbose) {
                if(STREAM == OutputRedirector::STDOUT_STREAM) {
                    puts("Testing 'stdoutMessageHandler'.");
                } else {
                    puts("Testing 'stderrMessageHandler'.");
                }
            }

            if(veryVerbose) { P_(i) P_(STREAM)}

            OutputRedirector redirector(STREAM);

            // 1 Basic Writing Operation'
            if(verbose) puts("Writing simple string.");
            redirector.enable();
            HANDLER("testfile.cpp", 1073, "Testing basic operation.");
            redirector.load();
            redirector.disable();
            LOOP2_ASSERT(i, STREAM, redirector.isOutputReady());

            // 2 Proper Simple Format
            if(verbose) puts("Confirming simple string.");
            LOOP3_ASSERT(i,
                         STREAM,
                         redirector.getOutput(),
                         0 == redirector.compare(
                              "testfile.cpp:1073 Testing basic operation.\n"));

            // 3 Proper Complex Format
            if(verbose) puts("Checking complex combinations.");
            for(size_t j = 0; j < NUM_DEFAULT_DATA; j++) {
                const int          SOURCE_LINE
                                    = DEFAULT_DATA[j].d_sourceLine;

                const char * const FILE
                                    = DEFAULT_DATA[j].d_file;

                const int          LINE
                                    = DEFAULT_DATA[j].d_line;

                const char * const MESSAGE
                                    = DEFAULT_DATA[j].d_message;

                const char * const EXPECTED
                                    = DEFAULT_DATA[j].d_expected;

                if(veryVerbose)
                { T_
                  P_(SOURCE_LINE)
                  P_(i)
                  P_(STREAM)
                  P_(j)
                  P_(FILE)
                  P_(LINE)
                  P_(MESSAGE)
                  P_(EXPECTED)
                }

                redirector.enable();
                HANDLER(FILE, LINE, MESSAGE);
                redirector.load();
                redirector.disable();

                LOOP5_ASSERT(i,
                             j,
                             SOURCE_LINE,
                             EXPECTED,
                             redirector.getOutput(),
                             0 == redirector.compare(EXPECTED));

            }

            // 4 INT_MAX handling
            if(verbose) puts("Checking INT_MAX.");

            const char * const normalFile    = "some_file.cpp";
            const int          extremeLine   = INT_MAX;
            const char * const normalMessage = "Some message!";

            // There will be no system in which INT_MAX will be more than ~20
            // characters (in hypothetical 64-bit case), so we have a very good
            // buffer space here:
            char expectedString[50];

#ifdef BSLS_PLATFORM_OS_WINDOWS
            const int status = _snprintf(
                              expectedString,
                              sizeof(expectedString)/sizeof(expectedString[0]),
                              "%s:%d %s\n",
                              normalFile,
                              extremeLine,
                              normalMessage);
#else
            const int status = snprintf(
                              expectedString,
                              sizeof(expectedString)/sizeof(expectedString[0]),
                              "%s:%d %s\n",
                              normalFile,
                              extremeLine,
                              normalMessage);
#endif

            ASSERT(status > 0);
            ASSERT(static_cast<unsigned int>(status)
                           < sizeof(expectedString)/sizeof(expectedString[0]));

            redirector.enable();
            HANDLER(normalFile, extremeLine, normalMessage);
            redirector.load();
            redirector.disable();

            LOOP4_ASSERT(i,
                         STREAM,
                         expectedString,
                         redirector.getOutput(),
                         0 == redirector.compare(expectedString));

        }
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // TEST-DRIVER LOG MESSAGE HANDLER APPARATUS TEST
        //:  Ensure that the locally defined handler exhibits proper behavior.
        //:
        // Concerns:
        //:  1 The 'static' state of 'LogMessageSink' is initialized as valid.
        //:
        //:  2 Calling 'LogMessageSink::testMessageHandler' results in the
        //:    proper values being set.
        //:
        //:  3 Calling 'LogMessageSink::reset' puts the 'static' state in the
        //:    same state that existed at initialization.
        //:
        // Plan:
        //:  1 Confirm that all four 'static' members of 'LogMessageSink' are
        //     valid empty values.
        //:
        //:  2 Call 'LogMessageSink::testMessageHandler' with valid values, and
        //:    confirm that the state variables are valid.
        //:
        //:  3 Confirm that 'ftell(stdout)' succeeds.  This demonstrates that
        //:    'stdout' is a seekable file.
        //:
        //:  4 Write a simple string to 'stdout' and read it back with
        //:    'OutputRedirector::load'.  Confirm that
        //:    'OutputRedirector::compare' gives the correct results.
        //
        // Testing:
        //   TEST-DRIVER LOG MESSAGE HANDLER APPARATUS
        // --------------------------------------------------------------------
        if (verbose) {
            fputs("\nTESTING TEST APPARATUS PART 2"
                  "\n-----------------------------\n",
                  stdout);
        }

        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);;

        {
            // 1 Object has valid state
            ASSERT(stdoutRedirector.redirectedStreamId()
                   == OutputRedirector::STDOUT_STREAM);
            stdoutRedirector.enable();
            ASSERT(stdoutRedirector.isRedirecting());
        }

        {
            // 2 Object has consistent internals
            ASSERT(stdoutRedirector.redirectedStream()    == stdout);
            ASSERT(stdoutRedirector.nonRedirectedStream() == stderr);
        }

        {
            // 3 Output is redirected
            ASSERT(-1 != ftell(stdout));
        }

        {
            // 4 'compare' works
            fprintf(stdout, "Hello\nWorld\n%s", "World, Hello");
            ASSERT(stdoutRedirector.load());
            ASSERT(0==stdoutRedirector.compare("Hello\nWorld\nWorld, Hello"));
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // STREAM REDIRECTION APPARATUS TEST
        //
        // Concerns:
        //:  1 Object can be initialized for 'stderr' and is in a proper state
        //:
        //:  2 'redirectedStream' and 'nonRedirectedStream' work
        //:
        //:  3 'enable' works and can be called many times in order
        //:
        //:  4 Output is redirected
        //:
        //:  5 Captured output is readable
        //:
        //:  6 'load' works
        //:
        //:  7 'clear' works
        //:
        //:  8 'compare' works
        //:
        //:  9 Incorrect output is correctly diagnosed
        //:
        //: 10 Embedded newlines work
        //:
        //: 11 Empty output works
        //:
        //: 12 Embedded nulls work
        //:
        //: 13 Filesystem-dependent control sequences work
        //:
        //: 14 Calling 'enable' after any number of 'disable' calls produces
        //:    consistent behavior
        //:
        //: 15 Object can be enabled for 'stdout' and is in a proper state
        //:
        //: 17 Output is redirected for 'stdout'
        //:
        //: 18 'compare' works for 'stdout'
        //:
        //
        // Plan:
        //:  1 Initialize an 'OutputRedirector' object and ask it to redirect
        //:    'stderr'.  Confirm that a call to 'redirectedStreamId' returns
        //:    'STDERR_STREAM'. (C-1)
        //:
        //:  2 Confirm that 'redirectedStream' returns a pointer equal to
        //:    'stderr', and 'nonRedirectedStream' returns a pointer equal to
        //:    'stdout'. (C-2)
        //:
        //:  3 Call 'enable', and ensure that 'isRedirecting' returns 'true'.
        //:    Again call 'enable' and ensure that 'isRedirecting' still
        //:    returns 'true'. (C-3)
        //:
        //:  4 Confirm that 'ftell(stderr)' succeeds.  This demonstrates that
        //:    'stderr' is a seekable file. (C-4)
        //:
        //:  5 Write a string to 'stderr', confirm that 'stderr's seek position
        //:    has changed, read back the contents of 'stderr' and compare them
        //:    to the original string. (C-5)
        //:
        //:  6 Write a string to 'stderr'.  Confirm that
        //:    'OutputRedirector::load' changes the contents of the output
        //:    buffer and that it changes the result of
        //:    'OutputRedirector::isOutputReady' from 'false' to 'true'.
        //:    Confirm that the contents of the output buffer match the
        //:    original string. (C-6)
        //:
        //:  7 Write a string to 'stderr' and load it with
        //:    'OutputRedirector::load'.  Confirm that
        //:    'OutputRedirector::clear' rewinds 'stderr', changes the output
        //:    of 'OutputRedirector::isOutputReady' from 'true' to 'false' and
        //:    sets the length of the output buffer to 0. (C-7)
        //:
        //:  8 Write a string to 'stderr' and read it back with
        //:    'OutputRedirector::load'.  Confirm that
        //:    'OutputRedirector::compare' gives the correct results when the
        //:    captured output is compared with the following data: (C-8)
        //:
        //:        Data                            Comparison Result
        //:    ------------                    -------------------------
        //:    input string                            true
        //:    input string with appended data         false
        //:    input string truncated                  false
        //:    string different from input:
        //:    at beginning                            false
        //:    at end                                  false
        //:    elsewhere                               false
        //:
        //:  9 Confirm that 'load' fails when there is more data in 'stderr'
        //:    than can be fit in the capture buffer.  Confirm that 'compare'
        //:    fails if 'load' has not been first called to read data into the
        //:    capture buffer. (C-9)
        //:
        //: 10 Confirm that strings containing embedded newlines are correctly
        //:    captured and correctly identified by 'compare'. (C-10)
        //:
        //: 11 Write an empty string to 'stderr'.  Confirm that it can be
        //:    correctly loaded and compared with the original. (C-11)
        //:
        //: 12 Write a series of strings to 'stderr', containing '\0' at the
        //:    beginning, end or interior of the string.  Confirm that the
        //:    captured output can be correctly loaded and compared with the
        //:    original input. (C-12)
        //:
        //: 13 Write a series of strings to 'stderr' containing '^D' and
        //:    '<CRLF>' and confirm that these strings are correctly captured
        //:    and loaded. (C-13)
        //:
        //: 14 Clear the scratch buffer.  Write a string to 'stderr'. Call
        //:    'load'.  Confirm 'isOutputReady'.  Call 'compare'.  Call
        //:    'disable'.  Confirm that 'isRedirecting' is false.  Call
        //:    'disable' and again confirm that 'isRedirecting' is false.
        //:    Confirm 'isOutputReady'.  Call 'compare'.  Call 'enable'.
        //:    Confirm that 'isRedirecting' is true.  Confirm 'isOutputReady'.
        //:    Call 'compare'.  Confirm that 'ftell(stderr)' succeeds.  This
        //:    demonstrates that 'stderr' is a seekable file.  Call 'load'.
        //:    Confirm 'isOutputReady'. Ensure that the buffer is now empty.
        //:    (C-14)
        //:
        //: 15 Initialize an 'OutputRedirector' object and ask it to redirect
        //:    'stdout'.  Confirm that a call to redirectionStreamId' returns
        //:    'STDOUT_STREAM'.  Confirm that 'redirectedStream' returns a
        //:    pointer equal to 'stdout', and 'nonRedirectedStream' returns a
        //:    pointer equal to 'stderr'.  Call 'enable' and confirm
        //:    'isRedirecting'.
        //:
        //: 17 Confirm that 'ftell(stdout)' succeeds.  This demonstrates that
        //:    'stdout' is a seekable file.
        //:
        //: 18 Write a simple string to 'stdout' and read it back with
        //:    'OutputRedirector::load'.  Confirm that
        //:    'OutputRedirector::compare' gives the correct results.
        //
        // Testing:
        //   STREAM REDIRECTION APPARATUS
        // --------------------------------------------------------------------

        // Test cases re-purposed from 'OutputRedirector' test cases in
        // 'bsls_bsltestutil.cpp'.

        if (verbose) {
            fputs("\nTESTING TEST APPARATUS"
                  "\n----------------------\n",
                  stdout);
        }

        OutputRedirector stderrRedirector(OutputRedirector::STDERR_STREAM);

        {
            // 1 Object has valid state
            if (verbose) {
                fputs("\nTESTING OBJECT INITIALIZATION"
                      "\n-----------------------------\n",
                      stdout);
            }

            ASSERT(stderrRedirector.redirectedStreamId()
                   == OutputRedirector::STDERR_STREAM);
        }

        {
            // 3 Object has consistent internals
            if (verbose) {
                fputs("\nTESTING OBJECT CONSISTENCY"
                      "\n--------------------------\n",
                      stdout);
            }

            ASSERT(stderrRedirector.redirectedStream()    == stderr);
            ASSERT(stderrRedirector.nonRedirectedStream() == stdout);
        }

        {
            // 3 'enable' works
            if (verbose) {
                fputs("\nTESTING ENABLE CALL"
                      "\n-------------------\n",
                      stdout);
            }

            stderrRedirector.enable();
            ASSERT(stderrRedirector.isRedirecting());

            stderrRedirector.enable();
            ASSERT(stderrRedirector.isRedirecting());
        }

        {
            // 4 Output is redirected
            if (verbose) {
                fputs("\nTESTING OUTPUT REDIRECTION"
                      "\n--------------------------\n",
                      stdout);
            }

            ASSERT(-1 != ftell(stderr));
        }

        {
            // 5 Captured output is readable
            if (verbose) {
                fputs("\nTESTING OUTPUT CAPTURE"
                      "\n----------------------\n",
                      stdout);
            }

            enum { TEST_STRING_SIZE = 15 };
            const char *testString = "This is output";
            char buffer[TEST_STRING_SIZE];

            ASSERT(TEST_STRING_SIZE == strlen(testString) + 1);

            rewind(stderr);
            long initialStderrPosition = ftell(stderr);
            ASSERT(0 == initialStderrPosition);
            fprintf(stderr, "%s", testString);
            long finalStderrPosition = ftell(stderr);
            ASSERT(-1 != finalStderrPosition);
            ASSERT(finalStderrPosition > initialStderrPosition);
            long outputSize = finalStderrPosition - initialStderrPosition;
            ASSERT(outputSize + 1 == TEST_STRING_SIZE);
            rewind(stderr);
            size_t bytesWritten =
                fread(buffer, sizeof(char), outputSize, stderr);
            ASSERT(static_cast<long>(bytesWritten) == outputSize);
            buffer[TEST_STRING_SIZE - 1] = '\0';
            ASSERT(0 == strcmp(testString, buffer));
        }

        {
            // 6 'load' works
            if (verbose) {
                fputs("\nTESTING OUTPUT CAPTURE LOAD"
                      "\n---------------------------\n",
                      stdout);
            }

            const char *testString = "This is output";
            size_t testStringLength = strlen(testString);

            rewind(stderr);
            fprintf(stderr, "%s", testString);
            ASSERT(static_cast<long>(testStringLength) == ftell(stderr));
            ASSERT(false == stderrRedirector.isOutputReady());
            ASSERT(0 == stderrRedirector.outputSize());
            ASSERT(stderrRedirector.load());
            ASSERT(static_cast<long>(testStringLength) == ftell(stderr));
            ASSERT(true == stderrRedirector.isOutputReady());
            ASSERT(testStringLength == stderrRedirector.outputSize());
            ASSERT(0 == memcmp(testString,
                               stderrRedirector.getOutput(),
                               stderrRedirector.outputSize()));
        }

        {
            // 7 'clear' works
            if (verbose) {
                fputs("\nTESTING OUTPUT CAPTURE RESET"
                      "\n----------------------------\n",
                      stdout);
            }

            const char *testString = "This is output";
            size_t testStringLength = strlen(testString);

            rewind(stderr);
            fprintf(stderr, "%s", testString);
            stderrRedirector.load();
            ASSERT(static_cast<long>(testStringLength) == ftell(stderr));
            ASSERT(true == stderrRedirector.isOutputReady());
            ASSERT(testStringLength == stderrRedirector.outputSize());
            stderrRedirector.clear();
            ASSERT(0 == ftell(stderr));
            ASSERT(true != stderrRedirector.isOutputReady());
            ASSERT(0 == stderrRedirector.outputSize());
        }

        {
            // 8 'compare' works
            if (verbose) {
                fputs("\nTESTING OUTPUT CAPTURE VERIFICATION"
                      "\n-----------------------------------\n",
                      stdout);
            }

            const char *testString            = "This is output";
            const char *longString            = "This is outputA";
            const char *shortString           = "This is outpu";
            const char *differentStartString  = "Xhis is output";
            const char *differentEndString    = "This is outpuy";
            const char *differentMiddleString = "This iz output";

            stderrRedirector.clear();
            fprintf(stderr, "%s", testString);
            ASSERT(stderrRedirector.load());
            ASSERT(!!strcmp(testString, testString) ==
                       !!stderrRedirector.compare(testString));
            ASSERT(!!strcmp(testString, shortString) ==
                       !!stderrRedirector.compare(shortString));
            ASSERT(!!strcmp(testString, longString) ==
                       !!stderrRedirector.compare(longString));
            ASSERT(!!strcmp(testString, differentStartString) ==
                       !!stderrRedirector.compare(differentStartString));
            ASSERT(!!strcmp(testString, differentEndString) ==
                       !!stderrRedirector.compare(differentEndString));
            ASSERT(!!strcmp(testString, differentMiddleString) ==
                       !!stderrRedirector.compare(differentMiddleString));
        }

        {
            // 9 Incorrect output is correctly diagnosed
            if (verbose) {
                fputs("\nTESTING OUTPUT CAPTURE ERROR"
                      "\n----------------------------\n",
                      stdout);
            }

            // Reset verbosity levels to suppress expected error output
            const bool tempVeryVerbose     = veryVerbose;
            const bool tempVeryVeryVerbose = veryVeryVerbose;
            veryVerbose     = false;
            veryVeryVerbose = false;

            const char *testString = "This is good output";

            stderrRedirector.clear();
            size_t strLength = strlen(testString);

            // Print this string enough times to be *larger* than the expected
            // output buffer size
            for (int i=0; i*strLength <= OUTPUT_REDIRECTOR_BUFFER_SIZE; ++i) {
                fprintf(stderr, "%s", testString);
            }
            fprintf(stderr, "%s", testString);
            ASSERT(!stderrRedirector.load());

            stderrRedirector.clear();
            fprintf(stderr, "%s", testString);
            ASSERT(0 != stderrRedirector.compare(testString));

            stderrRedirector.clear();
            ASSERT(0 != stderrRedirector.compare("", 0));

            veryVerbose     = tempVeryVerbose;
            veryVeryVerbose = tempVeryVeryVerbose;
        }

        {
            // 10 Embedded newlines work
            if (verbose) {
                fputs("\nTESTING EMBEDDED NEWLINES"
                      "\n-------------------------\n",
                      stdout);
            }

            const char *testString = "This has an\nembedded newline";

            stderrRedirector.clear();
            fprintf(stderr, "%s", testString);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(testString));

            const char *twoNewlineTestString =
                "This has two\nembedded newlines\n";

            stderrRedirector.clear();
            fprintf(stderr, "%s", twoNewlineTestString);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(twoNewlineTestString));
        }

        {
            // 11 Empty output works
            if (verbose) {
                fputs("\nTESTING EMPTY OUTPUT"
                      "\n--------------------\n",
                      stdout);
            }

            const char *testString = "";

            stderrRedirector.clear();
            fprintf(stderr, "%s", testString);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(testString));
        }

        {
            // 12 Embedded nulls work
            if (verbose) {
                fputs("\nTESTING EMBEDDED NULLS"
                      "\n----------------------\n",
                      stdout);
            }

            const char *testString1 = "abc\0def";
            const char *testString2 = "\0def";
            const char *testString3 = "abc\0";

            stderrRedirector.clear();
            fwrite(testString1, sizeof(char), 7, stderr);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(testString1, 7));

            stderrRedirector.clear();
            fwrite(testString2, sizeof(char), 4, stderr);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(testString2, 4));

            stderrRedirector.clear();
            fwrite(testString3, sizeof(char), 4, stderr);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(testString3, 4));
        }

        {
            // 13 Filesystem-dependent control sequences work
            if (verbose) {
                fputs("\nTESTING CONTROL SEQUENCES"
                      "\n-------------------------\n",
                      stdout);
            }

            const char *crnlTestString = "ab\r\ncd";
            const char *ctrlDTestString = "ab" "\x04" "cd";

            stderrRedirector.clear();
            fprintf(stderr, "%s", crnlTestString);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(crnlTestString));

            stderrRedirector.clear();
            fprintf(stderr, "%s", ctrlDTestString);
            ASSERT(stderrRedirector.load());
            ASSERT(0 == stderrRedirector.compare(ctrlDTestString));
        }

        {
            // 14 'disable' then 'enable' works
            if (verbose) {
                fputs("\nTESTING DISABLE THEN ENABLE"
                      "\n---------------------------\n",
                      stdout);
            }

            const char *testString = "helloWorld\n123\nabcdefg\n";

            stderrRedirector.clear();
            fputs(testString, stderr);
            ASSERT(stderrRedirector.load());

            ASSERT(stderrRedirector.isOutputReady());

            ASSERT(0 == stderrRedirector.compare(testString));

            stderrRedirector.disable();

            ASSERT(!stderrRedirector.isRedirecting());

            stderrRedirector.disable();

            ASSERT(!stderrRedirector.isRedirecting());

            ASSERT(stderrRedirector.isOutputReady());

            ASSERT(0 == stderrRedirector.compare(testString));

            stderrRedirector.enable();

            ASSERT(stderrRedirector.isRedirecting());

            ASSERT(stderrRedirector.isOutputReady());

            ASSERT(0 == stderrRedirector.compare(testString));

            ASSERT(-1 != ftell(stderr));

            stderrRedirector.load();

            ASSERT(stderrRedirector.isOutputReady());

            ASSERT(0 == stderrRedirector.compare(""));
        }

        stderrRedirector.disable();

        OutputRedirector stdoutRedirector(OutputRedirector::STDOUT_STREAM);;

        {
            // 15 Object has valid state for 'stdout'
            if (verbose) {
                fputs("\nTESTING REDIRECTOR FOR STDOUT"
                      "\n-----------------------------\n",
                      stdout);
            }

            ASSERT(stdoutRedirector.redirectedStreamId()
                   == OutputRedirector::STDOUT_STREAM);
            ASSERT(stdoutRedirector.redirectedStream()    == stdout);
            ASSERT(stdoutRedirector.nonRedirectedStream() == stderr);
            stdoutRedirector.enable();
            ASSERT(stdoutRedirector.isRedirecting());
            stdoutRedirector.disable();
        }

        {
            // 16 Output is redirected
            if (verbose) {
                fputs("\nTESTING FTELL STATE"
                      "\n-------------------\n",
                      stdout);
            }
            stdoutRedirector.enable();
            ASSERT(-1 != ftell(stdout));
            stdoutRedirector.disable();
        }

        {
            // 17 'compare' works
            if (verbose) {
                fputs("\nTESTING A SIMPLE STRING"
                      "\n-----------------------\n",
                      stdout);
            }
            stdoutRedirector.enable();
            fprintf(stdout, "Hello\nWorld\n%s", "World, Hello");
            ASSERT(stdoutRedirector.load());
            ASSERT(stdoutRedirector.isOutputReady());
            ASSERT(0==stdoutRedirector.compare("Hello\nWorld\nWorld, Hello"));
            stdoutRedirector.disable();
        }

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
// Copyright (C) 2014 Bloomberg Finance L.P.
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