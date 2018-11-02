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

#include <monothek/audio/monothek_recall_factory.h>

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

void monothek_recall_factory_class_init(MonothekRecallFactoryClass *recall_factory);
void monothek_recall_factory_init(MonothekRecallFactory *recall_factory);
void monothek_recall_factory_finalize(GObject *gobject);

GList* monothek_recall_factory_create_copy_pattern(AgsAudio *audio,
						   AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
						   gchar *plugin_name,
						   guint start_audio_channel, guint stop_audio_channel,
						   guint start_pad, guint stop_pad,
						   guint create_flags, guint recall_flags);
GList* monothek_recall_factory_create_play_wave(AgsAudio *audio,
						AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
						gchar *plugin_name,
						guint start_audio_channel, guint stop_audio_channel,
						guint start_pad, guint stop_pad,
						guint create_flags, guint recall_flags);

/**
 * SECTION:monothek_recall_factory
 * @short_description: The start model object.
 * @title: MonothekRecallFactory
 * @section_id:
 * @include: monothek/audio_file/monothek_recall_factory.h
 *
 * #MonothekRecallFactory is the MVC's start model.
 */

static gpointer monothek_recall_factory_parent_class = NULL;

MonothekRecallFactory *monothek_recall_factory = NULL;

GType
monothek_recall_factory_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_recall_factory = 0;

    static const GTypeInfo monothek_recall_factory_info = {
      sizeof (MonothekRecallFactoryClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_recall_factory_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekRecallFactory),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_recall_factory_init,
    };

    monothek_type_recall_factory = g_type_register_static(G_TYPE_OBJECT,
							  "MonothekRecallFactory", &monothek_recall_factory_info,
							  0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_recall_factory);
  }

  return g_define_type_id__volatile;
}

void
monothek_recall_factory_class_init(MonothekRecallFactoryClass *recall_factory)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_recall_factory_parent_class = g_type_class_peek_parent(recall_factory);

  /* GObjectClass */
  gobject = (GObjectClass *) recall_factory;

  gobject->finalize = monothek_recall_factory_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_recall_factory_init(MonothekRecallFactory *recall_factory)
{
  //empty
}

void
monothek_recall_factory_finalize(GObject *gobject)
{
  //empty

  /* call parent */
  G_OBJECT_CLASS(monothek_recall_factory_parent_class)->finalize(gobject);
}

GList*
monothek_recall_factory_create_copy_pattern(AgsAudio *audio,
					    AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
					    gchar *plugin_name,
					    guint start_audio_channel, guint stop_audio_channel,
					    guint start_pad, guint stop_pad,
					    guint create_flags, guint recall_flags)
{
  MonothekCopyPatternAudio *copy_pattern_audio;
  MonothekCopyPatternAudioRun *copy_pattern_audio_run;
  MonothekCopyPatternChannel *copy_pattern_channel;
  MonothekCopyPatternChannelRun *copy_pattern_channel_run;
  AgsChannel *output, *input;
  AgsChannel *start, *channel;
  AgsPort *port;

  GObject *output_soundcard;

  GList *list_start, *list;
  GList *recall;

  guint audio_channels;
  guint i, j;

  pthread_mutex_t *audio_mutex;
  pthread_mutex_t *channel_mutex;

  if(!AGS_IS_AUDIO(audio)){
    return(NULL);
  }

  /* get audio mutex */
  pthread_mutex_lock(ags_audio_get_class_mutex());

  audio_mutex = audio->obj_mutex;
  
  pthread_mutex_unlock(ags_audio_get_class_mutex());

  /* get some fields */
  pthread_mutex_lock(audio_mutex);

  output_soundcard = audio->output_soundcard;

  output = audio->output;
  input = audio->input;
  
  audio_channels = audio->audio_channels;
  
  pthread_mutex_unlock(audio_mutex);

  /* get channel */
  if((AGS_RECALL_FACTORY_OUTPUT & (create_flags)) != 0){
    start =
      channel = ags_channel_nth(output,
				start_pad * audio_channels);
  }else{
    start =
      channel = ags_channel_nth(input,
				start_pad * audio_channels);
  }

  recall = NULL;

  /* play */
  if((AGS_RECALL_FACTORY_PLAY & (create_flags)) != 0){
    if((AGS_RECALL_FACTORY_REMAP & (create_flags)) == 0 ||
       ags_recall_find_type(audio->play, MONOTHEK_TYPE_COPY_PATTERN_AUDIO) == NULL){
      if(play_container == NULL){
	play_container = ags_recall_container_new();
      }

      play_container->flags |= AGS_RECALL_CONTAINER_PLAY;
      ags_audio_add_recall_container(audio, (GObject *) play_container);

      /* MonothekCopyPatternAudio */
      copy_pattern_audio = (MonothekCopyPatternAudio *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_AUDIO,
								     "output-soundcard", output_soundcard,
								     "audio", audio,
								     "recall-container", play_container,
								     NULL);
      ags_recall_set_flags(copy_pattern_audio,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(copy_pattern_audio,
				   (AGS_SOUND_ABILITY_SEQUENCER));
      ags_recall_set_behaviour_flags(copy_pattern_audio,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) copy_pattern_audio, TRUE);
      recall = g_list_prepend(recall,
			      copy_pattern_audio);

      g_object_set(play_container,
		   "recall-audio", copy_pattern_audio,
		   NULL);

      /* MonothekCopyPatternAudioRun */
      copy_pattern_audio_run = (MonothekCopyPatternAudioRun *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_AUDIO_RUN,
									    "output-soundcard", output_soundcard,
									    "audio", audio,
									    "recall-container", play_container,
									    "recall-audio", copy_pattern_audio,
									    //TODO:JK: add missing dependency "count_beats_audio_run"
									    NULL);
      ags_recall_set_flags(copy_pattern_audio_run,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(copy_pattern_audio_run,
				   (AGS_SOUND_ABILITY_SEQUENCER));
      ags_recall_set_behaviour_flags(copy_pattern_audio_run,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) copy_pattern_audio_run, TRUE);
      recall = g_list_prepend(recall,
			      copy_pattern_audio_run);

      g_object_set(play_container,
		   "recall-audio-run", copy_pattern_audio_run,
		   NULL);
    }else{
      if(play_container == NULL){
	g_object_get(audio,
		     "play", &list_start,
		     NULL);

	list = ags_recall_find_type(list_start,
				    MONOTHEK_TYPE_COPY_PATTERN_AUDIO);
	copy_pattern_audio = AGS_COPY_PATTERN_AUDIO(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				copy_pattern_audio);

	g_object_get(copy_pattern_audio,
		     "recall-container", &play_container,
		     NULL);

	g_object_get(play_container,
		     "recall-audio-run", &list_start,
		     NULL);
	
	list = ags_recall_find_template(list_start);
	copy_pattern_audio_run = AGS_COPY_PATTERN_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				copy_pattern_audio_run);
      }else{
	g_object_get(play_container,
		     "recall-audio", &copy_pattern_audio,
		     NULL);
	recall = g_list_prepend(recall,
				copy_pattern_audio);

	g_object_get(play_container,
		     "recall-audio-run", &list_start,
		     NULL);

	list = ags_recall_find_template(list_start);
	copy_pattern_audio_run = AGS_COPY_PATTERN_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				copy_pattern_audio_run);
      }
    }

    for(i = 0; i < stop_pad - start_pad; i++){
      channel = ags_channel_nth(channel,
				start_audio_channel);
      
      for(j = 0; j < stop_audio_channel - start_audio_channel; j++){
	/* get channel mutex */
	pthread_mutex_lock(ags_channel_get_class_mutex());

	channel_mutex = channel->obj_mutex;
  
	pthread_mutex_unlock(ags_channel_get_class_mutex());

	/* add recall container */
	ags_channel_add_recall_container(channel,
					 (GObject *) play_container);

	/* MonothekCopyPatternChannel in channel->recall */
	copy_pattern_channel = (MonothekCopyPatternChannel *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL,
									   "output-soundcard", output_soundcard,
									   "source", channel,
									   // "destination", destination,
									   "recall-container", play_container,
									   "recall-audio", copy_pattern_audio,
									   // "pattern", channel->pattern->data,
									   NULL);
	ags_recall_set_flags(copy_pattern_channel,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(copy_pattern_channel,
				     (AGS_SOUND_ABILITY_SEQUENCER));
	ags_recall_set_behaviour_flags(copy_pattern_channel,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) copy_pattern_channel, TRUE);
	recall = g_list_prepend(recall,
				copy_pattern_channel);

	g_object_set(play_container,
		     "recall-channel", copy_pattern_channel,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(copy_pattern_channel));

	/* MonothekCopyPatternChannelRun */
	copy_pattern_channel_run = (MonothekCopyPatternChannelRun *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN,
										  "output-soundcard", output_soundcard,
										  "source", channel,
										  // "destination", destination,
										  // "recall_audio_run", copy_pattern_audio_run,
										  "recall-container", play_container,
										  "recall-audio", copy_pattern_audio,
										  "recall-audio-run", copy_pattern_audio_run,
										  "recall-channel", copy_pattern_channel,
										  NULL);
	ags_recall_set_flags(copy_pattern_channel_run,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(copy_pattern_channel_run,
				     (AGS_SOUND_ABILITY_SEQUENCER));
	ags_recall_set_behaviour_flags(copy_pattern_channel_run,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) copy_pattern_channel_run, TRUE);   
	recall = g_list_prepend(recall,
				copy_pattern_channel_run);

	g_object_set(play_container,
		     "recall-channel-run", copy_pattern_channel_run,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(copy_pattern_channel_run));

	/* iterate */
	pthread_mutex_lock(channel_mutex);
	
	channel = channel->next;

	pthread_mutex_unlock(channel_mutex);
      }

      channel = ags_channel_nth(channel,
				audio_channels - stop_audio_channel);
    }
  }

  /* recall */
  if((AGS_RECALL_FACTORY_RECALL & (create_flags)) != 0){
    channel = start;
 
    if((AGS_RECALL_FACTORY_REMAP & (create_flags)) == 0 ||
       ags_recall_find_type(audio->recall, MONOTHEK_TYPE_COPY_PATTERN_AUDIO) == NULL){
      if(recall_container == NULL){
	recall_container = ags_recall_container_new();
      }

      ags_audio_add_recall_container(audio, (GObject *) recall_container);

      /* MonothekCopyPatternAudio */
      copy_pattern_audio = (MonothekCopyPatternAudio *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_AUDIO,
								     "output-soundcard", output_soundcard,
								     "audio", audio,
								     "recall-container", recall_container,
								     NULL);
      ags_recall_set_flags(copy_pattern_audio,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(copy_pattern_audio,
				   (AGS_SOUND_ABILITY_SEQUENCER));
      ags_recall_set_behaviour_flags(copy_pattern_audio,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) copy_pattern_audio, FALSE);
      recall = g_list_prepend(recall,
			      copy_pattern_audio);

      g_object_set(recall_container,
		   "recall-audio", copy_pattern_audio,
		   NULL);
      
      /* MonothekCopyPatternAudioRun */
      copy_pattern_audio_run = (MonothekCopyPatternAudioRun *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_AUDIO_RUN,
									    "output-soundcard", output_soundcard,
									    "recall-container", recall_container,
									    "recall-audio", copy_pattern_audio,
									    //TODO:JK: add missing dependency "count_beats_audio_run"
									    NULL);
      ags_recall_set_flags(copy_pattern_audio_run,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(copy_pattern_audio_run,
				   (AGS_SOUND_ABILITY_SEQUENCER));
      ags_recall_set_behaviour_flags(copy_pattern_audio_run,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) copy_pattern_audio_run, FALSE);
      recall = g_list_prepend(recall,
			      copy_pattern_audio_run);

      g_object_set(recall_container,
		   "recall-audio-run", copy_pattern_audio_run,
		   NULL);
    }else{
      if(recall_container == NULL){	
	g_object_get(audio,
		     "recall", &list_start,
		     NULL);

	list = ags_recall_find_type(list_start,
				    MONOTHEK_TYPE_COPY_PATTERN_AUDIO);
	copy_pattern_audio = AGS_COPY_PATTERN_AUDIO(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				copy_pattern_audio);

	g_object_get(copy_pattern_audio,
		     "recall-container", &recall_container,
		     NULL);

	g_object_get(recall_container,
		     "recall-audio-run", &list_start,
		     NULL);
	
	list = ags_recall_find_template(list_start);
	copy_pattern_audio_run = AGS_COPY_PATTERN_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				copy_pattern_audio_run);
      }else{
	g_object_get(recall_container,
		     "recall-audio", &copy_pattern_audio,
		     NULL);
	recall = g_list_prepend(recall,
				copy_pattern_audio);

	g_object_get(recall_container,
		     "recall-audio-run", &list_start,
		     NULL);

	list = ags_recall_template_find_type(list_start,
					     MONOTHEK_TYPE_COPY_PATTERN_AUDIO_RUN);
	copy_pattern_audio_run = list->data;
	recall = g_list_prepend(recall,
				copy_pattern_audio_run);

	g_list_free(list_start);
      }
    }

    for(i = 0; i < stop_pad - start_pad; i++){
      channel = ags_channel_nth(channel,
				start_audio_channel);
      
      for(j = 0; j < stop_audio_channel - start_audio_channel; j++){
	/* get channel mutex */
	pthread_mutex_lock(ags_channel_get_class_mutex());

	channel_mutex = channel->obj_mutex;
  
	pthread_mutex_unlock(ags_channel_get_class_mutex());

	/* add recall container */
	ags_channel_add_recall_container(channel,
					 (GObject *) recall_container);

	/* MonothekCopyPatternChannel in channel->recall */
	copy_pattern_channel = (MonothekCopyPatternChannel *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL,
									   "output-soundcard", output_soundcard,
									   "source", channel,
									   // "destination", destination,
									   "recall-container", recall_container,
									   "recall-audio", copy_pattern_audio,
									   //"pattern", channel->pattern->data,
									   NULL);
	ags_recall_set_flags(copy_pattern_channel,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(copy_pattern_channel,
				     (AGS_SOUND_ABILITY_SEQUENCER));
	ags_recall_set_behaviour_flags(copy_pattern_channel,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) copy_pattern_channel, FALSE);
	recall = g_list_prepend(recall,
				copy_pattern_channel);

	g_object_set(recall_container,
		     "recall-channel", copy_pattern_channel,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(copy_pattern_channel));

	/* MonothekCopyPatternChannelRun */
	copy_pattern_channel_run = (MonothekCopyPatternChannelRun *) g_object_new(MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN,
										  "output-soundcard", output_soundcard,
										  "source", channel,
										  // "destination", destination,
										  // "recall_audio_run", copy_pattern_audio_run,
										  "recall-container", recall_container,
										  "recall-audio", copy_pattern_audio,
										  "recall-audio-run", copy_pattern_audio_run,
										  "recall-channel", copy_pattern_channel,
										  NULL);
	ags_recall_set_flags(copy_pattern_channel_run,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(copy_pattern_channel_run,
				     (AGS_SOUND_ABILITY_SEQUENCER));
	ags_recall_set_behaviour_flags(copy_pattern_channel_run,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) copy_pattern_channel_run, FALSE);   
	recall = g_list_prepend(recall,
				copy_pattern_channel_run);

	g_object_set(recall_container,
		     "recall-channel-run", copy_pattern_channel_run,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(copy_pattern_channel_run));

	/* iterate */
	pthread_mutex_lock(channel_mutex);
	
	channel = channel->next;

	pthread_mutex_unlock(channel_mutex);
      }

      channel = ags_channel_nth(channel,
				audio_channels - stop_audio_channel);
    }
  }

  /* return instantiated recall */
  recall = g_list_reverse(recall);

  return(recall);
}

GList*
monothek_recall_factory_create_play_wave(AgsAudio *audio,
					 AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
					 gchar *plugin_name,
					 guint start_audio_channel, guint stop_audio_channel,
					 guint start_pad, guint stop_pad,
					 guint create_flags, guint recall_flags)
{
  MonothekPlayWaveAudio *play_wave_audio;
  MonothekPlayWaveAudioRun *play_wave_audio_run;
  MonothekPlayWaveChannel *play_wave_channel;
  MonothekPlayWaveChannelRun *play_wave_channel_run;
  AgsChannel *output, *input;
  AgsChannel *start, *channel;
  AgsPort *port;

  GObject *output_soundcard;

  GList *list_start, *list;
  GList *recall;

  guint audio_channels;
  guint i, j;

  pthread_mutex_t *audio_mutex;
  pthread_mutex_t *channel_mutex;

  if(!AGS_IS_AUDIO(audio)){
    return(NULL);
  }

  /* get audio mutex */
  pthread_mutex_lock(ags_audio_get_class_mutex());

  audio_mutex = audio->obj_mutex;
  
  pthread_mutex_unlock(ags_audio_get_class_mutex());

  /* get some fields */
  pthread_mutex_lock(audio_mutex);

  output_soundcard = audio->output_soundcard;

  output = audio->output;
  input = audio->input;
  
  audio_channels = audio->audio_channels;
  
  pthread_mutex_unlock(audio_mutex);

  /* get channel */
  if((AGS_RECALL_FACTORY_OUTPUT & (create_flags)) != 0){
    start =
      channel = ags_channel_nth(output,
				start_pad * audio_channels);
  }else{
    start =
      channel = ags_channel_nth(input,
				start_pad * audio_channels);
  }

  recall = NULL;

  /* play */
  if((AGS_RECALL_FACTORY_PLAY & (create_flags)) != 0){
    if((AGS_RECALL_FACTORY_REMAP & (create_flags)) == 0 ||
       ags_recall_find_type(audio->play, MONOTHEK_TYPE_PLAY_WAVE_AUDIO) == NULL){
      if(play_container == NULL){
	play_container = ags_recall_container_new();
      }

      play_container->flags |= AGS_RECALL_CONTAINER_PLAY;
      ags_audio_add_recall_container(audio, (GObject *) play_container);

      /* MonothekPlayWaveAudio */
      play_wave_audio = (MonothekPlayWaveAudio *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO,
							       "output-soundcard", output_soundcard,
							       "audio", audio,
							       "recall-container", play_container,
							       NULL);
      ags_recall_set_flags(play_wave_audio,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(play_wave_audio,
				   (AGS_SOUND_ABILITY_WAVE));
      ags_recall_set_behaviour_flags(play_wave_audio,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) play_wave_audio, TRUE);
      recall = g_list_prepend(recall,
			      play_wave_audio);

      g_object_set(play_container,
		   "recall-audio", play_wave_audio,
		   NULL);

      /* MonothekPlayWaveAudioRun */
      play_wave_audio_run = (MonothekPlayWaveAudioRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO_RUN,
								      "output-soundcard", output_soundcard,
								      "audio", audio,
								      "recall-container", play_container,
								      "recall-audio", play_wave_audio,
								      //TODO:JK: add missing dependency "count_beats_audio_run"
								      NULL);
      ags_recall_set_flags(play_wave_audio_run,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(play_wave_audio_run,
				   (AGS_SOUND_ABILITY_WAVE));
      ags_recall_set_behaviour_flags(play_wave_audio_run,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) play_wave_audio_run, TRUE);
      recall = g_list_prepend(recall,
			      play_wave_audio_run);

      g_object_set(play_container,
		   "recall-audio-run", play_wave_audio_run,
		   NULL);
    }else{
      if(play_container == NULL){
	g_object_get(audio,
		     "play", &list_start,
		     NULL);

	list = ags_recall_find_type(list_start,
				    MONOTHEK_TYPE_PLAY_WAVE_AUDIO);
	play_wave_audio = AGS_PLAY_WAVE_AUDIO(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				play_wave_audio);

	g_object_get(play_wave_audio,
		     "recall-container", &play_container,
		     NULL);

	g_object_get(play_container,
		     "recall-audio-run", &list_start,
		     NULL);
	
	list = ags_recall_find_template(list_start);
	play_wave_audio_run = AGS_PLAY_WAVE_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				play_wave_audio_run);
      }else{
	g_object_get(play_container,
		     "recall-audio", &play_wave_audio,
		     NULL);
	recall = g_list_prepend(recall,
				play_wave_audio);

	g_object_get(play_container,
		     "recall-audio-run", &list_start,
		     NULL);

	list = ags_recall_find_template(list_start);
	play_wave_audio_run = AGS_PLAY_WAVE_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				play_wave_audio_run);
      }
    }

    for(i = 0; i < stop_pad - start_pad; i++){
      channel = ags_channel_nth(channel,
				start_audio_channel);
      
      for(j = 0; j < stop_audio_channel - start_audio_channel; j++){
	/* get channel mutex */
	pthread_mutex_lock(ags_channel_get_class_mutex());

	channel_mutex = channel->obj_mutex;
  
	pthread_mutex_unlock(ags_channel_get_class_mutex());

	/* add recall container */
	ags_channel_add_recall_container(channel,
					 (GObject *) play_container);

	/* MonothekPlayWaveChannel in channel->recall */
	play_wave_channel = (MonothekPlayWaveChannel *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_CHANNEL,
								     "output-soundcard", output_soundcard,
								     "source", channel,
								     // "destination", destination,
								     "recall-container", play_container,
								     "recall-audio", play_wave_audio,
								     // "pattern", channel->pattern->data,
								     NULL);
	ags_recall_set_flags(play_wave_channel,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(play_wave_channel,
				     (AGS_SOUND_ABILITY_WAVE));
	ags_recall_set_behaviour_flags(play_wave_channel,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) play_wave_channel, TRUE);
	recall = g_list_prepend(recall,
				play_wave_channel);

	g_object_set(play_container,
		     "recall-channel", play_wave_channel,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(play_wave_channel));

	/* MonothekPlayWaveChannelRun */
	play_wave_channel_run = (MonothekPlayWaveChannelRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_CHANNEL_RUN,
									    "output-soundcard", output_soundcard,
									    "source", channel,
									    // "destination", destination,
									    // "recall_audio_run", play_wave_audio_run,
									    "recall-container", play_container,
									    "recall-audio", play_wave_audio,
									    "recall-audio-run", play_wave_audio_run,
									    "recall-channel", play_wave_channel,
									    NULL);
	ags_recall_set_flags(play_wave_channel_run,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(play_wave_channel_run,
				     (AGS_SOUND_ABILITY_WAVE));
	ags_recall_set_behaviour_flags(play_wave_channel_run,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) play_wave_channel_run, TRUE);   
	recall = g_list_prepend(recall,
				play_wave_channel_run);

	g_object_set(play_container,
		     "recall-channel-run", play_wave_channel_run,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(play_wave_channel_run));

	/* iterate */
	pthread_mutex_lock(channel_mutex);
	
	channel = channel->next;

	pthread_mutex_unlock(channel_mutex);
      }

      channel = ags_channel_nth(channel,
				audio_channels - stop_audio_channel);
    }
  }

  /* recall */
  if((AGS_RECALL_FACTORY_RECALL & (create_flags)) != 0){
    channel = start;
 
    if((AGS_RECALL_FACTORY_REMAP & (create_flags)) == 0 ||
       ags_recall_find_type(audio->recall, MONOTHEK_TYPE_PLAY_WAVE_AUDIO) == NULL){
      if(recall_container == NULL){
	recall_container = ags_recall_container_new();
      }

      ags_audio_add_recall_container(audio, (GObject *) recall_container);

      /* MonothekPlayWaveAudio */
      play_wave_audio = (MonothekPlayWaveAudio *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO,
							       "output-soundcard", output_soundcard,
							       "audio", audio,
							       "recall-container", recall_container,
							       NULL);
      ags_recall_set_flags(play_wave_audio,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(play_wave_audio,
				   (AGS_SOUND_ABILITY_WAVE));
      ags_recall_set_behaviour_flags(play_wave_audio,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) play_wave_audio, FALSE);
      recall = g_list_prepend(recall,
			      play_wave_audio);

      g_object_set(recall_container,
		   "recall-audio", play_wave_audio,
		   NULL);
      
      /* MonothekPlayWaveAudioRun */
      play_wave_audio_run = (MonothekPlayWaveAudioRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_AUDIO_RUN,
								      "output-soundcard", output_soundcard,
								      "recall-container", recall_container,
								      "recall-audio", play_wave_audio,
								      //TODO:JK: add missing dependency "count_beats_audio_run"
								      NULL);
      ags_recall_set_flags(play_wave_audio_run,
			   (AGS_RECALL_TEMPLATE));
      ags_recall_set_ability_flags(play_wave_audio_run,
				   (AGS_SOUND_ABILITY_WAVE));
      ags_recall_set_behaviour_flags(play_wave_audio_run,
				     (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

      ags_audio_add_recall(audio, (GObject *) play_wave_audio_run, FALSE);
      recall = g_list_prepend(recall,
			      play_wave_audio_run);

      g_object_set(recall_container,
		   "recall-audio-run", play_wave_audio_run,
		   NULL);
    }else{
      if(recall_container == NULL){	
	g_object_get(audio,
		     "recall", &list_start,
		     NULL);

	list = ags_recall_find_type(list_start,
				    MONOTHEK_TYPE_PLAY_WAVE_AUDIO);
	play_wave_audio = AGS_PLAY_WAVE_AUDIO(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				play_wave_audio);

	g_object_get(play_wave_audio,
		     "recall-container", &recall_container,
		     NULL);

	g_object_get(recall_container,
		     "recall-audio-run", &list_start,
		     NULL);
	
	list = ags_recall_find_template(list_start);
	play_wave_audio_run = AGS_PLAY_WAVE_AUDIO_RUN(list->data);
	g_list_free(list_start);
	
	recall = g_list_prepend(recall,
				play_wave_audio_run);
      }else{
	g_object_get(recall_container,
		     "recall-audio", &play_wave_audio,
		     NULL);
	recall = g_list_prepend(recall,
				play_wave_audio);

	g_object_get(recall_container,
		     "recall-audio-run", &list_start,
		     NULL);

	list = ags_recall_template_find_type(list_start,
					     MONOTHEK_TYPE_PLAY_WAVE_AUDIO_RUN);
	play_wave_audio_run = list->data;
	recall = g_list_prepend(recall,
				play_wave_audio_run);

	g_list_free(list_start);
      }
    }

    for(i = 0; i < stop_pad - start_pad; i++){
      channel = ags_channel_nth(channel,
				start_audio_channel);
      
      for(j = 0; j < stop_audio_channel - start_audio_channel; j++){
	/* get channel mutex */
	pthread_mutex_lock(ags_channel_get_class_mutex());

	channel_mutex = channel->obj_mutex;
  
	pthread_mutex_unlock(ags_channel_get_class_mutex());

	/* add recall container */
	ags_channel_add_recall_container(channel,
					 (GObject *) recall_container);
	
	/* MonothekPlayWaveChannel in channel->recall */
	play_wave_channel = (MonothekPlayWaveChannel *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_CHANNEL,
								     "output-soundcard", output_soundcard,
								     "source", channel,
								     // "destination", destination,
								     "recall-container", recall_container,
								     "recall-audio", play_wave_audio,
								     //"pattern", channel->pattern->data,
								     NULL);
	ags_recall_set_flags(play_wave_channel,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(play_wave_channel,
				     (AGS_SOUND_ABILITY_WAVE));
	ags_recall_set_behaviour_flags(play_wave_channel,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) play_wave_channel, FALSE);
	recall = g_list_prepend(recall,
				play_wave_channel);

	g_object_set(recall_container,
		     "recall-channel", play_wave_channel,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(play_wave_channel));

	/* MonothekPlayWaveChannelRun */
	play_wave_channel_run = (MonothekPlayWaveChannelRun *) g_object_new(MONOTHEK_TYPE_PLAY_WAVE_CHANNEL_RUN,
									    "output-soundcard", output_soundcard,
									    "source", channel,
									    // "destination", destination,
									    // "recall_audio_run", play_wave_audio_run,
									    "recall-container", recall_container,
									    "recall-audio", play_wave_audio,
									    "recall-audio-run", play_wave_audio_run,
									    "recall-channel", play_wave_channel,
									    NULL);
	ags_recall_set_flags(play_wave_channel_run,
			     (AGS_RECALL_TEMPLATE));
	ags_recall_set_ability_flags(play_wave_channel_run,
				     (AGS_SOUND_ABILITY_WAVE));
	ags_recall_set_behaviour_flags(play_wave_channel_run,
				       (((AGS_RECALL_FACTORY_OUTPUT & create_flags) != 0) ? AGS_SOUND_BEHAVIOUR_CHAINED_TO_OUTPUT: AGS_SOUND_BEHAVIOUR_CHAINED_TO_INPUT));

	ags_channel_add_recall(channel, (GObject *) play_wave_channel_run, FALSE);   
	recall = g_list_prepend(recall,
				play_wave_channel_run);

	g_object_set(recall_container,
		     "recall-channel-run", play_wave_channel_run,
		     NULL);
	ags_connectable_connect(AGS_CONNECTABLE(play_wave_channel_run));

	/* iterate */
	pthread_mutex_lock(channel_mutex);
	
	channel = channel->next;

	pthread_mutex_unlock(channel_mutex);
      }

      channel = ags_channel_nth(channel,
				audio_channels - stop_audio_channel);
    }
  }

  /* return instantiated recall */
  recall = g_list_reverse(recall);

  return(recall);
}

/**
 * monothek_recall_factory_create:
 * @audio: an #AgsAudio that should keep the recalls
 * @play_container: an #AgsRecallContainer to indetify what recall to use
 * @recall_container: an #AgsRecallContainer to indetify what recall to use
 * @plugin_name: the plugin identifier to instantiate 
 * @start_audio_channel: the first audio channel to apply
 * @stop_audio_channel: the last audio channel to apply
 * @start_pad: the first pad to apply
 * @stop_pad: the last pad to apply
 * @create_flags: modify the behaviour of this function
 * @recall_flags: flags to be set for #AgsRecall
 *
 * Instantiate #AgsRecall by this factory.
 *
 * Returns: The available AgsPort objects of the plugin to modify.
 * 
 * Since: 1.0.0
 */
GList*
monothek_recall_factory_create(AgsAudio *audio,
			       AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
			       gchar *plugin_name,
			       guint start_audio_channel, guint stop_audio_channel,
			       guint start_pad, guint stop_pad,
			       guint create_flags, guint recall_flags)
{
  GList *recall;
  
  recall = NULL;

#ifdef AGS_DEBUG
  g_message("MonothekRecallFactory creating: %s[%d,%d]", plugin_name, stop_pad, stop_audio_channel);
#endif

  if(!strncmp(plugin_name,
	      "monothek-copy-pattern",
	      22)){
    recall = monothek_recall_factory_create_copy_pattern(audio,
							 play_container, recall_container,
							 plugin_name,
							 start_audio_channel, stop_audio_channel,
							 start_pad, stop_pad,
							 create_flags, recall_flags);
  }else if(!strncmp(plugin_name,
		    "monothek-play-wave",
		    19)){
    recall = monothek_recall_factory_create_play_wave(audio,
						      play_container, recall_container,
						      plugin_name,
						      start_audio_channel, stop_audio_channel,
						      start_pad, stop_pad,
						      create_flags, recall_flags);
  }
  
  return(recall);
}

/**
 * monothek_recall_factory_new:
 *
 * Creates an #MonothekRecallFactory
 *
 * Returns: a new #MonothekRecallFactory
 *
 * Since: 1.0.0
 */
MonothekRecallFactory*
monothek_recall_factory_new()
{
  MonothekRecallFactory *recall_factory;

  recall_factory = (MonothekRecallFactory *) g_object_new(MONOTHEK_TYPE_RECALL_FACTORY,
							  NULL);
  
  return(recall_factory);
}
