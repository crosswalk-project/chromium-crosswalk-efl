// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOFILL_PASSWORD_GENERATION_POPUP_VIEW_TESTER_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOFILL_PASSWORD_GENERATION_POPUP_VIEW_TESTER_VIEWS_H_

#include "chrome/browser/ui/autofill/password_generation_popup_view_tester.h"

namespace autofill {

class PasswordGenerationPopupViewViews;

class PasswordGenerationPopupViewTesterViews :
      public PasswordGenerationPopupViewTester {
 public:
  explicit PasswordGenerationPopupViewTesterViews(
      PasswordGenerationPopupViewViews* view);
  virtual ~PasswordGenerationPopupViewTesterViews();

  virtual void SimulateMouseMovementAt(const gfx::Point& point) OVERRIDE;

 private:
  // Weak reference
  PasswordGenerationPopupViewViews* view_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationPopupViewTesterViews);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_VIEWS_AUTOFILL_PASSWORD_GENERATION_POPUP_VIEW_TESTER_VIEWS_H_
