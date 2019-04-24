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

#include <monothek/ui/controller/monothek_jukebox_info_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_info_model.h>

#include <monothek/ui/view/monothek_jukebox_info_view.h>
#include <monothek/ui/view/monothek_jukebox_playlist_view.h>
#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_info_controller_class_init(MonothekJukeboxInfoControllerClass *jukebox_info_controller);
void monothek_jukebox_info_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_info_controller_init(MonothekJukeboxInfoController *jukebox_info_controller);
void monothek_jukebox_info_controller_finalize(GObject *gobject);

void monothek_jukebox_info_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_info_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_info_controller_jukebox_ok_enter_callback(MonothekActionBox *action_box,
								MonothekJukeboxInfoController *jukebox_info_controller);
void monothek_jukebox_info_controller_jukebox_ok_leave_callback(MonothekActionBox *action_box,
								MonothekJukeboxInfoController *jukebox_info_controller);
void monothek_jukebox_info_controller_jukebox_ok_clicked_callback(MonothekActionBox *action_box,
								  MonothekJukeboxInfoController *jukebox_info_controller);

void monothek_jukebox_info_controller_real_ok(MonothekJukeboxInfoController *jukebox_info_controller);

/**
 * SECTION:monothek_jukebox_info_controller
 * @short_description: The jukebox info controller object.
 * @title: MonothekJukeboxInfoController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_info_controller.h
 *
 * #MonothekJukeboxInfoController is the MVC's jukebox info controller.
 */

enum{
  OK,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_info_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_info_controller_parent_connectable_interface;

static guint jukebox_info_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_info_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_info_controller = 0;

    static const GTypeInfo monothek_jukebox_info_controller_info = {
      sizeof (MonothekJukeboxInfoControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_info_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxInfoController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_info_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_info_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_info_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								   "MonothekJukeboxInfoController", &monothek_jukebox_info_controller_info,
								   0);

    g_type_add_interface_static(monothek_type_jukebox_info_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_info_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_info_controller_class_init(MonothekJukeboxInfoControllerClass *jukebox_info_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_jukebox_info_controller_parent_class = g_type_class_peek_parent(jukebox_info_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_info_controller;

  gobject->finalize = monothek_jukebox_info_controller_finalize;

  /* MonothekJukeboxInfoController */
  jukebox_info_controller->ok = monothek_jukebox_info_controller_real_ok;

  /* signals */
  /**
   * MonothekJukeboxInfoController::ok:
   * @jukebox_info_controller: the #MonothekJukeboxInfoController
   *
   * The ::ok signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_info_controller_signals[OK] =
    g_signal_new("ok",
		 G_TYPE_FROM_CLASS(jukebox_info_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxInfoControllerClass, ok),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_info_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_info_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_info_controller_connect;
  connectable->disconnect = monothek_jukebox_info_controller_disconnect;
}

void
monothek_jukebox_info_controller_init(MonothekJukeboxInfoController *jukebox_info_controller)
{
  MonothekActionBox *action_box;

  /* ok */
  jukebox_info_controller->jukebox_ok = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-ok",
						    "x0", 740,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_info_controller,
				     action_box);
}

void
monothek_jukebox_info_controller_finalize(GObject *gobject)
{
  MonothekJukeboxInfoController *jukebox_info_controller;

  jukebox_info_controller = (MonothekJukeboxInfoController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_info_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_info_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxInfoController *jukebox_info_controller;

  jukebox_info_controller = MONOTHEK_JUKEBOX_INFO_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_info_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_info_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_info_controller->jukebox_ok, "enter",
		   G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_enter_callback), jukebox_info_controller);
  g_signal_connect(jukebox_info_controller->jukebox_ok, "leave",
		   G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_leave_callback), jukebox_info_controller);
  g_signal_connect(jukebox_info_controller->jukebox_ok, "clicked",
		   G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_clicked_callback), jukebox_info_controller);
}

void
monothek_jukebox_info_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxInfoController *jukebox_info_controller;

  jukebox_info_controller = MONOTHEK_JUKEBOX_INFO_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_info_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_info_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_info_controller->jukebox_ok,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_enter_callback),
		      jukebox_info_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_leave_callback),
		      jukebox_info_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_info_controller_jukebox_ok_clicked_callback),
		      jukebox_info_controller,
		      NULL);
}

void
monothek_jukebox_info_controller_jukebox_ok_enter_callback(MonothekActionBox *action_box,
							   MonothekJukeboxInfoController *jukebox_info_controller)
{
  MonothekJukeboxInfoView *view;
  
  MonothekJukeboxInfoModel *model;
  
  g_object_get(jukebox_info_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-ok-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_info_controller_jukebox_ok_leave_callback(MonothekActionBox *action_box,
							   MonothekJukeboxInfoController *jukebox_info_controller)
{
  MonothekJukeboxInfoView *view;
  
  MonothekJukeboxInfoModel *model;

  g_object_get(jukebox_info_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-ok-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_info_controller_jukebox_ok_clicked_callback(MonothekActionBox *action_box,
							     MonothekJukeboxInfoController *jukebox_info_controller)
{
  monothek_jukebox_info_controller_ok(jukebox_info_controller);
}

void
monothek_jukebox_info_controller_real_ok(MonothekJukeboxInfoController *jukebox_info_controller)
{
  MonothekWindow *window;
  MonothekJukeboxInfoView *view;
  
  /* change view */
  g_object_get(jukebox_info_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_PLAYLIST_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_info_controller_ok:
 * @jukebox_info_controller: the #MonothekJukeboxInfoController
 * 
 * Ok info.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_info_controller_ok(MonothekJukeboxInfoController *jukebox_info_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_INFO_CONTROLLER(jukebox_info_controller));
  
  g_object_ref((GObject *) jukebox_info_controller);
  g_signal_emit(G_OBJECT(jukebox_info_controller),
		jukebox_info_controller_signals[OK], 0);
  g_object_unref((GObject *) jukebox_info_controller);
}

/**
 * monothek_jukebox_info_controller_new:
 *
 * Creates an #MonothekJukeboxInfoController
 *
 * Returns: a new #MonothekJukeboxInfoController
 *
 * Since: 1.0.0
 */
MonothekJukeboxInfoController*
monothek_jukebox_info_controller_new()
{
  MonothekJukeboxInfoController *jukebox_info_controller;

  jukebox_info_controller = (MonothekJukeboxInfoController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_INFO_CONTROLLER,
									   NULL);
  
  return(jukebox_info_controller);
}
