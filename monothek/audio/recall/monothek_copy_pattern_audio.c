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

#include <monothek/audio/recall/monothek_copy_pattern_audio.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_copy_pattern_audio_class_init(MonothekCopyPatternAudioClass *copy_pattern_audio);
void monothek_copy_pattern_audio_init(MonothekCopyPatternAudio *copy_pattern_audio);
void monothek_copy_pattern_audio_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_copy_pattern_audio_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_copy_pattern_audio_finalize(GObject *gobject);

/**
 * SECTION:monothek_copy_pattern_audio
 * @short_description: The play wave recall
 * @title: MonothekCopyPatternAudio
 * @section_id:
 * @include: monothek/audio/recall/monothek_copy_pattern_audio.h
 *
 * #MonothekCopyPatternAudio is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_copy_pattern_audio_parent_class = NULL;

GType
monothek_copy_pattern_audio_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_copy_pattern_audio = 0;

    static const GTypeInfo monothek_copy_pattern_audio_info = {
      sizeof (MonothekCopyPatternAudioClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_copy_pattern_audio_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCopyPatternAudio),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_copy_pattern_audio_init,
    };

    monothek_type_copy_pattern_audio = g_type_register_static(AGS_TYPE_COPY_PATTERN_AUDIO,
							      "MonothekCopyPatternAudio", &monothek_copy_pattern_audio_info,
							      0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_copy_pattern_audio);
  }

  return g_define_type_id__volatile;
}

void
monothek_copy_pattern_audio_class_init(MonothekCopyPatternAudioClass *copy_pattern_audio)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_copy_pattern_audio_parent_class = g_type_class_peek_parent(copy_pattern_audio);

  /* GObjectClass */
  gobject = (GObjectClass *) copy_pattern_audio;

  gobject->set_property = monothek_copy_pattern_audio_set_property;
  gobject->get_property = monothek_copy_pattern_audio_get_property;

  gobject->finalize = monothek_copy_pattern_audio_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_copy_pattern_audio_init(MonothekCopyPatternAudio *copy_pattern_audio)
{
}

void
monothek_copy_pattern_audio_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekCopyPatternAudio *copy_pattern_audio;

  copy_pattern_audio = MONOTHEK_COPY_PATTERN_AUDIO(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_audio_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekCopyPatternAudio *copy_pattern_audio;

  copy_pattern_audio = MONOTHEK_COPY_PATTERN_AUDIO(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_copy_pattern_audio_finalize(GObject *gobject)
{
  MonothekCopyPatternAudio *copy_pattern_audio;

  copy_pattern_audio = (MonothekCopyPatternAudio *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_copy_pattern_audio_parent_class)->finalize(gobject);
}

/**
 * monothek_copy_pattern_audio_new:
 *
 * Creates an #MonothekCopyPatternAudio
 *
 * Returns: a new #MonothekCopyPatternAudio
 *
 * Since: 1.0.0
 */
MonothekCopyPatternAudio*
monothek_copy_pattern_audio_new()
{
  MonothekCopyPatternAudio *copy_pattern_audio;

  copy_pattern_audio = (MonothekCopyPatternAudio *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_AUDIO,
								 NULL);
  
  return(copy_pattern_audio);
}
