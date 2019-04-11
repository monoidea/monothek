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

#include <monothek/audio/task/monothek_export_output.h>

#include <monothek/audio/thread/monothek_export_thread.h>

#include <sys/time.h>
#include <time.h>

#include <monothek/i18n.h>

void monothek_export_output_class_init(MonothekExportOutputClass *export_output);
void monothek_export_output_init(MonothekExportOutput *export_output);
void monothek_export_output_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec);
void monothek_export_output_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec);
void monothek_export_output_finalize(GObject *gobject);

void monothek_export_output_launch(AgsTask *task);

/**
 * SECTION:monothek_export_output
 * @short_description: export output task
 * @title: MonothekExportOutput
 * @section_id:
 * @include: monothek/audio/task/monothek_export_output.h
 *
 * The #MonothekExportOutput task exports #GObject to file.
 */
static gpointer monothek_export_output_parent_class = NULL;

enum{
  PROP_0,
  PROP_DURATION,
};

GType
monothek_export_output_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_export_output = 0;

    static const GTypeInfo monothek_export_output_info = {
      sizeof(MonothekExportOutputClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_export_output_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof(MonothekExportOutput),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_export_output_init,
    };

    monothek_type_export_output = g_type_register_static(AGS_TYPE_EXPORT_OUTPUT,
							 "MonothekExportOutput",
							 &monothek_export_output_info,
							 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_export_output);
  }

  return g_define_type_id__volatile;
}

void
monothek_export_output_class_init(MonothekExportOutputClass *export_output)
{
  GObjectClass *gobject;
  AgsTaskClass *task;
  GParamSpec *param_spec;

  monothek_export_output_parent_class = g_type_class_peek_parent(export_output);

  /* gobject */
  gobject = (GObjectClass *) export_output;

  gobject->set_property = monothek_export_output_set_property;
  gobject->get_property = monothek_export_output_get_property;

  gobject->finalize = monothek_export_output_finalize;

  /* properties */
  /**
   * MonothekExportOutput:duration:
   *
   * Duration to export.
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

  /* task */
  task = (AgsTaskClass *) export_output;

  task->launch = monothek_export_output_launch;
}

void
monothek_export_output_init(MonothekExportOutput *export_output)
{
  g_object_set(export_output,
	       "live-performance", TRUE,
	       NULL);

  export_output->duration = (struct timespec *) malloc(sizeof(struct timespec));
  export_output->duration->tv_sec = 0;
  export_output->duration->tv_nsec = 0;
}

void
monothek_export_output_set_property(GObject *gobject,
				    guint prop_id,
				    const GValue *value,
				    GParamSpec *param_spec)
{
  MonothekExportOutput *export_output;

  export_output = MONOTHEK_EXPORT_OUTPUT(gobject);

  switch(prop_id){
  case PROP_DURATION:
  {
    struct timespec *duration;
    
    duration = g_value_get_pointer(value);

    export_output->duration->tv_sec = duration->tv_sec;
    export_output->duration->tv_nsec = duration->tv_nsec;
  }
  break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_export_output_get_property(GObject *gobject,
				    guint prop_id,
				    GValue *value,
				    GParamSpec *param_spec)
{
  MonothekExportOutput *export_output;

  export_output = MONOTHEK_EXPORT_OUTPUT(gobject);

  switch(prop_id){
  case PROP_DURATION:
  {
    g_value_set_pointer(value, export_output->duration);
  }
  break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_export_output_finalize(GObject *gobject)
{
  MonothekExportOutput *export_output;

  export_output = MONOTHEK_EXPORT_OUTPUT(gobject);

  if(export_output->duration != NULL){
    free(export_output->duration);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_export_output_parent_class)->finalize(gobject);
}

void
monothek_export_output_launch(AgsTask *task)
{
  MonothekExportOutput *export_output;

  MonothekExportThread *export_thread;
  
  AgsAudioFile *audio_file;

  struct timespec start_time;

  gchar *filename;
  char str_buffer[256];

  guint duration_sec;  

  export_output = MONOTHEK_EXPORT_OUTPUT(task);
  
  export_thread = MONOTHEK_EXPORT_THREAD(AGS_EXPORT_OUTPUT(export_output)->export_thread);

  clock_gettime(CLOCK_MONOTONIC, &start_time);

  duration_sec = export_output->duration->tv_sec;
  
  strftime(str_buffer, 256, "%Y-%j-%H-%M-%S", localtime(start_time.tv_sec));
  filename = g_strdup_printf("snd-%s-%06d-%06d.wav", str_buffer, start_time.tv_nsec / 1000, duration_sec);

  g_object_set(task,
	       "filename", filename,
	       NULL);

  g_object_set(G_OBJECT(export_thread),
	       "start-time", &start_time,
	       "duration", export_output->duration,
	       NULL);

  /* call parent */
  AGS_TASK_CLASS(monothek_export_output_parent_class)->launch(task);  
}

/**
 * monothek_export_output_new:
 * @export_thread: the #MonothekExportThread to start
 * @soundcard: the #GObject to export
 * @duration: the duration as #timespec-struct
 *
 * Creates an #MonothekExportOutput.
 *
 * Returns: an new #MonothekExportOutput.
 *
 * Since: 1.0.0
 */
MonothekExportOutput*
monothek_export_output_new(AgsExportThread *export_thread,
			   GObject *soundcard,
			   struct timespec *duration)
{
  MonothekExportOutput *export_output;

  export_output = (MonothekExportOutput *) g_object_new(MONOTHEK_TYPE_EXPORT_OUTPUT,
							"export-thread", export_thread,
							"soundcard", soundcard,
							"duration", duration,
							NULL);

  return(export_output);
}
