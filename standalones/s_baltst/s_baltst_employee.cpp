// s_baltst_employee.cpp           *DO NOT EDIT*           @generated -*-C++-*-

#include <bsls_ident.h>
BSLS_IDENT_RCSID(s_baltst_employee_cpp, "$Id$ $CSID$")

#include <s_baltst_employee.h>

#include <bdlat_formattingmode.h>
#include <bdlat_valuetypefunctions.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>
#include <bdlb_string.h>

#include <bsl_string.h>
#include <s_baltst_address.h>
#include <bslim_printer.h>
#include <bsls_assert.h>

#include <bsl_cstring.h>
#include <bsl_iomanip.h>
#include <bsl_limits.h>
#include <bsl_ostream.h>
#include <bsl_utility.h>

namespace BloombergLP {
namespace s_baltst {

                               // --------------
                               // class Employee
                               // --------------

// CONSTANTS

const char Employee::CLASS_NAME[] = "Employee";

const bdlat_AttributeInfo Employee::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_NAME,
        "name",
        sizeof("name") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_HOME_ADDRESS,
        "homeAddress",
        sizeof("homeAddress") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        ATTRIBUTE_ID_AGE,
        "age",
        sizeof("age") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *Employee::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    Employee::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *Employee::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_NAME:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME];
      case ATTRIBUTE_ID_HOME_ADDRESS:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS];
      case ATTRIBUTE_ID_AGE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE];
      default:
        return 0;
    }
}

// CREATORS

Employee::Employee(bslma::Allocator *basicAllocator)
: d_homeAddress(basicAllocator)
, d_name(basicAllocator)
, d_age()
{
}

Employee::Employee(const Employee& original,
                   bslma::Allocator *basicAllocator)
: d_homeAddress(original.d_homeAddress, basicAllocator)
, d_name(original.d_name, basicAllocator)
, d_age(original.d_age)
{
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
Employee::Employee(Employee&& original) noexcept
: d_homeAddress(bsl::move(original.d_homeAddress))
, d_name(bsl::move(original.d_name))
, d_age(bsl::move(original.d_age))
{
}

Employee::Employee(Employee&& original,
                   bslma::Allocator *basicAllocator)
: d_homeAddress(bsl::move(original.d_homeAddress), basicAllocator)
, d_name(bsl::move(original.d_name), basicAllocator)
, d_age(bsl::move(original.d_age))
{
}
#endif

Employee::~Employee()
{
}

// MANIPULATORS

Employee&
Employee::operator=(const Employee& rhs)
{
    if (this != &rhs) {
        d_name = rhs.d_name;
        d_homeAddress = rhs.d_homeAddress;
        d_age = rhs.d_age;
    }

    return *this;
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
Employee&
Employee::operator=(Employee&& rhs)
{
    if (this != &rhs) {
        d_name = bsl::move(rhs.d_name);
        d_homeAddress = bsl::move(rhs.d_homeAddress);
        d_age = bsl::move(rhs.d_age);
    }

    return *this;
}
#endif

void Employee::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_name);
    bdlat_ValueTypeFunctions::reset(&d_homeAddress);
    bdlat_ValueTypeFunctions::reset(&d_age);
}

// ACCESSORS

bsl::ostream& Employee::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("name", this->name());
    printer.printAttribute("homeAddress", this->homeAddress());
    printer.printAttribute("age", this->age());
    printer.end();
    return stream;
}


}  // close package namespace
}  // close enterprise namespace

// GENERATED BY @BLP_BAS_CODEGEN_VERSION@
// USING bas_codegen.pl s_baltst_employee.xsd --mode msg --includedir . --msgComponent employee --noRecurse --noExternalization --noHashSupport --noAggregateConversion
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright 2022 Bloomberg Finance L.P. All rights reserved.
//      Property of Bloomberg Finance L.P. (BFLP)
//      This software is made available solely pursuant to the
//      terms of a BFLP license agreement which governs its use.
// ------------------------------- END-OF-FILE --------------------------------
