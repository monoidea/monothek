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

#include <monothek/audio/recall/monothek_play_wave_audio.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_play_wave_audio_class_init(MonothekPlayWaveAudioClass *play_wave_audio);
void monothek_play_wave_audio_init(MonothekPlayWaveAudio *play_wave_audio);
void monothek_play_wave_audio_set_property(GObject *gobject,
					   guint prop_id,
					   const GValue *value,
					   GParamSpec *param_spec);
void monothek_play_wave_audio_get_property(GObject *gobject,
					   guint prop_id,
					   GValue *value,
					   GParamSpec *param_spec);
void monothek_play_wave_audio_finalize(GObject *gobject);

/**
 * SECTION:monothek_play_wave_audio
 * @short_description: The play wave recall
 * @title: MonothekPlayWaveAudio
 * @section_id:
 * @include: monothek/audio/recall/monothek_play_wave_audio.h
 *
 * #MonothekPlayWaveAudio is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_play_wave_audio_parent_class = NULL;

GType
monothek_play_wave_audio_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_play_wave_audio = 0;

    static const GTypeInfo monothek_play_wave_audio_info = {
      sizeof (MonothekPlayWaveAudioClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_play_wave_audio_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekPlayWaveAudio),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_play_wave_audio_init,
    };

    monothek_type_play_wave_audio = g_type_register_static(AGS_TYPE_PLAY_WAVE_AUDIO,
							   "MonothekPlayWaveAudio", &monothek_play_wave_audio_info,
							   0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_play_wave_audio);
  }

  return g_define_type_id__volatile;
}

void
monothek_play_wave_audio_class_init(MonothekPlayWaveAudioClass *play_wave_audio)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_play_wave_audio_parent_class = g_type_class_peek_parent(play_wave_audio);

  /* GObjectClass */
  gobject = (GObjectClass *) play_wave_audio;

  gobject->set_property = monothek_play_wave_audio_set_property;
  gobject->get_property = monothek_play_wave_audio_get_property;

  gobject->finalize = monothek_play_wave_audio_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_play_wave_audio_init(MonothekPlayWaveAudio *play_wave_audio)
{
}

void
monothek_play_wave_audio_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec)
{
  MonothekPlayWaveAudio *play_wave_audio;

  play_wave_audio = MONOTHEK_PLAY_WAVE_AUDIO(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_audio_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec)
{
  MonothekPlayWaveAudio *play_wave_audio;

  play_wave_audio = MONOTHEK_PLAY_WAVE_AUDIO(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_play_wave_audio_finalize(GObject *gobject)
{
  MonothekPlayWaveAudio *play_wave_audio;

  play_wave_audio = (MonothekPlayWaveAudio *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_play_wave_audio_parent_class)->finalize(gobject);
}

/**
 * monothek_play_wave_audio_new:
 *
 * Creates an #MonothekPlayWaveAudio
 *
 * Returns: a new #MonothekPlayWaveAudio
 *
 * Since: 1.0.0
 */
MonothekPlayWaveAudio*
monothek_play_wave_audio_new()
{
  MonothekPlayWaveAudio *play_wave_audio;

  play_wave_audio = (MonothekPlayWaveAudio *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO,
							   NULL);
  
  return(play_wave_audio);
}
