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

#include <monothek/ui/model/monothek_outage_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_outage_model_class_init(MonothekOutageModelClass *outage_model);
void monothek_outage_model_init(MonothekOutageModel *outage_model);
void monothek_outage_model_set_property(GObject *gobject,
					guint prop_id,
					const GValue *value,
					GParamSpec *param_spec);
void monothek_outage_model_get_property(GObject *gobject,
					guint prop_id,
					GValue *value,
					GParamSpec *param_spec);
void monothek_outage_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_outage_model
 * @short_description: The outage model object.
 * @title: MonothekOutageModel
 * @section_id:
 * @include: monothek/ui/model/monothek_outage_model.h
 *
 * #MonothekOutageModel is the MVC's outage model.
 */

enum{
  PROP_0,
};

static gpointer monothek_outage_model_parent_class = NULL;

GType
monothek_outage_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_outage_model = 0;

    static const GTypeInfo monothek_outage_model_info = {
      sizeof (MonothekOutageModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_outage_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekOutageModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_outage_model_init,
    };

    monothek_type_outage_model = g_type_register_static(G_TYPE_OBJECT,
							"MonothekOutageModel", &monothek_outage_model_info,
							0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_outage_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_outage_model_class_init(MonothekOutageModelClass *outage_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_outage_model_parent_class = g_type_class_peek_parent(outage_model);

  /* GObjectClass */
  gobject = (GObjectClass *) outage_model;

  gobject->set_property = monothek_outage_model_set_property;
  gobject->get_property = monothek_outage_model_get_property;

  gobject->finalize = monothek_outage_model_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_outage_model_init(MonothekOutageModel *outage_model)
{
  //TODO:JK: implement me
}

void
monothek_outage_model_set_property(GObject *gobject,
				   guint prop_id,
				   const GValue *value,
				   GParamSpec *param_spec)
{
  MonothekOutageModel *outage_model;

  outage_model = MONOTHEK_OUTAGE_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_outage_model_get_property(GObject *gobject,
				   guint prop_id,
				   GValue *value,
				   GParamSpec *param_spec)
{
  MonothekOutageModel *outage_model;

  outage_model = MONOTHEK_OUTAGE_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_outage_model_finalize(GObject *gobject)
{
  MonothekOutageModel *outage_model;

  outage_model = (MonothekOutageModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_outage_model_parent_class)->finalize(gobject);
}

/**
 * monothek_outage_model_new:
 *
 * Creates an #MonothekOutageModel
 *
 * Returns: a new #MonothekOutageModel
 *
 * Since: 1.0.0
 */
MonothekOutageModel*
monothek_outage_model_new()
{
  MonothekOutageModel *outage_model;

  outage_model = (MonothekOutageModel *) g_object_new(MONOTHEK_TYPE_OUTAGE_MODEL,
						      NULL);
  
  return(outage_model);
}
