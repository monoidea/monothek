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

#include <monothek/ui/model/monothek_credits_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_credits_model_class_init(MonothekCreditsModelClass *credits_model);
void monothek_credits_model_init(MonothekCreditsModel *credits_model);
void monothek_credits_model_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec);
void monothek_credits_model_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec);
void monothek_credits_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_credits_model
 * @short_description: The credits model object.
 * @title: MonothekCreditsModel
 * @section_id:
 * @include: monothek/ui/model/monothek_credits_model.h
 *
 * #MonothekCreditsModel is the MVC's credits model.
 */

enum{
  PROP_0,
};

static gpointer monothek_credits_model_parent_class = NULL;

GType
monothek_credits_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_credits_model = 0;

    static const GTypeInfo monothek_credits_model_info = {
      sizeof (MonothekCreditsModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_credits_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCreditsModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_credits_model_init,
    };

    monothek_type_credits_model = g_type_register_static(G_TYPE_OBJECT,
							 "MonothekCreditsModel", &monothek_credits_model_info,
							 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_credits_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_credits_model_class_init(MonothekCreditsModelClass *credits_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_credits_model_parent_class = g_type_class_peek_parent(credits_model);

  /* GObjectClass */
  gobject = (GObjectClass *) credits_model;

  gobject->set_property = monothek_credits_model_set_property;
  gobject->get_property = monothek_credits_model_get_property;

  gobject->finalize = monothek_credits_model_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_credits_model_init(MonothekCreditsModel *credits_model)
{
  //TODO:JK: implement me
}

void
monothek_credits_model_set_property(GObject *gobject,
				    guint prop_id,
				    const GValue *value,
				    GParamSpec *param_spec)
{
  MonothekCreditsModel *credits_model;

  credits_model = MONOTHEK_CREDITS_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_credits_model_get_property(GObject *gobject,
				    guint prop_id,
				    GValue *value,
				    GParamSpec *param_spec)
{
  MonothekCreditsModel *credits_model;

  credits_model = MONOTHEK_CREDITS_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_credits_model_finalize(GObject *gobject)
{
  MonothekCreditsModel *credits_model;

  credits_model = (MonothekCreditsModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_credits_model_parent_class)->finalize(gobject);
}

/**
 * monothek_credits_model_new:
 *
 * Creates an #MonothekCreditsModel
 *
 * Returns: a new #MonothekCreditsModel
 *
 * Since: 1.0.0
 */
MonothekCreditsModel*
monothek_credits_model_new()
{
  MonothekCreditsModel *credits_model;

  credits_model = (MonothekCreditsModel *) g_object_new(MONOTHEK_TYPE_CREDITS_MODEL,
							NULL);
  
  return(credits_model);
}
