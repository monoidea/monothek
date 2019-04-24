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

#include <monothek/ui/model/monothek_jukebox_mode_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_mode_model_class_init(MonothekJukeboxModeModelClass *jukebox_mode_model);
void monothek_jukebox_mode_model_init(MonothekJukeboxModeModel *jukebox_mode_model);
void monothek_jukebox_mode_model_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_mode_model_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_mode_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_mode_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxModeModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_mode_model.h
 *
 * #MonothekJukeboxModeModel is the MVC's start model.
 */

enum{
  PROP_0,
  PROP_ATTEMPTS,
  PROP_MAX_ATTEMPTS,
  PROP_JUKEBOX_TEST_ACTIVE,
  PROP_JUKEBOX_PLAY_ACTIVE,
  PROP_JUKEBOX_CANCEL_ACTIVE,
};

static gpointer monothek_jukebox_mode_model_parent_class = NULL;

GType
monothek_jukebox_mode_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_mode_model = 0;

    static const GTypeInfo monothek_jukebox_mode_model_info = {
      sizeof (MonothekJukeboxModeModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_mode_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxModeModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_mode_model_init,
    };

    monothek_type_jukebox_mode_model = g_type_register_static(G_TYPE_OBJECT,
							      "MonothekJukeboxModeModel", &monothek_jukebox_mode_model_info,
							      0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_mode_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_mode_model_class_init(MonothekJukeboxModeModelClass *jukebox_mode_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_mode_model_parent_class = g_type_class_peek_parent(jukebox_mode_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_mode_model;

  gobject->set_property = monothek_jukebox_mode_model_set_property;
  gobject->get_property = monothek_jukebox_mode_model_get_property;

  gobject->finalize = monothek_jukebox_mode_model_finalize;

  /* properties */
  /**
   * MonothekJukeboxModeModel:attempts:
   *
   * The number of jukebox test attempts.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("attempts",
				 i18n_pspec("test attempts"),
				 i18n_pspec("Test attempts count"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ATTEMPTS,
				  param_spec);

  /**
   * MonothekJukeboxModeModel:max-attempts:
   *
   * The number of jukebox max test attempts.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("max-attempts",
				 i18n_pspec("maximum attempts"),
				 i18n_pspec("Test maximum attempts count"),
				 0,
				 G_MAXUINT32,
				 MONOTHEK_JUKEBOX_MODE_MODEL_DEFAULT_MAX_ATTEMPTS,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_MAX_ATTEMPTS,
				  param_spec);

  /**
   * MonothekJukeboxModeModel:jukebox-test-active:
   *
   * If jukebox test is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("jukebox-test-active",
				    i18n_pspec("jukebox test active"),
				    i18n_pspec("If jukebox test is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_TEST_ACTIVE,
				  param_spec);

  /**
   * MonothekJukeboxModeModel:jukebox-play-active:
   *
   * If jukebox play is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("jukebox-play-active",
				    i18n_pspec("jukebox play active"),
				    i18n_pspec("If jukebox play is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_PLAY_ACTIVE,
				  param_spec);

  /**
   * MonothekJukeboxModeModel:jukebox-cancel-active:
   *
   * If jukebox cancel is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("jukebox-cancel-active",
				    i18n_pspec("jukebox cancel active"),
				    i18n_pspec("If jukebox cancel is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_CANCEL_ACTIVE,
				  param_spec);

  /* MonothekModel */
}

void
monothek_jukebox_mode_model_init(MonothekJukeboxModeModel *jukebox_mode_model)
{
  jukebox_mode_model->attempts = 0;
  jukebox_mode_model->max_attempts = MONOTHEK_JUKEBOX_MODE_MODEL_DEFAULT_MAX_ATTEMPTS;

  jukebox_mode_model->jukebox_test_active = FALSE;
  jukebox_mode_model->jukebox_play_active = FALSE;
  jukebox_mode_model->jukebox_cancel_active = FALSE;
}

void
monothek_jukebox_mode_model_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxModeModel *jukebox_mode_model;

  jukebox_mode_model = MONOTHEK_JUKEBOX_MODE_MODEL(gobject);

  switch(prop_id){
  case PROP_ATTEMPTS:
    {
      jukebox_mode_model->attempts = g_value_get_uint(value);
    }
    break;
  case PROP_MAX_ATTEMPTS:
    {
      jukebox_mode_model->max_attempts = g_value_get_uint(value);
    }
    break;
  case PROP_JUKEBOX_TEST_ACTIVE:
    {
      jukebox_mode_model->jukebox_test_active = g_value_get_boolean(value);
    }
    break;
  case PROP_JUKEBOX_PLAY_ACTIVE:
    {
      jukebox_mode_model->jukebox_play_active = g_value_get_boolean(value);
    }
    break;
  case PROP_JUKEBOX_CANCEL_ACTIVE:
    {
      jukebox_mode_model->jukebox_cancel_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_mode_model_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxModeModel *jukebox_mode_model;

  jukebox_mode_model = MONOTHEK_JUKEBOX_MODE_MODEL(gobject);

  switch(prop_id){
  case PROP_ATTEMPTS:
    {
      g_value_set_uint(value,
		       jukebox_mode_model->attempts);
    }
    break;
  case PROP_MAX_ATTEMPTS:
    {
      g_value_set_uint(value,
		       jukebox_mode_model->max_attempts);
    }
    break;
  case PROP_JUKEBOX_TEST_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_mode_model->jukebox_test_active);
    }
    break;
  case PROP_JUKEBOX_PLAY_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_mode_model->jukebox_play_active);
    }
    break;
  case PROP_JUKEBOX_CANCEL_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_mode_model->jukebox_cancel_active);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_mode_model_finalize(GObject *gobject)
{
  MonothekJukeboxModeModel *jukebox_mode_model;

  jukebox_mode_model = (MonothekJukeboxModeModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_mode_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_mode_model_new:
 *
 * Creates an #MonothekJukeboxModeModel
 *
 * Returns: a new #MonothekJukeboxModeModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxModeModel*
monothek_jukebox_mode_model_new()
{
  MonothekJukeboxModeModel *jukebox_mode_model;

  jukebox_mode_model = (MonothekJukeboxModeModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_MODE_MODEL,
								 NULL);
  
  return(jukebox_mode_model);
}
