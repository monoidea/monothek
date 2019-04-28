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

#include <monothek/audio/recall/monothek_delay_audio_run.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_delay_audio_run_class_init(MonothekDelayAudioRunClass *delay_audio_run);
void monothek_delay_audio_run_init(MonothekDelayAudioRun *delay_audio_run);
void monothek_delay_audio_run_set_property(GObject *gobject,
					   guint prop_id,
					   const GValue *value,
					   GParamSpec *param_spec);
void monothek_delay_audio_run_get_property(GObject *gobject,
					   guint prop_id,
					   GValue *value,
					   GParamSpec *param_spec);
void monothek_delay_audio_run_finalize(GObject *gobject);

void monothek_delay_audio_run_run_pre(AgsRecall *recall);

/**
 * SECTION:monothek_delay_audio_run
 * @short_description: The play wave recall
 * @title: MonothekDelayAudioRun
 * @section_id:
 * @include: monothek/audio/recall/monothek_delay_audio_run.h
 *
 * #MonothekDelayAudioRun is a recall.
 */

enum{
  PROP_0,
};

static gpointer monothek_delay_audio_run_parent_class = NULL;

GType
monothek_delay_audio_run_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_delay_audio_run = 0;

    static const GTypeInfo monothek_delay_audio_run_info = {
      sizeof (MonothekDelayAudioRunClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_delay_audio_run_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDelayAudioRun),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_delay_audio_run_init,
    };

    monothek_type_delay_audio_run = g_type_register_static(AGS_TYPE_DELAY_AUDIO_RUN,
							   "MonothekDelayAudioRun", &monothek_delay_audio_run_info,
							   0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_delay_audio_run);
  }

  return g_define_type_id__volatile;
}

void
monothek_delay_audio_run_class_init(MonothekDelayAudioRunClass *delay_audio_run)
{
  GObjectClass *gobject;
  AgsRecallClass *recall;

  GParamSpec *param_spec;

  monothek_delay_audio_run_parent_class = g_type_class_peek_parent(delay_audio_run);

  /* GObjectClass */
  gobject = (GObjectClass *) delay_audio_run;

  gobject->set_property = monothek_delay_audio_run_set_property;
  gobject->get_property = monothek_delay_audio_run_get_property;

  gobject->finalize = monothek_delay_audio_run_finalize;

  /* properties */

  /* AgsRecallClass */
  recall = (AgsRecallClass *) delay_audio_run;
  
  recall->run_pre = ags_delay_audio_run_run_pre;
}

void
monothek_delay_audio_run_init(MonothekDelayAudioRun *delay_audio_run)
{
}

void
monothek_delay_audio_run_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec)
{
  MonothekDelayAudioRun *delay_audio_run;

  delay_audio_run = MONOTHEK_DELAY_AUDIO_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_delay_audio_run_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec)
{
  MonothekDelayAudioRun *delay_audio_run;

  delay_audio_run = MONOTHEK_DELAY_AUDIO_RUN(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_delay_audio_run_finalize(GObject *gobject)
{
  MonothekDelayAudioRun *delay_audio_run;

  delay_audio_run = (MonothekDelayAudioRun *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_delay_audio_run_parent_class)->finalize(gobject);
}

void
monothek_delay_audio_run_run_pre(AgsRecall *recall)
{
  MonothekDelayAudio *delay_audio;
  MonothekDelayAudioRun *delay_audio_run;

  AgsPort *port;

  GValue value = {0,};
  
  void (*parent_class_run_pre)(AgsRecall *recall);

  delay_audio_run = MONOTHEK_DELAY_AUDIO_RUN(recall);

  /* get mutex */
  pthread_mutex_lock(ags_recall_get_class_mutex());

  parent_class_run_pre = AGS_RECALL_CLASS(monothek_delay_audio_run_parent_class)->run_pre;

  pthread_mutex_unlock(ags_recall_get_class_mutex());

  /* get delay audio */
  g_object_get(delay_audio_run,
	       "recall-audio", &delay_audio,
	       NULL);

  /* get sequencer paused */
  g_object_get(delay_audio,
	       "sequencer-paused", &port,
	       NULL);

  g_value_init(&value, G_TYPE_BOOLEAN);

  ags_port_safe_read(port, &value);
  
  /* call parent */
  if(!g_value_get_boolean(&value)){
    parent_class_run_pre(recall);
  }
}

/**
 * monothek_delay_audio_run_new:
 *
 * Creates an #MonothekDelayAudioRun
 *
 * Returns: a new #MonothekDelayAudioRun
 *
 * Since: 1.0.0
 */
MonothekDelayAudioRun*
monothek_delay_audio_run_new()
{
  MonothekDelayAudioRun *delay_audio_run;

  delay_audio_run = (MonothekDelayAudioRun *) g_object_new(MONOTHEK_TYPE_DELAY_AUDIO_RUN,
							   NULL);
  
  return(delay_audio_run);
}
