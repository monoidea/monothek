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

#include <monothek/audio/file/monothek_recall_factory.h>

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

GList*
monothek_recall_factory_create_copy_pattern(AgsAudio *audio,
					    AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
					    gchar *plugin_name,
					    guint start_audio_channel, guint stop_audio_channel,
					    guint start_pad, guint stop_pad,
					    guint create_flags, guint recall_flags)
{
  return(NULL);
}

GList*
monothek_recall_factory_create_play_wave(AgsAudio *audio,
					 AgsRecallContainer *play_container, AgsRecallContainer *recall_container,
					 gchar *plugin_name,
					 guint start_audio_channel, guint stop_audio_channel,
					 guint start_pad, guint stop_pad,
					 guint create_flags, guint recall_flags)
{
  return(NULL);
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
ags_recall_factory_create(AgsAudio *audio,
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
