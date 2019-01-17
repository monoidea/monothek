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

#include <monothek/ui/model/monothek_jukebox_track_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/model/monothek_jukebox_playlist_model.h>

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

void monothek_jukebox_track_model_class_init(MonothekJukeboxTrackModelClass *jukebox_track_model);
void monothek_jukebox_track_model_init(MonothekJukeboxTrackModel *jukebox_track_model);
void monothek_jukebox_track_model_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec);
void monothek_jukebox_track_model_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec);
void monothek_jukebox_track_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_track_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxTrackModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_track_model.h
 *
 * #MonothekJukeboxTrackModel is the MVC's start model.
 */

enum{
  PROP_0,
  PROP_SONG_FILENAME,
  PROP_TEST_MODE,
  PROP_COVER_FILENAME,
  PROP_SONG_TITLE,
  PROP_ARTIST,
  PROP_ALBUM,
  PROP_DURATION,
};

static gpointer monothek_jukebox_track_model_parent_class = NULL;

GType
monothek_jukebox_track_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_track_model = 0;

    static const GTypeInfo monothek_jukebox_track_model_info = {
      sizeof (MonothekJukeboxTrackModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_track_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxTrackModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_track_model_init,
    };

    monothek_type_jukebox_track_model = g_type_register_static(G_TYPE_OBJECT,
							       "MonothekJukeboxTrackModel", &monothek_jukebox_track_model_info,
							       0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_track_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_track_model_class_init(MonothekJukeboxTrackModelClass *jukebox_track_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_track_model_parent_class = g_type_class_peek_parent(jukebox_track_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_track_model;

  gobject->set_property = monothek_jukebox_track_model_set_property;
  gobject->get_property = monothek_jukebox_track_model_get_property;

  gobject->finalize = monothek_jukebox_track_model_finalize;

  /* properties */
  /**
   * MonothekJukeboxTrackModel:song-filename:
   *
   * The assigned song filename.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("song-filename",
				   i18n_pspec("song filename"),
				   i18n_pspec("The assigned song filename"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SONG_FILENAME,
				  param_spec);

  /**
   * MonothekJukeboxTrackModel:test-mode:
   *
   * The assigned test mode property.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("test-mode",
				    i18n_pspec("test mode"),
				    i18n_pspec("The test mode property"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_TEST_MODE,
				  param_spec);
  
  /**
   * MonothekJukeboxTrackModel:cover-filename:
   *
   * The assigned cover filename.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("cover-filename",
				   i18n_pspec("cover filename"),
				   i18n_pspec("The assigned cover filename"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_COVER_FILENAME,
				  param_spec);

  /**
   * MonothekJukeboxTrackModel:song-title:
   *
   * The assigned song title.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("song-title",
				   i18n_pspec("song title"),
				   i18n_pspec("The assigned song title"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SONG_TITLE,
				  param_spec);
  
  /**
   * MonothekJukeboxTrackModel:artist:
   *
   * The assigned artist.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("artist",
				   i18n_pspec("artist"),
				   i18n_pspec("The assigned artist"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ARTIST,
				  param_spec);

  /**
   * MonothekJukeboxTrackModel:album:
   *
   * The assigned album.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("album",
				   i18n_pspec("album"),
				   i18n_pspec("The assigned album"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ALBUM,
				  param_spec);
  
  /**
   * MonothekJukeboxTrackModel:duration:
   *
   * The assigned duration.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("duration",
				   i18n_pspec("duration"),
				   i18n_pspec("The assigned duration"),
				   G_PARAM_READABLE);
  g_object_class_install_property(gobject,
				  PROP_DURATION,
				  param_spec);

  /* MonothekModel */
}

void
monothek_jukebox_track_model_init(MonothekJukeboxTrackModel *jukebox_track_model)
{
  jukebox_track_model->song_filename = NULL;
  
  jukebox_track_model->test_mode = FALSE;
  
  jukebox_track_model->cover_filename = NULL;
  
  jukebox_track_model->song_title = NULL;
  jukebox_track_model->artist = NULL;
  jukebox_track_model->album = NULL;

  jukebox_track_model->duration = (struct timespec *) malloc(sizeof(struct timespec));

  jukebox_track_model->duration->tv_sec = 0;
  jukebox_track_model->duration->tv_nsec = 0;
}

void
monothek_jukebox_track_model_set_property(GObject *gobject,
					  guint prop_id,
					  const GValue *value,
					  GParamSpec *param_spec)
{
  MonothekJukeboxTrackModel *jukebox_track_model;

  jukebox_track_model = MONOTHEK_JUKEBOX_TRACK_MODEL(gobject);

  switch(prop_id){
  case PROP_SONG_FILENAME:
    {
      gchar *song_filename;

      song_filename = (gchar *) g_value_get_string(value);

      if(song_filename == jukebox_track_model->song_filename){
	return;
      }

      if(jukebox_track_model->song_filename != NULL){
	g_free(jukebox_track_model->song_filename);
      }
      
      jukebox_track_model->song_filename = g_strdup(song_filename);
    }
    break; 
  case PROP_TEST_MODE:
    {
      gboolean test_mode;

      test_mode = g_value_get_boolean(value);

      jukebox_track_model->test_mode = test_mode;
    }
    break; 
  case PROP_COVER_FILENAME:
    {
      gchar *cover_filename;

      cover_filename = (gchar *) g_value_get_string(value);

      if(cover_filename == jukebox_track_model->cover_filename){
	return;
      }

      if(jukebox_track_model->cover_filename != NULL){
	g_free(jukebox_track_model->cover_filename);
      }
      
      jukebox_track_model->cover_filename = g_strdup(cover_filename);
    }
    break; 
  case PROP_SONG_TITLE:
    {
      gchar *song_title;

      song_title = (gchar *) g_value_get_string(value);

      if(song_title == jukebox_track_model->song_title){
	return;
      }

      if(jukebox_track_model->song_title != NULL){
	g_free(jukebox_track_model->song_title);
      }
      
      jukebox_track_model->song_title = g_strdup(song_title);
    }
    break; 
  case PROP_ARTIST:
    {
      gchar *artist;

      artist = (gchar *) g_value_get_string(value);

      if(artist == jukebox_track_model->artist){
	return;
      }

      if(jukebox_track_model->artist != NULL){
	g_free(jukebox_track_model->artist);
      }
      
      jukebox_track_model->artist = g_strdup(artist);
    }
    break; 
  case PROP_ALBUM:
    {
      gchar *album;

      album = (gchar *) g_value_get_string(value);

      if(album == jukebox_track_model->album){
	return;
      }

      if(jukebox_track_model->album != NULL){
	g_free(jukebox_track_model->album);
      }
      
      jukebox_track_model->album = g_strdup(album);
    }
    break; 
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_model_get_property(GObject *gobject,
					  guint prop_id,
					  GValue *value,
					  GParamSpec *param_spec)
{
  MonothekJukeboxTrackModel *jukebox_track_model;

  jukebox_track_model = MONOTHEK_JUKEBOX_TRACK_MODEL(gobject);

  switch(prop_id){
  case PROP_SONG_FILENAME:
    {
      g_value_set_string(value, jukebox_track_model->song_filename);
    }
    break;
  case PROP_TEST_MODE:
    {
      g_value_set_boolean(value, jukebox_track_model->test_mode);
    }
    break;
  case PROP_COVER_FILENAME:
    {
      g_value_set_string(value, jukebox_track_model->cover_filename);
    }
    break;
  case PROP_SONG_TITLE:
    {
      g_value_set_string(value, jukebox_track_model->song_title);
    }
    break;
  case PROP_ARTIST:
    {
      g_value_set_string(value, jukebox_track_model->artist);
    }
    break;
  case PROP_ALBUM:
    {
      g_value_set_string(value, jukebox_track_model->album);
    }
    break;
  case PROP_DURATION:
    {
      g_value_set_pointer(value, jukebox_track_model->duration);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_model_finalize(GObject *gobject)
{
  MonothekJukeboxTrackModel *jukebox_track_model;

  jukebox_track_model = (MonothekJukeboxTrackModel *) gobject;

  g_free(jukebox_track_model->song_filename);
  
  g_free(jukebox_track_model->cover_filename);

  g_free(jukebox_track_model->song_title);

  g_free(jukebox_track_model->artist);
  g_free(jukebox_track_model->album);

  if(jukebox_track_model->duration != NULL){
    free(jukebox_track_model->duration);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_track_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_track_model_load_song_filename:
 * @jukebox_track_model: the #MonothekJukeboxTrackModel
 * @song_filename: song filename as string
 * 
 * Load @song_filename related fields.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_model_load_song_filename(MonothekJukeboxTrackModel *jukebox_track_model,
						gchar *song_filename)
{
  xmlDoc *doc;
  xmlNode *current_node;
  xmlXPathContext *xpath_context;
  xmlXPathObject *xpath_object;
  xmlNode **node;

  gchar *xpath;
  gchar *playlist_filename;

  guint i;

  if(!MONOTHEK_IS_JUKEBOX_TRACK_MODEL(jukebox_track_model) ||
     song_filename == NULL){
    return;
  }
  
  /* read from XML */  
  playlist_filename = MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME;

  doc = xmlReadFile(playlist_filename, NULL, 0);

  xpath = g_strdup_printf("/playlist/song/file[text()='%s']/ancestor::*[self::song][1]", ags_string_util_escape_single_quote(song_filename));

  xpath_context = xmlXPathNewContext(doc);
  xpath_object = xmlXPathEval((xmlChar *) xpath,
			      xpath_context);

  if(xpath_object->nodesetval != NULL){
    node = xpath_object->nodesetval->nodeTab;

    for(i = 0; i < xpath_object->nodesetval->nodeNr; i++){
      if(node[i]->type == XML_ELEMENT_NODE){
	current_node = node[i]->children;

	while(current_node != NULL){
	  if(current_node->type == XML_ELEMENT_NODE){
	    gchar *str;
	      
	    if(!xmlStrncmp("artist",
			   current_node->name,
			   7)){
	      str = xmlNodeGetContent(current_node);

	      g_object_set(jukebox_track_model,
			   "artist", str,
			   NULL);
	    }else if(!xmlStrncmp("song-title",
				 current_node->name,
				 11)){
	      str = xmlNodeGetContent(current_node);

	      g_object_set(jukebox_track_model,
			   "song-title", str,
			   NULL);
	    }else if(!xmlStrncmp("album",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      g_object_set(jukebox_track_model,
			   "album", str,
			   NULL);
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
		      
		    jukebox_track_model->duration->tv_sec = duration_sec;
		    
		    break;
		  }
		}

		child = child->next;
	      }
	    }else if(!xmlStrncmp("cover",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      g_object_set(jukebox_track_model,
			   "cover-filename", str,
			   NULL);
	    }
	  }

	  current_node = current_node->next;
	}

	break;
      }
    }
  }

  g_free(xpath);
  
  /* free XML doc */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();
}

/**
 * monothek_jukebox_track_model_new:
 *
 * Creates an #MonothekJukeboxTrackModel
 *
 * Returns: a new #MonothekJukeboxTrackModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxTrackModel*
monothek_jukebox_track_model_new()
{
  MonothekJukeboxTrackModel *jukebox_track_model;

  jukebox_track_model = (MonothekJukeboxTrackModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_TRACK_MODEL,
								   NULL);
  
  return(jukebox_track_model);
}
