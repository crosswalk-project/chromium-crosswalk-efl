// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler_utils.h"

namespace chromeos {

namespace {

template <typename StringListType>
bool ParseStringList(const base::Value* value, StringListType* out_value) {
  const base::ListValue* list = NULL;
  if (!value->GetAsList(&list))
    return false;
  out_value->resize(list->GetSize());
  for (size_t i = 0; i < list->GetSize(); ++i) {
    if (!list->GetString(i, &((*out_value)[i])))
      return false;
  }
  return true;
}

}  // namespace

bool ParseValue(const base::Value* value, bool* out_value) {
  return value->GetAsBoolean(out_value);
}

bool ParseValue(const base::Value* value, int* out_value) {
  return value->GetAsInteger(out_value);
}

bool ParseValue(const base::Value* value, double* out_value) {
  return value->GetAsDouble(out_value);
}

bool ParseValue(const base::Value* value, std::string* out_value) {
  return value->GetAsString(out_value);
}

bool ParseValue(const base::Value* value, base::string16* out_value) {
  return value->GetAsString(out_value);
}

bool ParseValue(const base::Value* value,
                const base::DictionaryValue** out_value) {
  return value->GetAsDictionary(out_value);
}

bool ParseValue(const base::Value* value, StringList* out_value) {
  return ParseStringList(value, out_value);
}

bool ParseValue(const base::Value* value, String16List* out_value) {
  return ParseStringList(value, out_value);
}

base::FundamentalValue MakeValue(bool v) {
  return base::FundamentalValue(v);
}

base::FundamentalValue MakeValue(int v) {
  return base::FundamentalValue(v);
}

base::FundamentalValue MakeValue(double v) {
  return base::FundamentalValue(v);
}

base::StringValue MakeValue(const std::string& v) {
  return base::StringValue(v);
}

base::StringValue MakeValue(const base::string16& v) {
  return base::StringValue(v);
}

void CallbackWrapper0(base::Callback<void()> callback,
                      const base::ListValue* args) {
  DCHECK(args);
  DCHECK(args->empty());
  callback.Run();
}

}  // namespace chromeos
