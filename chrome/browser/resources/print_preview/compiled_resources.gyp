# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'targets': [
    {
      'target_name': 'print_preview',
      'variables': {
        'depends': [
          '../../../../ui/webui/resources/js/cr.js',
          '../../../../ui/webui/resources/js/cr/event_target.js',
          '../../../../ui/webui/resources/js/cr/ui.js',
          '../../../../ui/webui/resources/js/cr/ui/focus_manager.js',
          '../../../../ui/webui/resources/js/cr/ui/focus_outline_manager.js',
          '../../../../ui/webui/resources/js/event_tracker.js',
          '../../../../ui/webui/resources/js/local_strings.js',
          '../../../../ui/webui/resources/js/util.js',
        ],
        'externs': ['<(CLOSURE_DIR)/externs/chrome_send_externs.js'],
      },
      'includes': ['../../../../third_party/closure_compiler/compile_js.gypi'],
    }
  ],
}
