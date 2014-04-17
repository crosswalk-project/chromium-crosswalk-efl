// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_GALLERIES_DIALOG_CONTROLLER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_GALLERIES_DIALOG_CONTROLLER_H_

#include <list>
#include <map>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/media_galleries/media_galleries_preferences.h"
#include "components/storage_monitor/removable_storage_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

namespace ui {
class MenuModel;
}

class MediaGalleriesDialogController;
class MediaGalleryContextMenu;
class Profile;

// Newly added galleries are not added to preferences until the dialog commits,
// so they do not have a pref id while the dialog is open; leading to
// complicated code in the dialogs. To solve this complication, the controller
// maps pref ids into a new space where it can also assign ids to new galleries.
// The new number space is only valid for the lifetime of the controller. To
// make it more clear where real pref ids are used and where the fake ids are
// used, the GalleryDialogId type is used where fake ids are needed.
typedef MediaGalleryPrefId GalleryDialogId;

// The view.
class MediaGalleriesDialog {
 public:
  virtual ~MediaGalleriesDialog();

  // Tell the dialog to update its display list of galleries.
  virtual void UpdateGalleries() = 0;

  // Constructs a platform-specific dialog owned and controlled by |controller|.
  static MediaGalleriesDialog* Create(
      MediaGalleriesDialogController* controller);
};

// The controller is responsible for handling the logic of the dialog and
// interfacing with the model (i.e., MediaGalleriesPreferences). It shows
// the dialog and owns itself.
class MediaGalleriesDialogController
    : public ui::SelectFileDialog::Listener,
      public storage_monitor::RemovableStorageObserver,
      public MediaGalleriesPreferences::GalleryChangeObserver {
 public:
  struct GalleryPermission {
    GalleryPermission(GalleryDialogId gallery_id,
                      const MediaGalleryPrefInfo& pref_info,
                      bool allowed)
        : gallery_id(gallery_id),
          pref_info(pref_info),
          allowed(allowed) {
    }
    GalleryPermission() {}

    GalleryDialogId gallery_id;
    MediaGalleryPrefInfo pref_info;
    bool allowed;
  };

  typedef std::vector<GalleryPermission> GalleryPermissionsVector;

  // The constructor creates a dialog controller which owns itself.
  MediaGalleriesDialogController(content::WebContents* web_contents,
                                 const extensions::Extension& extension,
                                 const base::Closure& on_finish);

  // The title of the dialog view.
  base::string16 GetHeader() const;

  // Explanatory text directly below the title.
  base::string16 GetSubtext() const;

  // Header for unattached devices part of the dialog.
  base::string16 GetUnattachedLocationsHeader() const;

  // Initial state of whether the dialog's confirmation button will be enabled.
  bool IsAcceptAllowed() const;

  // Get the set of permissions to attached galleries.
  virtual GalleryPermissionsVector AttachedPermissions() const;

  // Get the set of permissions to unattached galleries.
  virtual GalleryPermissionsVector UnattachedPermissions() const;

  // Called when the add-folder button in the dialog is clicked.
  virtual void OnAddFolderClicked();

  // A checkbox beside a gallery permission was checked. The full set
  // of gallery permissions checkbox settings is sent on every checkbox toggle.
  virtual void DidToggleGallery(GalleryDialogId gallery_id, bool enabled);

  // The forget command in the context menu was selected.
  virtual void DidForgetGallery(GalleryDialogId gallery_id);

  // The dialog is being deleted.
  virtual void DialogFinished(bool accepted);

  virtual content::WebContents* web_contents();

  ui::MenuModel* GetContextMenu(GalleryDialogId gallery_id);

 protected:
  friend class MediaGalleriesDialogControllerTest;

  typedef base::Callback<MediaGalleriesDialog* (
      MediaGalleriesDialogController*)> CreateDialogCallback;

  // For use with tests.
  MediaGalleriesDialogController(
      const extensions::Extension& extension,
      MediaGalleriesPreferences* preferences,
      const CreateDialogCallback& create_dialog_callback,
      const base::Closure& on_finish);

  virtual ~MediaGalleriesDialogController();

 private:
  // This type keeps track of media galleries already known to the prefs system.
  typedef std::map<GalleryDialogId, GalleryPermission>
      GalleryPermissionsMap;

  class DialogIdMap {
   public:
    DialogIdMap();
    ~DialogIdMap();
    GalleryDialogId GetDialogId(MediaGalleryPrefId pref_id);

   private:
    GalleryDialogId next_dialog_id_;
    std::map<GalleryDialogId, MediaGalleryPrefId> mapping_;
    DISALLOW_COPY_AND_ASSIGN(DialogIdMap);
  };


  // Bottom half of constructor -- called when |preferences_| is initialized.
  void OnPreferencesInitialized();

  // SelectFileDialog::Listener implementation:
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

  // RemovableStorageObserver implementation.
  // Used to keep dialog in sync with removable device status.
  virtual void OnRemovableStorageAttached(
      const storage_monitor::StorageInfo& info) OVERRIDE;
  virtual void OnRemovableStorageDetached(
      const storage_monitor::StorageInfo& info) OVERRIDE;

  // MediaGalleriesPreferences::GalleryChangeObserver implementations.
  // Used to keep the dialog in sync when the preferences change.
  virtual void OnPermissionAdded(MediaGalleriesPreferences* pref,
                                 const std::string& extension_id,
                                 MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnPermissionRemoved(MediaGalleriesPreferences* pref,
                                   const std::string& extension_id,
                                   MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryAdded(MediaGalleriesPreferences* pref,
                              MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryRemoved(MediaGalleriesPreferences* pref,
                                MediaGalleryPrefId pref_id) OVERRIDE;
  virtual void OnGalleryInfoUpdated(MediaGalleriesPreferences* pref,
                                    MediaGalleryPrefId pref_id) OVERRIDE;

  // Populates |known_galleries_| from |preferences_|. Subsequent calls merge
  // into |known_galleries_| and do not change permissions for user toggled
  // galleries.
  void InitializePermissions();

  // Saves state of |known_galleries_|, |new_galleries_| and
  // |forgotten_galleries_| to model.
  //
  // NOTE: possible states for a gallery:
  //   K   N   F   (K = Known, N = New, F = Forgotten)
  // +---+---+---+
  // | Y | N | N |
  // +---+---+---+
  // | N | Y | N |
  // +---+---+---+
  // | Y | N | Y |
  // +---+---+---+
  void SavePermissions();

  // Updates the model and view when |preferences_| changes. Some of the
  // possible changes includes a gallery getting blacklisted, or a new
  // auto detected gallery becoming available.
  void UpdateGalleriesOnPreferencesEvent();

  // Updates the model and view when a device is attached or detached.
  void UpdateGalleriesOnDeviceEvent(const std::string& device_id);

  // Return a sorted vector of either attached or unattached gallery
  // permissions.
  GalleryPermissionsVector FillPermissions(bool attached) const;

  GalleryDialogId GetDialogId(MediaGalleryPrefId pref_id);

  Profile* GetProfile();

  // The web contents from which the request originated.
  content::WebContents* web_contents_;

  // This is just a reference, but it's assumed that it won't become invalid
  // while the dialog is showing.
  const extensions::Extension* extension_;

  // Mapping between pref ids and dialog ids.
  DialogIdMap id_map_;

  // This map excludes those galleries which have been blacklisted; it only
  // counts active known galleries.
  GalleryPermissionsMap known_galleries_;

  // Galleries in |known_galleries_| that the user have toggled.
  std::set<GalleryDialogId> toggled_galleries_;

  // Map of new galleries the user added, but have not saved. This list should
  // never overlap with |known_galleries_|.
  GalleryPermissionsMap new_galleries_;

  // Galleries in |known_galleries_| that the user has forgotten.
  std::set<GalleryDialogId> forgotten_galleries_;

  // Callback to run when the dialog closes.
  base::Closure on_finish_;

  // The model that tracks galleries and extensions' permissions.
  // This is the authoritative source for gallery information.
  MediaGalleriesPreferences* preferences_;

  // The view that's showing.
  scoped_ptr<MediaGalleriesDialog> dialog_;

  scoped_refptr<ui::SelectFileDialog> select_folder_dialog_;

  scoped_ptr<MediaGalleryContextMenu> context_menu_;

  // Creates the dialog. Only changed for unit tests.
  CreateDialogCallback create_dialog_callback_;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesDialogController);
};

#endif  // CHROME_BROWSER_MEDIA_GALLERIES_MEDIA_GALLERIES_DIALOG_CONTROLLER_H_
