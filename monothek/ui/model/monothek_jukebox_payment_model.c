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

#include <monothek/ui/model/monothek_jukebox_payment_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_payment_model_class_init(MonothekJukeboxPaymentModelClass *jukebox_payment_model);
void monothek_jukebox_payment_model_init(MonothekJukeboxPaymentModel *jukebox_payment_model);
void monothek_jukebox_payment_model_set_property(GObject *gobject,
						 guint prop_id,
						 const GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_payment_model_get_property(GObject *gobject,
						 guint prop_id,
						 GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_payment_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_jukebox_payment_model
 * @short_description: The start model object.
 * @title: MonothekJukeboxPaymentModel
 * @section_id:
 * @include: monothek/ui/model/monothek_jukebox_payment_model.h
 *
 * #MonothekJukeboxPaymentModel is the MVC's start model.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_payment_model_parent_class = NULL;

GType
monothek_jukebox_payment_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_payment_model = 0;

    static const GTypeInfo monothek_jukebox_payment_model_info = {
      sizeof (MonothekJukeboxPaymentModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_payment_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPaymentModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_payment_model_init,
    };

    monothek_type_jukebox_payment_model = g_type_register_static(G_TYPE_OBJECT,
								 "MonothekJukeboxPaymentModel", &monothek_jukebox_payment_model_info,
								 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_payment_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_payment_model_class_init(MonothekJukeboxPaymentModelClass *jukebox_payment_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_jukebox_payment_model_parent_class = g_type_class_peek_parent(jukebox_payment_model);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_payment_model;

  gobject->set_property = monothek_jukebox_payment_model_set_property;
  gobject->get_property = monothek_jukebox_payment_model_get_property;

  gobject->finalize = monothek_jukebox_payment_model_finalize;

  /* properties */

  /* MonothekModel */
}

void
monothek_jukebox_payment_model_init(MonothekJukeboxPaymentModel *jukebox_payment_model)
{
  //TODO:JK: implement me
}

void
monothek_jukebox_payment_model_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxPaymentModel *jukebox_payment_model;

  jukebox_payment_model = MONOTHEK_JUKEBOX_PAYMENT_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_payment_model_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxPaymentModel *jukebox_payment_model;

  jukebox_payment_model = MONOTHEK_JUKEBOX_PAYMENT_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_payment_model_finalize(GObject *gobject)
{
  MonothekJukeboxPaymentModel *jukebox_payment_model;

  jukebox_payment_model = (MonothekJukeboxPaymentModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_payment_model_parent_class)->finalize(gobject);
}

/**
 * monothek_jukebox_payment_model_new:
 *
 * Creates an #MonothekJukeboxPaymentModel
 *
 * Returns: a new #MonothekJukeboxPaymentModel
 *
 * Since: 1.0.0
 */
MonothekJukeboxPaymentModel*
monothek_jukebox_payment_model_new()
{
  MonothekJukeboxPaymentModel *jukebox_payment_model;

  jukebox_payment_model = (MonothekJukeboxPaymentModel *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL,
								       NULL);
  
  return(jukebox_payment_model);
}
