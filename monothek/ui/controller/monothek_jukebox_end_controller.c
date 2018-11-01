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

#include <monothek/ui/controller/monothek_jukebox_end_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_end_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_jukebox_end_view.h>
#include <monothek/ui/view/monothek_jukebox_qrcode_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_end_controller_class_init(MonothekJukeboxEndControllerClass *jukebox_end_controller);
void monothek_jukebox_end_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_end_controller_init(MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_finalize(GObject *gobject);

void monothek_jukebox_end_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_end_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_end_controller_jukebox_restart_enter_callback(MonothekActionBox *action_box,
								    MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_jukebox_restart_leave_callback(MonothekActionBox *action_box,
								    MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_jukebox_restart_clicked_callback(MonothekActionBox *action_box,
								      MonothekJukeboxEndController *jukebox_end_controller);

void monothek_jukebox_end_controller_jukebox_quit_and_save_enter_callback(MonothekActionBox *action_box,
									  MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_jukebox_quit_and_save_leave_callback(MonothekActionBox *action_box,
									  MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_jukebox_quit_and_save_clicked_callback(MonothekActionBox *action_box,
									    MonothekJukeboxEndController *jukebox_end_controller);

void monothek_jukebox_end_controller_real_restart(MonothekJukeboxEndController *jukebox_end_controller);
void monothek_jukebox_end_controller_real_quit_and_save(MonothekJukeboxEndController *jukebox_end_controller);

/**
 * SECTION:monothek_jukebox_end_controller
 * @short_description: The jukebox no test controller object.
 * @title: MonothekJukeboxEndController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_end_controller.h
 *
 * #MonothekJukeboxEndController is the MVC's jukebox no test controller.
 */

enum{
  RESTART,
  QUIT_AND_SAVE,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_end_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_end_controller_parent_connectable_interface;

static guint jukebox_end_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_end_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_end_controller = 0;

    static const GTypeInfo monothek_jukebox_end_controller_info = {
      sizeof (MonothekJukeboxEndControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_end_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxEndController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_end_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_end_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_end_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								  "MonothekJukeboxEndController", &monothek_jukebox_end_controller_info,
								  0);

    g_type_add_interface_static(monothek_type_jukebox_end_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_end_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_end_controller_class_init(MonothekJukeboxEndControllerClass *jukebox_end_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_jukebox_end_controller_parent_class = g_type_class_peek_parent(jukebox_end_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_end_controller;

  gobject->finalize = monothek_jukebox_end_controller_finalize;

  /* MonothekJukeboxEndController */
  jukebox_end_controller->restart = monothek_jukebox_end_controller_real_restart;
  jukebox_end_controller->quit_and_save = monothek_jukebox_end_controller_real_quit_and_save;

  /* signals */
  /**
   * MonothekJukeboxEndController::restart:
   * @jukebox_end_controller: the #MonothekJukeboxEndController
   *
   * The ::restart signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_end_controller_signals[RESTART] =
    g_signal_new("restart",
		 G_TYPE_FROM_CLASS(jukebox_end_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxEndControllerClass, restart),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxEndController::quit-and-save:
   * @jukebox_end_controller: the #MonothekJukeboxEndController
   *
   * The ::quit-and-save signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_end_controller_signals[QUIT_AND_SAVE] =
    g_signal_new("quit-and-save",
		 G_TYPE_FROM_CLASS(jukebox_end_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxEndControllerClass, quit_and_save),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_end_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_end_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_end_controller_connect;
  connectable->disconnect = monothek_jukebox_end_controller_disconnect;
}

void
monothek_jukebox_end_controller_init(MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekActionBox *action_box;

  jukebox_end_controller->jukebox_restart = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-restart",
						    "x0", 520,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_end_controller,
				     action_box);

  jukebox_end_controller->jukebox_quit_and_save = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-quit-and-save",
						    "x0", 1000,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_end_controller,
				     action_box);
}

void
monothek_jukebox_end_controller_finalize(GObject *gobject)
{
  MonothekJukeboxEndController *jukebox_end_controller;

  jukebox_end_controller = (MonothekJukeboxEndController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_end_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_end_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxEndController *jukebox_end_controller;

  jukebox_end_controller = MONOTHEK_JUKEBOX_END_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_end_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_end_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_end_controller->jukebox_restart, "enter",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_enter_callback), jukebox_end_controller);
  g_signal_connect(jukebox_end_controller->jukebox_restart, "leave",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_leave_callback), jukebox_end_controller);
  g_signal_connect(jukebox_end_controller->jukebox_restart, "clicked",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_clicked_callback), jukebox_end_controller);

  g_signal_connect(jukebox_end_controller->jukebox_quit_and_save, "enter",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_enter_callback), jukebox_end_controller);
  g_signal_connect(jukebox_end_controller->jukebox_quit_and_save, "leave",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_leave_callback), jukebox_end_controller);
  g_signal_connect(jukebox_end_controller->jukebox_quit_and_save, "clicked",
		   G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_clicked_callback), jukebox_end_controller);
}

void
monothek_jukebox_end_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxEndController *jukebox_end_controller;

  jukebox_end_controller = MONOTHEK_JUKEBOX_END_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_end_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_end_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_end_controller->jukebox_restart,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_enter_callback),
		      jukebox_end_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_leave_callback),
		      jukebox_end_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_restart_clicked_callback),
		      jukebox_end_controller,
		      NULL);

  g_object_disconnect(jukebox_end_controller->jukebox_quit_and_save,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_enter_callback),
		      jukebox_end_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_leave_callback),
		      jukebox_end_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_end_controller_jukebox_quit_and_save_clicked_callback),
		      jukebox_end_controller,
		      NULL);
}

void
monothek_jukebox_end_controller_jukebox_restart_enter_callback(MonothekActionBox *action_box,
							       MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekJukeboxEndView *view;
  
  MonothekJukeboxEndModel *model;
  
  g_object_get(jukebox_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-restart-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_end_controller_jukebox_restart_leave_callback(MonothekActionBox *action_box,
							       MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekJukeboxEndView *view;
  
  MonothekJukeboxEndModel *model;

  g_object_get(jukebox_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-restart-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_end_controller_jukebox_restart_clicked_callback(MonothekActionBox *action_box,
								 MonothekJukeboxEndController *jukebox_end_controller)
{
  monothek_jukebox_end_controller_restart(jukebox_end_controller);
}

void
monothek_jukebox_end_controller_jukebox_quit_and_save_enter_callback(MonothekActionBox *action_box,
								     MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekJukeboxEndView *view;
  
  MonothekJukeboxEndModel *model;
  
  g_object_get(jukebox_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-quit-and-save-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_end_controller_jukebox_quit_and_save_leave_callback(MonothekActionBox *action_box,
								     MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekJukeboxEndView *view;
  
  MonothekJukeboxEndModel *model;

  g_object_get(jukebox_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-quit-and-save-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_end_controller_jukebox_quit_and_save_clicked_callback(MonothekActionBox *action_box,
								       MonothekJukeboxEndController *jukebox_end_controller)
{
  monothek_jukebox_end_controller_quit_and_save(jukebox_end_controller);
}

void
monothek_jukebox_end_controller_real_restart(MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekWindow *window;
  MonothekJukeboxEndView *view;
  
  /* change view */
  g_object_get(jukebox_end_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_end_controller_restart:
 * @jukebox_end_controller: the #MonothekJukeboxEndController
 * 
 * Restart playback.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_end_controller_restart(MonothekJukeboxEndController *jukebox_end_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_END_CONTROLLER(jukebox_end_controller));
  
  g_object_ref((GObject *) jukebox_end_controller);
  g_signal_emit(G_OBJECT(jukebox_end_controller),
		jukebox_end_controller_signals[RESTART], 0);
  g_object_unref((GObject *) jukebox_end_controller);
}

void
monothek_jukebox_end_controller_real_quit_and_save(MonothekJukeboxEndController *jukebox_end_controller)
{
  MonothekWindow *window;
  MonothekJukeboxEndView *view;

  /* change view */
  g_object_get(jukebox_end_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_QRCODE_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_end_controller_quit_and_save:
 * @jukebox_end_controller: the #MonothekJukeboxEndController
 * 
 * Quit_And_Save.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_end_controller_quit_and_save(MonothekJukeboxEndController *jukebox_end_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_END_CONTROLLER(jukebox_end_controller));
  
  g_object_ref((GObject *) jukebox_end_controller);
  g_signal_emit(G_OBJECT(jukebox_end_controller),
		jukebox_end_controller_signals[QUIT_AND_SAVE], 0);
  g_object_unref((GObject *) jukebox_end_controller);
}

/**
 * monothek_jukebox_end_controller_new:
 *
 * Creates an #MonothekJukeboxEndController
 *
 * Returns: a new #MonothekJukeboxEndController
 *
 * Since: 1.0.0
 */
MonothekJukeboxEndController*
monothek_jukebox_end_controller_new()
{
  MonothekJukeboxEndController *jukebox_end_controller;

  jukebox_end_controller = (MonothekJukeboxEndController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_END_CONTROLLER,
									 NULL);
  
  return(jukebox_end_controller);
}
