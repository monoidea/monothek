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

#include <monothek/ui/controller/monothek_credits_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_credits_controller_class_init(MonothekCreditsControllerClass *credits_controller);
void monothek_credits_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_credits_controller_init(MonothekCreditsController *credits_controller);
void monothek_credits_controller_finalize(GObject *gobject);

void monothek_credits_controller_connect(AgsConnectable *connectable);
void monothek_credits_controller_disconnect(AgsConnectable *connectable);

void monothek_credits_controller_reset(MonothekController *controller);

/**
 * SECTION:monothek_credits_controller
 * @short_description: The credits controller object.
 * @title: MonothekCreditsController
 * @section_id:
 * @include: monothek/ui/controller/monothek_credits_controller.h
 *
 * #MonothekCreditsController is the MVC's credits controller.
 */

enum{
  LAST_SIGNAL,
};

static gpointer monothek_credits_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_credits_controller_parent_connectable_interface;

static guint credits_controller_signals[LAST_SIGNAL];

GType
monothek_credits_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_credits_controller = 0;

    static const GTypeInfo monothek_credits_controller_info = {
      sizeof (MonothekCreditsControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_credits_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCreditsController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_credits_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_credits_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_credits_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							      "MonothekCreditsController", &monothek_credits_controller_info,
							      0);

    g_type_add_interface_static(monothek_type_credits_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_credits_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_credits_controller_class_init(MonothekCreditsControllerClass *credits_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_credits_controller_parent_class = g_type_class_peek_parent(credits_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) credits_controller;

  gobject->finalize = monothek_credits_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) credits_controller;
  
  controller->reset = monothek_credits_controller_reset;
}

void
monothek_credits_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_credits_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_credits_controller_connect;
  connectable->disconnect = monothek_credits_controller_disconnect;
}

void
monothek_credits_controller_init(MonothekCreditsController *credits_controller)
{
  //TODO:JK: implement me
}

void
monothek_credits_controller_finalize(GObject *gobject)
{
  MonothekCreditsController *credits_controller;

  credits_controller = (MonothekCreditsController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_credits_controller_parent_class)->finalize(gobject);
}

void
monothek_credits_controller_connect(AgsConnectable *connectable)
{
  MonothekCreditsController *credits_controller;

  credits_controller = MONOTHEK_CREDITS_CONTROLLER(connectable);

  if(monothek_controller_test_flags(credits_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_credits_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_credits_controller_disconnect(AgsConnectable *connectable)
{
  MonothekCreditsController *credits_controller;

  credits_controller = MONOTHEK_CREDITS_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(credits_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_credits_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_credits_controller_reset(MonothekController *controller)
{
  //TODO:JK: implemente me
}

/**
 * monothek_credits_controller_new:
 *
 * Creates an #MonothekCreditsController
 *
 * Returns: a new #MonothekCreditsController
 *
 * Since: 1.0.0
 */
MonothekCreditsController*
monothek_credits_controller_new()
{
  MonothekCreditsController *credits_controller;

  credits_controller = (MonothekCreditsController *) g_object_new(MONOTHEK_TYPE_CREDITS_CONTROLLER,
								  NULL);
  
  return(credits_controller);
}
