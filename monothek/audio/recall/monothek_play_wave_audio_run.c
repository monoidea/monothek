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

#include <monothek/audio/recall/monothek_play_wave_audio_run.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_play_wave_audio_run_class_init(MonothekPlayWaveAudioRunClass *play_wave_audio_run);
void monothek_play_wave_audio_run_init(MonothekPlayWaveAudioRun *play_wave_audio_run);
void monothek_play_wave_audio_run_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec);
void monothek_play_wave_audio_run_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec);
void monothek_play_wave_audio_run_finalize(GObject *gobject);

/**
 * SECTION:monothek_play_wave_audio_run
 * @short_description: The play wave recall
 * @title: MonothekPlayWaveAudioRun
 * @section_id:
 * @include: monothek/audio/recall/monothek_play_wave_audio_run.h
 *
 * #MonothekPlayWaveAudioRun is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_play_wave_audio_run_parent_class = NULL;

GType
monothek_play_wave_audio_run_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_play_wave_audio_run = 0;

    static const GTypeInfo monothek_play_wave_audio_run_info = {
      sizeof (MonothekPlayWaveAudioRunClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_play_wave_audio_run_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekPlayWaveAudioRun),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_play_wave_audio_run_init,
    };

    monothek_type_play_wave_audio_run = g_type_register_static(AGS_TYPE_PLAY_WAVE_AUDIO_RUN,
							       "MonothekPlayWaveAudioRun", &monothek_play_wave_audio_run_info,
							       0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_play_wave_audio_run);
  }

  return g_define_type_id__volatile;
}

void
monothek_play_wave_audio_run_class_init(MonothekPlayWaveAudioRunClass *play_wave_audio_run)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_play_wave_audio_run_parent_class = g_type_class_peek_parent(play_wave_audio_run);

  /* GObjectClass */
  gobject = (GObjectClass *) play_wave_audio_run;

  gobject->set_property = monothek_play_wave_audio_run_set_property;
  gobject->get_property = monothek_play_wave_audio_run_get_property;

  gobject->finalize = monothek_play_wave_audio_run_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_play_wave_audio_run_init(MonothekPlayWaveAudioRun *play_wave_audio_run)
{
}

void
monothek_play_wave_audio_run_set_property(GObject *gobject,
					  guint prop_id,
					  const GValue *value,
					  GParamSpec *param_spec)
{
  MonothekPlayWaveAudioRun *play_wave_audio_run;

  play_wave_audio_run = MONOTHEK_PLAY_WAVE_AUDIO_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_audio_run_get_property(GObject *gobject,
					  guint prop_id,
					  GValue *value,
					  GParamSpec *param_spec)
{
  MonothekPlayWaveAudioRun *play_wave_audio_run;

  play_wave_audio_run = MONOTHEK_PLAY_WAVE_AUDIO_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_audio_run_finalize(GObject *gobject)
{
  MonothekPlayWaveAudioRun *play_wave_audio_run;

  play_wave_audio_run = (MonothekPlayWaveAudioRun *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_play_wave_audio_run_parent_class)->finalize(gobject);
}

/**
 * monothek_play_wave_audio_run_new:
 *
 * Creates an #MonothekPlayWaveAudioRun
 *
 * Returns: a new #MonothekPlayWaveAudioRun
 *
 * Since: 1.0.0
 */
MonothekPlayWaveAudioRun*
monothek_play_wave_audio_run_new()
{
  MonothekPlayWaveAudioRun *play_wave_audio_run;

  play_wave_audio_run = (MonothekPlayWaveAudioRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO_RUN,
								  NULL);
  
  return(play_wave_audio_run);
}
