/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2020 Joël Krähemann
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

#include <monothek/ui/controller/monothek_jukebox_track_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/audio/monothek_rack.h>

#include <monothek/audio/task/monothek_export_output.h>

#include <monothek/audio/thread/monothek_export_thread.h>

#include <monothek/audio/file/monothek_audio_file_manager.h>

#include <monothek/ui/monothek_application_context.h>
#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_track_model.h>

#include <monothek/ui/view/monothek_jukebox_track_view.h>
#include <monothek/ui/view/monothek_jukebox_mode_view.h>
#include <monothek/ui/view/monothek_jukebox_info_view.h>
#include <monothek/ui/view/monothek_jukebox_playlist_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_track_controller_class_init(MonothekJukeboxTrackControllerClass *jukebox_track_controller);
void monothek_jukebox_track_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_track_controller_init(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_finalize(GObject *gobject);

void monothek_jukebox_track_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_track_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_track_controller_reset(MonothekController *controller);

void monothek_jukebox_track_controller_jukebox_test_enter_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_test_leave_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_test_clicked_callback(MonothekActionBox *action_box,
								    MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_jukebox_play_enter_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_play_leave_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_play_clicked_callback(MonothekActionBox *action_box,
								    MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_jukebox_back_enter_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_back_leave_callback(MonothekActionBox *action_box,
								  MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_jukebox_back_clicked_callback(MonothekActionBox *action_box,
								    MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_real_test(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_real_play(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_real_back(MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_real_progress(MonothekJukeboxTrackController *jukebox_track_controller,
						     gdouble value);
void monothek_jukebox_track_controller_real_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_real_run(MonothekJukeboxTrackController *jukebox_track_controller,
						gboolean do_run);
void monothek_jukebox_track_controller_real_completed(MonothekJukeboxTrackController *jukebox_track_controller);

/**
 * SECTION:monothek_jukebox_track_controller
 * @short_description: The jukebox track controller object.
 * @title: MonothekJukeboxTrackController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_track_controller.h
 *
 * #MonothekJukeboxTrackController is the MVC's jukebox track controller.
 */

enum{
  TEST,
  PLAY,
  BACK,
  PROGRESS,
  TEST_TIME_EXPIRED,
  RUN,
  COMPLETED,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_track_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_track_controller_parent_connectable_interface;

static guint jukebox_track_controller_signals[LAST_SIGNAL];

GHashTable *monothek_jukebox_track_controller_progress_increase = NULL;

GType
monothek_jukebox_track_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_track_controller = 0;

    static const GTypeInfo monothek_jukebox_track_controller_info = {
      sizeof (MonothekJukeboxTrackControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_track_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxTrackController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_track_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_track_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_track_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								    "MonothekJukeboxTrackController", &monothek_jukebox_track_controller_info,
								    0);

    g_type_add_interface_static(monothek_type_jukebox_track_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_track_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_track_controller_class_init(MonothekJukeboxTrackControllerClass *jukebox_track_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_jukebox_track_controller_parent_class = g_type_class_peek_parent(jukebox_track_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_track_controller;

  gobject->finalize = monothek_jukebox_track_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) jukebox_track_controller;
  
  controller->reset = monothek_jukebox_track_controller_reset;

  /* MonothekJukeboxTrackController */
  jukebox_track_controller->test = monothek_jukebox_track_controller_real_test;
  jukebox_track_controller->play = monothek_jukebox_track_controller_real_play;
  jukebox_track_controller->back = monothek_jukebox_track_controller_real_back;

  jukebox_track_controller->progress = monothek_jukebox_track_controller_real_progress;
  jukebox_track_controller->test_time_expired = monothek_jukebox_track_controller_real_test_time_expired;
  jukebox_track_controller->run = monothek_jukebox_track_controller_real_run;
  jukebox_track_controller->completed = monothek_jukebox_track_controller_real_completed;

  /* signals */
  /**
   * MonothekJukeboxTrackController::test:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::test signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[TEST] =
    g_signal_new("test",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, test),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxTrackController::play:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::play signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[PLAY] =
    g_signal_new("play",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, play),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxTrackController::back:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::back signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[BACK] =
    g_signal_new("back",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, back),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxTrackController::progress:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   * @value: a gdouble ranging from 0.0 to 1.0
   *
   * The ::progress signal notifies about progress.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[PROGRESS] =
    g_signal_new("progress",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, progress),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__DOUBLE,
		 G_TYPE_NONE, 1,
		 G_TYPE_DOUBLE);

  /**
   * MonothekJukeboxTrackController::test-time-expired:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::test-time-expired signal notifies about test time expired.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[TEST_TIME_EXPIRED] =
    g_signal_new("test-time-expired",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, test_time_expired),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxTrackController::run:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   * @do_run: if %TRUE start playback, else if %FALSE stop playback
   *
   * The ::run signal notifies about start/stop playback.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[RUN] =
    g_signal_new("run",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, run),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__BOOLEAN,
		 G_TYPE_NONE, 1,
		 G_TYPE_BOOLEAN);

  /**
   * MonothekJukeboxTrackController::completed:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::completed signal notifies about terminating playback.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[COMPLETED] =
    g_signal_new("completed",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, completed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_track_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_track_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_track_controller_connect;
  connectable->disconnect = monothek_jukebox_track_controller_disconnect;
}

void
monothek_jukebox_track_controller_init(MonothekJukeboxTrackController *jukebox_track_controller)
{  
  MonothekActionBox *action_box;

  if(monothek_jukebox_track_controller_progress_increase == NULL){
    monothek_jukebox_track_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										NULL,
										NULL);
  }

  /* test */
  jukebox_track_controller->jukebox_test = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-test",
						    "x0", 700,
						    "y0", 840,
						    "width", 520,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_track_controller,
				     action_box);

  /* play */
  jukebox_track_controller->jukebox_play = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-play",
						    "x0", 120,
						    "y0", 840,
						    "width", 520,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_track_controller,
				     action_box);

  /* back */
  jukebox_track_controller->jukebox_back = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-back",
						    "x0", 1280,
						    "y0", 840,
						    "width", 520,
						    "height", 140,
						    NULL);
  monothek_controller_add_action_box(jukebox_track_controller,
				     action_box);
  
  jukebox_track_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  jukebox_track_controller->start_time->tv_sec = 0;
  jukebox_track_controller->start_time->tv_nsec = 0;
  
  jukebox_track_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  jukebox_track_controller->timer->tv_sec = 0;
  jukebox_track_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_jukebox_track_controller_progress_increase,
		      jukebox_track_controller, monothek_jukebox_track_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 30,
		(GSourceFunc) monothek_jukebox_track_controller_progress_increase_timeout,
		(gpointer) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_finalize(GObject *gobject)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = (MonothekJukeboxTrackController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_jukebox_track_controller_progress_increase,
		      jukebox_track_controller);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_track_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_track_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = MONOTHEK_JUKEBOX_TRACK_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_track_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_track_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_track_controller->jukebox_test, "enter",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_enter_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_test, "leave",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_leave_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_test, "clicked",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_clicked_callback), jukebox_track_controller);

  g_signal_connect(jukebox_track_controller->jukebox_play, "enter",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_enter_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_play, "leave",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_leave_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_play, "clicked",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_clicked_callback), jukebox_track_controller);

  g_signal_connect(jukebox_track_controller->jukebox_back, "enter",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_enter_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_back, "leave",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_leave_callback), jukebox_track_controller);
  g_signal_connect(jukebox_track_controller->jukebox_back, "clicked",
		   G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_clicked_callback), jukebox_track_controller);
}

void
monothek_jukebox_track_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = MONOTHEK_JUKEBOX_TRACK_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_track_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_track_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_track_controller->jukebox_test,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_enter_callback),
		      jukebox_track_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_leave_callback),
		      jukebox_track_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_test_clicked_callback),
		      jukebox_track_controller,
		      NULL);

  g_object_disconnect(jukebox_track_controller->jukebox_play,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_enter_callback),
		      jukebox_track_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_leave_callback),
		      jukebox_track_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_play_clicked_callback),
		      jukebox_track_controller,
		      NULL);

  g_object_disconnect(jukebox_track_controller->jukebox_back,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_enter_callback),
		      jukebox_track_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_leave_callback),
		      jukebox_track_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_track_controller_jukebox_back_clicked_callback),
		      jukebox_track_controller,
		      NULL);
}

void
monothek_jukebox_track_controller_reset(MonothekController *controller)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = MONOTHEK_JUKEBOX_TRACK_CONTROLLER(controller);

  jukebox_track_controller->start_time->tv_sec = 0;
  jukebox_track_controller->start_time->tv_nsec = 0;

  jukebox_track_controller->timer->tv_sec = 0;
  jukebox_track_controller->timer->tv_nsec = 0;
}

void
monothek_jukebox_track_controller_jukebox_test_enter_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;
  
  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-test-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_test_leave_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;

  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-test-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_test_clicked_callback(MonothekActionBox *action_box,
							       MonothekJukeboxTrackController *jukebox_track_controller)
{
  monothek_jukebox_track_controller_test(jukebox_track_controller);
}

void
monothek_jukebox_track_controller_jukebox_play_enter_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;
  
  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-play-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_play_leave_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;

  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-play-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_play_clicked_callback(MonothekActionBox *action_box,
							       MonothekJukeboxTrackController *jukebox_track_controller)
{
  monothek_jukebox_track_controller_play(jukebox_track_controller);
}

void
monothek_jukebox_track_controller_jukebox_back_enter_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;
  
  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-back-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_back_leave_callback(MonothekActionBox *action_box,
							     MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekJukeboxTrackView *view;
  
  MonothekJukeboxTrackModel *model;

  g_object_get(jukebox_track_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-back-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_track_controller_jukebox_back_clicked_callback(MonothekActionBox *action_box,
							       MonothekJukeboxTrackController *jukebox_track_controller)
{
  monothek_jukebox_track_controller_back(jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_test(MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekWindow *window;
  MonothekJukeboxTrackView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - test */
  value = g_hash_table_lookup(session->value,
			      "jukebox-mode");

  g_value_set_string(value,
		     "test");

  /* change view */
  g_object_get(jukebox_track_controller,
	       "view", &view,
	       NULL);

  gtk_adjustment_set_value(view->progress,
			   0.0);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  view->flags |= MONOTHEK_JUKEBOX_TRACK_VIEW_PLAYBACK_CONTROLS;    
  view->flags &= (~(MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_CONTROLS));
      
  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, G_TYPE_NONE);

  /* start playback */
  monothek_jukebox_track_controller_run(jukebox_track_controller,
					TRUE);
}

/**
 * monothek_jukebox_track_controller_test:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Test track.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_test(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[TEST], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_play(MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekWindow *window;
  MonothekJukeboxTrackView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - play */
  value = g_hash_table_lookup(session->value,
			      "jukebox-mode");

  g_value_set_string(value,
		     "play");
  
  /* change view */
  g_object_get(jukebox_track_controller,
	       "view", &view,
	       NULL);

  gtk_adjustment_set_value(view->progress,
			   0.0);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  view->flags |= MONOTHEK_JUKEBOX_TRACK_VIEW_PLAYBACK_CONTROLS;    
  view->flags &= (~(MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_CONTROLS));

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, G_TYPE_NONE);

  /* start playback */
  monothek_jukebox_track_controller_run(jukebox_track_controller,
					TRUE);
}

/**
 * monothek_jukebox_track_controller_play:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Play track.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_play(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[PLAY], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_back(MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekWindow *window;
  MonothekJukeboxTrackView *view;

  /* change view */
  g_object_get(jukebox_track_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_PLAYLIST_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_track_controller_back:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Back track.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_back(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[BACK], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}


void
monothek_jukebox_track_controller_real_progress(MonothekJukeboxTrackController *jukebox_track_controller,
						gdouble value)
{
  MonothekJukeboxTrackView *jukebox_track_view;
  
  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_mode;

  g_object_get(jukebox_track_controller,
	       "view", &jukebox_track_view,
	       NULL);

  gtk_adjustment_set_value(jukebox_track_view->progress,
			   value);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* get jukebox mode */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  if(!g_strcmp0("play",
		g_value_get_string(jukebox_mode))){
    if(value >= 1.0){
      monothek_jukebox_track_controller_run(jukebox_track_controller,
					    FALSE);

      jukebox_track_controller->start_time->tv_sec = 0;
      jukebox_track_controller->start_time->tv_nsec = 0;
      
      /* emit completed */
      monothek_jukebox_track_controller_completed(jukebox_track_controller);
    }
  }else{
    if(jukebox_track_controller->timer->tv_sec > MONOTHEK_JUKEBOX_TRACK_CONTROLLER_TEST_TIME_SEC){
      monothek_jukebox_track_controller_run(jukebox_track_controller,
					    FALSE);

      jukebox_track_controller->start_time->tv_sec = 0;
      jukebox_track_controller->start_time->tv_nsec = 0;

      /* emit completed */
      monothek_jukebox_track_controller_completed(jukebox_track_controller);
    }
  }
}

/**
 * monothek_jukebox_track_controller_progress:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * @value: a gdouble ranging from 0.0 to 1.0
 * 
 * Notify about progress.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_progress(MonothekJukeboxTrackController *jukebox_track_controller,
					   gdouble value)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[PROGRESS], 0,
		value);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  monothek_jukebox_track_controller_run(jukebox_track_controller,
					FALSE);
}

/**
 * monothek_jukebox_track_controller_test_time_expired:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Test time of track expired.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[TEST_TIME_EXPIRED], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_run(MonothekJukeboxTrackController *jukebox_track_controller,
					   gboolean do_run)
{
  MonothekJukeboxTrackModel *jukebox_track_model;

  MonothekRack *rack;

  AgsTaskLauncher *task_launcher;
  
  AgsApplicationContext *application_context;  
  MonothekSessionManager *session_manager;
  MonothekSession *session;
  
  GValue *rack_value;
  GValue *jukebox_mode;

  application_context = ags_application_context_get_instance();

  task_launcher = ags_concurrency_provider_get_task_launcher(AGS_CONCURRENCY_PROVIDER(application_context));
  
  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  if(rack_value == NULL){
    return;
  }
  
  rack = g_value_get_object(rack_value);

  /* get jukebox mode */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  g_object_get(jukebox_track_controller,
	       "model", &jukebox_track_model,
	       NULL);
  
  if(do_run){
    MonothekAudioFileManager *audio_file_manager;
    AgsAudioFile *audio_file;
    
    AgsStartAudio *start_audio;
    MonothekExportOutput *export_output;
    AgsStartSoundcard *start_soundcard;

    MonothekExportThread *export_thread;

    GObject *output_soundcard;

    struct timespec *duration;

    GList *start_wave, *wave;
    GList *task;
    
    gchar *filename;
    
#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    GValue *filename_value;

    g_object_get(jukebox_track_model,
		 "duration", &duration,
		 NULL);

    export_thread = MONOTHEK_APPLICATION_CONTEXT(application_context)->default_export_thread;
    
    g_object_get(rack->player,
		 "output-soundcard", &output_soundcard,
		 NULL);

#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    jukebox_track_controller->start_time->tv_sec = mts.tv_sec;
    jukebox_track_controller->start_time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, jukebox_track_controller->start_time);
#endif

    jukebox_track_controller->timer->tv_sec = 0;
    jukebox_track_controller->timer->tv_nsec = 0;

    /* load track */
    audio_file_manager = monothek_audio_file_manager_get_instance();

    filename_value = g_hash_table_lookup(session->value,
					 "jukebox-song-filename");

    filename = g_value_get_string(filename_value);
    audio_file = monothek_audio_file_manager_find_audio_file(audio_file_manager,
							     filename);

    /* clear wave */
    g_object_get(rack->player,
		 "wave", &start_wave,
		 NULL);

    wave = start_wave;

    while(wave != NULL){
      ags_audio_remove_wave(rack->player,
			    wave->data);

      wave = wave->next;
    }

    g_list_free(start_wave);
    
    /* add wave */
    g_object_get(audio_file,
		 "wave", &start_wave,
		 NULL);
    
    wave = start_wave;

    while(wave != NULL){
      ags_audio_add_wave(rack->player,
			 wave->data);

      wave = wave->next;
    }

    g_list_free(start_wave);
    
    /* start audio */
    task = NULL;
    
    start_audio = ags_start_audio_new(rack->player,
				      AGS_SOUND_SCOPE_WAVE);
    task = g_list_prepend(task,
			  start_audio);
    
    if(!g_strcmp0("play",
		  g_value_get_string(jukebox_mode))){
      export_output = monothek_export_output_new(export_thread,
						 output_soundcard,
						 duration);
      task = g_list_prepend(task,
			    export_output);
    }
    
    start_soundcard = ags_start_soundcard_new(application_context);
    task = g_list_prepend(task,
			  start_soundcard);

    task = g_list_reverse(task);
    
    ags_task_launcher_add_task_all(task_launcher,
				   task);
  }else{
    AgsCancelAudio *cancel_audio;

    GList *task;
    
    /* cancel audio */
    task = NULL;
    
    cancel_audio = ags_cancel_audio_new(rack->player,
					AGS_SOUND_SCOPE_WAVE);
    task = g_list_prepend(task,
			  cancel_audio);

    task = g_list_reverse(task);
    
    ags_task_launcher_add_task_all(task_launcher,
				   task);
  }
}

/**
 * monothek_jukebox_track_controller_run:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * @do_run: if %TRUE start playback, otherwise %FALSE stop playback
 * 
 * Start/stop playback.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_run(MonothekJukeboxTrackController *jukebox_track_controller,
				      gboolean do_run)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[RUN], 0,
		do_run);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_completed(MonothekJukeboxTrackController *jukebox_track_controller)
{
  MonothekWindow *window;
  MonothekJukeboxTrackView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_mode;

  /* change view */
  g_object_get(jukebox_track_controller,
	       "view", &view,
	       NULL);

  view->flags |= MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_CONTROLS;    
  view->flags &= (~MONOTHEK_JUKEBOX_TRACK_VIEW_PLAYBACK_CONTROLS);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox mode - test */
  jukebox_mode = g_hash_table_lookup(session->value,
				     "jukebox-mode");

  if(!g_strcmp0("play",
		g_value_get_string(jukebox_mode))){
    monothek_view_reset(view,
			TRUE, TRUE);

    monothek_window_change_view(window,
				MONOTHEK_TYPE_JUKEBOX_INFO_VIEW, G_TYPE_NONE);
  }else{
    guint test_count;
    
    GValue *jukebox_test_count;

    jukebox_test_count = g_hash_table_lookup(session->value,
					     "jukebox-test-count");

    test_count = g_value_get_uint(jukebox_test_count);
    
    test_count++;
    g_value_set_uint(jukebox_test_count,
		     test_count);
    
    monothek_window_change_view(window,
				MONOTHEK_TYPE_JUKEBOX_MODE_VIEW, G_TYPE_NONE);
  }
}

/**
 * monothek_jukebox_track_controller_completed:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Termintating playback.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_completed(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[COMPLETED], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

/**
 * monothek_jukebox_track_controller_progress_increase_timeout:
 * @gobject: the gobject
 *
 * Queue draw widget
 *
 * Returns: %TRUE if proceed with redraw, otherwise %FALSE
 *
 * Since: 1.0.0
 */
gboolean
monothek_jukebox_track_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = gobject;
  
  if(g_hash_table_lookup(monothek_jukebox_track_controller_progress_increase,
			 gobject) != NULL){
    MonothekJukeboxTrackModel *jukebox_track_model;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(jukebox_track_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(jukebox_track_controller,
		 "model", &jukebox_track_model,
		 NULL);

    g_object_get(jukebox_track_model,
		 "duration", &duration,
		 NULL);
    
#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    time_now.tv_sec = mts.tv_sec;
    time_now.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, &time_now);
#endif

    /* calculate timer */
    if(time_now.tv_nsec >= jukebox_track_controller->start_time->tv_nsec){
      jukebox_track_controller->timer->tv_sec = time_now.tv_sec - jukebox_track_controller->start_time->tv_sec;
      jukebox_track_controller->timer->tv_nsec = time_now.tv_nsec - jukebox_track_controller->start_time->tv_nsec;
    }else{
      jukebox_track_controller->timer->tv_sec = time_now.tv_sec - jukebox_track_controller->start_time->tv_sec - 1;
      jukebox_track_controller->timer->tv_nsec = AGS_NSEC_PER_SEC - jukebox_track_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(duration->tv_sec > 0){
      if(duration->tv_sec < jukebox_track_controller->timer->tv_sec){
	value = 1.0;
      }else{
	value = 1.0 / duration->tv_sec * jukebox_track_controller->timer->tv_sec;
      }
    }else{
      value = 0.0;
    }
    
    monothek_jukebox_track_controller_progress(jukebox_track_controller,
					       value);
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_jukebox_track_controller_new:
 *
 * Creates an #MonothekJukeboxTrackController
 *
 * Returns: a new #MonothekJukeboxTrackController
 *
 * Since: 1.0.0
 */
MonothekJukeboxTrackController*
monothek_jukebox_track_controller_new()
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = (MonothekJukeboxTrackController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER,
									     NULL);
  
  return(jukebox_track_controller);
}
