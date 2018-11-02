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

#include <monothek/audio/file/monothek_audio_file_manager.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_audio_file_manager_class_init(MonothekAudioFileManagerClass *audio_file_manager);
void monothek_audio_file_manager_init(MonothekAudioFileManager *audio_file_manager);
void monothek_audio_file_manager_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_audio_file_manager_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_audio_file_manager_finalize(GObject *gobject);

/**
 * SECTION:monothek_audio_file_manager
 * @short_description: The audio file manager
 * @title: MonothekAudioFileManager
 * @section_id:
 * @include: monothek/audio_file/monothek_audio_file_manager.h
 *
 * #MonothekAudioFileManager stores audio files.
 */

enum{
  PROP_0,
  PROP_AUDIO_FILE,
};

static gpointer monothek_audio_file_manager_parent_class = NULL;

MonothekAudioFileManager *monothek_audio_file_manager = NULL;

GType
monothek_audio_file_manager_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_audio_file_manager = 0;

    static const GTypeInfo monothek_audio_file_manager_info = {
      sizeof (MonothekAudioFileManagerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_audio_file_manager_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekAudioFileManager),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_audio_file_manager_init,
    };

    monothek_type_audio_file_manager = g_type_register_static(G_TYPE_OBJECT,
							      "MonothekAudioFileManager", &monothek_audio_file_manager_info,
							      0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_audio_file_manager);
  }

  return g_define_type_id__volatile;
}

void
monothek_audio_file_manager_class_init(MonothekAudioFileManagerClass *audio_file_manager)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_audio_file_manager_parent_class = g_type_class_peek_parent(audio_file_manager);

  /* GObjectClass */
  gobject = (GObjectClass *) audio_file_manager;

  gobject->set_property = monothek_audio_file_manager_set_property;
  gobject->get_property = monothek_audio_file_manager_get_property;

  gobject->finalize = monothek_audio_file_manager_finalize;

  /* properties */
  /**
   * MonothekAudioFileManager:audio_file:
   *
   * The containing #MonothekAudioFile.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("audio_file",
				    i18n_pspec("containing audio_file"),
				    i18n_pspec("The audio_file it contains"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_AUDIO_FILE,
				  param_spec);

  /* MonothekModel */
}

void
monothek_audio_file_manager_init(MonothekAudioFileManager *audio_file_manager)
{
  audio_file_manager->audio_file = NULL;
}

void
monothek_audio_file_manager_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekAudioFileManager *audio_file_manager;

  audio_file_manager = MONOTHEK_AUDIO_FILE_MANAGER(gobject);

  switch(prop_id){
  case PROP_AUDIO_FILE:
    {
      AgsAudioFile *audio_file;
      
      /*  */
      audio_file = (AgsAudioFile *) g_value_get_pointer(value);

      monothek_audio_file_manager_add_audio_file(audio_file_manager,
						 (GObject *) audio_file);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_audio_file_manager_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekAudioFileManager *audio_file_manager;

  audio_file_manager = MONOTHEK_AUDIO_FILE_MANAGER(gobject);

  switch(prop_id){
  case PROP_AUDIO_FILE:
    {      
      g_value_set_pointer(value,
			  g_list_copy(audio_file_manager->audio_file));
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_audio_file_manager_finalize(GObject *gobject)
{
  MonothekAudioFileManager *audio_file_manager;

  audio_file_manager = (MonothekAudioFileManager *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_audio_file_manager_parent_class)->finalize(gobject);
}

/**
 * monothek_audio_file_manager_add_audio_file:
 * @audio_file_manager: the #MonothekAudioFileManager
 * @audio_file: the #MonothekAudioFile
 * 
 * Add @audio_file to @audio_file_manager.
 * 
 * Since: 1.0.0
 */
void
monothek_audio_file_manager_add_audio_file(MonothekAudioFileManager *audio_file_manager,
					   GObject *audio_file)
{
  if(!MONOTHEK_IS_AUDIO_FILE_MANAGER(audio_file_manager) ||
     !AGS_IS_AUDIO_FILE(audio_file)){
    return;
  }

  if(g_list_find(audio_file_manager->audio_file,
		 audio_file) == NULL){
    g_object_ref(audio_file);
    audio_file_manager->audio_file = g_list_prepend(audio_file_manager->audio_file,
						    audio_file);
  }
}

/**
 * monothek_audio_file_manager_remove_audio_file:
 * @audio_file_manager: the #MonothekAudioFileManager
 * @audio_file: the #MonothekAudioFile
 * 
 * Remove @audio_file from @audio_file_manager.
 * 
 * Since: 1.0.0
 */
void
monothek_audio_file_manager_remove_audio_file(MonothekAudioFileManager *audio_file_manager,
					      GObject *audio_file)
{
  if(!MONOTHEK_IS_AUDIO_FILE_MANAGER(audio_file_manager) ||
     !AGS_IS_AUDIO_FILE(audio_file)){
    return;
  }

  if(g_list_find(audio_file_manager->audio_file,
		 audio_file) != NULL){
    g_object_unref(audio_file);
    audio_file_manager->audio_file = g_list_remove(audio_file_manager->audio_file,
						   audio_file);
  }
}

/**
 * monothek_audio_file_manager_find_audio_file:
 * @audio_file_manager: the #MonothekAudioFileManager
 * @filename: the string identifier
 * 
 * Find audio file by @filename.
 *
 * Returns: the matching #MonothekAudioFile if found, otherwise %NULL
 * 
 * Since: 1.0.0
 */
GObject*
monothek_audio_file_manager_find_audio_file(MonothekAudioFileManager *audio_file_manager,
					    gchar *filename)
{
  AgsAudioFile *audio_file;
  
  GList *start_list, *list;
  
  if(!MONOTHEK_IS_AUDIO_FILE_MANAGER(audio_file_manager) ||
     filename == NULL){
    return(NULL);
  }

  g_object_get(audio_file_manager,
	       "audio_file", &start_list,
	       NULL);

  audio_file = NULL;
  
  list = start_list;
  
  while(list != NULL){
    if(!g_strcmp0(AGS_AUDIO_FILE(list->data)->filename,
		  filename)){
      audio_file = list->data;
      
      break;
    }
    
    list = list->next;
  }

  g_list_free(start_list);
  
  return(audio_file);
}

/**
 * monothek_audio_file_manager_load_playlist:
 * @audio_file_manager: the #MonothekAudioFileManager
 * @filename: the #GSList-struct containing filename as string
 * 
 * Load playlist from @filename and read #AgsWave.
 * 
 * Since: 1.0.0
 */
void
monothek_audio_file_manager_load_playlist(MonothekAudioFileManager *audio_file_manager,
					  GSList *filename)
{
  if(!MONOTHEK_IS_AUDIO_FILE_MANAGER(audio_file_manager) ||
     filename == NULL){
    return;
  }

  while(filename != NULL){
    AgsAudioFile *audio_file;

    if(monothek_audio_file_manager_find_audio_file(audio_file_manager,
						   filename->data) != NULL){
      filename = filename->next;
      
      continue;
    }
    
    audio_file = ags_audio_file_new(filename->data,
				    NULL,
				    -1);

    if(!ags_audio_file_open(audio_file)){
      filename = filename->next;
      
      continue;
    }

    g_message("MonothekAudioFileManager - reading: %s", filename->data);
    ags_audio_file_read_wave(audio_file,
			     0,
			     0.0, 0);
    ags_audio_file_close(audio_file);
    
    monothek_audio_file_manager_add_audio_file(audio_file_manager,
					       audio_file);
    
    filename = filename->next;
  }
}

/**
 * monothek_audio_file_manager_get_instance:
 * 
 * Get monothek audio_file manager instance.
 *
 * Returns: the #MonothekAudioFileManager singleton
 * 
 * Since: 1.0.0
 */
MonothekAudioFileManager*
monothek_audio_file_manager_get_instance()
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&mutex);

  if(monothek_audio_file_manager == NULL){
    monothek_audio_file_manager = monothek_audio_file_manager_new();
  }
  
  pthread_mutex_unlock(&mutex);

  return(monothek_audio_file_manager);
}

/**
 * monothek_audio_file_manager_new:
 *
 * Creates an #MonothekAudioFileManager
 *
 * Returns: a new #MonothekAudioFileManager
 *
 * Since: 1.0.0
 */
MonothekAudioFileManager*
monothek_audio_file_manager_new()
{
  MonothekAudioFileManager *audio_file_manager;

  audio_file_manager = (MonothekAudioFileManager *) g_object_new(MONOTHEK_TYPE_AUDIO_FILE_MANAGER,
								 NULL);
  
  return(audio_file_manager);
}
