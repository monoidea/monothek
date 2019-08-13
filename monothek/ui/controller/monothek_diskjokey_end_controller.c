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

#include <monothek/ui/controller/monothek_diskjokey_end_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_end_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_diskjokey_end_view.h>
#include <monothek/ui/view/monothek_diskjokey_qrcode_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_end_controller_class_init(MonothekDiskjokeyEndControllerClass *diskjokey_end_controller);
void monothek_diskjokey_end_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_end_controller_init(MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_finalize(GObject *gobject);

void monothek_diskjokey_end_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_end_controller_disconnect(AgsConnectable *connectable);

void monothek_diskjokey_end_controller_reset(MonothekController *controller);

void monothek_diskjokey_end_controller_diskjokey_restart_enter_callback(MonothekActionBox *action_box,
									MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_diskjokey_restart_leave_callback(MonothekActionBox *action_box,
									MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_diskjokey_restart_clicked_callback(MonothekActionBox *action_box,
									  MonothekDiskjokeyEndController *diskjokey_end_controller);

void monothek_diskjokey_end_controller_diskjokey_quit_and_save_enter_callback(MonothekActionBox *action_box,
									      MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_diskjokey_quit_and_save_leave_callback(MonothekActionBox *action_box,
									      MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_diskjokey_quit_and_save_clicked_callback(MonothekActionBox *action_box,
										MonothekDiskjokeyEndController *diskjokey_end_controller);

void monothek_diskjokey_end_controller_real_restart(MonothekDiskjokeyEndController *diskjokey_end_controller);
void monothek_diskjokey_end_controller_real_quit_and_save(MonothekDiskjokeyEndController *diskjokey_end_controller);

/**
 * SECTION:monothek_diskjokey_end_controller
 * @short_description: The diskjokey no test controller object.
 * @title: MonothekDiskjokeyEndController
 * @section_id:
 * @include: monothek/ui/controller/monothek_diskjokey_end_controller.h
 *
 * #MonothekDiskjokeyEndController is the MVC's diskjokey no test controller.
 */

enum{
     RESTART,
     QUIT_AND_SAVE,
     LAST_SIGNAL,
};

static gpointer monothek_diskjokey_end_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_end_controller_parent_connectable_interface;

static guint diskjokey_end_controller_signals[LAST_SIGNAL];

GType
monothek_diskjokey_end_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_end_controller = 0;

    static const GTypeInfo monothek_diskjokey_end_controller_info = {
								     sizeof (MonothekDiskjokeyEndControllerClass),
								     NULL, /* base_init */
								     NULL, /* base_finalize */
								     (GClassInitFunc) monothek_diskjokey_end_controller_class_init,
								     NULL, /* class_finalize */
								     NULL, /* class_data */
								     sizeof (MonothekDiskjokeyEndController),
								     0,    /* n_preallocs */
								     (GInstanceInitFunc) monothek_diskjokey_end_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
								       (GInterfaceInitFunc) monothek_diskjokey_end_controller_connectable_interface_init,
								       NULL, /* interface_finalize */
								       NULL, /* interface_data */
    };

    monothek_type_diskjokey_end_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								    "MonothekDiskjokeyEndController", &monothek_diskjokey_end_controller_info,
								    0);

    g_type_add_interface_static(monothek_type_diskjokey_end_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_end_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_end_controller_class_init(MonothekDiskjokeyEndControllerClass *diskjokey_end_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_diskjokey_end_controller_parent_class = g_type_class_peek_parent(diskjokey_end_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_end_controller;

  gobject->finalize = monothek_diskjokey_end_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) diskjokey_end_controller;
  
  controller->reset = monothek_diskjokey_end_controller_reset;

  /* MonothekDiskjokeyEndController */
  diskjokey_end_controller->restart = monothek_diskjokey_end_controller_real_restart;
  diskjokey_end_controller->quit_and_save = monothek_diskjokey_end_controller_real_quit_and_save;

  /* signals */
  /**
   * MonothekDiskjokeyEndController::restart:
   * @diskjokey_end_controller: the #MonothekDiskjokeyEndController
   *
   * The ::restart signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  diskjokey_end_controller_signals[RESTART] =
    g_signal_new("restart",
		 G_TYPE_FROM_CLASS(diskjokey_end_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeyEndControllerClass, restart),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekDiskjokeyEndController::quit-and-save:
   * @diskjokey_end_controller: the #MonothekDiskjokeyEndController
   *
   * The ::quit-and-save signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  diskjokey_end_controller_signals[QUIT_AND_SAVE] =
    g_signal_new("quit-and-save",
		 G_TYPE_FROM_CLASS(diskjokey_end_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeyEndControllerClass, quit_and_save),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_diskjokey_end_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_diskjokey_end_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_diskjokey_end_controller_connect;
  connectable->disconnect = monothek_diskjokey_end_controller_disconnect;
}

void
monothek_diskjokey_end_controller_init(MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekActionBox *action_box;

  diskjokey_end_controller->diskjokey_restart = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "diskjokey-restart",
						    "x0", 520,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(diskjokey_end_controller,
				     action_box);

  diskjokey_end_controller->diskjokey_quit_and_save = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "diskjokey-quit-and-save",
						    "x0", 1000,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(diskjokey_end_controller,
				     action_box);
}

void
monothek_diskjokey_end_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeyEndController *diskjokey_end_controller;

  diskjokey_end_controller = (MonothekDiskjokeyEndController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_end_controller_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_end_controller_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeyEndController *diskjokey_end_controller;

  diskjokey_end_controller = MONOTHEK_DISKJOKEY_END_CONTROLLER(connectable);

  if(monothek_controller_test_flags(diskjokey_end_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_end_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(diskjokey_end_controller->diskjokey_restart, "enter",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_enter_callback), diskjokey_end_controller);
  g_signal_connect(diskjokey_end_controller->diskjokey_restart, "leave",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_leave_callback), diskjokey_end_controller);
  g_signal_connect(diskjokey_end_controller->diskjokey_restart, "clicked",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_clicked_callback), diskjokey_end_controller);

  g_signal_connect(diskjokey_end_controller->diskjokey_quit_and_save, "enter",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_enter_callback), diskjokey_end_controller);
  g_signal_connect(diskjokey_end_controller->diskjokey_quit_and_save, "leave",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_leave_callback), diskjokey_end_controller);
  g_signal_connect(diskjokey_end_controller->diskjokey_quit_and_save, "clicked",
		   G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_clicked_callback), diskjokey_end_controller);
}

void
monothek_diskjokey_end_controller_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeyEndController *diskjokey_end_controller;

  diskjokey_end_controller = MONOTHEK_DISKJOKEY_END_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(diskjokey_end_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_end_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(diskjokey_end_controller->diskjokey_restart,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_enter_callback),
		      diskjokey_end_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_leave_callback),
		      diskjokey_end_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_restart_clicked_callback),
		      diskjokey_end_controller,
		      NULL);

  g_object_disconnect(diskjokey_end_controller->diskjokey_quit_and_save,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_enter_callback),
		      diskjokey_end_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_leave_callback),
		      diskjokey_end_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_end_controller_diskjokey_quit_and_save_clicked_callback),
		      diskjokey_end_controller,
		      NULL);
}

void
monothek_diskjokey_end_controller_reset(MonothekController *controller)
{
  MonothekDiskjokeyEndController *diskjokey_end_controller;
  MonothekDiskjokeyEndModel *model;

  diskjokey_end_controller = MONOTHEK_DISKJOKEY_END_CONTROLLER(controller);
  
  g_object_get(diskjokey_end_controller,
	       "model", &model,
	       NULL);

  model->diskjokey_restart_active = FALSE;
  model->diskjokey_quit_and_save_active = FALSE;
}

void
monothek_diskjokey_end_controller_diskjokey_restart_enter_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekDiskjokeyEndView *view;
  
  MonothekDiskjokeyEndModel *model;
  
  g_object_get(diskjokey_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-restart-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_end_controller_diskjokey_restart_leave_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekDiskjokeyEndView *view;
  
  MonothekDiskjokeyEndModel *model;

  g_object_get(diskjokey_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-restart-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_end_controller_diskjokey_restart_clicked_callback(MonothekActionBox *action_box,
								     MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  monothek_diskjokey_end_controller_restart(diskjokey_end_controller);
}

void
monothek_diskjokey_end_controller_diskjokey_quit_and_save_enter_callback(MonothekActionBox *action_box,
									 MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekDiskjokeyEndView *view;
  
  MonothekDiskjokeyEndModel *model;
  
  g_object_get(diskjokey_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-quit-and-save-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_end_controller_diskjokey_quit_and_save_leave_callback(MonothekActionBox *action_box,
									 MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekDiskjokeyEndView *view;
  
  MonothekDiskjokeyEndModel *model;

  g_object_get(diskjokey_end_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-quit-and-save-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_end_controller_diskjokey_quit_and_save_clicked_callback(MonothekActionBox *action_box,
									   MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  monothek_diskjokey_end_controller_quit_and_save(diskjokey_end_controller);
}

void
monothek_diskjokey_end_controller_real_restart(MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeyEndView *view;
  
  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  /* change view */
  g_object_get(diskjokey_end_controller,
	       "view", &view,
	       NULL);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set preserve diskjokey - TRUE */
  value = g_hash_table_lookup(session->value,
			      "preserve-diskjokey");

  g_value_set_boolean(value,
		      TRUE);

  /* change view */
  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_end_controller_restart:
 * @diskjokey_end_controller: the #MonothekDiskjokeyEndController
 * 
 * Restart playback.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_end_controller_restart(MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_END_CONTROLLER(diskjokey_end_controller));
  
  g_object_ref((GObject *) diskjokey_end_controller);
  g_signal_emit(G_OBJECT(diskjokey_end_controller),
		diskjokey_end_controller_signals[RESTART], 0);
  g_object_unref((GObject *) diskjokey_end_controller);
}

void
monothek_diskjokey_end_controller_real_quit_and_save(MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeyEndView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  /* change view */
  g_object_get(diskjokey_end_controller,
	       "view", &view,
	       NULL);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set preserve diskjokey - FALSE */
  value = g_hash_table_lookup(session->value,
			      "preserve-diskjokey");

  g_value_set_boolean(value,
		      FALSE);

  /* change view */
  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_end_controller_quit_and_save:
 * @diskjokey_end_controller: the #MonothekDiskjokeyEndController
 * 
 * Quit_And_Save.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_end_controller_quit_and_save(MonothekDiskjokeyEndController *diskjokey_end_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_END_CONTROLLER(diskjokey_end_controller));
  
  g_object_ref((GObject *) diskjokey_end_controller);
  g_signal_emit(G_OBJECT(diskjokey_end_controller),
		diskjokey_end_controller_signals[QUIT_AND_SAVE], 0);
  g_object_unref((GObject *) diskjokey_end_controller);
}

/**
 * monothek_diskjokey_end_controller_new:
 *
 * Creates an #MonothekDiskjokeyEndController
 *
 * Returns: a new #MonothekDiskjokeyEndController
 *
 * Since: 1.0.0
 */
MonothekDiskjokeyEndController*
monothek_diskjokey_end_controller_new()
{
  MonothekDiskjokeyEndController *diskjokey_end_controller;

  diskjokey_end_controller = (MonothekDiskjokeyEndController *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_END_CONTROLLER,
									     NULL);
  
  return(diskjokey_end_controller);
}
