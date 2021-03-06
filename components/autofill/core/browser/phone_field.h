// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_FIELD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_FIELD_H_

#include <vector>

#include "base/compiler_specific.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "components/autofill/core/browser/form_field.h"
#include "components/autofill/core/browser/phone_number.h"

namespace autofill {

class AutofillField;
class AutofillScanner;

// A phone number in one of the following formats:
// - area code, prefix, suffix
// - area code, number
// - number
class PhoneField : public FormField {
 public:
  virtual ~PhoneField();

  static FormField* Parse(AutofillScanner* scanner);

 protected:
  // FormField:
  virtual bool ClassifyField(ServerFieldTypeMap* map) const OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, ParseOneLinePhone);
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, ParseTwoLinePhone);
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, ThreePartPhoneNumber);
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, ThreePartPhoneNumberPrefixSuffix);
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, ThreePartPhoneNumberPrefixSuffix2);
  FRIEND_TEST_ALL_PREFIXES(PhoneFieldTest, CountryAndCityAndPhoneNumber);

  // This is for easy description of the possible parsing paths of the phone
  // fields.
  enum RegexType {
    REGEX_COUNTRY,
    REGEX_AREA,
    REGEX_AREA_NOTEXT,
    REGEX_PHONE,
    REGEX_PREFIX_SEPARATOR,
    REGEX_PREFIX,
    REGEX_SUFFIX_SEPARATOR,
    REGEX_SUFFIX,
    REGEX_EXTENSION,

    // Separates regexps in grammar.
    REGEX_SEPARATOR,
  };

  // Parsed fields.
  enum PhonePart {
    FIELD_NONE = -1,
    FIELD_COUNTRY_CODE,
    FIELD_AREA_CODE,
    FIELD_PHONE,
    FIELD_SUFFIX,
    FIELD_EXTENSION,

    FIELD_MAX,
  };

  struct Parser {
    RegexType regex;       // Field matching reg-ex.
    PhonePart phone_part;  // Index of the field.
    size_t max_size;       // Max size of the field to match. 0 means any.
  };

  static const Parser kPhoneFieldGrammars[];

  PhoneField();

  // Returns the regular expression string correspoding to |regex_id|
  static base::string16 GetRegExp(RegexType regex_id);

  // FIELD_PHONE is always present; holds suffix if prefix is present.
  // The rest could be NULL.
  AutofillField* parsed_phone_fields_[FIELD_MAX];

  DISALLOW_COPY_AND_ASSIGN(PhoneField);
};

}  // namespace autofill

#endif  // COMPONENTS_AUTOFILL_CORE_BROWSER_PHONE_FIELD_H_
