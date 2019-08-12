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

#ifndef __MONOTHEK_RECALL_FACTORY_H__
#define __MONOTHEK_RECALL_FACTORY_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#define MONOTHEK_TYPE_RECALL_FACTORY                (monothek_recall_factory_get_type())
#define MONOTHEK_RECALL_FACTORY(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_RECALL_FACTORY, MonothekRecallFactory))
#define MONOTHEK_RECALL_FACTORY_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_RECALL_FACTORY, MonothekRecallFactoryClass))
#define MONOTHEK_IS_RECALL_FACTORY(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_RECALL_FACTORY))
#define MONOTHEK_IS_RECALL_FACTORY_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_RECALL_FACTORY))
#define MONOTHEK_RECALL_FACTORY_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_RECALL_FACTORY, MonothekRecallFactoryClass))

typedef struct _MonothekRecallFactory MonothekRecallFactory;
typedef struct _MonothekRecallFactoryClass MonothekRecallFactoryClass;

struct _MonothekRecallFactory
{
  GObject gobject;
};

struct _MonothekRecallFactoryClass
{
  GObjectClass gobject;
};

GType monothek_recall_factory_get_type();

GList* monothek_recall_factory_create(AgsAudio *audio,
				      AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
				      gchar *plugin_name,
				      guint start_audio_channel, guint stop_audio_channel,
				      guint start_pad, guint stop_pad,
				      guint create_flags, guint recall_flags);

/* instance */
MonothekRecallFactory* monothek_recall_factory_new();

#endif /*__MONOTHEK_RECALL_FACTORY_H__*/
