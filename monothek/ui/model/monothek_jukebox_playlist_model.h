/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2019 Joël Krähemann
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

#ifndef __MONOTHEK_JUKEBOX_PLAYLIST_MODEL_H__
#define __MONOTHEK_JUKEBOX_PLAYLIST_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <time.h>

#define MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL                (monothek_jukebox_playlist_model_get_type())
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL, MonothekJukeboxPlaylistModel))
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL, MonothekJukeboxPlaylistModelClass))
#define MONOTHEK_IS_JUKEBOX_PLAYLIST_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL))
#define MONOTHEK_IS_JUKEBOX_PLAYLIST_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL))
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL, MonothekJukeboxPlaylistModelClass))

#if MONOTHEK_DEVEL_MODE
#if defined(MONOTHEK_W32API)
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME "share\\monothek\\playlist\\jukebox.xml"
#else
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME SRCDIR "/monothek.share/monothek/playlist/jukebox.xml"
#endif
#else
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME "/usr/share/monothek/playlist/jukebox.xml"
#endif

#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT (12)

#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_DEFAULT_SONG_TITEL "FANTASY GIRL"
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_DEFAULT_ARTIST "KLANGTHERAPEUTEN"
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_DEFAULT_ALBUM "FANTASY GIRL / BOOTY CITY"
#define MONOTHEK_JUKEBOX_PLAYLIST_MODEL_DEFAULT_DURATION_SEC (372)

typedef struct _MonothekJukeboxPlaylistModel MonothekJukeboxPlaylistModel;
typedef struct _MonothekJukeboxPlaylistModelClass MonothekJukeboxPlaylistModelClass;

struct _MonothekJukeboxPlaylistModel
{
  GObject gobject;

  gboolean *song_select_active;
 
  gchar **song_filename;
  gchar **cover_filename;

  gchar **song_title;
  gchar **artist;
  gchar **album;

  struct timespec **duration;
};

struct _MonothekJukeboxPlaylistModelClass
{
  GObjectClass gobject;
};

GType monothek_jukebox_playlist_model_get_type(void);

MonothekJukeboxPlaylistModel* monothek_jukebox_playlist_model_new();

#endif /*__MONOTHEK_JUKEBOX_PLAYLIST_MODEL_H__*/
