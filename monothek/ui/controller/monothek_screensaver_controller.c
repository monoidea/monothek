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

#include <monothek/ui/controller/monothek_screensaver_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_screensaver_model.h>

#include <monothek/ui/view/monothek_banner_view.h>
#include <monothek/ui/view/monothek_screensaver_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_screensaver_controller_class_init(MonothekScreensaverControllerClass *screensaver_controller);
void monothek_screensaver_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_screensaver_controller_init(MonothekScreensaverController *screensaver_controller);
void monothek_screensaver_controller_finalize(GObject *gobject);

void monothek_screensaver_controller_connect(AgsConnectable *connectable);
void monothek_screensaver_controller_disconnect(AgsConnectable *connectable);

void monothek_screensaver_controller_reset(MonothekController *controller);

void monothek_screensaver_controller_stop_screensaver_enter_callback(MonothekActionBox *action_box,
								     MonothekScreensaverController *screensaver_controller);
void monothek_screensaver_controller_stop_screensaver_leave_callback(MonothekActionBox *action_box,
								     MonothekScreensaverController *screensaver_controller);
void monothek_screensaver_controller_stop_screensaver_clicked_callback(MonothekActionBox *action_box,
								       MonothekScreensaverController *screensaver_controller);

void monothek_screensaver_controller_real_stop_screensaver(MonothekScreensaverController *screensaver_controller);

/**
 * SECTION:monothek_screensaver_controller
 * @short_description: The screensaver controller object.
 * @title: MonothekScreensaverController
 * @section_id:
 * @include: monothek/ui/controller/monothek_screensaver_controller.h
 *
 * #MonothekScreensaverController is the MVC's screensaver controller.
 */

enum{
  STOP_SCREENSAVER,
  LAST_SIGNAL,
};

static gpointer monothek_screensaver_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_screensaver_controller_parent_connectable_interface;

static guint screensaver_controller_signals[LAST_SIGNAL];

GType
monothek_screensaver_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_screensaver_controller = 0;

    static const GTypeInfo monothek_screensaver_controller_info = {
      sizeof (MonothekScreensaverControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_screensaver_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekScreensaverController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_screensaver_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_screensaver_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_screensaver_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								  "MonothekScreensaverController", &monothek_screensaver_controller_info,
								  0);

    g_type_add_interface_static(monothek_type_screensaver_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_screensaver_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_screensaver_controller_class_init(MonothekScreensaverControllerClass *screensaver_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_screensaver_controller_parent_class = g_type_class_peek_parent(screensaver_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) screensaver_controller;

  gobject->finalize = monothek_screensaver_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) screensaver_controller;
  
  controller->reset = monothek_screensaver_controller_reset;

  /* MonothekScreensaverController */
  screensaver_controller->stop_screensaver = monothek_screensaver_controller_real_stop_screensaver;

  /* signals */
  /**
   * MonothekScreensaverController::stop-screensaver:
   * @screensaver_controller: the #MonothekScreensaverController
   *
   * The ::stop-screensaver signal notifies about stop screensaver.
   *
   * Since: 1.0.0
   */
  screensaver_controller_signals[STOP_SCREENSAVER] =
    g_signal_new("stop-screensaver",
		 G_TYPE_FROM_CLASS(screensaver_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekScreensaverControllerClass, stop_screensaver),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_screensaver_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_screensaver_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_screensaver_controller_connect;
  connectable->disconnect = monothek_screensaver_controller_disconnect;
}

void
monothek_screensaver_controller_init(MonothekScreensaverController *screensaver_controller)
{
  MonothekActionBox *action_box;

  screensaver_controller->stop_screensaver = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "stop-screensaver",
						    "x0", 0,
						    "y0", 0,
						    "width", 1920,
						    "height", 1080,
						    NULL);
  monothek_controller_add_action_box(screensaver_controller,
				     action_box);
}

void
monothek_screensaver_controller_finalize(GObject *gobject)
{
  MonothekScreensaverController *screensaver_controller;

  screensaver_controller = (MonothekScreensaverController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_screensaver_controller_parent_class)->finalize(gobject);
}

void
monothek_screensaver_controller_connect(AgsConnectable *connectable)
{
  MonothekScreensaverController *screensaver_controller;

  screensaver_controller = MONOTHEK_SCREENSAVER_CONTROLLER(connectable);

  if(monothek_controller_test_flags(screensaver_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_screensaver_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(screensaver_controller->stop_screensaver, "enter",
		   G_CALLBACK(monothek_screensaver_controller_stop_screensaver_enter_callback), screensaver_controller);
  g_signal_connect(screensaver_controller->stop_screensaver, "leave",
		   G_CALLBACK(monothek_screensaver_controller_stop_screensaver_leave_callback), screensaver_controller);
  g_signal_connect(screensaver_controller->stop_screensaver, "clicked",
		   G_CALLBACK(monothek_screensaver_controller_stop_screensaver_clicked_callback), screensaver_controller);
}

void
monothek_screensaver_controller_disconnect(AgsConnectable *connectable)
{
  MonothekScreensaverController *screensaver_controller;

  screensaver_controller = MONOTHEK_SCREENSAVER_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(screensaver_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_screensaver_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(screensaver_controller->stop_screensaver,
		      "any_signal::enter",
		      G_CALLBACK(monothek_screensaver_controller_stop_screensaver_enter_callback),
		      screensaver_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_screensaver_controller_stop_screensaver_leave_callback),
		      screensaver_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_screensaver_controller_stop_screensaver_clicked_callback),
		      screensaver_controller,
		      NULL);
}

void
monothek_screensaver_controller_reset(MonothekController *controller)
{
  //TODO:JK: implement me
}

void
monothek_screensaver_controller_stop_screensaver_enter_callback(MonothekActionBox *action_box,
								MonothekScreensaverController *screensaver_controller)
{
  //TODO:JK: implement me
}

void
monothek_screensaver_controller_stop_screensaver_leave_callback(MonothekActionBox *action_box,
								MonothekScreensaverController *screensaver_controller)
{
  //TODO:JK: implement me
}

void
monothek_screensaver_controller_stop_screensaver_clicked_callback(MonothekActionBox *action_box,
								  MonothekScreensaverController *screensaver_controller)
{
  monothek_screensaver_controller_stop_screensaver(screensaver_controller);
}

void
monothek_screensaver_controller_real_stop_screensaver(MonothekScreensaverController *screensaver_controller)
{
  MonothekWindow *window;
  MonothekScreensaverView *view;
  
  g_object_get(screensaver_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_BANNER_VIEW, G_TYPE_NONE);
}

/**
 * monothek_screensaver_controller_stop_screensaver:
 * @screensaver_controller: the #MonothekScreensaverController
 * 
 * Stop screensaver.
 * 
 * Since: 1.0.0
 */
void
monothek_screensaver_controller_stop_screensaver(MonothekScreensaverController *screensaver_controller)
{
  g_return_if_fail(MONOTHEK_IS_SCREENSAVER_CONTROLLER(screensaver_controller));
  
  g_object_ref((GObject *) screensaver_controller);
  g_signal_emit(G_OBJECT(screensaver_controller),
		screensaver_controller_signals[STOP_SCREENSAVER], 0);
  g_object_unref((GObject *) screensaver_controller);
}

/**
 * monothek_screensaver_controller_new:
 *
 * Creates an #MonothekScreensaverController
 *
 * Returns: a new #MonothekScreensaverController
 *
 * Since: 1.0.0
 */
MonothekScreensaverController*
monothek_screensaver_controller_new()
{
  MonothekScreensaverController *screensaver_controller;

  screensaver_controller = (MonothekScreensaverController *) g_object_new(MONOTHEK_TYPE_SCREENSAVER_CONTROLLER,
									  NULL);
  
  return(screensaver_controller);
}
