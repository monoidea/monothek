/* Monothek - monoidea's monothek
 * Copyright (C) 2018 Joël Krähemann
 *
 * This file is part of Monothek.
 *
 * Monothek is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Monothek is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Monothek.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONOTHEK_AUDIO_FILE_MANAGER_H__
#define __MONOTHEK_AUDIO_FILE_MANAGER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_AUDIO_FILE_MANAGER                (monothek_audio_file_manager_get_type())
#define MONOTHEK_AUDIO_FILE_MANAGER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_AUDIO_FILE_MANAGER, MonothekAudioFileManager))
#define MONOTHEK_AUDIO_FILE_MANAGER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_AUDIO_FILE_MANAGER, MonothekAudioFileManagerClass))
#define MONOTHEK_IS_AUDIO_FILE_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_AUDIO_FILE_MANAGER))
#define MONOTHEK_IS_AUDIO_FILE_MANAGER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_AUDIO_FILE_MANAGER))
#define MONOTHEK_AUDIO_FILE_MANAGER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_AUDIO_FILE_MANAGER, MonothekAudioFileManagerClass))

typedef struct _MonothekAudioFileManager MonothekAudioFileManager;
typedef struct _MonothekAudioFileManagerClass MonothekAudioFileManagerClass;

struct _MonothekAudioFileManager
{
  GObject gobject;
  
  GList *audio_file;
};

struct _MonothekAudioFileManagerClass
{
  GObjectClass gobject;
};

GType monothek_audio_file_manager_get_type();

void monothek_audio_file_manager_add_audio_file(MonothekAudioFileManager *audio_file_manager,
						GObject *audio_file);
void monothek_audio_file_manager_remove_audio_file(MonothekAudioFileManager *audio_file_manager,
						   GObject *audio_file);

GObject* monothek_audio_file_manager_find_audio_file(MonothekAudioFileManager *audio_file_manager,
						     gchar *filename);

void monothek_audio_file_manager_load_playlist(MonothekAudioFileManager *audio_file_manager,
					       GSList *filename);

/* instance */
MonothekAudioFileManager* monothek_audio_file_manager_get_instance();

MonothekAudioFileManager* monothek_audio_file_manager_new();

#endif /*__MONOTHEK_AUDIO_FILE_MANAGER_H__*/
