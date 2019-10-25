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

#include <monothek/ui/controller/monothek_start_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_start_model.h>

#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_screensaver_view.h>
#include <monothek/ui/view/monothek_jukebox_payment_view.h>
#include <monothek/ui/view/monothek_diskjokey_payment_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_start_controller_class_init(MonothekStartControllerClass *start_controller);
void monothek_start_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_start_controller_init(MonothekStartController *start_controller);
void monothek_start_controller_finalize(GObject *gobject);

void monothek_start_controller_connect(AgsConnectable *connectable);
void monothek_start_controller_disconnect(AgsConnectable *connectable);

void monothek_start_controller_reset(MonothekController *controller);

void monothek_start_controller_jukebox_launch_enter_callback(MonothekActionBox *action_box,
							     MonothekStartController *start_controller);
void monothek_start_controller_jukebox_launch_leave_callback(MonothekActionBox *action_box,
							     MonothekStartController *start_controller);
void monothek_start_controller_jukebox_launch_clicked_callback(MonothekActionBox *action_box,
							       MonothekStartController *start_controller);

void monothek_start_controller_diskjokey_launch_enter_callback(MonothekActionBox *action_box,
							       MonothekStartController *start_controller);
void monothek_start_controller_diskjokey_launch_leave_callback(MonothekActionBox *action_box,
							       MonothekStartController *start_controller);
void monothek_start_controller_diskjokey_launch_clicked_callback(MonothekActionBox *action_box,
								 MonothekStartController *start_controller);

void monothek_start_controller_real_timeout(MonothekStartController *start_controller);
void monothek_start_controller_real_activate_screensaver(MonothekStartController *start_controller);
void monothek_start_controller_real_launch_jukebox(MonothekStartController *start_controller);
void monothek_start_controller_real_launch_diskjokey(MonothekStartController *start_controller);

/**
 * SECTION:monothek_start_controller
 * @short_description: The start controller object.
 * @title: MonothekStartController
 * @section_id:
 * @include: monothek/ui/controller/monothek_start_controller.h
 *
 * #MonothekStartController is the MVC's start controller.
 */

enum{
  TIMEOUT,
  ACTIVATE_SCREENSAVER,
  LAUNCH_JUKEBOX,
  LAUNCH_DISKJOKEY,
  LAST_SIGNAL,
};

static gpointer monothek_start_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_start_controller_parent_connectable_interface;

static guint start_controller_signals[LAST_SIGNAL];

GHashTable *monothek_start_controller_progress_increase = NULL;

GType
monothek_start_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_start_controller = 0;

    static const GTypeInfo monothek_start_controller_info = {
      sizeof (MonothekStartControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_start_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekStartController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_start_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_start_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_start_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							    "MonothekStartController", &monothek_start_controller_info,
							    0);

    g_type_add_interface_static(monothek_type_start_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_start_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_start_controller_class_init(MonothekStartControllerClass *start_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_start_controller_parent_class = g_type_class_peek_parent(start_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) start_controller;

  gobject->finalize = monothek_start_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) start_controller;
  
  controller->reset = monothek_start_controller_reset;

  /* MonothekStartController */
  start_controller->timeout = monothek_start_controller_real_timeout;
  start_controller->activate_screensaver = monothek_start_controller_real_activate_screensaver;
  start_controller->launch_jukebox = monothek_start_controller_real_launch_jukebox;
  start_controller->launch_diskjokey = monothek_start_controller_real_launch_diskjokey;

  /* signals */
  /**
   * MonothekStartController::timeout:
   * @start_controller: the #MonothekStartController
   *
   * The ::timeout signal notifies about timeout expired.
   *
   * Since: 1.0.0
   */
  start_controller_signals[TIMEOUT] =
    g_signal_new("timeout",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, timeout),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekStartController::activate-screensaver:
   * @start_controller: the #MonothekStartController
   *
   * The ::activate-screensaver signal notifies about screensaver activation.
   *
   * Since: 1.0.0
   */
  start_controller_signals[ACTIVATE_SCREENSAVER] =
    g_signal_new("activate-screensaver",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, activate_screensaver),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
  
  /**
   * MonothekStartController::launch-jukebox:
   * @start_controller: the #MonothekStartController
   *
   * The ::launch-jukebox signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  start_controller_signals[LAUNCH_JUKEBOX] =
    g_signal_new("launch-jukebox",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, launch_jukebox),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekStartController::launch-diskjokey:
   * @start_controller: the #MonothekStartController
   *
   * The ::launch-diskjokey signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  start_controller_signals[LAUNCH_DISKJOKEY] =
    g_signal_new("launch-diskjokey",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, launch_diskjokey),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_start_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_start_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_start_controller_connect;
  connectable->disconnect = monothek_start_controller_disconnect;
}

void
monothek_start_controller_init(MonothekStartController *start_controller)
{
  MonothekActionBox *action_box;

  start_controller->jukebox_launch = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "jukebox-launch",
						    "x0", 320,
						    "y0", 600,
						    "width", 360,
						    "height", 120,
						    NULL);
  monothek_controller_add_action_box(start_controller,
				     action_box);
  
  start_controller->diskjokey_launch = 
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "diskjokey-launch",
						    "x0", 1240,
						    "y0", 600,
						    "width", 360,
						    "height", 120,
						    NULL);
  monothek_controller_add_action_box(start_controller,
				     action_box);  

  /* progress */
  if(monothek_start_controller_progress_increase == NULL){
    monothek_start_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
									NULL,
									NULL);
  }

  start_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  start_controller->start_time->tv_sec = 0;
  start_controller->start_time->tv_nsec = 0;
  
  start_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  start_controller->timer->tv_sec = 0;
  start_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_start_controller_progress_increase,
		      start_controller, monothek_start_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 2,
		(GSourceFunc) monothek_start_controller_progress_increase_timeout,
		(gpointer) start_controller);
}

void
monothek_start_controller_finalize(GObject *gobject)
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) gobject;

  /* progress timeout - remove */
  g_hash_table_remove(monothek_start_controller_progress_increase,
		      start_controller);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_start_controller_parent_class)->finalize(gobject);
}

void
monothek_start_controller_connect(AgsConnectable *connectable)
{
  MonothekStartController *start_controller;

  start_controller = MONOTHEK_START_CONTROLLER(connectable);

  if(monothek_controller_test_flags(start_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_start_controller_parent_connectable_interface->connect(connectable);
  
  g_signal_connect(start_controller->jukebox_launch, "enter",
		   G_CALLBACK(monothek_start_controller_jukebox_launch_enter_callback), start_controller);
  g_signal_connect(start_controller->jukebox_launch, "leave",
		   G_CALLBACK(monothek_start_controller_jukebox_launch_leave_callback), start_controller);
  g_signal_connect(start_controller->jukebox_launch, "clicked",
		   G_CALLBACK(monothek_start_controller_jukebox_launch_clicked_callback), start_controller);

  g_signal_connect(start_controller->diskjokey_launch, "enter",
		   G_CALLBACK(monothek_start_controller_diskjokey_launch_enter_callback), start_controller);
  g_signal_connect(start_controller->diskjokey_launch, "leave",
		   G_CALLBACK(monothek_start_controller_diskjokey_launch_leave_callback), start_controller);
  g_signal_connect(start_controller->diskjokey_launch, "clicked",
		   G_CALLBACK(monothek_start_controller_diskjokey_launch_clicked_callback), start_controller);
}

void
monothek_start_controller_disconnect(AgsConnectable *connectable)
{
  MonothekStartController *start_controller;

  start_controller = MONOTHEK_START_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(start_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_start_controller_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(start_controller->jukebox_launch,
		      "any_signal::enter",
		      G_CALLBACK(monothek_start_controller_jukebox_launch_enter_callback),
		      start_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_start_controller_jukebox_launch_leave_callback),
		      start_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_start_controller_jukebox_launch_clicked_callback),
		      start_controller,
		      NULL);

  g_object_disconnect(start_controller->diskjokey_launch,
		      "any_signal::enter",
		      G_CALLBACK(monothek_start_controller_diskjokey_launch_enter_callback),
		      start_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_start_controller_diskjokey_launch_leave_callback),
		      start_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_start_controller_diskjokey_launch_clicked_callback),
		      start_controller,
		      NULL);
}

void
monothek_start_controller_reset(MonothekController *controller)
{
  MonothekStartController *start_controller;
  MonothekStartModel *model;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *preserve_diskjokey, *preserve_jukebox;
  
  start_controller = MONOTHEK_START_CONTROLLER(controller);

  /* reset timer */
#ifdef __APPLE__
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
    
  start_controller->start_time->tv_sec = mts.tv_sec;
  start_controller->start_time->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, start_controller->start_time);
#endif  
  
  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  g_object_get(start_controller,
	       "model", &model,
	       NULL);
  
  preserve_diskjokey = g_hash_table_lookup(session->value,
					   "preserve-diskjokey");

  preserve_jukebox = g_hash_table_lookup(session->value,
					 "preserve-jukebox");

  if(!g_value_get_boolean(preserve_diskjokey) &&
     !g_value_get_boolean(preserve_jukebox)){
    g_free(model->purchase_filename);
    
    model->purchase_filename = NULL;
  }
}

void
monothek_start_controller_jukebox_launch_enter_callback(MonothekActionBox *action_box,
							MonothekStartController *start_controller)
{
  MonothekStartView *view;
  
  MonothekStartModel *model;
  
  g_object_get(start_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-start-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_start_controller_jukebox_launch_leave_callback(MonothekActionBox *action_box,
							MonothekStartController *start_controller)
{
  MonothekStartView *view;
  
  MonothekStartModel *model;

  g_object_get(start_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "jukebox-start-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_start_controller_jukebox_launch_clicked_callback(MonothekActionBox *action_box,
							  MonothekStartController *start_controller)
{
  MonothekStartModel *model;

  gchar *purchase_path;
  gchar *purchase_filename;
  gchar *product_name;
  gchar *position_id;
  gchar *purchase_id;
  gchar *str;

  gdouble invoice_amount;
  guint nth_position;
  gint exit_status;

  GError *error;

#if !defined(MONOTHEK_DEVEL_MODE)
  g_object_get(start_controller,
	       "model", &model,
	       NULL);
  
  purchase_path = getenv("MONOTHEK_PURCHASE_PATH");

  if(purchase_path == NULL){
    purchase_path = MONOTHEK_START_MODEL_PURCHASE_PATH;
  }

  nth_position = model->nth_position;
  
  product_name = g_strdup("jukebox");

  position_id = g_strdup_printf("MJUK%06d", nth_position);
  recipe_id = g_uuid_string_random();

  purchase_filename = g_strdup_printf("%s/purchase-%s.xml ", purchase_path, recipe_id);

  invoice_amount = model->jukebox_price;
  
  str = g_strdup_printf("prepare-purchase.pl %s %s %s %s %f", purchase_filename, product_name, position_id, recipe_id, invoice_amount);
  
  error = NULL;
  g_spawn_command_line_sync(str,
			    NULL,
			    NULL,
			    &exit_status,
			    &error);

  if(exit_status){
    monothek_start_controller_launch_jukebox(start_controller);
  }
#else
  monothek_start_controller_launch_jukebox(start_controller);
#endif
}

void
monothek_start_controller_diskjokey_launch_enter_callback(MonothekActionBox *action_box,
							  MonothekStartController *start_controller)
{
  MonothekStartView *view;
  
  MonothekStartModel *model;

  g_object_get(start_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-start-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_start_controller_diskjokey_launch_leave_callback(MonothekActionBox *action_box,
							  MonothekStartController *start_controller)
{
  MonothekStartView *view;
  
  MonothekStartModel *model;

  g_object_get(start_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "diskjokey-start-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_start_controller_diskjokey_launch_clicked_callback(MonothekActionBox *action_box,
							    MonothekStartController *start_controller)
{
  MonothekStartModel *model;

  gchar *purchase_path;
  gchar *purchase_filename;
  gchar *product_name;
  gchar *position_id;
  gchar *purchase_id;
  gchar *str;

  gdouble invoice_amount;
  guint nth_position;
  gint exit_status;

  GError *error;

#if !defined(MONOTHEK_DEVEL_MODE)
  g_object_get(start_controller,
	       "model", &model,
	       NULL);
  
  purchase_path = getenv("MONOTHEK_PURCHASE_PATH");

  if(purchase_path == NULL){
    purchase_path = MONOTHEK_START_MODEL_PURCHASE_PATH;
  }

  nth_position = model->nth_position;
  
  product_name = g_strdup("diskjokey");

  position_id = g_strdup_printf("MDIS%06d", nth_position);
  recipe_id = g_uuid_string_random();
  
  purchase_filename = g_strdup_printf("%s/purchase-%s.xml ", purchase_path, recipe_id);

  invoice_amount = model->diskjokey_price;
  
  str = g_strdup_printf("prepare-purchase.pl %s %s %s %s %f", purchase_filename, product_name, position_id, recipe_id, invoice_amount);
  
  error = NULL;
  g_spawn_command_line_sync(str,
			    NULL,
			    NULL,
			    &exit_status,
			    &error);

  if(exit_status){
    monothek_start_controller_launch_diskjokey(start_controller);
  }
#else
  monothek_start_controller_launch_diskjokey(start_controller);
#endif
}

void
monothek_start_controller_real_timeout(MonothekStartController *start_controller)
{
  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  start_controller->start_time->tv_sec = 0;
  start_controller->start_time->tv_nsec = 0;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set preserve diskjokey - FALSE */
  value = g_hash_table_lookup(session->value,
			      "preserve-diskjokey");

  g_value_set_boolean(value,
		      FALSE);

  /* activate screensaver */
  monothek_start_controller_activate_screensaver(start_controller);
}

/**
 * monothek_start_controller_timeout:
 * @start_controller: the #MonothekStartController
 * 
 * Timeout expired.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_timeout(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[TIMEOUT], 0);
  g_object_unref((GObject *) start_controller);
}

void
monothek_start_controller_real_activate_screensaver(MonothekStartController *start_controller)
{
  MonothekWindow *window;
  MonothekStartView *view;
  
  g_object_get(start_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_SCREENSAVER_VIEW, G_TYPE_NONE);
}

/**
 * monothek_start_controller_activate_screensaver:
 * @start_controller: the #MonothekStartController
 * 
 * Activate screensaver.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_activate_screensaver(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[ACTIVATE_SCREENSAVER], 0);
  g_object_unref((GObject *) start_controller);
}

void
monothek_start_controller_real_launch_jukebox(MonothekStartController *start_controller)
{
  MonothekWindow *window;
  MonothekStartView *view;
  
  start_controller->start_time->tv_sec = 0;
  start_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(start_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW, G_TYPE_NONE);
}

/**
 * monothek_start_controller_launch_jukebox:
 * @start_controller: the #MonothekStartController
 * 
 * Launch jukebox.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_launch_jukebox(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[LAUNCH_JUKEBOX], 0);
  g_object_unref((GObject *) start_controller);
}

void
monothek_start_controller_real_launch_diskjokey(MonothekStartController *start_controller)
{
  MonothekWindow *window;
  MonothekStartView *view;

  start_controller->start_time->tv_sec = 0;
  start_controller->start_time->tv_nsec = 0;

  /* change view */
  g_object_get(start_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_DISKJOKEY_PAYMENT_VIEW, G_TYPE_NONE);
}

/**
 * monothek_start_controller_launch_diskjokey:
 * @start_controller: the #MonothekStartController
 * 
 * Launch diskjokey.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_launch_diskjokey(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[LAUNCH_DISKJOKEY], 0);
  g_object_unref((GObject *) start_controller);
}

gboolean
monothek_start_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekStartController *start_controller;

  start_controller = gobject;
  
  if(g_hash_table_lookup(monothek_start_controller_progress_increase,
			 gobject) != NULL){
    MonothekStartModel *start_model;

    MonothekStartView *start_view;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(start_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(start_controller,
		 "model", &start_model,
		 "view", &start_view,
		 NULL);
    
    g_object_get(start_model,
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
    if(time_now.tv_nsec >= start_controller->start_time->tv_nsec){
      start_controller->timer->tv_sec = time_now.tv_sec - start_controller->start_time->tv_sec;
      start_controller->timer->tv_nsec = time_now.tv_nsec - start_controller->start_time->tv_nsec;
    }else{
      start_controller->timer->tv_sec = time_now.tv_sec - start_controller->start_time->tv_sec - 1;
      start_controller->timer->tv_nsec = NSEC_PER_SEC - start_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(time_now.tv_sec - start_controller->start_time->tv_sec < duration->tv_sec){
      gtk_widget_queue_draw(start_view);
    }else{
      monothek_start_controller_timeout(start_controller);
    }
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_start_controller_new:
 *
 * Creates an #MonothekStartController
 *
 * Returns: a new #MonothekStartController
 *
 * Since: 1.0.0
 */
MonothekStartController*
monothek_start_controller_new()
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) g_object_new(MONOTHEK_TYPE_START_CONTROLLER,
							      NULL);
  
  return(start_controller);
}
