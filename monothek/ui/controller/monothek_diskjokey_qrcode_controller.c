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

#include <monothek/ui/controller/monothek_diskjokey_qrcode_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_qrcode_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_diskjokey_qrcode_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_qrcode_controller_class_init(MonothekDiskjokeyQrcodeControllerClass *diskjokey_qrcode_controller);
void monothek_diskjokey_qrcode_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_qrcode_controller_init(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);
void monothek_diskjokey_qrcode_controller_finalize(GObject *gobject);

void monothek_diskjokey_qrcode_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_qrcode_controller_disconnect(AgsConnectable *connectable);

void monothek_diskjokey_qrcode_controller_reset(MonothekController *controller);

void monothek_diskjokey_qrcode_controller_quit_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);
void monothek_diskjokey_qrcode_controller_quit_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);
void monothek_diskjokey_qrcode_controller_quit_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);

void monothek_diskjokey_qrcode_controller_real_timeout(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);
void monothek_diskjokey_qrcode_controller_real_quit(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);

/**
 * SECTION:monothek_diskjokey_qrcode_controller
 * @short_description: The diskjokey no test controller object.
 * @title: MonothekDiskjokeyQrcodeController
 * @section_id:
 * @include: monothek/ui/controller/monothek_diskjokey_qrcode_controller.h
 *
 * #MonothekDiskjokeyQrcodeController is the MVC's diskjokey no test controller.
 */

enum{
  TIMEOUT,
  QUIT,
  LAST_SIGNAL,
};

static gpointer monothek_diskjokey_qrcode_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_qrcode_controller_parent_connectable_interface;

static guint diskjokey_qrcode_controller_signals[LAST_SIGNAL];

GHashTable *monothek_diskjokey_qrcode_controller_progress_increase = NULL;

GType
monothek_diskjokey_qrcode_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_qrcode_controller = 0;

    static const GTypeInfo monothek_diskjokey_qrcode_controller_info = {
      sizeof (MonothekDiskjokeyQrcodeControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_qrcode_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeyQrcodeController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_qrcode_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_diskjokey_qrcode_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_diskjokey_qrcode_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								       "MonothekDiskjokeyQrcodeController", &monothek_diskjokey_qrcode_controller_info,
								       0);

    g_type_add_interface_static(monothek_type_diskjokey_qrcode_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_qrcode_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_qrcode_controller_class_init(MonothekDiskjokeyQrcodeControllerClass *diskjokey_qrcode_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_diskjokey_qrcode_controller_parent_class = g_type_class_peek_parent(diskjokey_qrcode_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_qrcode_controller;

  gobject->finalize = monothek_diskjokey_qrcode_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) diskjokey_qrcode_controller;
  
  controller->reset = monothek_diskjokey_qrcode_controller_reset;

  /* MonothekDiskjokeyQrcodeController */
  diskjokey_qrcode_controller->timeout = monothek_diskjokey_qrcode_controller_real_timeout;

  diskjokey_qrcode_controller->quit = monothek_diskjokey_qrcode_controller_real_quit;

  /* signals */
  /**
   * MonothekDiskjokeyQrcodeController::timeout:
   * @diskjokey_qrcode_controller: the #MonothekDiskjokeyQrcodeController
   *
   * The ::timeout signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  diskjokey_qrcode_controller_signals[TIMEOUT] =
    g_signal_new("timeout",
		 G_TYPE_FROM_CLASS(diskjokey_qrcode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeyQrcodeControllerClass, timeout),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekDiskjokeyQrcodeController::quit:
   * @diskjokey_qrcode_controller: the #MonothekDiskjokeyQrcodeController
   *
   * The ::quit signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  diskjokey_qrcode_controller_signals[QUIT] =
    g_signal_new("quit",
		 G_TYPE_FROM_CLASS(diskjokey_qrcode_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeyQrcodeControllerClass, quit),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_diskjokey_qrcode_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_diskjokey_qrcode_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_diskjokey_qrcode_controller_connect;
  connectable->disconnect = monothek_diskjokey_qrcode_controller_disconnect;
}

void
monothek_diskjokey_qrcode_controller_init(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  MonothekActionBox *action_box;

  diskjokey_qrcode_controller->quit =
    action_box = g_object_new(MONOTHEK_TYPE_ACTION_BOX,
			      "action-identifier", "quit",
			      "x0", 800,
			      "y0", 840,
			      "width", 320,
			      "height", 140,
			      NULL);
  monothek_controller_add_action_box(diskjokey_qrcode_controller,
				     action_box);

  /* progress */
  if(monothek_diskjokey_qrcode_controller_progress_increase == NULL){
    monothek_diskjokey_qrcode_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										   NULL,
										   NULL);
  }

  diskjokey_qrcode_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_qrcode_controller->start_time->tv_sec = 0;
  diskjokey_qrcode_controller->start_time->tv_nsec = 0;
  
  diskjokey_qrcode_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_qrcode_controller->timer->tv_sec = 0;
  diskjokey_qrcode_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_diskjokey_qrcode_controller_progress_increase,
		      diskjokey_qrcode_controller, monothek_diskjokey_qrcode_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 2,
		(GSourceFunc) monothek_diskjokey_qrcode_controller_progress_increase_timeout,
		(gpointer) diskjokey_qrcode_controller);
}

void
monothek_diskjokey_qrcode_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = (MonothekDiskjokeyQrcodeController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_diskjokey_qrcode_controller_progress_increase,
		      diskjokey_qrcode_controller);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_qrcode_controller_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_qrcode_controller_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER(connectable);

  if(monothek_controller_test_flags(diskjokey_qrcode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_qrcode_controller_parent_connectable_interface->connect(connectable);

  g_signal_connect(diskjokey_qrcode_controller->quit, "enter",
		   G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_enter_callback), diskjokey_qrcode_controller);
  g_signal_connect(diskjokey_qrcode_controller->quit, "leave",
		   G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_leave_callback), diskjokey_qrcode_controller);
  g_signal_connect(diskjokey_qrcode_controller->quit, "clicked",
		   G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_clicked_callback), diskjokey_qrcode_controller);
}

void
monothek_diskjokey_qrcode_controller_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(diskjokey_qrcode_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_qrcode_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(diskjokey_qrcode_controller->quit,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_enter_callback),
		      diskjokey_qrcode_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_leave_callback),
		      diskjokey_qrcode_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_qrcode_controller_quit_clicked_callback),
		      diskjokey_qrcode_controller,
		      NULL);
}

void
monothek_diskjokey_qrcode_controller_reset(MonothekController *controller)
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER(controller);

  /* reset timer */
#ifdef __APPLE__
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
    
  diskjokey_qrcode_controller->start_time->tv_sec = mts.tv_sec;
  diskjokey_qrcode_controller->start_time->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, diskjokey_qrcode_controller->start_time);
#endif  
}

void
monothek_diskjokey_qrcode_controller_quit_enter_callback(MonothekActionBox *action_box,
							 MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  MonothekDiskjokeyQrcodeView *view;
  
  MonothekDiskjokeyQrcodeModel *model;
  
  g_object_get(diskjokey_qrcode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  model->quit_active = TRUE;

  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_qrcode_controller_quit_leave_callback(MonothekActionBox *action_box,
							 MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  MonothekDiskjokeyQrcodeView *view;
  
  MonothekDiskjokeyQrcodeModel *model;
  
  g_object_get(diskjokey_qrcode_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  model->quit_active = FALSE;

  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_qrcode_controller_quit_clicked_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  monothek_diskjokey_qrcode_controller_quit(diskjokey_qrcode_controller);
}

void
monothek_diskjokey_qrcode_controller_real_timeout(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeyQrcodeView *view;
  
  diskjokey_qrcode_controller->start_time->tv_sec = 0;
  diskjokey_qrcode_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(diskjokey_qrcode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_qrcode_controller_timeout:
 * @diskjokey_qrcode_controller: the #MonothekDiskjokeyQrcodeController
 * 
 * Timeout qrcode.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_qrcode_controller_timeout(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_QRCODE_CONTROLLER(diskjokey_qrcode_controller));
  
  g_object_ref((GObject *) diskjokey_qrcode_controller);
  g_signal_emit(G_OBJECT(diskjokey_qrcode_controller),
		diskjokey_qrcode_controller_signals[TIMEOUT], 0);
  g_object_unref((GObject *) diskjokey_qrcode_controller);
}

void
monothek_diskjokey_qrcode_controller_real_quit(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeyQrcodeView *view;
  
  diskjokey_qrcode_controller->start_time->tv_sec = 0;
  diskjokey_qrcode_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(diskjokey_qrcode_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_qrcode_controller_quit:
 * @diskjokey_qrcode_controller: the #MonothekDiskjokeyQrcodeController
 * 
 * Quit qrcode.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_qrcode_controller_quit(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_QRCODE_CONTROLLER(diskjokey_qrcode_controller));
  
  g_object_ref((GObject *) diskjokey_qrcode_controller);
  g_signal_emit(G_OBJECT(diskjokey_qrcode_controller),
		diskjokey_qrcode_controller_signals[QUIT], 0);
  g_object_unref((GObject *) diskjokey_qrcode_controller);
}

gboolean
monothek_diskjokey_qrcode_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = gobject;
  
  if(g_hash_table_lookup(monothek_diskjokey_qrcode_controller_progress_increase,
			 gobject) != NULL){
    MonothekDiskjokeyQrcodeModel *diskjokey_qrcode_model;

    MonothekDiskjokeyQrcodeView *diskjokey_qrcode_view;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(diskjokey_qrcode_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(diskjokey_qrcode_controller,
		 "model", &diskjokey_qrcode_model,
		 "view", &diskjokey_qrcode_view,
		 NULL);
    
    g_object_get(diskjokey_qrcode_model,
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
    if(time_now.tv_nsec >= diskjokey_qrcode_controller->start_time->tv_nsec){
      diskjokey_qrcode_controller->timer->tv_sec = time_now.tv_sec - diskjokey_qrcode_controller->start_time->tv_sec;
      diskjokey_qrcode_controller->timer->tv_nsec = time_now.tv_nsec - diskjokey_qrcode_controller->start_time->tv_nsec;
    }else{
      diskjokey_qrcode_controller->timer->tv_sec = time_now.tv_sec - diskjokey_qrcode_controller->start_time->tv_sec - 1;
      diskjokey_qrcode_controller->timer->tv_nsec = AGS_NSEC_PER_SEC - diskjokey_qrcode_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(time_now.tv_sec - diskjokey_qrcode_controller->start_time->tv_sec < duration->tv_sec){
      gtk_widget_queue_draw(diskjokey_qrcode_view);
    }else{
      monothek_diskjokey_qrcode_controller_timeout(diskjokey_qrcode_controller);
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_diskjokey_qrcode_controller_new:
 *
 * Creates an #MonothekDiskjokeyQrcodeController
 *
 * Returns: a new #MonothekDiskjokeyQrcodeController
 *
 * Since: 1.0.0
 */
MonothekDiskjokeyQrcodeController*
monothek_diskjokey_qrcode_controller_new()
{
  MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller;

  diskjokey_qrcode_controller = (MonothekDiskjokeyQrcodeController *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER,
										   NULL);
  
  return(diskjokey_qrcode_controller);
}
