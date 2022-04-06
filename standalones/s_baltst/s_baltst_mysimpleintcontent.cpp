// s_baltst_mysimpleintcontent.cpp      *DO NOT EDIT*      @generated -*-C++-*-

#include <bsls_ident.h>
BSLS_IDENT_RCSID(s_baltst_mysimpleintcontent_cpp, "$Id$ $CSID$")

#include <s_baltst_mysimpleintcontent.h>

#include <bdlat_formattingmode.h>
#include <bdlat_valuetypefunctions.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>
#include <bdlb_string.h>

#include <bdlb_nullablevalue.h>
#include <bsl_string.h>
#include <bslim_printer.h>
#include <bsls_assert.h>

#include <bsl_cstring.h>
#include <bsl_iomanip.h>
#include <bsl_limits.h>
#include <bsl_ostream.h>
#include <bsl_utility.h>

namespace BloombergLP {
namespace s_baltst {

                          // ------------------------
                          // class MySimpleIntContent
                          // ------------------------

// CONSTANTS

const char MySimpleIntContent::CLASS_NAME[] = "MySimpleIntContent";

const bdlat_AttributeInfo MySimpleIntContent::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
      | bdlat_FormattingMode::e_ATTRIBUTE
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
      | bdlat_FormattingMode::e_ATTRIBUTE
    },
    {
        ATTRIBUTE_ID_THE_CONTENT,
        "TheContent",
        sizeof("TheContent") - 1,
        "",
        bdlat_FormattingMode::e_DEC
      | bdlat_FormattingMode::e_SIMPLE_CONTENT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySimpleIntContent::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySimpleIntContent::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySimpleIntContent::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      case ATTRIBUTE_ID_THE_CONTENT:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_THE_CONTENT];
      default:
        return 0;
    }
}

// CREATORS

MySimpleIntContent::MySimpleIntContent(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_theContent()
, d_attribute1()
{
}

MySimpleIntContent::MySimpleIntContent(const MySimpleIntContent& original,
                                       bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_theContent(original.d_theContent)
, d_attribute1(original.d_attribute1)
{
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
MySimpleIntContent::MySimpleIntContent(MySimpleIntContent&& original) noexcept
: d_attribute2(bsl::move(original.d_attribute2))
, d_theContent(bsl::move(original.d_theContent))
, d_attribute1(bsl::move(original.d_attribute1))
{
}

MySimpleIntContent::MySimpleIntContent(MySimpleIntContent&& original,
                                       bslma::Allocator *basicAllocator)
: d_attribute2(bsl::move(original.d_attribute2), basicAllocator)
, d_theContent(bsl::move(original.d_theContent))
, d_attribute1(bsl::move(original.d_attribute1))
{
}
#endif

MySimpleIntContent::~MySimpleIntContent()
{
}

// MANIPULATORS

MySimpleIntContent&
MySimpleIntContent::operator=(const MySimpleIntContent& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_attribute2 = rhs.d_attribute2;
        d_theContent = rhs.d_theContent;
    }

    return *this;
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
MySimpleIntContent&
MySimpleIntContent::operator=(MySimpleIntContent&& rhs)
{
    if (this != &rhs) {
        d_attribute1 = bsl::move(rhs.d_attribute1);
        d_attribute2 = bsl::move(rhs.d_attribute2);
        d_theContent = bsl::move(rhs.d_theContent);
    }

    return *this;
}
#endif

void MySimpleIntContent::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
    bdlat_ValueTypeFunctions::reset(&d_theContent);
}

// ACCESSORS

bsl::ostream& MySimpleIntContent::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", this->attribute1());
    printer.printAttribute("attribute2", this->attribute2());
    printer.printAttribute("theContent", this->theContent());
    printer.end();
    return stream;
}


}  // close package namespace
}  // close enterprise namespace

// GENERATED BY @BLP_BAS_CODEGEN_VERSION@
// USING bas_codegen.pl s_baltst_mysimpleintcontent.xsd --mode msg --includedir . --msgComponent mysimpleintcontent --noRecurse --noExternalization --noHashSupport --noAggregateConversion
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright 2021 Bloomberg Finance L.P. All rights reserved.
//      Property of Bloomberg Finance L.P. (BFLP)
//      This software is made available solely pursuant to the
//      terms of a BFLP license agreement which governs its use.
// ------------------------------- END-OF-FILE --------------------------------
