// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_NPOBJECT_LIFETIME_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_NPOBJECT_LIFETIME_TEST_H_

#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

// The NPObjectLifeTime class tests the case where a plugin has an NPObject
// which points to a different plugin instance on a different frame in the
// page and whether refcounts on this npobject are valid when the source frame
// is destroyed.
class NPObjectLifetimeTest : public PluginTest {
 public:
  // Constructor.
  NPObjectLifetimeTest(NPP id, NPNetscapeFuncs *host_functions);

  // NPAPI SetWindow handler.
  NPError SetWindow(NPWindow* pNPWindow) override;

  void URLNotify(const char* url, NPReason reason, void* data) override;

 protected:
  NPObject* other_plugin_instance_object_;

#if defined(OS_WIN)
  static void CALLBACK TimerProc(HWND window, UINT message, UINT_PTR timer_id,
                                 DWORD elapsed_milli_seconds);
  UINT_PTR timer_id_;
#endif
  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectLifetimeTest);
};

// The NPObjectLifetimeTestInstance2 class represents the plugin instance
// which is deleted by the NPObjectLifeTime class via a javascript function.
class NPObjectLifetimeTestInstance2 : public PluginTest {
 public:
  // Constructor.
  NPObjectLifetimeTestInstance2(NPP id, NPNetscapeFuncs *host_functions);
  ~NPObjectLifetimeTestInstance2() override;

  // NPAPI SetWindow handler.
  NPError SetWindow(NPWindow* pNPWindow) override;

 protected:
  static NPObject* plugin_instance_object_;
  friend class NPObjectLifetimeTest;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectLifetimeTestInstance2);
};

// The NPObjectLifeTime class tests the case where a plugin instance is
// destroyed in NPN_Evaluate
class NPObjectDeletePluginInNPN_Evaluate : public PluginTest {
 public:
  // Constructor.
  NPObjectDeletePluginInNPN_Evaluate(NPP id, NPNetscapeFuncs *host_functions);
  ~NPObjectDeletePluginInNPN_Evaluate() override;

  // NPAPI SetWindow handler.
  NPError SetWindow(NPWindow* pNPWindow) override;

 protected:
  NPObject* plugin_instance_object_;
#if defined(OS_WIN)
  static void CALLBACK TimerProc(HWND window, UINT message, UINT_PTR timer_id,
                                 DWORD elapsed_milli_seconds);
  UINT_PTR timer_id_;
#endif

 private:
  static NPObjectDeletePluginInNPN_Evaluate* g_npn_evaluate_test_instance_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NPObjectDeletePluginInNPN_Evaluate);
};

}  // namespace NPAPIClient

#endif  // CONTENT_TEST_PLUGIN_PLUGIN_NPOBJECT_LIFETIME_TEST_H_
