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

#include <monothek/ui/controller/monothek_closed_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_closed_controller_class_init(MonothekClosedControllerClass *closed_controller);
void monothek_closed_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_closed_controller_init(MonothekClosedController *closed_controller);
void monothek_closed_controller_finalize(GObject *gobject);

void monothek_closed_controller_connect(AgsConnectable *connectable);
void monothek_closed_controller_disconnect(AgsConnectable *connectable);

void monothek_closed_controller_reset(MonothekController *controller);

/**
 * SECTION:monothek_closed_controller
 * @short_description: The closed controller object.
 * @title: MonothekClosedController
 * @section_id:
 * @include: monothek/ui/controller/monothek_closed_controller.h
 *
 * #MonothekClosedController is the MVC's closed controller.
 */

enum{
  LAST_SIGNAL,
};

static gpointer monothek_closed_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_closed_controller_parent_connectable_interface;

static guint closed_controller_signals[LAST_SIGNAL];

GType
monothek_closed_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_closed_controller = 0;

    static const GTypeInfo monothek_closed_controller_info = {
      sizeof (MonothekClosedControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_closed_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekClosedController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_closed_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_closed_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_closed_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							     "MonothekClosedController", &monothek_closed_controller_info,
							     0);

    g_type_add_interface_static(monothek_type_closed_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_closed_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_closed_controller_class_init(MonothekClosedControllerClass *closed_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_closed_controller_parent_class = g_type_class_peek_parent(closed_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) closed_controller;

  gobject->finalize = monothek_closed_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) closed_controller;
  
  controller->reset = monothek_closed_controller_reset;
}

void
monothek_closed_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_closed_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_closed_controller_connect;
  connectable->disconnect = monothek_closed_controller_disconnect;
}

void
monothek_closed_controller_init(MonothekClosedController *closed_controller)
{
  //TODO:JK: implement me
}

void
monothek_closed_controller_finalize(GObject *gobject)
{
  MonothekClosedController *closed_controller;

  closed_controller = (MonothekClosedController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_closed_controller_parent_class)->finalize(gobject);
}

void
monothek_closed_controller_connect(AgsConnectable *connectable)
{
  MonothekClosedController *closed_controller;

  closed_controller = MONOTHEK_CLOSED_CONTROLLER(connectable);

  if(monothek_controller_test_flags(closed_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_closed_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_closed_controller_disconnect(AgsConnectable *connectable)
{
  MonothekClosedController *closed_controller;

  closed_controller = MONOTHEK_CLOSED_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(closed_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_closed_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_closed_controller_reset(MonothekController *controller)
{
  //TODO:JK: implemente me
}

/**
 * monothek_closed_controller_new:
 *
 * Creates an #MonothekClosedController
 *
 * Returns: a new #MonothekClosedController
 *
 * Since: 1.0.0
 */
MonothekClosedController*
monothek_closed_controller_new()
{
  MonothekClosedController *closed_controller;

  closed_controller = (MonothekClosedController *) g_object_new(MONOTHEK_TYPE_CLOSED_CONTROLLER,
								NULL);
  
  return(closed_controller);
}
