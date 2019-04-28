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

#include <monothek/audio/recall/monothek_delay_audio.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_delay_audio_class_init(MonothekDelayAudioClass *delay_audio);
void monothek_delay_audio_init(MonothekDelayAudio *delay_audio);
void monothek_delay_audio_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec);
void monothek_delay_audio_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec);
void monothek_delay_audio_dispose(GObject *gobject);
void monothek_delay_audio_finalize(GObject *gobject);

/**
 * SECTION:monothek_delay_audio
 * @short_description: The play wave recall
 * @title: MonothekDelayAudio
 * @section_id:
 * @include: monothek/audio/recall/monothek_delay_audio.h
 *
 * #MonothekDelayAudio is a recall.
 */

enum{
  PROP_0,
  PROP_SEQUENCER_PAUSED,
};

static gpointer monothek_delay_audio_parent_class = NULL;

static const gchar *monothek_delay_audio_plugin_name = "monothek-delay";
static const gchar *monothek_delay_audio_specifier[] = {
  "./sequencer_paused[0]",
};

static const gchar *monothek_delay_audio_control_port[] = {
  "1/1",
};

GType
monothek_delay_audio_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_delay_audio = 0;

    static const GTypeInfo monothek_delay_audio_info = {
      sizeof (MonothekDelayAudioClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_delay_audio_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDelayAudio),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_delay_audio_init,
    };

    monothek_type_delay_audio = g_type_register_static(AGS_TYPE_DELAY_AUDIO,
						       "MonothekDelayAudio", &monothek_delay_audio_info,
						       0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_delay_audio);
  }

  return g_define_type_id__volatile;
}

void
monothek_delay_audio_class_init(MonothekDelayAudioClass *delay_audio)
{
  GObjectClass *gobject;

  GParamSpec *param_spec;

  monothek_delay_audio_parent_class = g_type_class_peek_parent(delay_audio);

  /* GObjectClass */
  gobject = (GObjectClass *) delay_audio;

  gobject->set_property = monothek_delay_audio_set_property;
  gobject->get_property = monothek_delay_audio_get_property;

  gobject->dispose = monothek_delay_audio_dispose;
  gobject->finalize = monothek_delay_audio_finalize;

  /* properties */
  /**
   * MonothekDelayAudio:sequencer-paused:
   *
   * The beats per minute.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("sequencer-paused",
				   i18n_pspec("sequencer paused"),
				   i18n_pspec("If sequencer is paused"),
				   AGS_TYPE_PORT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SEQUENCER_PAUSED,
				  param_spec);

  /* recall */
}

void
monothek_delay_audio_init(MonothekDelayAudio *delay_audio)
{
  GList *port;

  port = NULL;

  /* sequencer_paused */
  delay_audio->sequencer_paused = g_object_new(AGS_TYPE_PORT,
					       "plugin-name", monothek_delay_audio_plugin_name,
					       "specifier", monothek_delay_audio_specifier[0],
					       "control-port", monothek_delay_audio_control_port[0],
					       "port-value-is-pointer", FALSE,
					       "port-value-type", G_TYPE_BOOLEAN,
					       "port-value-size", sizeof(gboolean),
					       "port-value-length", 1,
					       NULL);
  g_object_ref(delay_audio->sequencer_paused);
  
  delay_audio->sequencer_paused->port_value.ags_port_boolean = FALSE;

  /* add port */
  port = g_list_prepend(port, delay_audio->sequencer_paused);
  g_object_ref(delay_audio->sequencer_paused);

  /* concat port */
  AGS_RECALL(delay_audio)->port = g_list_concat(AGS_RECALL(delay_audio)->port,
						port)
}

void
monothek_delay_audio_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec)
{
  MonothekDelayAudio *delay_audio;

  pthread_mutex_t *recall_mutex;

  delay_audio = MONOTHEK_DELAY_AUDIO(gobject);

  /* get recall mutex */
  pthread_mutex_lock(ags_recall_get_class_mutex());
  
  recall_mutex = AGS_RECALL(gobject)->obj_mutex;

  pthread_mutex_unlock(ags_recall_get_class_mutex());

  switch(prop_id){
  case PROP_SEQUENCER_PAUSED:
    {
      AgsPort *port;

      port = (AgsPort *) g_value_get_object(value);

      pthread_mutex_lock(recall_mutex);

      if(port == delay_audio->sequencer_paused){
	pthread_mutex_unlock(recall_mutex);

	return;
      }

      if(delay_audio->sequencer_paused != NULL){
	g_object_unref(G_OBJECT(delay_audio->sequencer_paused));
      }
      
      if(port != NULL){
	g_object_ref(G_OBJECT(port));
      }

      delay_audio->sequencer_paused = port;

      pthread_mutex_unlock(recall_mutex);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_delay_audio_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec)
{
  MonothekDelayAudio *delay_audio;

  pthread_mutex_t *recall_mutex;

  delay_audio = MONOTHEK_DELAY_AUDIO(gobject);

  /* get recall mutex */
  pthread_mutex_lock(ags_recall_get_class_mutex());
  
  recall_mutex = AGS_RECALL(gobject)->obj_mutex;

  pthread_mutex_unlock(ags_recall_get_class_mutex());

  switch(prop_id){
  case PROP_SEQUENCER_PAUSED:
    {
      pthread_mutex_lock(recall_mutex);

      g_value_set_object(value, delay_audio->sequencer_paused);

      pthread_mutex_unlock(recall_mutex);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_delay_audio_dispose(GObject *gobject)
{
  MonothekDelayAudio *delay_audio;

  delay_audio = (MonothekDelayAudio *) gobject;

  /* sequencer paused */
  if(delay_audio->sequencer_paused != NULL){
    g_object_unref(G_OBJECT(delay_audio->sequencer_paused));

    delay_audio->sequencer_paused = NULL;
  }

  /* call parent */
  G_OBJECT_CLASS(monothek_delay_audio_parent_class)->dispose(gobject);
}

void
monothek_delay_audio_finalize(GObject *gobject)
{
  MonothekDelayAudio *delay_audio;

  delay_audio = (MonothekDelayAudio *) gobject;

  /* sequencer paused */
  if(delay_audio->sequencer_paused != NULL){
    g_object_unref(G_OBJECT(delay_audio->sequencer_paused));
  }

  /* call parent */
  G_OBJECT_CLASS(monothek_delay_audio_parent_class)->finalize(gobject);
}

/**
 * monothek_delay_audio_new:
 *
 * Creates an #MonothekDelayAudio
 *
 * Returns: a new #MonothekDelayAudio
 *
 * Since: 1.0.0
 */
MonothekDelayAudio*
monothek_delay_audio_new()
{
  MonothekDelayAudio *delay_audio;

  delay_audio = (MonothekDelayAudio *) g_object_new(MONOTHEK_TYPE_DELAY_AUDIO,
						    NULL);
  
  return(delay_audio);
}
