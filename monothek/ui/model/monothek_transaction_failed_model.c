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

#include <monothek/ui/model/monothek_transaction_failed_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_transaction_failed_model_class_init(MonothekTransactionFailedModelClass *transaction_failed_model);
void monothek_transaction_failed_model_init(MonothekTransactionFailedModel *transaction_failed_model);
void monothek_transaction_failed_model_set_property(GObject *gobject,
						    guint prop_id,
						    const GValue *value,
						    GParamSpec *param_spec);
void monothek_transaction_failed_model_get_property(GObject *gobject,
						    guint prop_id,
						    GValue *value,
						    GParamSpec *param_spec);
void monothek_transaction_failed_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_transaction_failed_model
 * @short_description: The transaction failed model object.
 * @title: MonothekTransactionFailedModel
 * @section_id:
 * @include: monothek/ui/model/monothek_transaction_failed_model.h
 *
 * #MonothekTransactionFailedModel is the MVC's transaction failed model.
 */

enum{
  PROP_0,
  PROP_DURATION,
};

static gpointer monothek_transaction_failed_model_parent_class = NULL;

GType
monothek_transaction_failed_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_transaction_failed_model = 0;

    static const GTypeInfo monothek_transaction_failed_model_info = {
      sizeof (MonothekTransactionFailedModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_transaction_failed_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekTransactionFailedModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_transaction_failed_model_init,
    };

    monothek_type_transaction_failed_model = g_type_register_static(G_TYPE_OBJECT,
								    "MonothekTransactionFailedModel", &monothek_transaction_failed_model_info,
								    0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_transaction_failed_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_transaction_failed_model_class_init(MonothekTransactionFailedModelClass *transaction_failed_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_transaction_failed_model_parent_class = g_type_class_peek_parent(transaction_failed_model);

  /* GObjectClass */
  gobject = (GObjectClass *) transaction_failed_model;

  gobject->set_property = monothek_transaction_failed_model_set_property;
  gobject->get_property = monothek_transaction_failed_model_get_property;

  gobject->finalize = monothek_transaction_failed_model_finalize;

  /* properties */
  /**
   * MonothekTransactionFailedModel:duration:
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
monothek_transaction_failed_model_init(MonothekTransactionFailedModel *transaction_failed_model)
{
  /* duration */
  transaction_failed_model->duration = (struct timespec *) malloc(sizeof(struct timespec));
  transaction_failed_model->duration->tv_sec = MONOTHEK_TRANSACTION_FAILED_MODEL_DEFAULT_DURATION_SEC;
  transaction_failed_model->duration->tv_nsec = 0;
}

void
monothek_transaction_failed_model_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec)
{
  MonothekTransactionFailedModel *transaction_failed_model;

  transaction_failed_model = MONOTHEK_TRANSACTION_FAILED_MODEL(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_transaction_failed_model_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec)
{
  MonothekTransactionFailedModel *transaction_failed_model;

  transaction_failed_model = MONOTHEK_TRANSACTION_FAILED_MODEL(gobject);

  switch(prop_id){
  case PROP_DURATION:
    {
      g_value_set_pointer(value, transaction_failed_model->duration);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_transaction_failed_model_finalize(GObject *gobject)
{
  MonothekTransactionFailedModel *transaction_failed_model;

  transaction_failed_model = (MonothekTransactionFailedModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_transaction_failed_model_parent_class)->finalize(gobject);
}

/**
 * monothek_transaction_failed_model_new:
 *
 * Creates an #MonothekTransactionFailedModel
 *
 * Returns: a new #MonothekTransactionFailedModel
 *
 * Since: 1.0.0
 */
MonothekTransactionFailedModel*
monothek_transaction_failed_model_new()
{
  MonothekTransactionFailedModel *transaction_failed_model;

  transaction_failed_model = (MonothekTransactionFailedModel *) g_object_new(MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL,
									     NULL);
  
  return(transaction_failed_model);
}
