// s_baltst_mysequencewithdoubleattribute.cpp*DO NOT EDIT* @generated -*-C++-*-

#include <bsls_ident.h>
BSLS_IDENT_RCSID(s_baltst_mysequencewithdoubleattribute_cpp, "$Id$ $CSID$")

#include <s_baltst_mysequencewithdoubleattribute.h>

#include <bdlat_formattingmode.h>
#include <bdlat_valuetypefunctions.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>
#include <bdlb_string.h>

#include <bdlb_nullablevalue.h>
#include <bslim_printer.h>
#include <bsls_assert.h>

#include <bsl_cstring.h>
#include <bsl_iomanip.h>
#include <bsl_limits.h>
#include <bsl_ostream.h>
#include <bsl_utility.h>

namespace BloombergLP {
namespace s_baltst {

                    // -----------------------------------
                    // class MySequenceWithDoubleAttribute
                    // -----------------------------------

// CONSTANTS

const char MySequenceWithDoubleAttribute::CLASS_NAME[] = "MySequenceWithDoubleAttribute";

const bdlat_AttributeInfo MySequenceWithDoubleAttribute::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
      | bdlat_FormattingMode::e_ATTRIBUTE
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithDoubleAttribute::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 1; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithDoubleAttribute::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithDoubleAttribute::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithDoubleAttribute::MySequenceWithDoubleAttribute()
: d_attribute1()
{
}

MySequenceWithDoubleAttribute::MySequenceWithDoubleAttribute(const MySequenceWithDoubleAttribute& original)
: d_attribute1(original.d_attribute1)
{
}

MySequenceWithDoubleAttribute::~MySequenceWithDoubleAttribute()
{
}

// MANIPULATORS

MySequenceWithDoubleAttribute&
MySequenceWithDoubleAttribute::operator=(const MySequenceWithDoubleAttribute& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
    }

    return *this;
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
MySequenceWithDoubleAttribute&
MySequenceWithDoubleAttribute::operator=(MySequenceWithDoubleAttribute&& rhs)
{
    if (this != &rhs) {
        d_attribute1 = bsl::move(rhs.d_attribute1);
    }

    return *this;
}
#endif

void MySequenceWithDoubleAttribute::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
}

// ACCESSORS

bsl::ostream& MySequenceWithDoubleAttribute::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", this->attribute1());
    printer.end();
    return stream;
}


}  // close package namespace
}  // close enterprise namespace

// GENERATED BY @BLP_BAS_CODEGEN_VERSION@
// USING bas_codegen.pl s_baltst_mysequencewithdoubleattribute.xsd --mode msg --includedir . --msgComponent mysequencewithdoubleattribute --noRecurse --noExternalization --noHashSupport --noAggregateConversion
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright 2021 Bloomberg Finance L.P. All rights reserved.
//      Property of Bloomberg Finance L.P. (BFLP)
//      This software is made available solely pursuant to the
//      terms of a BFLP license agreement which governs its use.
// ------------------------------- END-OF-FILE --------------------------------
