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

#include <monothek/audio/recall/monothek_copy_pattern_channel_run.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_copy_pattern_channel_run_class_init(MonothekCopyPatternChannelRunClass *copy_pattern_channel_run);
void monothek_copy_pattern_channel_run_init(MonothekCopyPatternChannelRun *copy_pattern_channel_run);
void monothek_copy_pattern_channel_run_set_property(GObject *gobject,
						    guint prop_id,
						    const GValue *value,
						    GParamSpec *param_spec);
void monothek_copy_pattern_channel_run_get_property(GObject *gobject,
						    guint prop_id,
						    GValue *value,
						    GParamSpec *param_spec);
void monothek_copy_pattern_channel_run_finalize(GObject *gobject);

/**
 * SECTION:monothek_copy_pattern_channel_run
 * @short_description: The play wave recall
 * @title: MonothekCopyPatternChannelRun
 * @section_id:
 * @include: monothek/audio/recall/monothek_copy_pattern_channel_run.h
 *
 * #MonothekCopyPatternChannelRun is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_copy_pattern_channel_run_parent_class = NULL;

GType
monothek_copy_pattern_channel_run_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_copy_pattern_channel_run = 0;

    static const GTypeInfo monothek_copy_pattern_channel_run_info = {
      sizeof (MonothekCopyPatternChannelRunClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_copy_pattern_channel_run_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCopyPatternChannelRun),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_copy_pattern_channel_run_init,
    };

    monothek_type_copy_pattern_channel_run = g_type_register_static(AGS_TYPE_COPY_PATTERN_CHANNEL_RUN,
								    "MonothekCopyPatternChannelRun", &monothek_copy_pattern_channel_run_info,
								    0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_copy_pattern_channel_run);
  }

  return g_define_type_id__volatile;
}

void
monothek_copy_pattern_channel_run_class_init(MonothekCopyPatternChannelRunClass *copy_pattern_channel_run)
{
  GObjectClass *gobject;

  GParamSpec *param_spec;

  monothek_copy_pattern_channel_run_parent_class = g_type_class_peek_parent(copy_pattern_channel_run);

  /* GObjectClass */
  gobject = (GObjectClass *) copy_pattern_channel_run;

  gobject->set_property = monothek_copy_pattern_channel_run_set_property;
  gobject->get_property = monothek_copy_pattern_channel_run_get_property;

  gobject->finalize = monothek_copy_pattern_channel_run_finalize;

  /* properties */

  /* AgsRecallClass */
}

void
monothek_copy_pattern_channel_run_init(MonothekCopyPatternChannelRun *copy_pattern_channel_run)
{
}

void
monothek_copy_pattern_channel_run_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec)
{
  MonothekCopyPatternChannelRun *copy_pattern_channel_run;

  copy_pattern_channel_run = MONOTHEK_COPY_PATTERN_CHANNEL_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_channel_run_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec)
{
  MonothekCopyPatternChannelRun *copy_pattern_channel_run;

  copy_pattern_channel_run = MONOTHEK_COPY_PATTERN_CHANNEL_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_channel_run_finalize(GObject *gobject)
{
  MonothekCopyPatternChannelRun *copy_pattern_channel_run;

  copy_pattern_channel_run = (MonothekCopyPatternChannelRun *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_copy_pattern_channel_run_parent_class)->finalize(gobject);
}

/**
 * monothek_copy_pattern_channel_run_new:
 *
 * Creates an #MonothekCopyPatternChannelRun
 *
 * Returns: a new #MonothekCopyPatternChannelRun
 *
 * Since: 1.0.0
 */
MonothekCopyPatternChannelRun*
monothek_copy_pattern_channel_run_new()
{
  MonothekCopyPatternChannelRun *copy_pattern_channel_run;

  copy_pattern_channel_run = (MonothekCopyPatternChannelRun *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN,
									    NULL);
  
  return(copy_pattern_channel_run);
}
