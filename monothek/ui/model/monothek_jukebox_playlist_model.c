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

#include <monothek/ui/model/monothek_jukebox_playlist_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xlink.h>
#include <libxml/xpath.h>
#include <libxml/valid.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlsave.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_playlist_model_class_init(MonothekJukeboxPlaylistModelClass *jukebox_playlist_model);
void monothek_jukebox_playlist_model_init(MonothekJukeboxPlaylistModel *jukebox_playlist_model);
void monothek_jukebox_playlist_model_set_property(GObject *gobject,
						  guint prop_id,
						  const GValue *value,
						  GParamSpec *param_spec);
void monothek_jukebox_playlist_model_get_property(GObject *gobject,
						  guint prop_id,
						  GValue *value,
						  GParamSpec *param_spec);
void monothek_jukebox_playlist_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_playlist_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxPlaylistModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_playlist_model.h
 *
 * #MonothekJukeboxPlaylistModel is the MVC's start model.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_playlist_model_parent_class = NULL;

GType
monothek_jukebox_playlist_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_playlist_model = 0;

    static const GTypeInfo monothek_jukebox_playlist_model_info = {
      sizeof (MonothekJukeboxPlaylistModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_playlist_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPlaylistModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_playlist_model_init,
    };

    monothek_type_jukebox_playlist_model = g_type_register_static(G_TYPE_OBJECT,
								  "MonothekJukeboxPlaylistModel", &monothek_jukebox_playlist_model_info,
								  0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_playlist_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_playlist_model_class_init(MonothekJukeboxPlaylistModelClass *jukebox_playlist_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_playlist_model_parent_class = g_type_class_peek_parent(jukebox_playlist_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_playlist_model;

  gobject->set_property = monothek_jukebox_playlist_model_set_property;
  gobject->get_property = monothek_jukebox_playlist_model_get_property;

  gobject->finalize = monothek_jukebox_playlist_model_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_jukebox_playlist_model_init(MonothekJukeboxPlaylistModel *jukebox_playlist_model)
{
  xmlDoc *doc;
  xmlNode *root_node;
  xmlNode *song_node;
  xmlNode *current_node;
  
  gchar *playlist_filename;
  
  guint i;

  /* set default value */
  jukebox_playlist_model->song_filename = (gchar **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(gchar *));
  jukebox_playlist_model->cover_filename = (gchar **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(gchar *));

  jukebox_playlist_model->song_select_active = (gboolean *) malloc(MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT * sizeof(gboolean));

  jukebox_playlist_model->song_title = (gchar **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(gchar *));
  jukebox_playlist_model->artist = (gchar **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(gchar *));
  jukebox_playlist_model->album = (gchar **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(gchar *));
  jukebox_playlist_model->duration = (struct timespec **) malloc((MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT + 1) * sizeof(struct timespec *));
  
  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
    jukebox_playlist_model->song_select_active[i] = FALSE;

    jukebox_playlist_model->song_filename[i] = NULL;
    jukebox_playlist_model->cover_filename[i] = NULL;

    jukebox_playlist_model->song_title[i] = NULL;
    jukebox_playlist_model->artist[i] = NULL;
    jukebox_playlist_model->album[i] = NULL;
    
    jukebox_playlist_model->duration[i] = (struct timespec *) malloc(sizeof(struct timespec));
    jukebox_playlist_model->duration[i]->tv_sec = MONOTHEK_JUKEBOX_PLAYLIST_MODEL_DEFAULT_DURATION_SEC;
    jukebox_playlist_model->duration[i]->tv_nsec = 0;    
  }

  jukebox_playlist_model->song_filename[MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT] = NULL;
  jukebox_playlist_model->cover_filename[MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT] = NULL;

  jukebox_playlist_model->song_title[MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT] = NULL;
  jukebox_playlist_model->artist[MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT] = NULL;
  jukebox_playlist_model->album[MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT] = NULL;

  /* read from XML */
  playlist_filename = MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME;

  doc = xmlReadFile(playlist_filename, NULL, 0);

  root_node = xmlDocGetRootElement(doc);

  song_node = root_node->children;
  i = 0;
  
  while(song_node != NULL){
    if(song_node->type == XML_ELEMENT_NODE){
      if(!xmlStrncmp("song",
		     song_node->name,
		     5)){
	current_node = song_node->children;

	while(current_node != NULL){
	  if(current_node->type == XML_ELEMENT_NODE){
	    gchar *str;
	      
	    if(!xmlStrncmp("file",
			   current_node->name,
			   5)){
	      str = xmlNodeGetContent(current_node);

	      jukebox_playlist_model->song_filename[i] = g_strdup(str);
	    }else if(!xmlStrncmp("artist",
				 current_node->name,
				 7)){
	      str = xmlNodeGetContent(current_node);

	      jukebox_playlist_model->artist[i] = g_strdup(str);
	    }else if(!xmlStrncmp("song-title",
				 current_node->name,
				 11)){
	      str = xmlNodeGetContent(current_node);

	      jukebox_playlist_model->song_title[i] = g_strdup(str);
	    }else if(!xmlStrncmp("album",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      jukebox_playlist_model->album[i] = g_strdup(str);
	    }else if(!xmlStrncmp("duration",
				 current_node->name,
				 9)){
	      xmlNode *child;

	      child = current_node->children;

	      while(child != NULL){
		if(child->type == XML_ELEMENT_NODE){
		  if(!xmlStrncmp("timeval-sec",
				 child->name,
				 12)){
		    guint duration_sec;
		    
		    str = xmlNodeGetContent(child);

		    duration_sec = 0;
		    
		    if(str != NULL){
		      duration_sec = g_ascii_strtoull(str,
						      NULL,
						      10);
		    }
		      
		    jukebox_playlist_model->duration[i]->tv_sec = duration_sec;
		    
		    break;
		  }
		}

		child = child->next;
	      }
	    }else if(!xmlStrncmp("cover",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      jukebox_playlist_model->cover_filename[i] = g_strdup(str);
	    }
	  }

	  current_node = current_node->next;
	}

	i++;
      }
    }

    song_node = song_node->next;
  }

  /* free XML doc */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();
}

void
monothek_jukebox_playlist_model_set_property(GObject *gobject,
					     guint prop_id,
					     const GValue *value,
					     GParamSpec *param_spec)
{
  MonothekJukeboxPlaylistModel *jukebox_playlist_model;

  jukebox_playlist_model = MONOTHEK_JUKEBOX_PLAYLIST_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_playlist_model_get_property(GObject *gobject,
					     guint prop_id,
					     GValue *value,
					     GParamSpec *param_spec)
{
  MonothekJukeboxPlaylistModel *jukebox_playlist_model;

  jukebox_playlist_model = MONOTHEK_JUKEBOX_PLAYLIST_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_playlist_model_finalize(GObject *gobject)
{
  MonothekJukeboxPlaylistModel *jukebox_playlist_model;

  guint i;
  
  jukebox_playlist_model = (MonothekJukeboxPlaylistModel *) gobject;
  
  if(jukebox_playlist_model->song_select_active != NULL){
    free(jukebox_playlist_model->song_select_active);
  }
    
  if(jukebox_playlist_model->song_filename != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      g_free(jukebox_playlist_model->song_filename[i]);
    }

    free(jukebox_playlist_model->song_filename);
  }
  
  if(jukebox_playlist_model->cover_filename != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      g_free(jukebox_playlist_model->cover_filename[i]);
    }

    free(jukebox_playlist_model->cover_filename);
  }

  if(jukebox_playlist_model->song_title != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      g_free(jukebox_playlist_model->song_title[i]);
    }

    free(jukebox_playlist_model->song_title);
  }

  if(jukebox_playlist_model->artist != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      g_free(jukebox_playlist_model->artist[i]);
    }

    free(jukebox_playlist_model->artist);
  }

  if(jukebox_playlist_model->album != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      g_free(jukebox_playlist_model->album[i]);
    }

    free(jukebox_playlist_model->album);
  }

  if(jukebox_playlist_model->duration != NULL){
    for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
      free(jukebox_playlist_model->duration[i]);
    }
    
    free(jukebox_playlist_model->duration);
  }

  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_playlist_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_playlist_model_new:
 *
 * Creates an #MonothekJukeboxPlaylistModel
 *
 * Returns: a new #MonothekJukeboxPlaylistModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxPlaylistModel*
monothek_jukebox_playlist_model_new()
{
  MonothekJukeboxPlaylistModel *jukebox_playlist_model;

  jukebox_playlist_model = (MonothekJukeboxPlaylistModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PLAYLIST_MODEL,
									 NULL);
  
  return(jukebox_playlist_model);
}
