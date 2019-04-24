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

#include <monothek/ui/controller/monothek_jukebox_mode_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_mode_model.h>

#include <monothek/ui/view/monothek_jukebox_mode_view.h>
#include <monothek/ui/view/monothek_jukebox_playlist_view.h>
#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_mode_controller_class_init(MonothekJukeboxModeControllerClass *jukebox_mode_controller);
void monothek_jukebox_mode_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_mode_controller_init(MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_finalize(GObject *gobject);

void monothek_jukebox_mode_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_mode_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_mode_controller_jukebox_test_enter_callback(MonothekActionBox *action_box,
								  MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_test_leave_callback(MonothekActionBox *action_box,
								  MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_test_clicked_callback(MonothekActionBox *action_box,
								    MonothekJukeboxModeController *jukebox_mode_controller);

void monothek_jukebox_mode_controller_jukebox_play_enter_callback(MonothekActionBox *action_box,
								  MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_play_leave_callback(MonothekActionBox *action_box,
								  MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_play_clicked_callback(MonothekActionBox *action_box,
								    MonothekJukeboxModeController *jukebox_mode_controller);

void monothek_jukebox_mode_controller_jukebox_cancel_enter_callback(MonothekActionBox *action_box,
								    MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_cancel_leave_callback(MonothekActionBox *action_box,
								    MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_jukebox_cancel_clicked_callback(MonothekActionBox *action_box,
								      MonothekJukeboxModeController *jukebox_mode_controller);

void monothek_jukebox_mode_controller_real_test(MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_real_play(MonothekJukeboxModeController *jukebox_mode_controller);
void monothek_jukebox_mode_controller_real_cancel(MonothekJukeboxModeController *jukebox_mode_controller);

/**
 * SECTION:monothek_jukebox_mode_controller
 * @short_description: The jukebox mode controller object.
 * @title: MonothekJukeboxModeController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_mode_controller.h
 *
 * #MonothekJukeboxModeController is the MVC's jukebox mode controller.
 */

enum{
  TEST,
  PLAY,
  CANCEL,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_mode_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_mode_controller_parent_connectable_interface;

static guint jukebox_mode_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_mode_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_mode_controller = 0;

    static const GTypeInfo monothek_jukebox_mode_controller_info = {
      sizeof (MonothekJukeboxModeControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_mode_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxModeController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_mode_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_mode_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_mode_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								   "MonothekJukeboxModeController", &monothek_jukebox_mode_controller_info,
								   0);

    g_type_add_interface_static(monothek_type_jukebox_mode_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_mode_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_mode_controller_class_init(MonothekJukeboxModeControllerClass *jukebox_mode_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_jukebox_mode_controller_parent_class = g_type_class_peek_parent(jukebox_mode_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_mode_controller;

  gobject->finalize = monothek_jukebox_mode_controller_finalize;

  /* MonothekJukeboxModeController */
  jukebox_mode_controller->test = monothek_jukebox_mode_controller_real_test;
  jukebox_mode_controller->play = monothek_jukebox_mode_controller_real_play;
  jukebox_mode_controller->cancel = monothek_jukebox_mode_controller_real_cancel;

  /* signals */
  /**
   * MonothekJukeboxModeController::test:
   * @jukebox_mode_controller: the #MonothekJukeboxModeController
   *
   * The ::test signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_mode_controller_signals[TEST] =
    g_signal_new("test",
		 G_TYPE_FROM_CLASS(jukebox_mode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxModeControllerClass, test),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxModeController::play:
   * @jukebox_mode_controller: the #MonothekJukeboxModeController
   *
   * The ::play signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_mode_controller_signals[PLAY] =
    g_signal_new("play",
		 G_TYPE_FROM_CLASS(jukebox_mode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxModeControllerClass, play),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxModeController::cancel:
   * @jukebox_mode_controller: the #MonothekJukeboxModeController
   *
   * The ::cancel signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_mode_controller_signals[CANCEL] =
    g_signal_new("cancel",
		 G_TYPE_FROM_CLASS(jukebox_mode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxModeControllerClass, cancel),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_mode_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_mode_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_mode_controller_connect;
  connectable->disconnect = monothek_jukebox_mode_controller_disconnect;
}

void
monothek_jukebox_mode_controller_init(MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekActionBox *action_box;

  /* test */
  jukebox_mode_controller->jukebox_test = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-test",
						    "x0", 740,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_mode_controller,
				     action_box);

  /* play */
  jukebox_mode_controller->jukebox_play = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-play",
						    "x0", 1000,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_mode_controller,
				     action_box);

  jukebox_mode_controller->jukebox_play->enabled = FALSE;

  /* cancel */
  jukebox_mode_controller->jukebox_cancel = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-cancel",
						    "x0", 1000,
						    "y0", 580,
						    "width", 400,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_mode_controller,
				     action_box);

  jukebox_mode_controller->jukebox_play->enabled = FALSE;
}

void
monothek_jukebox_mode_controller_finalize(GObject *gobject)
{
  MonothekJukeboxModeController *jukebox_mode_controller;

  jukebox_mode_controller = (MonothekJukeboxModeController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_mode_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_mode_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxModeController *jukebox_mode_controller;

  jukebox_mode_controller = MONOTHEK_JUKEBOX_MODE_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_mode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_mode_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_mode_controller->jukebox_test, "enter",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_enter_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_test, "leave",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_leave_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_test, "clicked",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_clicked_callback), jukebox_mode_controller);

  g_signal_connect(jukebox_mode_controller->jukebox_play, "enter",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_enter_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_play, "leave",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_leave_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_play, "clicked",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_clicked_callback), jukebox_mode_controller);

  g_signal_connect(jukebox_mode_controller->jukebox_cancel, "enter",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_enter_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_cancel, "leave",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_leave_callback), jukebox_mode_controller);
  g_signal_connect(jukebox_mode_controller->jukebox_cancel, "clicked",
		   G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_clicked_callback), jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxModeController *jukebox_mode_controller;

  jukebox_mode_controller = MONOTHEK_JUKEBOX_MODE_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_mode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_mode_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_mode_controller->jukebox_test,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_enter_callback),
		      jukebox_mode_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_leave_callback),
		      jukebox_mode_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_test_clicked_callback),
		      jukebox_mode_controller,
		      NULL);

  g_object_disconnect(jukebox_mode_controller->jukebox_play,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_enter_callback),
		      jukebox_mode_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_leave_callback),
		      jukebox_mode_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_play_clicked_callback),
		      jukebox_mode_controller,
		      NULL);

  g_object_disconnect(jukebox_mode_controller->jukebox_cancel,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_enter_callback),
		      jukebox_mode_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_leave_callback),
		      jukebox_mode_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_mode_controller_jukebox_cancel_clicked_callback),
		      jukebox_mode_controller,
		      NULL);
}

void
monothek_jukebox_mode_controller_jukebox_test_enter_callback(MonothekActionBox *action_box,
							     MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;
  
  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-test-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_test_leave_callback(MonothekActionBox *action_box,
							     MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;

  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-test-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_test_clicked_callback(MonothekActionBox *action_box,
							       MonothekJukeboxModeController *jukebox_mode_controller)
{
  monothek_jukebox_mode_controller_test(jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_jukebox_play_enter_callback(MonothekActionBox *action_box,
							     MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;
  
  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-play-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_play_leave_callback(MonothekActionBox *action_box,
							     MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;

  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-play-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_play_clicked_callback(MonothekActionBox *action_box,
							       MonothekJukeboxModeController *jukebox_mode_controller)
{
  monothek_jukebox_mode_controller_play(jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_jukebox_cancel_enter_callback(MonothekActionBox *action_box,
							       MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;
  
  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-cancel-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_cancel_leave_callback(MonothekActionBox *action_box,
							       MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekJukeboxModeView *view;
  
  MonothekJukeboxModeModel *model;

  g_object_get(jukebox_mode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-cancel-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_mode_controller_jukebox_cancel_clicked_callback(MonothekActionBox *action_box,
								 MonothekJukeboxModeController *jukebox_mode_controller)
{
  monothek_jukebox_mode_controller_cancel(jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_real_test(MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekWindow *window;
  MonothekJukeboxModeView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_mode;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - test */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  if(jukebox_mode == NULL){
    jukebox_mode = g_new0(GValue,
			  1);
    g_value_init(jukebox_mode,
		 G_TYPE_STRING);

    g_hash_table_insert(session->value,
			"jukebox-mode", jukebox_mode);
  }

  g_value_set_string(jukebox_mode,
		     "test");
  
  /* change view */
  g_object_get(jukebox_mode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_PLAYLIST_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_mode_controller_test:
 * @jukebox_mode_controller: the #MonothekJukeboxModeController
 * 
 * Test mode.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_mode_controller_test(MonothekJukeboxModeController *jukebox_mode_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_MODE_CONTROLLER(jukebox_mode_controller));
  
  g_object_ref((GObject *) jukebox_mode_controller);
  g_signal_emit(G_OBJECT(jukebox_mode_controller),
		jukebox_mode_controller_signals[TEST], 0);
  g_object_unref((GObject *) jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_real_play(MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekWindow *window;
  MonothekJukeboxModeView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_mode;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - play */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  if(jukebox_mode == NULL){
    jukebox_mode = g_new0(GValue,
			  1);
    g_value_init(jukebox_mode,
		 G_TYPE_STRING);

    g_hash_table_insert(session->value,
			"jukebox-mode", jukebox_mode);
  }

  g_value_set_string(jukebox_mode,
		     "play");
  
  /* change view */
  g_object_get(jukebox_mode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_mode_controller_play:
 * @jukebox_mode_controller: the #MonothekJukeboxModeController
 * 
 * Play mode.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_mode_controller_play(MonothekJukeboxModeController *jukebox_mode_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_MODE_CONTROLLER(jukebox_mode_controller));
  
  g_object_ref((GObject *) jukebox_mode_controller);
  g_signal_emit(G_OBJECT(jukebox_mode_controller),
		jukebox_mode_controller_signals[PLAY], 0);
  g_object_unref((GObject *) jukebox_mode_controller);
}

void
monothek_jukebox_mode_controller_real_cancel(MonothekJukeboxModeController *jukebox_mode_controller)
{
  MonothekWindow *window;
  MonothekJukeboxModeView *view;

  MonothekJukeboxModeModel *model;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_mode;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - play */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  if(jukebox_mode == NULL){
    jukebox_mode = g_new0(GValue,
			  1);
    g_value_init(jukebox_mode,
		 G_TYPE_STRING);

    g_hash_table_insert(session->value,
			"jukebox-mode", jukebox_mode);
  }

  g_value_set_string(jukebox_mode,
		     "play");
  
  /* change view */
  g_object_get(jukebox_mode_controller,
	       "view", &view,
	       "model", &model,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_PLAYLIST_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_mode_controller_cancel:
 * @jukebox_mode_controller: the #MonothekJukeboxModeController
 * 
 * Cancel.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_mode_controller_cancel(MonothekJukeboxModeController *jukebox_mode_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_MODE_CONTROLLER(jukebox_mode_controller));
  
  g_object_ref((GObject *) jukebox_mode_controller);
  g_signal_emit(G_OBJECT(jukebox_mode_controller),
		jukebox_mode_controller_signals[CANCEL], 0);
  g_object_unref((GObject *) jukebox_mode_controller);
}

/**
 * monothek_jukebox_mode_controller_new:
 *
 * Creates an #MonothekJukeboxModeController
 *
 * Returns: a new #MonothekJukeboxModeController
 *
 * Since: 1.0.0
 */
MonothekJukeboxModeController*
monothek_jukebox_mode_controller_new()
{
  MonothekJukeboxModeController *jukebox_mode_controller;

  jukebox_mode_controller = (MonothekJukeboxModeController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_MODE_CONTROLLER,
									   NULL);
  
  return(jukebox_mode_controller);
}
