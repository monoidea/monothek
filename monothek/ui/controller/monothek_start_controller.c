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

#include <monothek/ui/monothek_start_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_start_controller_class_init(MonothekStartControllerClass *start_controller);
void monothek_start_controller_init(MonothekStartController *start_controller);
void monothek_start_controller_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec);
void monothek_start_controller_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec);
void monothek_start_controller_finalize(GObject *gobject);

/**
 * SECTION:monothek_start_controller
 * @short_description: The start_controller object.
 * @title: MonothekStartController
 * @section_id:
 * @include: monothek/ui/monothek_start_controller.h
 *
 * #MonothekStartController is a composite toplevel widget. It contains the
 * menubar, the machine rack and the notation editor.
 */

enum{
  PROP_0,
};

static gpointer monothek_start_controller_parent_class = NULL;

GType
monothek_start_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_start_controller = 0;

    static const GTypeInfo monothek_start_controller_info = {
      sizeof (MonothekStartControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_start_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekStartController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_start_controller_init,
    };

    monothek_type_start_controller = g_type_register_static(G_TYPE_OBJECT,
							    "MonothekStartController", &monothek_start_controller_info,
							    0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_start_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_start_controller_class_init(MonothekStartControllerClass *start_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_start_controller_parent_class = g_type_class_peek_parent(start_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) start_controller;

  gobject->set_property = monothek_start_controller_set_property;
  gobject->get_property = monothek_start_controller_get_property;

  gobject->finalize = monothek_start_controller_finalize;

  /* properties */

  /* MonothekController */
}

void
monothek_start_controller_init(MonothekStartController *start_controller)
{
  //TODO:JK: implement me
}

void
monothek_start_controller_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec)
{
  MonothekStartController *start_controller;

  start_controller = MONOTHEK_START_CONTROLLER(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_controller_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec)
{
  MonothekStartController *start_controller;

  start_controller = MONOTHEK_START_CONTROLLER(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_controller_finalize(GObject *gobject)
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_start_controller_parent_class)->finalize(gobject);
}

/**
 * monothek_start_controller_new:
 *
 * Creates an #MonothekStartController
 *
 * Returns: a new #MonothekStartController
 *
 * Since: 1.0.0
 */
MonothekStartController*
monothek_start_controller_new()
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) g_object_new(MONOTHEK_TYPE_START_CONTROLLER,
							      NULL);
  
  return(start_controller);
}
