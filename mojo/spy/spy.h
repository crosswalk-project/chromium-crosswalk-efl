// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SPY_SPY_H_
#define MOJO_SPY_SPY_H_

#include <string>
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class Thread;
}

namespace mojo {

class ApplicationManager;
class SpyServerImpl;

// mojo::Spy is a troubleshooting and debugging aid. It helps tracking
// the mojo system core activities like messages, service creation, etc.
//
// The |options| parameter in the constructor comes from the command
// line of the mojo_shell. Which takes --spy=<options>. Each option is
// separated by ',' and each option is a key+ value pair separated by ':'.
//
// For example --spy=port:13333
//
class Spy {
 public:
  Spy(mojo::ApplicationManager* application_manager,
      const std::string& options);
  ~Spy();

 private:
  scoped_refptr<SpyServerImpl> spy_server_;
  // This thread runs the code that talks to the frontend.
  scoped_ptr<base::Thread> control_thread_;
};

}  // namespace mojo

#endif  // MOJO_SPY_SPY_H_
