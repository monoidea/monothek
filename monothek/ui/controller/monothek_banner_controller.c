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

#include <monothek/ui/model/monothek_banner_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_screensaver_view.h>
#include <monothek/ui/view/monothek_banner_view.h>

#include <stdlib.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <monothek/i18n.h>

void monothek_banner_controller_class_init(MonothekBannerControllerClass *banner_controller);
void monothek_banner_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_banner_controller_init(MonothekBannerController *banner_controller);
void monothek_banner_controller_finalize(GObject *gobject);

void monothek_banner_controller_connect(AgsConnectable *connectable);
void monothek_banner_controller_disconnect(AgsConnectable *connectable);

void monothek_banner_controller_reset(MonothekController *controller);

void monothek_banner_controller_enter_monothek_enter_callback(MonothekActionBox *action_box,
							      MonothekBannerController *banner_controller);
void monothek_banner_controller_enter_monothek_leave_callback(MonothekActionBox *action_box,
							      MonothekBannerController *banner_controller);
void monothek_banner_controller_enter_monothek_clicked_callback(MonothekActionBox *action_box,
								MonothekBannerController *banner_controller);

void monothek_banner_controller_real_timeout(MonothekBannerController *banner_controller);
void monothek_banner_controller_real_enter_monothek(MonothekBannerController *banner_controller);

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
  TIMEOUT,
  ENTER_MONOTHEK,
  LAST_SIGNAL,
};

static gpointer monothek_banner_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_banner_controller_parent_connectable_interface;

static guint banner_controller_signals[LAST_SIGNAL];

GHashTable *monothek_banner_controller_progress_increase = NULL;

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

  /* MonothekBannerController */
  banner_controller->timeout = monothek_banner_controller_real_timeout;

  banner_controller->enter_monothek = monothek_banner_controller_real_enter_monothek;

  /* signals */
  /**
   * MonothekBannerController::timeout:
   * @banner_controller: the #MonothekBannerController
   *
   * The ::timeout signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  banner_controller_signals[TIMEOUT] =
    g_signal_new("timeout",
		 G_TYPE_FROM_CLASS(banner_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekBannerControllerClass, timeout),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekBannerController::enter-monothek:
   * @banner_controller: the #MonothekBannerController
   *
   * The ::enter-monothek signal notifies about entering monothek.
   *
   * Since: 1.0.0
   */
  banner_controller_signals[ENTER_MONOTHEK] =
    g_signal_new("enter-monothek",
		 G_TYPE_FROM_CLASS(banner_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekBannerControllerClass, enter_monothek),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
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
  MonothekActionBox *action_box;

  banner_controller->enter_monothek = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "enter-monothek",
						    "x0", 0,
						    "y0", 0,
						    "width", 1920,
						    "height", 1080,
						    NULL);
  monothek_controller_add_action_box(banner_controller,
				     action_box);

  /* progress */
  if(monothek_banner_controller_progress_increase == NULL){
    monothek_banner_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										   NULL,
										   NULL);
  }

  banner_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  banner_controller->start_time->tv_sec = 0;
  banner_controller->start_time->tv_nsec = 0;
  
  banner_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  banner_controller->timer->tv_sec = 0;
  banner_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_banner_controller_progress_increase,
		      banner_controller, monothek_banner_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 2,
		(GSourceFunc) monothek_banner_controller_progress_increase_timeout,
		(gpointer) banner_controller);
}

void
monothek_banner_controller_finalize(GObject *gobject)
{
  MonothekBannerController *banner_controller;

  banner_controller = (MonothekBannerController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_banner_controller_progress_increase,
		      banner_controller);
  
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

  g_signal_connect(banner_controller->enter_monothek, "enter",
		   G_CALLBACK(monothek_banner_controller_enter_monothek_enter_callback), banner_controller);
  g_signal_connect(banner_controller->enter_monothek, "leave",
		   G_CALLBACK(monothek_banner_controller_enter_monothek_leave_callback), banner_controller);
  g_signal_connect(banner_controller->enter_monothek, "clicked",
		   G_CALLBACK(monothek_banner_controller_enter_monothek_clicked_callback), banner_controller);
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

  g_object_disconnect(banner_controller->enter_monothek,
		      "any_signal::enter",
		      G_CALLBACK(monothek_banner_controller_enter_monothek_enter_callback),
		      banner_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_banner_controller_enter_monothek_leave_callback),
		      banner_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_banner_controller_enter_monothek_clicked_callback),
		      banner_controller,
		      NULL);
}

void
monothek_banner_controller_reset(MonothekController *controller)
{
  MonothekBannerController *banner_controller;

  banner_controller = MONOTHEK_BANNER_CONTROLLER(controller);

  /* reset timer */
#ifdef __APPLE__
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
    
  banner_controller->start_time->tv_sec = mts.tv_sec;
  banner_controller->start_time->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, banner_controller->start_time);
#endif  
}

void
monothek_banner_controller_enter_monothek_enter_callback(MonothekActionBox *action_box,
							 MonothekBannerController *banner_controller)
{
  //TODO:JK: implement me
}

void
monothek_banner_controller_enter_monothek_leave_callback(MonothekActionBox *action_box,
							 MonothekBannerController *banner_controller)
{
  //TODO:JK: implement me
}

void
monothek_banner_controller_enter_monothek_clicked_callback(MonothekActionBox *action_box,
							   MonothekBannerController *banner_controller)
{
  monothek_banner_controller_enter_monothek(banner_controller);
}


void
monothek_banner_controller_real_timeout(MonothekBannerController *banner_controller)
{
  MonothekWindow *window;
  MonothekBannerView *view;
  
  banner_controller->start_time->tv_sec = 0;
  banner_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(banner_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_SCREENSAVER_VIEW, G_TYPE_NONE);
}

/**
 * monothek_banner_controller_timeout:
 * @banner_controller: the #MonothekBannerController
 * 
 * Timeout qrcode.
 * 
 * Since: 1.0.0
 */
void
monothek_banner_controller_timeout(MonothekBannerController *banner_controller)
{
  g_return_if_fail(MONOTHEK_IS_BANNER_CONTROLLER(banner_controller));
  
  g_object_ref((GObject *) banner_controller);
  g_signal_emit(G_OBJECT(banner_controller),
		banner_controller_signals[TIMEOUT], 0);
  g_object_unref((GObject *) banner_controller);
}

void
monothek_banner_controller_real_enter_monothek(MonothekBannerController *banner_controller)
{
  MonothekWindow *window;
  MonothekBannerView *view;
  
  banner_controller->start_time->tv_sec = 0;
  banner_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(banner_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_banner_controller_enter_monothek:
 * @banner_controller: the #MonothekBannerController
 * 
 * Stop banner.
 * 
 * Since: 1.0.0
 */
void
monothek_banner_controller_enter_monothek(MonothekBannerController *banner_controller)
{
  g_return_if_fail(MONOTHEK_IS_BANNER_CONTROLLER(banner_controller));
  
  g_object_ref((GObject *) banner_controller);
  g_signal_emit(G_OBJECT(banner_controller),
		banner_controller_signals[ENTER_MONOTHEK], 0);
  g_object_unref((GObject *) banner_controller);
}


gboolean
monothek_banner_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekBannerController *banner_controller;

  banner_controller = gobject;
  
  if(g_hash_table_lookup(monothek_banner_controller_progress_increase,
			 gobject) != NULL){
    MonothekBannerModel *banner_model;

    MonothekBannerView *banner_view;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(banner_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(banner_controller,
		 "model", &banner_model,
		 "view", &banner_view,
		 NULL);
    
    g_object_get(banner_model,
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
    if(time_now.tv_nsec >= banner_controller->start_time->tv_nsec){
      banner_controller->timer->tv_sec = time_now.tv_sec - banner_controller->start_time->tv_sec;
      banner_controller->timer->tv_nsec = time_now.tv_nsec - banner_controller->start_time->tv_nsec;
    }else{
      banner_controller->timer->tv_sec = time_now.tv_sec - banner_controller->start_time->tv_sec - 1;
      banner_controller->timer->tv_nsec = NSEC_PER_SEC - banner_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(time_now.tv_sec - banner_controller->start_time->tv_sec < duration->tv_sec){
      gtk_widget_queue_draw(banner_view);
    }else{
      monothek_banner_controller_timeout(banner_controller);
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
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
