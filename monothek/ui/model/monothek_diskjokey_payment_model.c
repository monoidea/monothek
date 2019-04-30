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

#include <monothek/ui/model/monothek_diskjokey_payment_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_payment_model_class_init(MonothekDiskjokeyPaymentModelClass *diskjokey_payment_model);
void monothek_diskjokey_payment_model_init(MonothekDiskjokeyPaymentModel *diskjokey_payment_model);
void monothek_diskjokey_payment_model_set_property(GObject *gobject,
						   guint prop_id,
						   const GValue *value,
						   GParamSpec *param_spec);
void monothek_diskjokey_payment_model_get_property(GObject *gobject,
						   guint prop_id,
						   GValue *value,
						   GParamSpec *param_spec);
void monothek_diskjokey_payment_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_diskjokey_payment_model
 * @short_description: The start model object.
 * @title: MonothekDiskjokeyPaymentModel
 * @section_id:
 * @include: monothek/ui/model/monothek_diskjokey_payment_model.h
 *
 * #MonothekDiskjokeyPaymentModel is the MVC's payment model.
 */

enum{
  PROP_0,
  PROP_CONTINUE_SESSION,
};

static gpointer monothek_diskjokey_payment_model_parent_class = NULL;

GType
monothek_diskjokey_payment_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_payment_model = 0;

    static const GTypeInfo monothek_diskjokey_payment_model_info = {
      sizeof (MonothekDiskjokeyPaymentModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_payment_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeyPaymentModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_payment_model_init,
    };

    monothek_type_diskjokey_payment_model = g_type_register_static(G_TYPE_OBJECT,
								   "MonothekDiskjokeyPaymentModel", &monothek_diskjokey_payment_model_info,
								   0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_payment_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_payment_model_class_init(MonothekDiskjokeyPaymentModelClass *diskjokey_payment_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_diskjokey_payment_model_parent_class = g_type_class_peek_parent(diskjokey_payment_model);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_payment_model;

  gobject->set_property = monothek_diskjokey_payment_model_set_property;
  gobject->get_property = monothek_diskjokey_payment_model_get_property;

  gobject->finalize = monothek_diskjokey_payment_model_finalize;

  /* properties */
  /**
   * MonothekDiskjokeySequencerModel:continue-session:
   *
   * If to continue the session.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("continue-session",
				    i18n_pspec("continue session"),
				    i18n_pspec("If to continue the session"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_CONTINUE_SESSION,
				  param_spec);

  /* MonothekModel */
}

void
monothek_diskjokey_payment_model_init(MonothekDiskjokeyPaymentModel *diskjokey_payment_model)
{
  diskjokey_payment_model->continue_session = FALSE;
}

void
monothek_diskjokey_payment_model_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec)
{
  MonothekDiskjokeyPaymentModel *diskjokey_payment_model;

  diskjokey_payment_model = MONOTHEK_DISKJOKEY_PAYMENT_MODEL(gobject);

  switch(prop_id){
  case PROP_CONTINUE_SESSION:
    {
      diskjokey_payment_model->continue_session = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_payment_model_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec)
{
  MonothekDiskjokeyPaymentModel *diskjokey_payment_model;

  diskjokey_payment_model = MONOTHEK_DISKJOKEY_PAYMENT_MODEL(gobject);

  switch(prop_id){
  case PROP_CONTINUE_SESSION:
    {
      g_value_set_boolean(value,
			  diskjokey_payment_model->continue_session);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_payment_model_finalize(GObject *gobject)
{
  MonothekDiskjokeyPaymentModel *diskjokey_payment_model;

  diskjokey_payment_model = (MonothekDiskjokeyPaymentModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_payment_model_parent_class)->finalize(gobject);
}

/**
 * monothek_diskjokey_payment_model_new:
 *
 * Creates an #MonothekDiskjokeyPaymentModel
 *
 * Returns: a new #MonothekDiskjokeyPaymentModel
 *
 * Since: 1.0.0
 */
MonothekDiskjokeyPaymentModel*
monothek_diskjokey_payment_model_new()
{
  MonothekDiskjokeyPaymentModel *diskjokey_payment_model;

  diskjokey_payment_model = (MonothekDiskjokeyPaymentModel *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL,
									   NULL);
  
  return(diskjokey_payment_model);
}
