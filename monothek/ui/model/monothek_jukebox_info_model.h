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

#ifndef __MONOTHEK_JUKEBOX_INFO_MODEL_H__
#define __MONOTHEK_JUKEBOX_INFO_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <time.h>

#define MONOTHEK_TYPE_JUKEBOX_INFO_MODEL                (monothek_jukebox_info_model_get_type())
#define MONOTHEK_JUKEBOX_INFO_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_INFO_MODEL, MonothekJukeboxInfoModel))
#define MONOTHEK_JUKEBOX_INFO_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_INFO_MODEL, MonothekJukeboxInfoModelClass))
#define MONOTHEK_IS_JUKEBOX_INFO_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_INFO_MODEL))
#define MONOTHEK_IS_JUKEBOX_INFO_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_INFO_MODEL))
#define MONOTHEK_JUKEBOX_INFO_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_INFO_MODEL, MonothekJukeboxInfoModelClass))

typedef struct _MonothekJukeboxInfoModel MonothekJukeboxInfoModel;
typedef struct _MonothekJukeboxInfoModelClass MonothekJukeboxInfoModelClass;

struct _MonothekJukeboxInfoModel
{
  GObject gobject;

  gchar *song_filename;
  
  gchar *song_title;
  
  gchar *artist;

  gboolean jukebox_ok_active;
};

struct _MonothekJukeboxInfoModelClass
{
  GObjectClass gobject;
};

GType monothek_jukebox_info_model_get_type(void);

void monothek_jukebox_info_model_load_song_filename(MonothekJukeboxInfoModel *jukebox_info_model,
						    gchar *song_filename);

MonothekJukeboxInfoModel* monothek_jukebox_info_model_new();

#endif /*__MONOTHEK_JUKEBOX_INFO_MODEL_H__*/
