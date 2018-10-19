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

#include <monothek/ui/model/monothek_closed_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_closed_model_class_init(MonothekClosedModelClass *closed_model);
void monothek_closed_model_init(MonothekClosedModel *closed_model);
void monothek_closed_model_set_property(GObject *gobject,
					guint prop_id,
					const GValue *value,
					GParamSpec *param_spec);
void monothek_closed_model_get_property(GObject *gobject,
					guint prop_id,
					GValue *value,
					GParamSpec *param_spec);
void monothek_closed_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_closed_model
 * @short_description: The closed model object.
 * @title: MonothekClosedModel
 * @section_id:
 * @include: monothek/ui/model/monothek_closed_model.h
 *
 * #MonothekClosedModel is the MVC's closed model.
 */

enum{
  PROP_0,
};

static gpointer monothek_closed_model_parent_class = NULL;

GType
monothek_closed_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_closed_model = 0;

    static const GTypeInfo monothek_closed_model_info = {
      sizeof (MonothekClosedModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_closed_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekClosedModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_closed_model_init,
    };

    monothek_type_closed_model = g_type_register_static(G_TYPE_OBJECT,
							"MonothekClosedModel", &monothek_closed_model_info,
							0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_closed_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_closed_model_class_init(MonothekClosedModelClass *closed_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_closed_model_parent_class = g_type_class_peek_parent(closed_model);

  /* GObjectClass */
  gobject = (GObjectClass *) closed_model;

  gobject->set_property = monothek_closed_model_set_property;
  gobject->get_property = monothek_closed_model_get_property;

  gobject->finalize = monothek_closed_model_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_closed_model_init(MonothekClosedModel *closed_model)
{
  //TODO:JK: implement me
}

void
monothek_closed_model_set_property(GObject *gobject,
				   guint prop_id,
				   const GValue *value,
				   GParamSpec *param_spec)
{
  MonothekClosedModel *closed_model;

  closed_model = MONOTHEK_CLOSED_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_closed_model_get_property(GObject *gobject,
				   guint prop_id,
				   GValue *value,
				   GParamSpec *param_spec)
{
  MonothekClosedModel *closed_model;

  closed_model = MONOTHEK_CLOSED_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_closed_model_finalize(GObject *gobject)
{
  MonothekClosedModel *closed_model;

  closed_model = (MonothekClosedModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_closed_model_parent_class)->finalize(gobject);
}

/**
 * monothek_closed_model_new:
 *
 * Creates an #MonothekClosedModel
 *
 * Returns: a new #MonothekClosedModel
 *
 * Since: 1.0.0
 */
MonothekClosedModel*
monothek_closed_model_new()
{
  MonothekClosedModel *closed_model;

  closed_model = (MonothekClosedModel *) g_object_new(MONOTHEK_TYPE_CLOSED_MODEL,
						      NULL);
  
  return(closed_model);
}
