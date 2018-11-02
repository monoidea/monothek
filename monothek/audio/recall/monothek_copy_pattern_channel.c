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

#include <monothek/audio/recall/monothek_copy_pattern_channel.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_copy_pattern_channel_class_init(MonothekCopyPatternChannelClass *copy_pattern_channel);
void monothek_copy_pattern_channel_init(MonothekCopyPatternChannel *copy_pattern_channel);
void monothek_copy_pattern_channel_set_property(GObject *gobject,
						guint prop_id,
						const GValue *value,
						GParamSpec *param_spec);
void monothek_copy_pattern_channel_get_property(GObject *gobject,
						guint prop_id,
						GValue *value,
						GParamSpec *param_spec);
void monothek_copy_pattern_channel_finalize(GObject *gobject);

/**
 * SECTION:monothek_copy_pattern_channel
 * @short_description: The play wave recall
 * @title: MonothekCopyPatternChannel
 * @section_id:
 * @include: monothek/audio/recall/monothek_copy_pattern_channel.h
 *
 * #MonothekCopyPatternChannel is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_copy_pattern_channel_parent_class = NULL;

GType
monothek_copy_pattern_channel_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_copy_pattern_channel = 0;

    static const GTypeInfo monothek_copy_pattern_channel_info = {
      sizeof (MonothekCopyPatternChannelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_copy_pattern_channel_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCopyPatternChannel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_copy_pattern_channel_init,
    };

    monothek_type_copy_pattern_channel = g_type_register_static(AGS_TYPE_COPY_PATTERN_CHANNEL,
								"MonothekCopyPatternChannel", &monothek_copy_pattern_channel_info,
								0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_copy_pattern_channel);
  }

  return g_define_type_id__volatile;
}

void
monothek_copy_pattern_channel_class_init(MonothekCopyPatternChannelClass *copy_pattern_channel)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_copy_pattern_channel_parent_class = g_type_class_peek_parent(copy_pattern_channel);

  /* GObjectClass */
  gobject = (GObjectClass *) copy_pattern_channel;

  gobject->set_property = monothek_copy_pattern_channel_set_property;
  gobject->get_property = monothek_copy_pattern_channel_get_property;

  gobject->finalize = monothek_copy_pattern_channel_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_copy_pattern_channel_init(MonothekCopyPatternChannel *copy_pattern_channel)
{
}

void
monothek_copy_pattern_channel_set_property(GObject *gobject,
					   guint prop_id,
					   const GValue *value,
					   GParamSpec *param_spec)
{
  MonothekCopyPatternChannel *copy_pattern_channel;

  copy_pattern_channel = MONOTHEK_COPY_PATTERN_CHANNEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_channel_get_property(GObject *gobject,
					   guint prop_id,
					   GValue *value,
					   GParamSpec *param_spec)
{
  MonothekCopyPatternChannel *copy_pattern_channel;

  copy_pattern_channel = MONOTHEK_COPY_PATTERN_CHANNEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_channel_finalize(GObject *gobject)
{
  MonothekCopyPatternChannel *copy_pattern_channel;

  copy_pattern_channel = (MonothekCopyPatternChannel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_copy_pattern_channel_parent_class)->finalize(gobject);
}

/**
 * monothek_copy_pattern_channel_new:
 *
 * Creates an #MonothekCopyPatternChannel
 *
 * Returns: a new #MonothekCopyPatternChannel
 *
 * Since: 1.0.0
 */
MonothekCopyPatternChannel*
monothek_copy_pattern_channel_new()
{
  MonothekCopyPatternChannel *copy_pattern_channel;

  copy_pattern_channel = (MonothekCopyPatternChannel *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL,
								     NULL);
  
  return(copy_pattern_channel);
}
