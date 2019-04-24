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

#include <monothek/audio/recall/monothek_play_wave_channel_run.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_play_wave_channel_run_class_init(MonothekPlayWaveChannelRunClass *play_wave_channel_run);
void monothek_play_wave_channel_run_init(MonothekPlayWaveChannelRun *play_wave_channel_run);
void monothek_play_wave_channel_run_set_property(GObject *gobject,
						 guint prop_id,
						 const GValue *value,
						 GParamSpec *param_spec);
void monothek_play_wave_channel_run_get_property(GObject *gobject,
						 guint prop_id,
						 GValue *value,
						 GParamSpec *param_spec);
void monothek_play_wave_channel_run_finalize(GObject *gobject);

/**
 * SECTION:monothek_play_wave_channel_run
 * @short_description: The play wave recall
 * @title: MonothekPlayWaveChannelRun
 * @section_id:
 * @include: monothek/audio/recall/monothek_play_wave_channel_run.h
 *
 * #MonothekPlayWaveChannelRun is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_play_wave_channel_run_parent_class = NULL;

GType
monothek_play_wave_channel_run_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_play_wave_channel_run = 0;

    static const GTypeInfo monothek_play_wave_channel_run_info = {
      sizeof (MonothekPlayWaveChannelRunClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_play_wave_channel_run_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekPlayWaveChannelRun),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_play_wave_channel_run_init,
    };

    monothek_type_play_wave_channel_run = g_type_register_static(AGS_TYPE_PLAY_WAVE_CHANNEL_RUN,
								 "MonothekPlayWaveChannelRun", &monothek_play_wave_channel_run_info,
								 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_play_wave_channel_run);
  }

  return g_define_type_id__volatile;
}

void
monothek_play_wave_channel_run_class_init(MonothekPlayWaveChannelRunClass *play_wave_channel_run)
{
  GObjectClass *gobject;

  GParamSpec *param_spec;

  monothek_play_wave_channel_run_parent_class = g_type_class_peek_parent(play_wave_channel_run);

  /* GObjectClass */
  gobject = (GObjectClass *) play_wave_channel_run;

  gobject->set_property = monothek_play_wave_channel_run_set_property;
  gobject->get_property = monothek_play_wave_channel_run_get_property;

  gobject->finalize = monothek_play_wave_channel_run_finalize;

  /* properties */

  /* AgsRecallClass */
}

void
monothek_play_wave_channel_run_init(MonothekPlayWaveChannelRun *play_wave_channel_run)
{
}

void
monothek_play_wave_channel_run_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec)
{
  MonothekPlayWaveChannelRun *play_wave_channel_run;

  play_wave_channel_run = MONOTHEK_PLAY_WAVE_CHANNEL_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_channel_run_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec)
{
  MonothekPlayWaveChannelRun *play_wave_channel_run;

  play_wave_channel_run = MONOTHEK_PLAY_WAVE_CHANNEL_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_channel_run_finalize(GObject *gobject)
{
  MonothekPlayWaveChannelRun *play_wave_channel_run;

  play_wave_channel_run = (MonothekPlayWaveChannelRun *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_play_wave_channel_run_parent_class)->finalize(gobject);
}

/**
 * monothek_play_wave_channel_run_new:
 *
 * Creates an #MonothekPlayWaveChannelRun
 *
 * Returns: a new #MonothekPlayWaveChannelRun
 *
 * Since: 1.0.0
 */
MonothekPlayWaveChannelRun*
monothek_play_wave_channel_run_new()
{
  MonothekPlayWaveChannelRun *play_wave_channel_run;

  play_wave_channel_run = (MonothekPlayWaveChannelRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_CHANNEL_RUN,
								      NULL);
  
  return(play_wave_channel_run);
}
