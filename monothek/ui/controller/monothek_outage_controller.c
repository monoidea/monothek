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

#include <monothek/ui/controller/monothek_outage_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_outage_controller_class_init(MonothekOutageControllerClass *outage_controller);
void monothek_outage_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_outage_controller_init(MonothekOutageController *outage_controller);
void monothek_outage_controller_finalize(GObject *gobject);

void monothek_outage_controller_connect(AgsConnectable *connectable);
void monothek_outage_controller_disconnect(AgsConnectable *connectable);

void monothek_outage_controller_reset(MonothekController *controller);

/**
 * SECTION:monothek_outage_controller
 * @short_description: The outage controller object.
 * @title: MonothekOutageController
 * @section_id:
 * @include: monothek/ui/controller/monothek_outage_controller.h
 *
 * #MonothekOutageController is the MVC's outage controller.
 */

enum{
  LAST_SIGNAL,
};

static gpointer monothek_outage_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_outage_controller_parent_connectable_interface;

static guint outage_controller_signals[LAST_SIGNAL];

GType
monothek_outage_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_outage_controller = 0;

    static const GTypeInfo monothek_outage_controller_info = {
      sizeof (MonothekOutageControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_outage_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekOutageController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_outage_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_outage_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_outage_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							     "MonothekOutageController", &monothek_outage_controller_info,
							     0);

    g_type_add_interface_static(monothek_type_outage_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_outage_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_outage_controller_class_init(MonothekOutageControllerClass *outage_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_outage_controller_parent_class = g_type_class_peek_parent(outage_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) outage_controller;

  gobject->finalize = monothek_outage_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) outage_controller;
  
  controller->reset = monothek_outage_controller_reset;
}

void
monothek_outage_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_outage_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_outage_controller_connect;
  connectable->disconnect = monothek_outage_controller_disconnect;
}

void
monothek_outage_controller_init(MonothekOutageController *outage_controller)
{
  //TODO:JK: implement me
}

void
monothek_outage_controller_finalize(GObject *gobject)
{
  MonothekOutageController *outage_controller;

  outage_controller = (MonothekOutageController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_outage_controller_parent_class)->finalize(gobject);
}

void
monothek_outage_controller_connect(AgsConnectable *connectable)
{
  MonothekOutageController *outage_controller;

  outage_controller = MONOTHEK_OUTAGE_CONTROLLER(connectable);

  if(monothek_controller_test_flags(outage_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_outage_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_outage_controller_disconnect(AgsConnectable *connectable)
{
  MonothekOutageController *outage_controller;

  outage_controller = MONOTHEK_OUTAGE_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(outage_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_outage_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_outage_controller_reset(MonothekController *controller)
{
  //TODO:JK: implemente me
}

/**
 * monothek_outage_controller_new:
 *
 * Creates an #MonothekOutageController
 *
 * Returns: a new #MonothekOutageController
 *
 * Since: 1.0.0
 */
MonothekOutageController*
monothek_outage_controller_new()
{
  MonothekOutageController *outage_controller;

  outage_controller = (MonothekOutageController *) g_object_new(MONOTHEK_TYPE_OUTAGE_CONTROLLER,
								NULL);
  
  return(outage_controller);
}
