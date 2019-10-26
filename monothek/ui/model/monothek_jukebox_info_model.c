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

#include <monothek/ui/model/monothek_jukebox_info_model.h>

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

void monothek_jukebox_info_model_class_init(MonothekJukeboxInfoModelClass *jukebox_info_model);
void monothek_jukebox_info_model_init(MonothekJukeboxInfoModel *jukebox_info_model);
void monothek_jukebox_info_model_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_info_model_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_info_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_info_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxInfoModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_info_model.h
 *
 * #MonothekJukeboxInfoModel is the MVC's start model.
 */
enum{
  PROP_0,
  PROP_SONG_FILENAME,
  PROP_SONG_TITLE,
  PROP_ARTIST,
  PROP_JUKEBOX_OK_ACTIVE,
};

static gpointer monothek_jukebox_info_model_parent_class = NULL;

GType
monothek_jukebox_info_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_info_model = 0;

    static const GTypeInfo monothek_jukebox_info_model_info = {
      sizeof (MonothekJukeboxInfoModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_info_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxInfoModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_info_model_init,
    };

    monothek_type_jukebox_info_model = g_type_register_static(G_TYPE_OBJECT,
							      "MonothekJukeboxInfoModel", &monothek_jukebox_info_model_info,
							      0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_info_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_info_model_class_init(MonothekJukeboxInfoModelClass *jukebox_info_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_info_model_parent_class = g_type_class_peek_parent(jukebox_info_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_info_model;

  gobject->set_property = monothek_jukebox_info_model_set_property;
  gobject->get_property = monothek_jukebox_info_model_get_property;

  gobject->finalize = monothek_jukebox_info_model_finalize;

  /* properties */
  /**
   * MonothekJukeboxInfoModel:song-filename:
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
   * MonothekJukeboxInfoModel:song-title:
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
   * MonothekJukeboxInfoModel:artist:
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
   * MonothekJukeboxInfoModel:jukebox-ok-active:
   *
   * If jukebox ok is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("jukebox-ok-active",
				    i18n_pspec("jukebox ok active"),
				    i18n_pspec("If jukebox ok is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_OK_ACTIVE,
				  param_spec);

  /* MonothekModel */
}

void
monothek_jukebox_info_model_init(MonothekJukeboxInfoModel *jukebox_info_model)
{
  jukebox_info_model->song_filename = NULL;
  
  jukebox_info_model->song_title = NULL;
  jukebox_info_model->artist = NULL;

  jukebox_info_model->jukebox_ok_active = FALSE;
}

void
monothek_jukebox_info_model_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxInfoModel *jukebox_info_model;

  jukebox_info_model = MONOTHEK_JUKEBOX_INFO_MODEL(gobject);

  switch(prop_id){
  case PROP_SONG_FILENAME:
  {
    gchar *song_filename;

    song_filename = (gchar *) g_value_get_string(value);

    if(song_filename == jukebox_info_model->song_filename){
      return;
    }

    if(jukebox_info_model->song_filename != NULL){
      g_free(jukebox_info_model->song_filename);
    }
      
    jukebox_info_model->song_filename = g_strdup(song_filename);
  }
  break; 
  case PROP_SONG_TITLE:
  {
    gchar *song_title;

    song_title = (gchar *) g_value_get_string(value);

    if(song_title == jukebox_info_model->song_title){
      return;
    }

    if(jukebox_info_model->song_title != NULL){
      g_free(jukebox_info_model->song_title);
    }
      
    jukebox_info_model->song_title = g_strdup(song_title);
  }
  break; 
  case PROP_ARTIST:
  {
    gchar *artist;

    artist = (gchar *) g_value_get_string(value);

    if(artist == jukebox_info_model->artist){
      return;
    }

    if(jukebox_info_model->artist != NULL){
      g_free(jukebox_info_model->artist);
    }
      
    jukebox_info_model->artist = g_strdup(artist);
  }
  break; 
  case PROP_JUKEBOX_OK_ACTIVE:
    {
      jukebox_info_model->jukebox_ok_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_info_model_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxInfoModel *jukebox_info_model;

  jukebox_info_model = MONOTHEK_JUKEBOX_INFO_MODEL(gobject);

  switch(prop_id){
  case PROP_SONG_FILENAME:
  {
    g_value_set_string(value, jukebox_info_model->song_filename);
  }
  break;
  case PROP_SONG_TITLE:
  {
    g_value_set_string(value, jukebox_info_model->song_title);
  }
  break;
  case PROP_ARTIST:
  {
    g_value_set_string(value, jukebox_info_model->artist);
  }
  break;
  case PROP_JUKEBOX_OK_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_info_model->jukebox_ok_active);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_info_model_finalize(GObject *gobject)
{
  MonothekJukeboxInfoModel *jukebox_info_model;

  jukebox_info_model = (MonothekJukeboxInfoModel *) gobject;

  g_free(jukebox_info_model->song_filename);
  
  g_free(jukebox_info_model->song_title);

  g_free(jukebox_info_model->artist);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_info_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_info_model_load_song_filename:
 * @jukebox_info_model: the #MonothekJukeboxInfoModel
 * @song_filename: song filename as string
 * 
 * Load @song_filename related fields.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_info_model_load_song_filename(MonothekJukeboxInfoModel *jukebox_info_model,
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

  if(!MONOTHEK_IS_JUKEBOX_INFO_MODEL(jukebox_info_model) ||
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

	      g_object_set(jukebox_info_model,
			   "artist", str,
			   NULL);
	    }else if(!xmlStrncmp("song-title",
				 current_node->name,
				 11)){
	      str = xmlNodeGetContent(current_node);

	      g_object_set(jukebox_info_model,
			   "song-title", str,
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
}

/**
 * monothek_jukebox_info_model_new:
 *
 * Creates an #MonothekJukeboxInfoModel
 *
 * Returns: a new #MonothekJukeboxInfoModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxInfoModel*
monothek_jukebox_info_model_new()
{
  MonothekJukeboxInfoModel *jukebox_info_model;

  jukebox_info_model = (MonothekJukeboxInfoModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_INFO_MODEL,
								 NULL);
  
  return(jukebox_info_model);
}
