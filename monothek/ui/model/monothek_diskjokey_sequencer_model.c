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

#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>

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

void monothek_diskjokey_sequencer_model_class_init(MonothekDiskjokeySequencerModelClass *diskjokey_sequencer_model);
void monothek_diskjokey_sequencer_model_init(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model);
void monothek_diskjokey_sequencer_model_set_property(GObject *gobject,
						     guint prop_id,
						     const GValue *value,
						     GParamSpec *param_spec);
void monothek_diskjokey_sequencer_model_get_property(GObject *gobject,
						     guint prop_id,
						     GValue *value,
						     GParamSpec *param_spec);
void monothek_diskjokey_sequencer_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_diskjokey_sequencer_model
 * @short_description: The start model object.
 * @title: MonothekDiskjokeySequencerModel
 * @section_id:
 * @include: monothek/ui/model/monothek_diskjokey_sequencer_model.h
 *
 * #MonothekDiskjokeySequencerModel is the MVC's sequencer model.
 */

enum{
  PROP_0,
  PROP_TECHNO_ACTIVE,
  PROP_HOUSE_ACTIVE,
  PROP_HIPHOP_ACTIVE,
  PROP_RANDOM_ACTIVE,
  PROP_CLEAR_ACTIVE,
  PROP_RUN_ACTIVE,
};

static gpointer monothek_diskjokey_sequencer_model_parent_class = NULL;

GType
monothek_diskjokey_sequencer_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_sequencer_model = 0;

    static const GTypeInfo monothek_diskjokey_sequencer_model_info = {
      sizeof (MonothekDiskjokeySequencerModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_sequencer_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeySequencerModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_sequencer_model_init,
    };

    monothek_type_diskjokey_sequencer_model = g_type_register_static(G_TYPE_OBJECT,
								     "MonothekDiskjokeySequencerModel", &monothek_diskjokey_sequencer_model_info,
								     0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_sequencer_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_sequencer_model_class_init(MonothekDiskjokeySequencerModelClass *diskjokey_sequencer_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_diskjokey_sequencer_model_parent_class = g_type_class_peek_parent(diskjokey_sequencer_model);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_sequencer_model;

  gobject->set_property = monothek_diskjokey_sequencer_model_set_property;
  gobject->get_property = monothek_diskjokey_sequencer_model_get_property;

  gobject->finalize = monothek_diskjokey_sequencer_model_finalize;

  /* properties */
  /**
   * MonothekDiskjokeySequencerModel:techno-active:
   *
   * If techno is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("techno-active",
				    i18n_pspec("techno active"),
				    i18n_pspec("If techno is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_TECHNO_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeySequencerModel:house-active:
   *
   * If house is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("house-active",
				    i18n_pspec("house active"),
				    i18n_pspec("If house is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_HOUSE_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeySequencerModel:hiphop-active:
   *
   * If hiphop is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("hiphop-active",
				    i18n_pspec("hiphop active"),
				    i18n_pspec("If hiphop is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_HIPHOP_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeySequencerModel:random-active:
   *
   * If random is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("random-active",
				    i18n_pspec("random active"),
				    i18n_pspec("If random is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_RANDOM_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeySequencerModel:clear-active:
   *
   * If clear is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("clear-active",
				    i18n_pspec("clear active"),
				    i18n_pspec("If clear is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_CLEAR_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeySequencerModel:run-active:
   *
   * If run is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("run-active",
				    i18n_pspec("run active"),
				    i18n_pspec("If run is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_RUN_ACTIVE,
				  param_spec);

  /* MonothekModel */
}

void
monothek_diskjokey_sequencer_model_init(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model)
{
  xmlDoc *doc;
  xmlNode *root_node;
  xmlNode *sample_node;
  xmlNode *current_node;

  gchar *techno_filename;
  gchar *house_filename;
  gchar *hiphop_filename;
  xmlChar *str;
  
  guint i, j;

  diskjokey_sequencer_model->pad_active = (gboolean **) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT * sizeof(gboolean *));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_model->pad_active[i] = (gboolean *) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT * sizeof(gboolean));
    
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
      diskjokey_sequencer_model->pad_active[i][j] = FALSE;
    }
  }

  diskjokey_sequencer_model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO;
  
  diskjokey_sequencer_model->techno_active = TRUE;
  diskjokey_sequencer_model->house_active = FALSE;
  diskjokey_sequencer_model->hiphop_active = FALSE;
  
  diskjokey_sequencer_model->random_active = FALSE;
  diskjokey_sequencer_model->clear_active = FALSE;

  diskjokey_sequencer_model->run_active = FALSE;

  diskjokey_sequencer_model->active_column = -1;

  diskjokey_sequencer_model->current_tab = 0;
  diskjokey_sequencer_model->tab_active = (gboolean *) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT * sizeof(gboolean));

  diskjokey_sequencer_model->tab_active[0] = TRUE;
  
  for(i = 1; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; i++){
    diskjokey_sequencer_model->tab_active[i] = FALSE;
  }
  
  diskjokey_sequencer_model->bpm = 120.0;

  diskjokey_sequencer_model->swing = 0.0;

  /* techno resources */
  diskjokey_sequencer_model->techno_label = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));
  diskjokey_sequencer_model->techno_sample = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_model->techno_label[i] = NULL;
    diskjokey_sequencer_model->techno_sample[i] = NULL;
  }
  
  diskjokey_sequencer_model->techno_label[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;
  diskjokey_sequencer_model->techno_sample[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;

  techno_filename = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME;
  
  doc = xmlReadFile(techno_filename, NULL, 0);

  root_node = xmlDocGetRootElement(doc);

  sample_node = root_node->children;
  i = 0;

  while(sample_node != NULL){
    if(sample_node->type == XML_ELEMENT_NODE){
      if(!xmlStrncmp("sample",
		     sample_node->name,
		     5)){
	current_node = sample_node->children;

	while(current_node != NULL){
	  if(current_node->type == XML_ELEMENT_NODE){
	    if(!xmlStrncmp("file",
			   current_node->name,
			   5)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->techno_sample[i] = g_strdup(str);
	    }else if(!xmlStrncmp("label",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->techno_label[i] = g_strdup(str);
	    }
	  }

	  current_node = current_node->next;
	}

	i++;
      }
    }

    sample_node = sample_node->next;
  }

  /* free XML doc */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();
  
  /* house resources */
  diskjokey_sequencer_model->house_label = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));
  diskjokey_sequencer_model->house_sample = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_model->house_label[i] = NULL;
    diskjokey_sequencer_model->house_sample[i] = NULL;
  }
  
  diskjokey_sequencer_model->house_label[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;
  diskjokey_sequencer_model->house_sample[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;

  house_filename = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME;
  
  doc = xmlReadFile(house_filename, NULL, 0);

  root_node = xmlDocGetRootElement(doc);

  sample_node = root_node->children;
  i = 0;

  while(sample_node != NULL){
    if(sample_node->type == XML_ELEMENT_NODE){
      if(!xmlStrncmp("sample",
		     sample_node->name,
		     5)){
	current_node = sample_node->children;

	while(current_node != NULL){
	  if(current_node->type == XML_ELEMENT_NODE){
	    if(!xmlStrncmp("file",
			   current_node->name,
			   5)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->house_sample[i] = g_strdup(str);
	    }else if(!xmlStrncmp("label",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->house_label[i] = g_strdup(str);
	    }
	  }

	  current_node = current_node->next;
	}

	i++;
      }
    }

    sample_node = sample_node->next;
  }

  /* free XML doc */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

  /* hip-hop resources */
  diskjokey_sequencer_model->hiphop_label = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));
  diskjokey_sequencer_model->hiphop_sample = (gchar **) malloc((MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT + 1) * sizeof(gchar *));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_model->hiphop_label[i] = NULL;
    diskjokey_sequencer_model->hiphop_sample[i] = NULL;
  }
  
  diskjokey_sequencer_model->hiphop_label[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;
  diskjokey_sequencer_model->hiphop_sample[MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT] = NULL;

  hiphop_filename = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME;

  doc = xmlReadFile(hiphop_filename, NULL, 0);

  root_node = xmlDocGetRootElement(doc);

  sample_node = root_node->children;
  i = 0;

  while(sample_node != NULL){
    if(sample_node->type == XML_ELEMENT_NODE){
      if(!xmlStrncmp("sample",
		     sample_node->name,
		     5)){
	current_node = sample_node->children;

	while(current_node != NULL){
	  if(current_node->type == XML_ELEMENT_NODE){
	    if(!xmlStrncmp("file",
			   current_node->name,
			   5)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->hiphop_sample[i] = g_strdup(str);
	    }else if(!xmlStrncmp("label",
				 current_node->name,
				 6)){
	      str = xmlNodeGetContent(current_node);

	      diskjokey_sequencer_model->hiphop_label[i] = g_strdup(str);
	    }
	  }

	  current_node = current_node->next;
	}

	i++;
      }
    }

    sample_node = sample_node->next;
  }

  /* free XML doc */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

  /* duration */
  diskjokey_sequencer_model->duration = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_sequencer_model->duration->tv_sec = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_DEFAULT_DURATION_SEC;
  diskjokey_sequencer_model->duration->tv_nsec = 0;    
}

void
monothek_diskjokey_sequencer_model_set_property(GObject *gobject,
						guint prop_id,
						const GValue *value,
						GParamSpec *param_spec)
{
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  diskjokey_sequencer_model = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL(gobject);

  switch(prop_id){
  case PROP_TECHNO_ACTIVE:
    {
      diskjokey_sequencer_model->techno_active = g_value_get_boolean(value);
    }
    break;
  case PROP_HOUSE_ACTIVE:
    {
      diskjokey_sequencer_model->house_active = g_value_get_boolean(value);
    }
    break;
  case PROP_HIPHOP_ACTIVE:
    {
      diskjokey_sequencer_model->hiphop_active = g_value_get_boolean(value);
    }
    break;
  case PROP_RANDOM_ACTIVE:
    {
      diskjokey_sequencer_model->random_active = g_value_get_boolean(value);
    }
    break;
  case PROP_CLEAR_ACTIVE:
    {
      diskjokey_sequencer_model->clear_active = g_value_get_boolean(value);
    }
    break;
  case PROP_RUN_ACTIVE:
    {
      diskjokey_sequencer_model->run_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_sequencer_model_get_property(GObject *gobject,
						guint prop_id,
						GValue *value,
						GParamSpec *param_spec)
{
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  diskjokey_sequencer_model = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL(gobject);

  switch(prop_id){
  case PROP_TECHNO_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->techno_active);
    }
    break;
  case PROP_HOUSE_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->house_active);
    }
    break;
  case PROP_HIPHOP_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->hiphop_active);
    }
    break;
  case PROP_RANDOM_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->random_active);
    }
    break;
  case PROP_CLEAR_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->clear_active);
    }
    break;
  case PROP_RUN_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_sequencer_model->run_active);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_sequencer_model_finalize(GObject *gobject)
{
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  diskjokey_sequencer_model = (MonothekDiskjokeySequencerModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_sequencer_model_parent_class)->finalize(gobject);
}

/**
 * monothek_diskjokey_sequencer_model_set_pad_active:
 * @diskjokey_sequencer_model: the #MonothekDiskjokeySequencerModel
 * @x: the pad's x position
 * @y: the pad's y position
 * @is_active: if %TRUE is active, otherwise %FALSE inactive
 * 
 * Set pad active at position @x and @y.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_model_set_pad_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						  guint x, guint y,
						  gboolean is_active)
{
  diskjokey_sequencer_model->pad_active[y][x] = is_active;
}

/**
 * monothek_diskjokey_sequencer_model_set_pad_active:
 * @diskjokey_sequencer_model: the #MonothekDiskjokeySequencerModel
 * @x: the pad's x position
 * @y: the pad's y position
 * 
 * Get pad active at position @x and @y.
 * 
 * Returns: %TRUE if active, otherwise %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_diskjokey_sequencer_model_get_pad_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						  guint x, guint y)
{
  gboolean is_active;
  
  is_active = diskjokey_sequencer_model->pad_active[y][x];

  return(is_active);
}

/**
 * monothek_diskjokey_sequencer_model_set_tab_active:
 * @diskjokey_sequencer_model: the #MonothekDiskjokeySequencerModel
 * @x: the tab's x position
 * @is_active: if %TRUE is active, otherwise %FALSE inactive
 * 
 * Set tab active at position @x.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_model_set_tab_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						  guint x,
						  gboolean is_active)
{
  diskjokey_sequencer_model->tab_active[x] = is_active;
}

/**
 * monothek_diskjokey_sequencer_model_set_tab_active:
 * @diskjokey_sequencer_model: the #MonothekDiskjokeySequencerModel
 * @x: the tab's x position
 * 
 * Get tab active at position @x.
 * 
 * Returns: %TRUE if active, otherwise %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_diskjokey_sequencer_model_get_tab_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						  guint x)
{
  gboolean is_active;
  
  is_active = diskjokey_sequencer_model->tab_active[x];

  return(is_active);
}

/**
 * monothek_diskjokey_sequencer_model_new:
 *
 * Creates an #MonothekDiskjokeySequencerModel
 *
 * Returns: a new #MonothekDiskjokeySequencerModel
 *
 * Since: 1.0.0
 */
MonothekDiskjokeySequencerModel*
monothek_diskjokey_sequencer_model_new()
{
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  diskjokey_sequencer_model = (MonothekDiskjokeySequencerModel *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL,
									       NULL);
  
  return(diskjokey_sequencer_model);
}
