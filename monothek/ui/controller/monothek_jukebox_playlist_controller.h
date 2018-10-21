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

#ifndef __MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_H__
#define __MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#define MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER                (monothek_jukebox_playlist_controller_get_type())
#define MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER, MonothekJukeboxPlaylistController))
#define MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER, MonothekJukeboxPlaylistControllerClass))
#define MONOTHEK_IS_JUKEBOX_PLAYLIST_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER))
#define MONOTHEK_IS_JUKEBOX_PLAYLIST_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER))
#define MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER, MonothekJukeboxPlaylistControllerClass))

#define MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_SONG_ROW_COUNT (12)

typedef struct _MonothekJukeboxPlaylistController MonothekJukeboxPlaylistController;
typedef struct _MonothekJukeboxPlaylistControllerClass MonothekJukeboxPlaylistControllerClass;

struct _MonothekJukeboxPlaylistController
{
  MonothekController controller;

  MonothekActionBox **song_select;
};

struct _MonothekJukeboxPlaylistControllerClass
{
  MonothekControllerClass controller;

  void (*select_song)(MonothekJukeboxPlaylistController *jukebox_playlist_controller,
		      gchar *song_filename);
};

GType monothek_jukebox_playlist_controller_get_type(void);

void monothek_jukebox_playlist_controller_select_song(MonothekJukeboxPlaylistController *jukebox_playlist_controller,
						      gchar *song_filename);

MonothekJukeboxPlaylistController* monothek_jukebox_playlist_controller_new();

#endif /*__MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_H__*/
