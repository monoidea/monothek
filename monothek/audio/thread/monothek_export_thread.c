/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2020 Joël Krähemann
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

#include <monothek/audio/thread/monothek_export_thread.h>

#include <stdlib.h>

#include <sys/time.h>
#include <time.h>

#include <monothek/i18n.h>

void monothek_export_thread_class_init(MonothekExportThreadClass *export_thread);
void monothek_export_thread_init(MonothekExportThread *export_thread);
void monothek_export_thread_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec);
void monothek_export_thread_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec);
void monothek_export_thread_finalize(GObject *gobject);

void monothek_export_thread_run(AgsThread *thread);

enum{
  PROP_0,
  PROP_START_TIME,
  PROP_DURATION,
};

static gpointer monothek_export_thread_parent_class = NULL;

GType
monothek_export_thread_get_type()
{
  static GType monothek_type_export_thread = 0;

  if(!monothek_type_export_thread){
    static const GTypeInfo monothek_export_thread_info = {
      sizeof (MonothekExportThreadClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_export_thread_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekExportThread),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_export_thread_init,
    };

    monothek_type_export_thread = g_type_register_static(AGS_TYPE_EXPORT_THREAD,
							 "MonothekExportThread",
							 &monothek_export_thread_info,
							 0);
  }
  
  return (monothek_type_export_thread);
}

void
monothek_export_thread_class_init(MonothekExportThreadClass *export_thread)
{
  GObjectClass *gobject;
  AgsThreadClass *thread;
  GParamSpec *param_spec;

  monothek_export_thread_parent_class = g_type_class_peek_parent(export_thread);

  /* GObject */
  gobject = (GObjectClass *) export_thread;

  gobject->get_property = monothek_export_thread_get_property;
  gobject->set_property = monothek_export_thread_set_property;

  gobject->finalize = monothek_export_thread_finalize;

  /* properties */
  /**
   * MonothekExportThread:start-time:
   *
   * The start time of the export.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("start-time",
				    i18n_pspec("start time"),
				    i18n_pspec("The start time of export"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_START_TIME,
				  param_spec);

  /**
   * MonothekExportThread:duration:
   *
   * Duration of export thread to run.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("duration",
				    i18n_pspec("duration"),
				    i18n_pspec("The duration to export"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_DURATION,
				  param_spec);

  /* AgsThread */
  thread = (AgsThreadClass *) export_thread;

  thread->run = monothek_export_thread_run;
}


void
monothek_export_thread_init(MonothekExportThread *export_thread)
{
  export_thread->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  export_thread->start_time->tv_sec = 0;
  export_thread->start_time->tv_nsec = 0;

  export_thread->duration = (struct timespec *) malloc(sizeof(struct timespec));
  export_thread->duration->tv_sec = 0;
  export_thread->duration->tv_nsec = 0;
}

void
monothek_export_thread_set_property(GObject *gobject,
				    guint prop_id,
				    const GValue *value,
				    GParamSpec *param_spec)
{
  MonothekExportThread *export_thread;

  GRecMutex *thread_mutex;

  export_thread = MONOTHEK_EXPORT_THREAD(gobject);

  /* get thread mutex */
  thread_mutex = AGS_THREAD_GET_OBJ_MUTEX(gobject);

  switch(prop_id){
  case PROP_START_TIME:
  {
    struct timespec *start_time;
    
    start_time = g_value_get_pointer(value);

    g_rec_mutex_lock(thread_mutex);
    
    export_thread->start_time->tv_sec = start_time->tv_sec;
    export_thread->start_time->tv_nsec = start_time->tv_nsec;

    g_rec_mutex_unlock(thread_mutex);
  }
  break;
  case PROP_DURATION:
  {
    struct timespec *duration;
    
    duration = g_value_get_pointer(value);

    g_rec_mutex_lock(thread_mutex);

    export_thread->duration->tv_sec = duration->tv_sec;
    export_thread->duration->tv_nsec = duration->tv_nsec;

    g_rec_mutex_unlock(thread_mutex);
  }
  break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_export_thread_get_property(GObject *gobject,
				    guint prop_id,
				    GValue *value,
				    GParamSpec *param_spec)
{
  MonothekExportThread *export_thread;

  GRecMutex *thread_mutex;

  export_thread = MONOTHEK_EXPORT_THREAD(gobject);

  /* get thread mutex */
  thread_mutex = AGS_THREAD_GET_OBJ_MUTEX(gobject);

  switch(prop_id){
  case PROP_START_TIME:
  {
    g_rec_mutex_lock(thread_mutex);

    g_value_set_pointer(value, export_thread->start_time);

    g_rec_mutex_unlock(thread_mutex);
  }
  break;
  case PROP_DURATION:
  {
    g_rec_mutex_lock(thread_mutex);

    g_value_set_pointer(value, export_thread->duration);

    g_rec_mutex_unlock(thread_mutex);
  }
  break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_export_thread_finalize(GObject *gobject)
{
  MonothekExportThread *export_thread;

  export_thread = MONOTHEK_EXPORT_THREAD(gobject);

  if(export_thread->start_time != NULL){
    free(export_thread->start_time);
  }

  if(export_thread->duration != NULL){
    free(export_thread->duration);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_export_thread_parent_class)->finalize(gobject);
}

void
monothek_export_thread_run(AgsThread *thread)
{
  MonothekExportThread *export_thread;

  AgsSoundcard *soundcard;

  struct timespec current_time;

  void *soundcard_buffer;

  guint sec;
  guint nsec;
  
  guint pcm_channels;
  guint buffer_size;
  guint format;

  export_thread = MONOTHEK_EXPORT_THREAD(thread);

  if(AGS_EXPORT_THREAD(export_thread)->audio_file == NULL){
    return;
  }

  clock_gettime(CLOCK_MONOTONIC, &current_time);

  sec = 0;
  nsec = export_thread->start_time->tv_nsec + export_thread->duration->tv_nsec;

  if(nsec > AGS_NSEC_PER_SEC){
    sec = 1;
    nsec -= AGS_NSEC_PER_SEC;
  }
  
  if(export_thread->start_time->tv_sec + export_thread->duration->tv_sec < current_time.tv_sec ||
     (export_thread->start_time->tv_sec + export_thread->duration->tv_sec + sec == current_time.tv_sec &&
      nsec < current_time.tv_nsec)){
    ags_thread_stop(thread);
  }

  /*  */
  soundcard = AGS_SOUNDCARD(AGS_EXPORT_THREAD(export_thread)->soundcard);

  soundcard_buffer = NULL;
  
  if(AGS_IS_DEVOUT(soundcard)){
    soundcard_buffer = ags_soundcard_get_buffer(soundcard);
  }else if(AGS_IS_JACK_DEVOUT(soundcard) ||
	   AGS_IS_PULSE_DEVOUT(soundcard)){
    soundcard_buffer = ags_soundcard_get_prev_buffer(soundcard);
  }else if(AGS_IS_CORE_AUDIO_DEVOUT(soundcard)){
    soundcard_buffer = ags_soundcard_get_buffer(soundcard);
  }
  
  ags_soundcard_get_presets(soundcard,
			    &pcm_channels,
			    NULL,
			    &buffer_size,
			    &format);

  ags_soundcard_lock_buffer(soundcard,
			    soundcard_buffer);
  
  ags_audio_file_write(AGS_EXPORT_THREAD(export_thread)->audio_file,
		       soundcard_buffer,
		       (guint) buffer_size,
		       format);

  ags_soundcard_unlock_buffer(soundcard,
			      soundcard_buffer);
}

/**
 * monothek_export_thread_new:
 * @soundcard: the #AgsSoundcard
 * @audio_file: the output file
 *
 * Create a new instance of #MonothekExportThread.
 *
 * Returns: the new #MonothekExportThread
 *
 * Since: 1.0.0
 */
MonothekExportThread*
monothek_export_thread_new(GObject *soundcard, AgsAudioFile *audio_file)
{
  MonothekExportThread *export_thread;

  export_thread = (MonothekExportThread *) g_object_new(MONOTHEK_TYPE_EXPORT_THREAD,
							"soundcard", soundcard,
							"audio-file", audio_file,
							NULL);
  
  return(export_thread);
}
