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

#include <monothek/audio/monothek_rack.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/audio/recall/monothek_copy_pattern_audio.h>
#include <monothek/audio/recall/monothek_copy_pattern_audio_run.h>
#include <monothek/audio/recall/monothek_copy_pattern_channel.h>
#include <monothek/audio/recall/monothek_copy_pattern_channel_run.h>
#include <monothek/audio/recall/monothek_play_wave_audio.h>
#include <monothek/audio/recall/monothek_play_wave_audio_run.h>
#include <monothek/audio/recall/monothek_play_wave_channel.h>
#include <monothek/audio/recall/monothek_play_wave_channel_run.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_rack_class_init(MonothekRackClass *rack);
void monothek_rack_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_rack_init(MonothekRack *rack);
void monothek_rack_set_property(GObject *gobject,
				guint prop_id,
				const GValue *value,
				GParamSpec *param_spec);
void monothek_rack_get_property(GObject *gobject,
				guint prop_id,
				GValue *value,
				GParamSpec *param_spec);
void monothek_rack_finalize(GObject *gobject);

AgsUUID* monothek_rack_get_uuid(AgsConnectable *connectable);
gboolean monothek_rack_has_resource(AgsConnectable *connectable);
gboolean monothek_rack_is_ready(AgsConnectable *connectable);
void monothek_rack_add_to_registry(AgsConnectable *connectable);
void monothek_rack_remove_from_registry(AgsConnectable *connectable);
xmlNode* monothek_rack_list_resource(AgsConnectable *connectable);
xmlNode* monothek_rack_xml_compose(AgsConnectable *connectable);
void monothek_rack_xml_parse(AgsConnectable *connectable,
			  xmlNode *node);
gboolean monothek_rack_is_connected(AgsConnectable *connectable);
void monothek_rack_connect(AgsConnectable *connectable);
void monothek_rack_disconnect(AgsConnectable *connectable);

/**
 * SECTION:monothek_rack
 * @short_description: The rack object
 * @title: MonothekRack
 * @section_id:
 * @include: monothek/rack/monothek_rack.h
 *
 * #MonothekRack creates the audio tree.
 */

enum{
  PROP_0,
  PROP_AUDIO_CHANNELS,
  PROP_OUTPUT_SOUNDCARD,
};

static gpointer monothek_rack_parent_class = NULL;

static pthread_mutex_t monothek_rack_class_mutex = PTHREAD_MUTEX_INITIALIZER;

GType
monothek_rack_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_rack = 0;

    static const GTypeInfo monothek_rack_info = {
      sizeof (MonothekRackClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_rack_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekRack),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_rack_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_rack_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_rack = g_type_register_static(G_TYPE_OBJECT,
						"MonothekRack", &monothek_rack_info,
						0);

    g_type_add_interface_static(monothek_type_rack,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_rack);
  }

  return g_define_type_id__volatile;
}

void
monothek_rack_class_init(MonothekRackClass *rack)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_rack_parent_class = g_type_class_peek_parent(rack);

  /* GObjectClass */
  gobject = (GObjectClass *) rack;

  gobject->set_property = monothek_rack_set_property;
  gobject->get_property = monothek_rack_get_property;

  gobject->finalize = monothek_rack_finalize;

  /* properties */
  /**
   * MonothekRack:audio-channels:
   *
   * The audio channels count.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("audio-channels",
				 i18n_pspec("audio channels count"),
				 i18n_pspec("The count of audio channels of audio"),
				 0,
				 G_MAXUINT32,
				 MONOTHEK_RACK_DEFAULT_AUDIO_CHANNELS,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_AUDIO_CHANNELS,
				  param_spec);

  /**
   * AgsRack:output-soundcard:
   *
   * The assigned soundcard.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("output-soundcard",
				   i18n_pspec("output soundcard"),
				   i18n_pspec("The output soundcard which this rack is packed into"),
				   G_TYPE_OBJECT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_OUTPUT_SOUNDCARD,
				  param_spec);

  /* MonothekRack */
}

void
monothek_rack_connectable_interface_init(AgsConnectableInterface *connectable)
{
  connectable->get_uuid = monothek_rack_get_uuid;
  connectable->has_resource = monothek_rack_has_resource;

  connectable->is_ready = monothek_rack_is_ready;
  connectable->add_to_registry = monothek_rack_add_to_registry;
  connectable->remove_from_registry = monothek_rack_remove_from_registry;

  connectable->list_resource = monothek_rack_list_resource;
  connectable->xml_compose = monothek_rack_xml_compose;
  connectable->xml_parse = monothek_rack_xml_parse;

  connectable->is_connected = monothek_rack_is_connected;  
  connectable->connect = monothek_rack_connect;
  connectable->disconnect = monothek_rack_disconnect;

  connectable->connect_connection = NULL;
  connectable->disconnect_connection = NULL;
}

void
monothek_rack_init(MonothekRack *rack)
{
  pthread_mutex_t *mutex;
  pthread_mutexattr_t *attr;

  rack->flags = 0;

  /* add rack mutex */
  rack->obj_mutexattr = 
    attr = (pthread_mutexattr_t *) malloc(sizeof(pthread_mutexattr_t));
  pthread_mutexattr_init(attr);
  pthread_mutexattr_settype(attr,
			    PTHREAD_MUTEX_RECURSIVE);

#ifdef __linux__
  pthread_mutexattr_setprotocol(attr,
				PTHREAD_PRIO_INHERIT);
#endif

  rack->obj_mutex = 
    mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(mutex,
		     attr);

  /* uuid */
  rack->uuid = ags_uuid_alloc();
  ags_uuid_generate(rack->uuid);

  rack->audio_channels = MONOTHEK_RACK_DEFAULT_AUDIO_CHANNELS;

  rack->output_soundcard = NULL;

  rack->panel = NULL;
  rack->mixer = NULL;
  rack->player = NULL;
  rack->sequencer = NULL;
}

void
monothek_rack_set_property(GObject *gobject,
			   guint prop_id,
			   const GValue *value,
			   GParamSpec *param_spec)
{
  MonothekRack *rack;

  pthread_mutex_t *rack_mutex;

  rack = MONOTHEK_RACK(gobject);

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  switch(prop_id){
  case PROP_AUDIO_CHANNELS:
    {
      guint audio_channels;

      audio_channels = g_value_get_uint(value);

      pthread_mutex_lock(rack_mutex);

      rack->audio_channels = audio_channels;
      
      pthread_mutex_unlock(rack_mutex);
    }
    break;
  case PROP_OUTPUT_SOUNDCARD:
    {
      GObject *output_soundcard;

      output_soundcard = (GObject *) g_value_get_object(value);

      pthread_mutex_lock(rack_mutex);

      if(rack->output_soundcard == output_soundcard){      
	pthread_mutex_unlock(rack_mutex);

	return;
      }

      if(rack->output_soundcard != NULL){
	g_object_unref(rack->output_soundcard);
      }

      if(output_soundcard != NULL){
	g_object_ref(output_soundcard);
      }
      
      rack->output_soundcard = output_soundcard;
      
      pthread_mutex_unlock(rack_mutex);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_rack_get_property(GObject *gobject,
			   guint prop_id,
			   GValue *value,
			   GParamSpec *param_spec)
{
  MonothekRack *rack;

  pthread_mutex_t *rack_mutex;

  rack = MONOTHEK_RACK(gobject);

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  switch(prop_id){
  case PROP_AUDIO_CHANNELS:
    {
      pthread_mutex_lock(rack_mutex);

      g_value_set_uint(value,
		       rack->audio_channels);

      pthread_mutex_unlock(rack_mutex);
    }
    break;
  case PROP_OUTPUT_SOUNDCARD:
    {
      pthread_mutex_lock(rack_mutex);

      g_value_set_object(value, rack->output_soundcard);

      pthread_mutex_unlock(rack_mutex);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_rack_finalize(GObject *gobject)
{
  MonothekRack *rack;

  rack = (MonothekRack *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_rack_parent_class)->finalize(gobject);
}

AgsUUID*
monothek_rack_get_uuid(AgsConnectable *connectable)
{
  MonothekRack *rack;
  
  AgsUUID *ptr;

  pthread_mutex_t *rack_mutex;

  rack = MONOTHEK_RACK(connectable);

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* get UUID */
  pthread_mutex_lock(rack_mutex);

  ptr = rack->uuid;

  pthread_mutex_unlock(rack_mutex);
  
  return(ptr);
}

gboolean
monothek_rack_has_resource(AgsConnectable *connectable)
{
  return(TRUE);
}

gboolean
monothek_rack_is_ready(AgsConnectable *connectable)
{
  MonothekRack *rack;
  
  gboolean is_ready;

  pthread_mutex_t *rack_mutex;

  rack = MONOTHEK_RACK(connectable);

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* check is added */
  pthread_mutex_lock(rack_mutex);

  is_ready = (((MONOTHEK_RACK_ADDED_TO_REGISTRY & (rack->flags)) != 0) ? TRUE: FALSE);

  pthread_mutex_unlock(rack_mutex);
  
  return(is_ready);
}

void
monothek_rack_add_to_registry(AgsConnectable *connectable)
{
  MonothekRack *rack;

  if(ags_connectable_is_ready(connectable)){
    return;
  }
  
  rack = MONOTHEK_RACK(connectable);

  monothek_rack_set_flags(rack, MONOTHEK_RACK_ADDED_TO_REGISTRY);

  //TODO:JK: implement me
}

void
monothek_rack_remove_from_registry(AgsConnectable *connectable)
{
  MonothekRack *rack;

  if(!ags_connectable_is_ready(connectable)){
    return;
  }

  rack = MONOTHEK_RACK(connectable);

  monothek_rack_unset_flags(rack, MONOTHEK_RACK_ADDED_TO_REGISTRY);

  //TODO:JK: implement me
}

xmlNode*
monothek_rack_list_resource(AgsConnectable *connectable)
{
  xmlNode *node;
  
  node = NULL;

  //TODO:JK: implement me
  
  return(node);
}

xmlNode*
monothek_rack_xml_compose(AgsConnectable *connectable)
{
  xmlNode *node;
  
  node = NULL;

  //TODO:JK: implement me
  
  return(node);
}

void
monothek_rack_xml_parse(AgsConnectable *connectable,
		     xmlNode *node)
{
  //TODO:JK: implement me
}

gboolean
monothek_rack_is_connected(AgsConnectable *connectable)
{
  MonothekRack *rack;
  
  gboolean is_connected;

  pthread_mutex_t *rack_mutex;

  rack = MONOTHEK_RACK(connectable);

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* check is connected */
  pthread_mutex_lock(rack_mutex);

  is_connected = (((MONOTHEK_RACK_CONNECTED & (rack->flags)) != 0) ? TRUE: FALSE);
  
  pthread_mutex_unlock(rack_mutex);
  
  return(is_connected);
}

void
monothek_rack_connect(AgsConnectable *connectable)
{
  MonothekRack *rack;

  if(ags_connectable_is_connected(connectable)){
    return;
  }

  rack = MONOTHEK_RACK(connectable);

  monothek_rack_set_flags(rack, MONOTHEK_RACK_CONNECTED);  
}

void
monothek_rack_disconnect(AgsConnectable *connectable)
{
  MonothekRack *rack;

  if(!ags_connectable_is_connected(connectable)){
    return;
  }

  rack = MONOTHEK_RACK(connectable);

  monothek_rack_unset_flags(rack, MONOTHEK_RACK_CONNECTED);    
}

/**
 * monothek_rack_get_class_mutex:
 * 
 * Use this function's returned mutex to access mutex fields.
 *
 * Returns: the class mutex
 * 
 * Since: 1.0.0
 */
pthread_mutex_t*
monothek_rack_get_class_mutex()
{
  return(&monothek_rack_class_mutex);
}

/**
 * monothek_rack_test_flags:
 * @rack: the #MonothekRack
 * @flags: the flags
 *
 * Test @flags to be set on @rack.
 * 
 * Returns: %TRUE if flags are set, else %FALSE
 *
 * Since: 1.0.0
 */
gboolean
monothek_rack_test_flags(MonothekRack *rack, guint flags)
{
  gboolean retval;  
  
  pthread_mutex_t *rack_mutex;

  if(!MONOTHEK_IS_RACK(rack)){
    return(FALSE);
  }

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* test */
  pthread_mutex_lock(rack_mutex);

  retval = ((flags & (rack->flags)) != 0) ? TRUE: FALSE;
  
  pthread_mutex_unlock(rack_mutex);

  return(retval);
}

/**
 * monothek_rack_set_flags:
 * @rack: the #MonothekRack
 * @flags: the flags
 *
 * Set flags.
 * 
 * Since: 1.0.0
 */
void
monothek_rack_set_flags(MonothekRack *rack, guint flags)
{
  pthread_mutex_t *rack_mutex;

  if(!MONOTHEK_IS_RACK(rack)){
    return;
  }

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* set flags */
  pthread_mutex_lock(rack_mutex);

  rack->flags |= flags;

  pthread_mutex_unlock(rack_mutex);
}

/**
 * monothek_rack_unset_flags:
 * @rack: the #MonothekRack
 * @flags: the flags
 *
 * Unset flags.
 * 
 * Since: 1.0.0
 */
void
monothek_rack_unset_flags(MonothekRack *rack, guint flags)
{
  pthread_mutex_t *rack_mutex;

  if(!MONOTHEK_IS_RACK(rack)){
    return;
  }

  /* get rack mutex */
  pthread_mutex_lock(monothek_rack_get_class_mutex());
  
  rack_mutex = rack->obj_mutex;
  
  pthread_mutex_unlock(monothek_rack_get_class_mutex());

  /* set flags */
  pthread_mutex_lock(rack_mutex);

  rack->flags &= (~flags);

  pthread_mutex_unlock(rack_mutex);
}

AgsAudio*
monothek_rack_create_panel(MonothekRack *rack)
{
  AgsAudio *audio;

  guint audio_channels;
  
  if(!MONOTHEK_IS_RACK(rack)){
    return(NULL);
  }

  g_object_get(rack,
	       "audio-channels", &audio_channels,
	       NULL);
  
  audio = ags_audio_new(rack->output_soundcard);
  ags_audio_set_flags(audio, (AGS_AUDIO_SYNC));

  ags_connectable_connect(audio);
  
  ags_audio_set_audio_channels(audio,
			       audio_channels, 0);
  
  ags_audio_set_pads(audio,
		     AGS_TYPE_INPUT,
		     1, 0);
  ags_audio_set_pads(audio,
		     AGS_TYPE_OUTPUT,
		     1, 0);

  /* ags-play */
  ags_recall_factory_create(audio,
			    NULL, NULL,
			    "ags-play-master",
			    0, audio_channels,
			    0, 1,
			    (AGS_RECALL_FACTORY_INPUT,
			     AGS_RECALL_FACTORY_PLAY |
			     AGS_RECALL_FACTORY_ADD),
			    0);
  
  return(audio);
}

AgsAudio*
monothek_rack_create_mixer(MonothekRack *rack)
{
  AgsAudio *audio;

  guint audio_channels;
  
  if(!MONOTHEK_IS_RACK(rack)){
    return(NULL);
  }

  g_object_get(rack,
	       "audio-channels", &audio_channels,
	       NULL);

  audio = ags_audio_new(rack->output_soundcard);
  ags_audio_set_flags(audio, (AGS_AUDIO_ASYNC));
  
  ags_connectable_connect(audio);
  
  ags_audio_set_audio_channels(audio,
			       audio_channels, 0);
  
  ags_audio_set_pads(audio,
		     AGS_TYPE_INPUT,
		     2, 0);
  ags_audio_set_pads(audio,
		     AGS_TYPE_OUTPUT,
		     1, 0);

  return(audio);
}

AgsAudio*
monothek_rack_create_player(MonothekRack *rack)
{
  AgsAudio *audio;

  guint audio_channels;
  
  if(!MONOTHEK_IS_RACK(rack)){
    return(NULL);
  }

  g_object_get(rack,
	       "audio-channels", &audio_channels,
	       NULL);

  audio = ags_audio_new(rack->output_soundcard);
  
  ags_connectable_connect(audio);
  
  ags_audio_set_audio_channels(audio,
			       audio_channels, 0);
  
  ags_audio_set_pads(audio,
		     AGS_TYPE_INPUT,
		     1, 0);
  ags_audio_set_pads(audio,
		     AGS_TYPE_OUTPUT,
		     1, 0);

  /* monothek-play-wave */
  monothek_recall_factory_create(audio,
				 NULL, NULL,
				 "monothek-play-wave",
				 0, audio_channels,
				 0, 1,
				 (AGS_RECALL_FACTORY_OUTPUT |
				  AGS_RECALL_FACTORY_ADD |
				  AGS_RECALL_FACTORY_PLAY),
				 0);

  return(audio);
}

AgsAudio*
monothek_rack_create_sequencer(MonothekRack *rack)
{
  AgsAudio *audio;

  AgsDelayAudio *play_delay_audio;
  AgsDelayAudioRun *play_delay_audio_run;
  AgsCountBeatsAudio *play_count_beats_audio;
  AgsCountBeatsAudioRun *play_count_beats_audio_run;
  MonothekCopyPatternAudio *recall_copy_pattern_audio;
  MonothekCopyPatternAudioRun *recall_copy_pattern_audio_run;

  GList *start_play, *play;
  GList *start_recall, *recall;

  guint audio_channels;

  GValue value = {0,};
  
  if(!MONOTHEK_IS_RACK(rack)){
    return(NULL);
  }

  g_object_get(rack,
	       "audio-channels", &audio_channels,
	       NULL);

  audio = ags_audio_new(rack->output_soundcard);
  
  ags_connectable_connect(audio);
  
  ags_audio_set_audio_channels(audio,
			       audio_channels, 0);
  
  ags_audio_set_pads(audio,
		     AGS_TYPE_INPUT,
		     MONOTHEK_RACK_DEFAULT_SEQUENCER_INPUT_PADS, 0);
  ags_audio_set_pads(audio,
		     AGS_TYPE_OUTPUT,
		     1, 0);

  /* ags-delay */
  ags_recall_factory_create(audio,
			    NULL, NULL,
			    "ags-delay",
			    0, audio_channels,
			    0, 1,
			    (AGS_RECALL_FACTORY_OUTPUT |
			     AGS_RECALL_FACTORY_ADD |
			     AGS_RECALL_FACTORY_PLAY),
			    0);

  g_object_get(audio,
	       "play", &start_play,
	       NULL);

  play = ags_recall_find_type(start_play,
			      AGS_TYPE_DELAY_AUDIO_RUN);

  if(play != NULL){
    play_delay_audio_run = AGS_DELAY_AUDIO_RUN(play->data);
    //    AGS_RECALL(play_delay_audio_run)->flags |= AGS_RECALL_PERSISTENT;
  }else{
    play_delay_audio_run = NULL;
  }

  g_list_free(start_play);
  
  /* ags-count-beats */
  ags_recall_factory_create(audio,
			    NULL, NULL,
			    "ags-count-beats",
			    0, audio_channels,
			    0, 1,
			    (AGS_RECALL_FACTORY_OUTPUT |
			     AGS_RECALL_FACTORY_ADD |
			     AGS_RECALL_FACTORY_PLAY),
			    0);
  
  g_object_get(audio,
	       "play", &start_play,
	       NULL);

  play = ags_recall_find_type(start_play,
			      AGS_TYPE_COUNT_BEATS_AUDIO_RUN);

  if(play != NULL){
    play_count_beats_audio_run = AGS_COUNT_BEATS_AUDIO_RUN(play->data);

    /* set dependency */  
    g_object_set(G_OBJECT(play_count_beats_audio_run),
		 "delay-audio-run", play_delay_audio_run,
		 NULL);
    ags_seekable_seek(AGS_SEEKABLE(play_count_beats_audio_run),
		      0.0,
		      TRUE);

    g_value_init(&value, G_TYPE_BOOLEAN);
    g_value_set_boolean(&value, TRUE);
    ags_port_safe_write(AGS_COUNT_BEATS_AUDIO(AGS_RECALL_AUDIO_RUN(play_count_beats_audio_run)->recall_audio)->notation_loop,
			&value);
  }else{
    play_count_beats_audio_run = NULL;
  }

  g_list_free(start_play);
  
  /* monothek-copy-pattern */
  monothek_recall_factory_create(audio,
				 NULL, NULL,
				 "monothek-copy-pattern",
				 0, audio_channels,
				 0, MONOTHEK_RACK_DEFAULT_SEQUENCER_INPUT_PADS,
				 (AGS_RECALL_FACTORY_INPUT |
				  AGS_RECALL_FACTORY_REMAP |
				  AGS_RECALL_FACTORY_RECALL),
				 0);

  g_object_get(audio,
	       "recall", &start_recall,
	       NULL);

  recall = ags_recall_find_type(start_recall,
				MONOTHEK_TYPE_COPY_PATTERN_AUDIO_RUN);

  if(recall != NULL){
    recall_copy_pattern_audio_run = MONOTHEK_COPY_PATTERN_AUDIO_RUN(recall->data);

    /* set dependency */
    g_object_set(G_OBJECT(recall_copy_pattern_audio_run),
		 "delay-audio-run", play_delay_audio_run,
		 "count-beats-audio-run", play_count_beats_audio_run,
		 NULL);
  }

  g_list_free(start_recall);

  return(audio);
}

void
monothek_rack_setup_tree(MonothekRack *rack)
{
  AgsChannel *output, *input;
  
  guint i;
  
  if(!MONOTHEK_IS_RACK(rack)){
    return;
  }
  
  /* create audio engine */
  rack->panel = monothek_rack_create_panel(rack);
  g_object_ref(rack->panel);
  
  rack->mixer = monothek_rack_create_mixer(rack);
  g_object_ref(rack->mixer);

  rack->player = monothek_rack_create_player(rack);
  g_object_ref(rack->player);

  rack->sequencer = monothek_rack_create_sequencer(rack);
  g_object_ref(rack->sequencer);

  /* link audio - panel and mixer */
  g_object_get(rack->panel,
	       "input", &input,
	       NULL);

  g_object_get(rack->mixer,
	       "output", &output,
	       NULL);
  
  for(i = 0; i < rack->audio_channels; i++){
    GError *error;

    error = NULL;
    ags_channel_set_link(input,
			 output,
			 &error);
    
    /* iterate */
    g_object_get(output,
		 "next", &output,
		 NULL);
    
    g_object_get(input,
		 "next", &input,
		 NULL);
  }
  
  /* link audio - mixer and player */
  g_object_get(rack->mixer,
	       "input", &input,
	       NULL);

  g_object_get(rack->player,
	       "output", &output,
	       NULL);
  
  for(i = 0; i < rack->audio_channels; i++){
    GError *error;

    error = NULL;
    ags_channel_set_link(input,
			 output,
			 &error);
    
    /* iterate */
    g_object_get(output,
		 "next", &output,
		 NULL);
    
    g_object_get(input,
		 "next", &input,
		 NULL);
  }

  /* link audio - mixer and sequencer */
  g_object_get(rack->player,
	       "output", &output,
	       NULL);
  
  for(i = 0; i < rack->audio_channels; i++){
    GError *error;

    error = NULL;
    ags_channel_set_link(input,
			 output,
			 &error);
    
    /* iterate */
    g_object_get(output,
		 "next", &output,
		 NULL);
    
    g_object_get(input,
		 "next", &input,
		 NULL);
  }
}


/**
 * monothek_rack_new:
 * @output_soundcard: the #AgsSoundcard
 *
 * Creates an #MonothekRack
 *
 * Returns: a new #MonothekRack
 *
 * Since: 1.0.0
 */
MonothekRack*
monothek_rack_new(GObject *output_soundcard)
{
  MonothekRack *rack;

  rack = (MonothekRack *) g_object_new(MONOTHEK_TYPE_RACK,
				       "output-soundcard", output_soundcard,
				       NULL);
  
  return(rack);
}
