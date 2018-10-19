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

#ifndef __MONOTHEK_JUKEBOX_END_MODEL_H__
#define __MONOTHEK_JUKEBOX_END_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_JUKEBOX_END_MODEL                (monothek_jukebox_end_model_get_type())
#define MONOTHEK_JUKEBOX_END_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_END_MODEL, MonothekJukeboxEndModel))
#define MONOTHEK_JUKEBOX_END_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_END_MODEL, MonothekJukeboxEndModelClass))
#define MONOTHEK_IS_JUKEBOX_END_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_END_MODEL))
#define MONOTHEK_IS_JUKEBOX_END_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_END_MODEL))
#define MONOTHEK_JUKEBOX_END_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_END_MODEL, MonothekJukeboxEndModelClass))

typedef struct _MonothekJukeboxEndModel MonothekJukeboxEndModel;
typedef struct _MonothekJukeboxEndModelClass MonothekJukeboxEndModelClass;

struct _MonothekJukeboxEndModel
{
  GObject gobject;
};

struct _MonothekJukeboxEndModelClass
{
  GObjectClass gobject;
};

GType monothek_jukebox_end_model_get_type(void);

MonothekJukeboxEndModel* monothek_jukebox_end_model_new();

#endif /*__MONOTHEK_JUKEBOX_END_MODEL_H__*/
