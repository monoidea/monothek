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

#include <monothek/ui/controller/monothek_transaction_failed_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/audio/file/monothek_audio_file_manager.h>

#include <monothek/ui/model/monothek_transaction_failed_model.h>

#include <monothek/ui/monothek_application_context.h>
#include <monothek/ui/monothek_window.h>

#include <monothek/ui/view/monothek_transaction_failed_view.h>
#include <monothek/ui/view/monothek_start_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_transaction_failed_controller_class_init(MonothekTransactionFailedControllerClass *transaction_failed_controller);
void monothek_transaction_failed_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_transaction_failed_controller_init(MonothekTransactionFailedController *transaction_failed_controller);
void monothek_transaction_failed_controller_finalize(GObject *gobject);

void monothek_transaction_failed_controller_connect(AgsConnectable *connectable);
void monothek_transaction_failed_controller_disconnect(AgsConnectable *connectable);

void monothek_transaction_failed_controller_reset(MonothekController *controller);

void monothek_transaction_failed_controller_real_timeout(MonothekTransactionFailedController *transaction_failed_controller);

/**
 * SECTION:monothek_transaction_failed_controller
 * @short_description: The transaction failed controller object.
 * @title: MonothekTransactionFailedController
 * @section_id:
 * @include: monothek/ui/controller/monothek_transaction_failed_controller.h
 *
 * #MonothekTransactionFailedController is the MVC's transaction failed controller.
 */

enum{
  TIMEOUT,
  LAST_SIGNAL,
};

static gpointer monothek_transaction_failed_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_transaction_failed_controller_parent_connectable_interface;

static guint transaction_failed_controller_signals[LAST_SIGNAL];

GHashTable *monothek_transaction_failed_controller_progress_increase = NULL;

GType
monothek_transaction_failed_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_transaction_failed_controller = 0;

    static const GTypeInfo monothek_transaction_failed_controller_info = {
      sizeof (MonothekTransactionFailedControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_transaction_failed_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekTransactionFailedController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_transaction_failed_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_transaction_failed_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_transaction_failed_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
									 "MonothekTransactionFailedController", &monothek_transaction_failed_controller_info,
									 0);

    g_type_add_interface_static(monothek_type_transaction_failed_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_transaction_failed_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_transaction_failed_controller_class_init(MonothekTransactionFailedControllerClass *transaction_failed_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_transaction_failed_controller_parent_class = g_type_class_peek_parent(transaction_failed_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) transaction_failed_controller;

  gobject->finalize = monothek_transaction_failed_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) transaction_failed_controller;

  controller->reset = monothek_transaction_failed_controller_reset;
  
  /* MonothekTransactionFailedController */
  transaction_failed_controller->timeout = monothek_transaction_failed_controller_real_timeout;

  /* signals */
  /**
   * MonothekTransactionFailedController::timeout:
   * @transaction_failed_controller: the #MonothekTransactionFailedController
   *
   * The ::timeout signal notifies about terminating playback.
   *
   * Since: 1.0.0
   */
  transaction_failed_controller_signals[TIMEOUT] =
    g_signal_new("timeout",
		 G_TYPE_FROM_CLASS(transaction_failed_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekTransactionFailedControllerClass, timeout),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_transaction_failed_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_transaction_failed_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_transaction_failed_controller_connect;
  connectable->disconnect = monothek_transaction_failed_controller_disconnect;
}

void
monothek_transaction_failed_controller_init(MonothekTransactionFailedController *transaction_failed_controller)
{  
  /* progress */
  if(monothek_transaction_failed_controller_progress_increase == NULL){
    monothek_transaction_failed_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										     NULL,
										     NULL);
  }

  transaction_failed_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  transaction_failed_controller->start_time->tv_sec = 0;
  transaction_failed_controller->start_time->tv_nsec = 0;
  
  transaction_failed_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  transaction_failed_controller->timer->tv_sec = 0;
  transaction_failed_controller->timer->tv_nsec = 0;

  /* timeout - add */
  g_hash_table_insert(monothek_transaction_failed_controller_progress_increase,
		      transaction_failed_controller, monothek_transaction_failed_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 30,
		(GSourceFunc) monothek_transaction_failed_controller_progress_increase_timeout,
		(gpointer) transaction_failed_controller);
}

void
monothek_transaction_failed_controller_finalize(GObject *gobject)
{
  MonothekTransactionFailedController *transaction_failed_controller;

  transaction_failed_controller = (MonothekTransactionFailedController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_transaction_failed_controller_progress_increase,
		      transaction_failed_controller);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_transaction_failed_controller_parent_class)->finalize(gobject);
}

void
monothek_transaction_failed_controller_connect(AgsConnectable *connectable)
{
  MonothekTransactionFailedController *transaction_failed_controller;

  transaction_failed_controller = MONOTHEK_TRANSACTION_FAILED_CONTROLLER(connectable);

  if(monothek_controller_test_flags(transaction_failed_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_transaction_failed_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_transaction_failed_controller_disconnect(AgsConnectable *connectable)
{
  MonothekTransactionFailedController *transaction_failed_controller;

  transaction_failed_controller = MONOTHEK_TRANSACTION_FAILED_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(transaction_failed_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_transaction_failed_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_transaction_failed_controller_reset(MonothekController *controller)
{
  MonothekTransactionFailedController *transaction_failed_controller;

#ifdef __APPLE__
  clock_serv_t cclock;
  mach_timespec_t mts;
#endif

  transaction_failed_controller = MONOTHEK_TRANSACTION_FAILED_CONTROLLER(controller);
  
#ifdef __APPLE__
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
    
  transaction_failed_controller->start_time->tv_sec = mts.tv_sec;
  transaction_failed_controller->start_time->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, transaction_failed_controller->start_time);
#endif
}

void
monothek_transaction_failed_controller_real_timeout(MonothekTransactionFailedController *transaction_failed_controller)
{
  MonothekWindow *window;
  MonothekTransactionFailedView *view;
  
  transaction_failed_controller->start_time->tv_sec = 0;
  transaction_failed_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(transaction_failed_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
}

/**
 * monothek_transaction_failed_controller_timeout:
 * @transaction_failed_controller: the #MonothekTransactionFailedController
 * 
 * Termintating playback.
 * 
 * Since: 1.0.0
 */
void
monothek_transaction_failed_controller_timeout(MonothekTransactionFailedController *transaction_failed_controller)
{
  g_return_if_fail(MONOTHEK_IS_TRANSACTION_FAILED_CONTROLLER(transaction_failed_controller));
  
  g_object_ref((GObject *) transaction_failed_controller);
  g_signal_emit(G_OBJECT(transaction_failed_controller),
		transaction_failed_controller_signals[TIMEOUT], 0);
  g_object_unref((GObject *) transaction_failed_controller);
}

gboolean
monothek_transaction_failed_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekTransactionFailedController *transaction_failed_controller;

  transaction_failed_controller = gobject;
  
  if(g_hash_table_lookup(monothek_transaction_failed_controller_progress_increase,
			 gobject) != NULL){
    MonothekTransactionFailedModel *transaction_failed_model;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(transaction_failed_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(transaction_failed_controller,
		 "model", &transaction_failed_model,
		 NULL);
    
    g_object_get(transaction_failed_model,
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
    if(time_now.tv_nsec >= transaction_failed_controller->start_time->tv_nsec){
      transaction_failed_controller->timer->tv_sec = time_now.tv_sec - transaction_failed_controller->start_time->tv_sec;
      transaction_failed_controller->timer->tv_nsec = time_now.tv_nsec - transaction_failed_controller->start_time->tv_nsec;
    }else{
      transaction_failed_controller->timer->tv_sec = time_now.tv_sec - transaction_failed_controller->start_time->tv_sec - 1;
      transaction_failed_controller->timer->tv_nsec = AGS_NSEC_PER_SEC - transaction_failed_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(duration->tv_sec > 0){
      if(duration->tv_sec < transaction_failed_controller->timer->tv_sec){
	value = 1.0;
      }else{
	value = 1.0 / duration->tv_sec * transaction_failed_controller->timer->tv_sec;
      }
    }else{
      value = 0.0;
    }

    if(value == 1.0){
      monothek_transaction_failed_controller_timeout(transaction_failed_controller);
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_transaction_failed_controller_new:
 *
 * Creates an #MonothekTransactionFailedController
 *
 * Returns: a new #MonothekTransactionFailedController
 *
 * Since: 1.0.0
 */
MonothekTransactionFailedController*
monothek_transaction_failed_controller_new()
{
  MonothekTransactionFailedController *transaction_failed_controller;

  transaction_failed_controller = (MonothekTransactionFailedController *) g_object_new(MONOTHEK_TYPE_TRANSACTION_FAILED_CONTROLLER,
										       NULL);
  
  return(transaction_failed_controller);
}
