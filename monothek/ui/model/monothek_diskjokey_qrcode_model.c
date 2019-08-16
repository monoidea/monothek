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

#include <monothek/ui/model/monothek_diskjokey_qrcode_model.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_qrcode_model_class_init(MonothekDiskjokeyQrcodeModelClass *diskjokey_qrcode_model);
void monothek_diskjokey_qrcode_model_init(MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model);
void monothek_diskjokey_qrcode_model_set_property(GObject *gobject,
						  guint prop_id,
						  const GValue *value,
						  GParamSpec *param_spec);
void monothek_diskjokey_qrcode_model_get_property(GObject *gobject,
						  guint prop_id,
						  GValue *value,
						  GParamSpec *param_spec);
void monothek_diskjokey_qrcode_model_finalize(GObject *gobject);

/**
 * SECTION:monothek_diskjokey_qrcode_model
 * @short_description: The start model object.
 * @title: MonothekDiskjokeyQrcodeModel
 * @section_id:
 * @include: monothek/ui/model/monothek_diskjokey_qrcode_model.h
 *
 * #MonothekDiskjokeyQrcodeModel is the MVC's start model.
 */

enum{
  PROP_0,
  PROP_QUIT_ACTIVE,
  PROP_DURATION,
};

static gpointer monothek_diskjokey_qrcode_model_parent_class = NULL;

GType
monothek_diskjokey_qrcode_model_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_qrcode_model = 0;

    static const GTypeInfo monothek_diskjokey_qrcode_model_info = {
      sizeof (MonothekDiskjokeyQrcodeModelClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_qrcode_model_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeyQrcodeModel),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_qrcode_model_init,
    };

    monothek_type_diskjokey_qrcode_model = g_type_register_static(G_TYPE_OBJECT,
								  "MonothekDiskjokeyQrcodeModel", &monothek_diskjokey_qrcode_model_info,
								  0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_qrcode_model);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_qrcode_model_class_init(MonothekDiskjokeyQrcodeModelClass *diskjokey_qrcode_model)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_diskjokey_qrcode_model_parent_class = g_type_class_peek_parent(diskjokey_qrcode_model);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_qrcode_model;

  gobject->set_property = monothek_diskjokey_qrcode_model_set_property;
  gobject->get_property = monothek_diskjokey_qrcode_model_get_property;

  gobject->finalize = monothek_diskjokey_qrcode_model_finalize;

  /* properties */
  /**
   * MonothekDiskjokeyQrcodeModel:quit-active:
   *
   * If quit is active.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_boolean("quit-active",
				    i18n_pspec("quit active"),
				    i18n_pspec("If quit is active"),
				    FALSE,
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_QUIT_ACTIVE,
				  param_spec);

  /**
   * MonothekDiskjokeyQrcodeModel:duration:
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
monothek_diskjokey_qrcode_model_init(MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model)
{
  diskjokey_qrcode_model->quit_active = FALSE;

  /* duration */
  diskjokey_qrcode_model->duration = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_qrcode_model->duration->tv_sec = MONOTHEK_DISKJOKEY_QRCODE_MODEL_DEFAULT_DURATION_SEC;
  diskjokey_qrcode_model->duration->tv_nsec = 0;
}

void
monothek_diskjokey_qrcode_model_set_property(GObject *gobject,
					     guint prop_id,
					     const GValue *value,
					     GParamSpec *param_spec)
{
  MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model;

  diskjokey_qrcode_model = MONOTHEK_DISKJOKEY_QRCODE_MODEL(gobject);

  switch(prop_id){
  case PROP_QUIT_ACTIVE:
    {
      diskjokey_qrcode_model->quit_active = g_value_get_boolean(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_qrcode_model_get_property(GObject *gobject,
					     guint prop_id,
					     GValue *value,
					     GParamSpec *param_spec)
{
  MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model;

  diskjokey_qrcode_model = MONOTHEK_DISKJOKEY_QRCODE_MODEL(gobject);

  switch(prop_id){
  case PROP_QUIT_ACTIVE:
    {
      g_value_set_boolean(value,
			  diskjokey_qrcode_model->quit_active);
    }
    break;
  case PROP_DURATION:
    {
      g_value_set_pointer(value, diskjokey_qrcode_model->duration);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_qrcode_model_finalize(GObject *gobject)
{
  MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model;

  diskjokey_qrcode_model = (MonothekDiskjokeyQrcodeModel *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_qrcode_model_parent_class)->finalize(gobject);
}

/**
 * monothek_diskjokey_qrcode_model_new:
 *
 * Creates an #MonothekDiskjokeyQrcodeModel
 *
 * Returns: a new #MonothekDiskjokeyQrcodeModel
 *
 * Since: 1.0.0
 */
MonothekDiskjokeyQrcodeModel*
monothek_diskjokey_qrcode_model_new()
{
  MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model;

  diskjokey_qrcode_model = (MonothekDiskjokeyQrcodeModel *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL,
									 NULL);
  
  return(diskjokey_qrcode_model);
}
