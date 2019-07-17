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

#include <monothek/ui/controller/monothek_banner_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_banner_controller_class_init(MonothekBannerControllerClass *banner_controller);
void monothek_banner_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_banner_controller_init(MonothekBannerController *banner_controller);
void monothek_banner_controller_finalize(GObject *gobject);

void monothek_banner_controller_connect(AgsConnectable *connectable);
void monothek_banner_controller_disconnect(AgsConnectable *connectable);

void monothek_banner_controller_reset(MonothekController *controller);

/**
 * SECTION:monothek_banner_controller
 * @short_description: The banner controller object.
 * @title: MonothekBannerController
 * @section_id:
 * @include: monothek/ui/controller/monothek_banner_controller.h
 *
 * #MonothekBannerController is the MVC's banner controller.
 */

enum{
  LAST_SIGNAL,
};

static gpointer monothek_banner_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_banner_controller_parent_connectable_interface;

static guint banner_controller_signals[LAST_SIGNAL];

GType
monothek_banner_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_banner_controller = 0;

    static const GTypeInfo monothek_banner_controller_info = {
      sizeof (MonothekBannerControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_banner_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekBannerController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_banner_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_banner_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_banner_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							     "MonothekBannerController", &monothek_banner_controller_info,
							     0);

    g_type_add_interface_static(monothek_type_banner_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_banner_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_banner_controller_class_init(MonothekBannerControllerClass *banner_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_banner_controller_parent_class = g_type_class_peek_parent(banner_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) banner_controller;

  gobject->finalize = monothek_banner_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) banner_controller;
  
  controller->reset = monothek_banner_controller_reset;
}

void
monothek_banner_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_banner_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_banner_controller_connect;
  connectable->disconnect = monothek_banner_controller_disconnect;
}

void
monothek_banner_controller_init(MonothekBannerController *banner_controller)
{
  //TODO:JK: implement me
}

void
monothek_banner_controller_finalize(GObject *gobject)
{
  MonothekBannerController *banner_controller;

  banner_controller = (MonothekBannerController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_banner_controller_parent_class)->finalize(gobject);
}

void
monothek_banner_controller_connect(AgsConnectable *connectable)
{
  MonothekBannerController *banner_controller;

  banner_controller = MONOTHEK_BANNER_CONTROLLER(connectable);

  if(monothek_controller_test_flags(banner_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_banner_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_banner_controller_disconnect(AgsConnectable *connectable)
{
  MonothekBannerController *banner_controller;

  banner_controller = MONOTHEK_BANNER_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(banner_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_banner_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_banner_controller_reset(MonothekController *controller)
{
  //TODO:JK: implemente me
}

/**
 * monothek_banner_controller_new:
 *
 * Creates an #MonothekBannerController
 *
 * Returns: a new #MonothekBannerController
 *
 * Since: 1.0.0
 */
MonothekBannerController*
monothek_banner_controller_new()
{
  MonothekBannerController *banner_controller;

  banner_controller = (MonothekBannerController *) g_object_new(MONOTHEK_TYPE_BANNER_CONTROLLER,
								NULL);
  
  return(banner_controller);
}
