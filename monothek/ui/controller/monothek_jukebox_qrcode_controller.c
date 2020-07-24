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

#include <monothek/ui/controller/monothek_jukebox_qrcode_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_qrcode_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_jukebox_qrcode_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_qrcode_controller_class_init(MonothekJukeboxQrcodeControllerClass *jukebox_qrcode_controller);
void monothek_jukebox_qrcode_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_qrcode_controller_init(MonothekJukeboxQrcodeController *jukebox_qrcode_controller);
void monothek_jukebox_qrcode_controller_finalize(GObject *gobject);

void monothek_jukebox_qrcode_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_qrcode_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_qrcode_controller_reset(MonothekController *controller);

void monothek_jukebox_qrcode_controller_quit_enter_callback(MonothekActionBox *action_box,
							    MonothekJukeboxQrcodeController *jukebox_qrcode_controller);
void monothek_jukebox_qrcode_controller_quit_leave_callback(MonothekActionBox *action_box,
							    MonothekJukeboxQrcodeController *jukebox_qrcode_controller);
void monothek_jukebox_qrcode_controller_quit_clicked_callback(MonothekActionBox *action_box,
							      MonothekJukeboxQrcodeController *jukebox_qrcode_controller);

void monothek_jukebox_qrcode_controller_real_timeout(MonothekJukeboxQrcodeController *jukebox_qrcode_controller);
void monothek_jukebox_qrcode_controller_real_quit(MonothekJukeboxQrcodeController *jukebox_qrcode_controller);

/**
 * SECTION:monothek_jukebox_qrcode_controller
 * @short_description: The jukebox no test controller object.
 * @title: MonothekJukeboxQrcodeController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_qrcode_controller.h
 *
 * #MonothekJukeboxQrcodeController is the MVC's jukebox no test controller.
 */

enum{
  TIMEOUT,
  QUIT,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_qrcode_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_qrcode_controller_parent_connectable_interface;

static guint jukebox_qrcode_controller_signals[LAST_SIGNAL];

GHashTable *monothek_jukebox_qrcode_controller_progress_increase = NULL;

GType
monothek_jukebox_qrcode_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_qrcode_controller = 0;

    static const GTypeInfo monothek_jukebox_qrcode_controller_info = {
      sizeof (MonothekJukeboxQrcodeControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_qrcode_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxQrcodeController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_qrcode_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_qrcode_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_qrcode_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								     "MonothekJukeboxQrcodeController", &monothek_jukebox_qrcode_controller_info,
								     0);

    g_type_add_interface_static(monothek_type_jukebox_qrcode_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_qrcode_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_qrcode_controller_class_init(MonothekJukeboxQrcodeControllerClass *jukebox_qrcode_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_jukebox_qrcode_controller_parent_class = g_type_class_peek_parent(jukebox_qrcode_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_qrcode_controller;

  gobject->finalize = monothek_jukebox_qrcode_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) jukebox_qrcode_controller;
  
  controller->reset = monothek_jukebox_qrcode_controller_reset;

  /* MonothekJukeboxQrcodeController */
  jukebox_qrcode_controller->timeout = monothek_jukebox_qrcode_controller_real_timeout;

  jukebox_qrcode_controller->quit = monothek_jukebox_qrcode_controller_real_quit;

  /* signals */
  /**
   * MonothekJukeboxQrcodeController::timeout:
   * @jukebox_qrcode_controller: the #MonothekJukeboxQrcodeController
   *
   * The ::timeout signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_qrcode_controller_signals[TIMEOUT] =
    g_signal_new("timeout",
		 G_TYPE_FROM_CLASS(jukebox_qrcode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxQrcodeControllerClass, timeout),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxQrcodeController::quit:
   * @jukebox_qrcode_controller: the #MonothekJukeboxQrcodeController
   *
   * The ::quit signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_qrcode_controller_signals[QUIT] =
    g_signal_new("quit",
		 G_TYPE_FROM_CLASS(jukebox_qrcode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxQrcodeControllerClass, quit),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_qrcode_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_qrcode_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_qrcode_controller_connect;
  connectable->disconnect = monothek_jukebox_qrcode_controller_disconnect;
}

void
monothek_jukebox_qrcode_controller_init(MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  MonothekActionBox *action_box;

  jukebox_qrcode_controller->quit =
    action_box = g_object_new(MONOTHEK_TYPE_ACTION_BOX,
			      "action-identifier", "quit",
			      "x0", 800,
			      "y0", 840,
			      "width", 320,
			      "height", 140,
			      NULL);
  monothek_controller_add_action_box(jukebox_qrcode_controller,
				     action_box);

  /* progress */
  if(monothek_jukebox_qrcode_controller_progress_increase == NULL){
    monothek_jukebox_qrcode_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										 NULL,
										 NULL);
  }

  jukebox_qrcode_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  jukebox_qrcode_controller->start_time->tv_sec = 0;
  jukebox_qrcode_controller->start_time->tv_nsec = 0;
  
  jukebox_qrcode_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  jukebox_qrcode_controller->timer->tv_sec = 0;
  jukebox_qrcode_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_jukebox_qrcode_controller_progress_increase,
		      jukebox_qrcode_controller, monothek_jukebox_qrcode_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 2,
		(GSourceFunc) monothek_jukebox_qrcode_controller_progress_increase_timeout,
		(gpointer) jukebox_qrcode_controller);
}

void
monothek_jukebox_qrcode_controller_finalize(GObject *gobject)
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = (MonothekJukeboxQrcodeController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_jukebox_qrcode_controller_progress_increase,
		      jukebox_qrcode_controller);
    
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_qrcode_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_qrcode_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = MONOTHEK_JUKEBOX_QRCODE_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_qrcode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_qrcode_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_qrcode_controller->quit, "enter",
		   G_CALLBACK(monothek_jukebox_qrcode_controller_quit_enter_callback), jukebox_qrcode_controller);
  g_signal_connect(jukebox_qrcode_controller->quit, "leave",
		   G_CALLBACK(monothek_jukebox_qrcode_controller_quit_leave_callback), jukebox_qrcode_controller);
  g_signal_connect(jukebox_qrcode_controller->quit, "clicked",
		   G_CALLBACK(monothek_jukebox_qrcode_controller_quit_clicked_callback), jukebox_qrcode_controller);
}

void
monothek_jukebox_qrcode_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = MONOTHEK_JUKEBOX_QRCODE_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_qrcode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_qrcode_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_qrcode_controller->quit,
		      "any_signal::enter",
		      G_CALLBACK(monothek_jukebox_qrcode_controller_quit_enter_callback),
		      jukebox_qrcode_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_jukebox_qrcode_controller_quit_leave_callback),
		      jukebox_qrcode_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_jukebox_qrcode_controller_quit_clicked_callback),
		      jukebox_qrcode_controller,
		      NULL);
}

void
monothek_jukebox_qrcode_controller_reset(MonothekController *controller)
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = MONOTHEK_JUKEBOX_QRCODE_CONTROLLER(controller);

  /* reset timer */
#ifdef __APPLE__
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
    
  jukebox_qrcode_controller->start_time->tv_sec = mts.tv_sec;
  jukebox_qrcode_controller->start_time->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, jukebox_qrcode_controller->start_time);
#endif  
}

void
monothek_jukebox_qrcode_controller_quit_enter_callback(MonothekActionBox *action_box,
						       MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  MonothekJukeboxQrcodeView *view;
  
  MonothekJukeboxQrcodeModel *model;
  
  g_object_get(jukebox_qrcode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  model->quit_active = TRUE;

  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_qrcode_controller_quit_leave_callback(MonothekActionBox *action_box,
						       MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  MonothekJukeboxQrcodeView *view;
  
  MonothekJukeboxQrcodeModel *model;
  
  g_object_get(jukebox_qrcode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  model->quit_active = FALSE;

  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_qrcode_controller_quit_clicked_callback(MonothekActionBox *action_box,
							 MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  monothek_jukebox_qrcode_controller_quit(jukebox_qrcode_controller);
}

void
monothek_jukebox_qrcode_controller_real_timeout(MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  MonothekWindow *window;
  MonothekJukeboxQrcodeView *view;
  
  jukebox_qrcode_controller->start_time->tv_sec = 0;
  jukebox_qrcode_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(jukebox_qrcode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_qrcode_controller_timeout:
 * @jukebox_qrcode_controller: the #MonothekJukeboxQrcodeController
 * 
 * Timeout qrcode.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_qrcode_controller_timeout(MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_QRCODE_CONTROLLER(jukebox_qrcode_controller));
  
  g_object_ref((GObject *) jukebox_qrcode_controller);
  g_signal_emit(G_OBJECT(jukebox_qrcode_controller),
		jukebox_qrcode_controller_signals[TIMEOUT], 0);
  g_object_unref((GObject *) jukebox_qrcode_controller);
}

void
monothek_jukebox_qrcode_controller_real_quit(MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  MonothekWindow *window;
  MonothekJukeboxQrcodeView *view;
  
  jukebox_qrcode_controller->start_time->tv_sec = 0;
  jukebox_qrcode_controller->start_time->tv_nsec = 0;
      
  /* change view */
  g_object_get(jukebox_qrcode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_qrcode_controller_quit:
 * @jukebox_qrcode_controller: the #MonothekJukeboxQrcodeController
 * 
 * Quit qrcode.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_qrcode_controller_quit(MonothekJukeboxQrcodeController *jukebox_qrcode_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_QRCODE_CONTROLLER(jukebox_qrcode_controller));
  
  g_object_ref((GObject *) jukebox_qrcode_controller);
  g_signal_emit(G_OBJECT(jukebox_qrcode_controller),
		jukebox_qrcode_controller_signals[QUIT], 0);
  g_object_unref((GObject *) jukebox_qrcode_controller);
}

gboolean
monothek_jukebox_qrcode_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = gobject;
  
  if(g_hash_table_lookup(monothek_jukebox_qrcode_controller_progress_increase,
			 gobject) != NULL){
    MonothekJukeboxQrcodeModel *jukebox_qrcode_model;

    MonothekJukeboxQrcodeView *jukebox_qrcode_view;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(jukebox_qrcode_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(jukebox_qrcode_controller,
		 "model", &jukebox_qrcode_model,
		 "view", &jukebox_qrcode_view,
		 NULL);
    
    g_object_get(jukebox_qrcode_model,
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
    if(time_now.tv_nsec >= jukebox_qrcode_controller->start_time->tv_nsec){
      jukebox_qrcode_controller->timer->tv_sec = time_now.tv_sec - jukebox_qrcode_controller->start_time->tv_sec;
      jukebox_qrcode_controller->timer->tv_nsec = time_now.tv_nsec - jukebox_qrcode_controller->start_time->tv_nsec;
    }else{
      jukebox_qrcode_controller->timer->tv_sec = time_now.tv_sec - jukebox_qrcode_controller->start_time->tv_sec - 1;
      jukebox_qrcode_controller->timer->tv_nsec = AGS_NSEC_PER_SEC - jukebox_qrcode_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(time_now.tv_sec - jukebox_qrcode_controller->start_time->tv_sec < duration->tv_sec){
      gtk_widget_queue_draw(jukebox_qrcode_view);
    }else{
      monothek_jukebox_qrcode_controller_timeout(jukebox_qrcode_controller);
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_jukebox_qrcode_controller_new:
 *
 * Creates an #MonothekJukeboxQrcodeController
 *
 * Returns: a new #MonothekJukeboxQrcodeController
 *
 * Since: 1.0.0
 */
MonothekJukeboxQrcodeController*
monothek_jukebox_qrcode_controller_new()
{
  MonothekJukeboxQrcodeController *jukebox_qrcode_controller;

  jukebox_qrcode_controller = (MonothekJukeboxQrcodeController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_QRCODE_CONTROLLER,
									       NULL);
  
  return(jukebox_qrcode_controller);
}
