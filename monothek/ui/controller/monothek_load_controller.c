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

#include <monothek/ui/controller/monothek_load_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/audio/file/monothek_audio_file_manager.h>

#include <monothek/ui/monothek_application_context.h>
#include <monothek/ui/monothek_window.h>

#include <monothek/ui/view/monothek_load_view.h>
#include <monothek/ui/view/monothek_start_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_load_controller_class_init(MonothekLoadControllerClass *load_controller);
void monothek_load_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_load_controller_init(MonothekLoadController *load_controller);
void monothek_load_controller_finalize(GObject *gobject);

void monothek_load_controller_connect(AgsConnectable *connectable);
void monothek_load_controller_disconnect(AgsConnectable *connectable);

void monothek_load_controller_real_progress(MonothekLoadController *load_controller,
					    gdouble value);
void monothek_load_controller_real_completed(MonothekLoadController *load_controller);

/**
 * SECTION:monothek_load_controller
 * @short_description: The load controller object.
 * @title: MonothekLoadController
 * @section_id:
 * @include: monothek/ui/controller/monothek_load_controller.h
 *
 * #MonothekLoadController is the MVC's load controller.
 */

enum{
  PROGRESS,
  COMPLETED,
  LAST_SIGNAL,
};

static gpointer monothek_load_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_load_controller_parent_connectable_interface;

static guint load_controller_signals[LAST_SIGNAL];

GHashTable *monothek_load_controller_message_monitor = NULL;
GHashTable *monothek_load_controller_progress_increase = NULL;

GType
monothek_load_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_load_controller = 0;

    static const GTypeInfo monothek_load_controller_info = {
      sizeof (MonothekLoadControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_load_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekLoadController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_load_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_load_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_load_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							   "MonothekLoadController", &monothek_load_controller_info,
							   0);

    g_type_add_interface_static(monothek_type_load_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_load_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_load_controller_class_init(MonothekLoadControllerClass *load_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_load_controller_parent_class = g_type_class_peek_parent(load_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) load_controller;

  gobject->finalize = monothek_load_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) load_controller;

  /* MonothekLoadController */
  load_controller->progress = monothek_load_controller_real_progress;
  load_controller->completed = monothek_load_controller_real_completed;

  /* signals */
  /**
   * MonothekLoadController::progress:
   * @load_controller: the #MonothekLoadController
   * @value: a gdouble ranging from 0.0 to 1.0
   *
   * The ::progress signal notifies about progress.
   *
   * Since: 1.0.0
   */
  load_controller_signals[PROGRESS] =
    g_signal_new("progress",
		 G_TYPE_FROM_CLASS(load_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekLoadControllerClass, progress),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__DOUBLE,
		 G_TYPE_NONE, 1,
		 G_TYPE_DOUBLE);

  /**
   * MonothekLoadController::completed:
   * @load_controller: the #MonothekLoadController
   *
   * The ::completed signal notifies about terminating playback.
   *
   * Since: 1.0.0
   */
  load_controller_signals[COMPLETED] =
    g_signal_new("completed",
		 G_TYPE_FROM_CLASS(load_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekLoadControllerClass, completed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_load_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_load_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_load_controller_connect;
  connectable->disconnect = monothek_load_controller_disconnect;
}

void
monothek_load_controller_init(MonothekLoadController *load_controller)
{
#ifdef __APPLE__
  clock_serv_t cclock;
  mach_timespec_t mts;
#endif

  if(monothek_load_controller_message_monitor == NULL){
    monothek_load_controller_message_monitor = g_hash_table_new_full(g_direct_hash, g_direct_equal,
								     NULL,
								     NULL);
  }

  if(monothek_load_controller_progress_increase == NULL){
    monothek_load_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
								       NULL,
								       NULL);
  }
  
  load_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  load_controller->start_time->tv_sec = 0;
  load_controller->start_time->tv_nsec = 0;
  
  load_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  load_controller->timer->tv_sec = 0;
  load_controller->timer->tv_nsec = 0;

  /* message monitor - completed */
  g_hash_table_insert(monothek_load_controller_message_monitor,
		      load_controller,
		      monothek_load_controller_message_monitor_timeout);

  g_timeout_add(1000,
		(GSourceFunc) monothek_load_controller_message_monitor_timeout,
		(gpointer) load_controller);

  /* progress timeout - add */
  g_hash_table_insert(monothek_load_controller_progress_increase,
		      load_controller, monothek_load_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 2,
		(GSourceFunc) monothek_load_controller_progress_increase_timeout,
		(gpointer) load_controller);

#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    load_controller->start_time->tv_sec = mts.tv_sec;
    load_controller->start_time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, load_controller->start_time);
#endif
}

void
monothek_load_controller_finalize(GObject *gobject)
{
  MonothekLoadController *load_controller;

  load_controller = (MonothekLoadController *) gobject;

  /* remove message monitor */
  g_hash_table_remove(monothek_load_controller_message_monitor,
		      load_controller);

  /* progress timeout - remove */
  g_hash_table_remove(monothek_load_controller_progress_increase,
		      load_controller);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_load_controller_parent_class)->finalize(gobject);
}

void
monothek_load_controller_connect(AgsConnectable *connectable)
{
  MonothekLoadController *load_controller;

  load_controller = MONOTHEK_LOAD_CONTROLLER(connectable);

  if(monothek_controller_test_flags(load_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_load_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_load_controller_disconnect(AgsConnectable *connectable)
{
  MonothekLoadController *load_controller;

  load_controller = MONOTHEK_LOAD_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(load_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_load_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_load_controller_real_progress(MonothekLoadController *load_controller,
				       gdouble value)
{
  MonothekLoadView *load_view;

  g_object_get(load_controller,
	       "view", &load_view,
	       NULL);
  
  gtk_widget_queue_draw(load_view);
}

/**
 * monothek_load_controller_progress:
 * @load_controller: the #MonothekLoadController
 * @value: a gdouble showing seconds
 * 
 * Notify about progress.
 * 
 * Since: 1.0.0
 */
void
monothek_load_controller_progress(MonothekLoadController *load_controller,
				  gdouble value)
{
  g_return_if_fail(MONOTHEK_IS_LOAD_CONTROLLER(load_controller));
  
  g_object_ref((GObject *) load_controller);
  g_signal_emit(G_OBJECT(load_controller),
		load_controller_signals[PROGRESS], 0,
		value);
  g_object_unref((GObject *) load_controller);
}

void
monothek_load_controller_real_completed(MonothekLoadController *load_controller)
{
  MonothekWindow *window;
  MonothekLoadView *view;

  g_message("real completed");
  
  /* change view */
  g_object_get(load_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

#if !defined(MONOTHEK_NO_LOAD_VIEW)
  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
#endif
}

/**
 * monothek_load_controller_completed:
 * @load_controller: the #MonothekLoadController
 * 
 * Termintating playback.
 * 
 * Since: 1.0.0
 */
void
monothek_load_controller_completed(MonothekLoadController *load_controller)
{
  g_return_if_fail(MONOTHEK_IS_LOAD_CONTROLLER(load_controller));
  
  g_object_ref((GObject *) load_controller);
  g_signal_emit(G_OBJECT(load_controller),
		load_controller_signals[COMPLETED], 0);
  g_object_unref((GObject *) load_controller);
}

/**
 * monothek_load_controller_message_monitor_timeout:
 * @load_controller: the #MonothekLoadController
 *
 * Monitor messages.
 *
 * Returns: %TRUE if proceed with monitor, otherwise %FALSE
 *
 * Since: 1.0.0
 */
gboolean
monothek_load_controller_message_monitor_timeout(MonothekLoadController *load_controller)
{
  if(g_hash_table_lookup(monothek_load_controller_message_monitor,
			 load_controller) != NULL){
    AgsMessageDelivery *message_delivery;

    GList *message_start, *message;

    /* retrieve message */
    message_delivery = ags_message_delivery_get_instance();

    message_start = 
      message = ags_message_delivery_find_sender(message_delivery,
						 "libmonothek",
						 (GObject *) monothek_audio_file_manager_get_instance());
    
    while(message != NULL){
      xmlNode *root_node;

      root_node = xmlDocGetRootElement(AGS_MESSAGE_ENVELOPE(message->data)->doc);
      
      if(!xmlStrncmp(root_node->name,
		     "monothek-command",
		     17)){
	g_message("monitor command");
	
	if(!xmlStrncmp(xmlGetProp(root_node,
				  "method"),
		       "MonothekAudioFileManager::load-playlist",
		       40)){
	  g_message("load playlist");
	  
	  monothek_load_controller_completed(load_controller); 
	}
      }
      
      ags_message_delivery_remove_message(message_delivery,
					  "libmonothek",
					  message->data);

      message = message->next;
    }
    
    g_list_free_full(message_start,
		     (GDestroyNotify) g_object_unref);

    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_load_controller_progress_increase_timeout:
 * @gobject: the gobject
 *
 * Queue draw widget
 *
 * Returns: %TRUE if proceed with redraw, otherwise %FALSE
 *
 * Since: 1.0.0
 */
gboolean
monothek_load_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekLoadController *load_controller;

  load_controller = gobject;
  
  if(g_hash_table_lookup(monothek_load_controller_progress_increase,
			 gobject) != NULL){
#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(load_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
        
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
    if(time_now.tv_nsec >= load_controller->start_time->tv_nsec){
      load_controller->timer->tv_sec = time_now.tv_sec - load_controller->start_time->tv_sec;
      load_controller->timer->tv_nsec = time_now.tv_nsec - load_controller->start_time->tv_nsec;
    }else{
      load_controller->timer->tv_sec = time_now.tv_sec - load_controller->start_time->tv_sec - 1;
      load_controller->timer->tv_nsec = AGS_NSEC_PER_SEC - load_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    value = load_controller->timer->tv_sec + (load_controller->timer->tv_nsec / AGS_NSEC_PER_SEC);
    
    monothek_load_controller_progress(load_controller,
				      value);
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_load_controller_new:
 *
 * Creates an #MonothekLoadController
 *
 * Returns: a new #MonothekLoadController
 *
 * Since: 1.0.0
 */
MonothekLoadController*
monothek_load_controller_new()
{
  MonothekLoadController *load_controller;

  load_controller = (MonothekLoadController *) g_object_new(MONOTHEK_TYPE_LOAD_CONTROLLER,
							    NULL);
  
  return(load_controller);
}
