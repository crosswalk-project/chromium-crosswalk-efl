// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/scoped_nsobject.h"
#include "chrome/browser/autofill/autofill_common_unittest.h"
#import "chrome/browser/autofill/autofill_credit_card_model_mac.h"
#include "chrome/browser/autofill/credit_card.h"
#include "chrome/browser/cocoa/browser_test_helper.h"
#import "chrome/browser/cocoa/cocoa_test_helper.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

typedef CocoaTest AutoFillCreditCardModelTest;

TEST(AutoFillCreditCardModelTest, Basic) {
  // A basic test that creates a new instance and releases.
  // Aids valgrind leak detection.
  CreditCard credit_card(ASCIIToUTF16("myCC"), 0);
  scoped_nsobject<AutoFillCreditCardModel> model(
      [[AutoFillCreditCardModel alloc] initWithCreditCard:credit_card]);
  EXPECT_TRUE(model.get());
}

TEST(AutoFillCreditCardModelTest, InitializationFromCreditCard) {
  CreditCard credit_card(string16(), 0);
  autofill_unittest::SetCreditCardInfo(
      &credit_card,
      "Corporate",
      "John Dillinger",
      "Visa",
      "123456789012",
      "01",
      "2010",
      "123",
      "Chicago",
      "Indianapolis");
  scoped_nsobject<AutoFillCreditCardModel> model(
      [[AutoFillCreditCardModel alloc] initWithCreditCard:credit_card]);
  EXPECT_TRUE(model.get());

  EXPECT_TRUE([[model label] isEqualToString:@"Corporate"]);
  EXPECT_TRUE([[model nameOnCard] isEqualToString:@"John Dillinger"]);
  EXPECT_TRUE([[model creditCardNumber] isEqualToString:@"123456789012"]);
  EXPECT_TRUE([[model expirationMonth] isEqualToString:@"01"]);
  EXPECT_TRUE([[model expirationYear] isEqualToString:@"2010"]);
  EXPECT_TRUE([[model cvcCode] isEqualToString:@"123"]);
  EXPECT_TRUE([[model billingAddress] isEqualToString:@"Chicago"]);
  EXPECT_TRUE([[model shippingAddress] isEqualToString:@"Indianapolis"]);
}

TEST(AutoFillCreditCardModelTest, CopyModelToCreditCard) {
  CreditCard credit_card(string16(), 0);
  autofill_unittest::SetCreditCardInfo(
      &credit_card,
      "Corporate",
      "John Dillinger",
      "Visa",
      "123456789012",
      "01",
      "2010",
      "123",
      "Chicago",
      "Indianapolis");
  scoped_nsobject<AutoFillCreditCardModel> model(
      [[AutoFillCreditCardModel alloc] initWithCreditCard:credit_card]);
  EXPECT_TRUE(model.get());

  [model setLabel:@"CorporateX"];
  [model setNameOnCard:@"John DillingerX"];
  [model setCreditCardNumber:@"223456789012"];
  [model setExpirationMonth:@"11"];
  [model setExpirationYear:@"2011"];
  [model setCvcCode:@"223"];
  [model setBillingAddress:@"New York"];
  [model setShippingAddress:@"Boston"];

  [model copyModelToCreditCard:&credit_card];

  EXPECT_EQ(ASCIIToUTF16("CorporateX"), credit_card.Label());
  EXPECT_EQ(ASCIIToUTF16("John DillingerX"),
            credit_card.GetFieldText(AutoFillType(CREDIT_CARD_NAME)));
  EXPECT_EQ(ASCIIToUTF16("223456789012"),
            credit_card.GetFieldText(AutoFillType(CREDIT_CARD_NUMBER)));
  EXPECT_EQ(ASCIIToUTF16("11"),
            credit_card.GetFieldText(AutoFillType(CREDIT_CARD_EXP_MONTH)));
  EXPECT_EQ(ASCIIToUTF16("2011"),
            credit_card.GetFieldText(
                AutoFillType(CREDIT_CARD_EXP_4_DIGIT_YEAR)));
  EXPECT_EQ(ASCIIToUTF16("223"),
            credit_card.GetFieldText(
                AutoFillType(CREDIT_CARD_VERIFICATION_CODE)));
  EXPECT_EQ(ASCIIToUTF16("New York"), credit_card.billing_address());
  EXPECT_EQ(ASCIIToUTF16("Boston"), credit_card.shipping_address());
}

}  // namespace
