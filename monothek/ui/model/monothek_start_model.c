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

#include <monothek/ui/model/monothek_start_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_start_model_class_init(MonothekStartModelClass *start_model);
void monothek_start_model_init(MonothekStartModel *start_model);
void monothek_start_model_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec);
void monothek_start_model_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec);
void monothek_start_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_start_model
 * @short_description: The start model object.
 * @title: MonothekStartModel
 * @section_id:
 * @include: monothek/ui/model/monothek_start_model.h
 *
 * #MonothekStartModel is the MVC's start model.
 */

enum{
  PROP_0,
  PROP_NTH_POSITION,
  PROP_JUKEBOX_START_ACTIVE,
  PROP_DISKJOKEY_START_ACTIVE,
};

static gpointer monothek_start_model_parent_class = NULL;

GType
monothek_start_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_start_model = 0;

    static const GTypeInfo monothek_start_model_info = {
      sizeof (MonothekStartModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_start_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekStartModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_start_model_init,
    };

    monothek_type_start_model = g_type_register_static(G_TYPE_OBJECT,
						       "MonothekStartModel", &monothek_start_model_info,
						       0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_start_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_start_model_class_init(MonothekStartModelClass *start_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_start_model_parent_class = g_type_class_peek_parent(start_model);

  /* GObjectClass */
  gobject = (GObjectClass *) start_model;

  gobject->set_property = monothek_start_model_set_property;
  gobject->get_property = monothek_start_model_get_property;

  gobject->finalize = monothek_start_model_finalize;

  /* properties */
  /**
   * MonothekStartModel:nth-position:
   *
   * The nth position.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("nth-position",
				 i18n_pspec("nth position"),
				 i18n_pspec("nth position"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_NTH_POSITION,
				  param_spec);

  /**
   * MonothekStartModel:jukebox-start-active:
   *
   * If jukebox start is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("jukebox-start-active",
				    i18n_pspec("jukebox start active"),
				    i18n_pspec("If jukebox start is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_START_ACTIVE,
				  param_spec);

  /**
   * MonothekStartModel:diskjokey-start-active:
   *
   * If diskjokey start is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("diskjokey-start-active",
				    i18n_pspec("diskjokey start active"),
				    i18n_pspec("If diskjokey start is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_DISKJOKEY_START_ACTIVE,
				  param_spec);

  /* MonothekModel */
}

void
monothek_start_model_init(MonothekStartModel *start_model)
{
  start_model->nth_position = 0;

  start_model->jukebox_start_active = FALSE;

  start_model->diskjokey_start_active = FALSE;
}

void
monothek_start_model_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec)
{
  MonothekStartModel *start_model;

  start_model = MONOTHEK_START_MODEL(gobject);

  switch(prop_id){
  case PROP_NTH_POSITION:
    {
      start_model->nth_position = g_value_get_uint(value);
    }
    break;
  case PROP_JUKEBOX_START_ACTIVE:
    {
      start_model->jukebox_start_active = g_value_get_boolean(value);
    }
    break;
  case PROP_DISKJOKEY_START_ACTIVE:
    {
      start_model->diskjokey_start_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_model_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec)
{
  MonothekStartModel *start_model;

  start_model = MONOTHEK_START_MODEL(gobject);

  switch(prop_id){
  case PROP_NTH_POSITION:
    {
      g_value_set_uint(value,
		       start_model->nth_position);
    }
    break;
  case PROP_JUKEBOX_START_ACTIVE:
    {
      g_value_set_boolean(value,
			  start_model->jukebox_start_active);
    }
    break;
  case PROP_DISKJOKEY_START_ACTIVE:
    {
      g_value_set_boolean(value,
			  start_model->diskjokey_start_active);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_model_finalize(GObject *gobject)
{
  MonothekStartModel *start_model;

  start_model = (MonothekStartModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_start_model_parent_class)->finalize(gobject);
}

/**
 * monothek_start_model_new:
 *
 * Creates an #MonothekStartModel
 *
 * Returns: a new #MonothekStartModel
 *
 * Since: 1.0.0
 */
MonothekStartModel*
monothek_start_model_new()
{
  MonothekStartModel *start_model;

  start_model = (MonothekStartModel *) g_object_new(MONOTHEK_TYPE_START_MODEL,
						    NULL);
  
  return(start_model);
}
