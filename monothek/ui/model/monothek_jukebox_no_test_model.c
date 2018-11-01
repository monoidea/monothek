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

#include <monothek/ui/model/monothek_jukebox_no_test_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_no_test_model_class_init(MonothekJukeboxNoTestModelClass *jukebox_no_test_model);
void monothek_jukebox_no_test_model_init(MonothekJukeboxNoTestModel *jukebox_no_test_model);
void monothek_jukebox_no_test_model_set_property(GObject *gobject,
						 guint prop_id,
						 const GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_no_test_model_get_property(GObject *gobject,
						 guint prop_id,
						 GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_no_test_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_no_test_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxNoTestModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_no_test_model.h
 *
 * #MonothekJukeboxNoTestModel is the MVC's start model.
 */

enum{
  PROP_0,
  PROP_JUKEBOX_PLAY_ACTIVE,
  PROP_JUKEBOX_CANCEL_ACTIVE,
};

static gpointer monothek_jukebox_no_test_model_parent_class = NULL;

GType
monothek_jukebox_no_test_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_no_test_model = 0;

    static const GTypeInfo monothek_jukebox_no_test_model_info = {
      sizeof (MonothekJukeboxNoTestModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_no_test_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxNoTestModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_no_test_model_init,
    };

    monothek_type_jukebox_no_test_model = g_type_register_static(G_TYPE_OBJECT,
								 "MonothekJukeboxNoTestModel", &monothek_jukebox_no_test_model_info,
								 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_no_test_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_no_test_model_class_init(MonothekJukeboxNoTestModelClass *jukebox_no_test_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_no_test_model_parent_class = g_type_class_peek_parent(jukebox_no_test_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_no_test_model;

  gobject->set_property = monothek_jukebox_no_test_model_set_property;
  gobject->get_property = monothek_jukebox_no_test_model_get_property;

  gobject->finalize = monothek_jukebox_no_test_model_finalize;

  /* properties */
  /**
   * MonothekJukeboxNoTestModel:jukebox-play-active:
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
   * MonothekJukeboxNoTestModel:jukebox-cancel-active:
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
monothek_jukebox_no_test_model_init(MonothekJukeboxNoTestModel *jukebox_no_test_model)
{
  jukebox_no_test_model->jukebox_play_active = FALSE;
  jukebox_no_test_model->jukebox_cancel_active = FALSE;
}

void
monothek_jukebox_no_test_model_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxNoTestModel *jukebox_no_test_model;

  jukebox_no_test_model = MONOTHEK_JUKEBOX_NO_TEST_MODEL(gobject);

  switch(prop_id){
  case PROP_JUKEBOX_PLAY_ACTIVE:
    {
      jukebox_no_test_model->jukebox_play_active = g_value_get_boolean(value);
    }
    break;
  case PROP_JUKEBOX_CANCEL_ACTIVE:
    {
      jukebox_no_test_model->jukebox_cancel_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_no_test_model_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxNoTestModel *jukebox_no_test_model;

  jukebox_no_test_model = MONOTHEK_JUKEBOX_NO_TEST_MODEL(gobject);

  switch(prop_id){
  case PROP_JUKEBOX_PLAY_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_no_test_model->jukebox_play_active);
    }
    break;
  case PROP_JUKEBOX_CANCEL_ACTIVE:
    {
      g_value_set_boolean(value,
			  jukebox_no_test_model->jukebox_cancel_active);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_no_test_model_finalize(GObject *gobject)
{
  MonothekJukeboxNoTestModel *jukebox_no_test_model;

  jukebox_no_test_model = (MonothekJukeboxNoTestModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_no_test_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_no_test_model_new:
 *
 * Creates an #MonothekJukeboxNoTestModel
 *
 * Returns: a new #MonothekJukeboxNoTestModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxNoTestModel*
monothek_jukebox_no_test_model_new()
{
  MonothekJukeboxNoTestModel *jukebox_no_test_model;

  jukebox_no_test_model = (MonothekJukeboxNoTestModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_NO_TEST_MODEL,
								      NULL);
  
  return(jukebox_no_test_model);
}
