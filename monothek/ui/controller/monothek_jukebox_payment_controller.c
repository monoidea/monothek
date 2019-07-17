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

#include <monothek/ui/controller/monothek_jukebox_payment_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_payment_model.h>

#include <monothek/ui/view/monothek_transaction_failed_view.h>
#include <monothek/ui/view/monothek_jukebox_payment_view.h>
#include <monothek/ui/view/monothek_jukebox_mode_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_payment_controller_class_init(MonothekJukeboxPaymentControllerClass *jukebox_payment_controller);
void monothek_jukebox_payment_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_payment_controller_init(MonothekJukeboxPaymentController *jukebox_payment_controller);
void monothek_jukebox_payment_controller_finalize(GObject *gobject);

void monothek_jukebox_payment_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_payment_controller_disconnect(AgsConnectable *connectable);

#ifdef MONOTHEK_DEVEL_MODE
gboolean monothek_jukebox_payment_controller_key_press_event_callback(GtkWidget *widget,
								      GdkEvent *event,
								      MonothekJukeboxPaymentController *jukebox_payment_controller);
#endif

void monothek_jukebox_payment_controller_real_do_transaction(MonothekJukeboxPaymentController *jukebox_payment_controller);

void monothek_jukebox_payment_controller_real_transaction_completed(MonothekJukeboxPaymentController *jukebox_payment_controller);
void monothek_jukebox_payment_controller_real_transaction_failed(MonothekJukeboxPaymentController *jukebox_payment_controller);

/**
 * SECTION:monothek_jukebox_payment_controller
 * @short_description: The jukebox_payment controller object.
 * @title: MonothekJukeboxPaymentController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_payment_controller.h
 *
 * #MonothekJukeboxPaymentController is the MVC's jukebox_payment controller.
 */

enum{
  DO_TRANSACTION,
  TRANSACTION_COMPLETED,
  TRANSACTION_FAILED,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_payment_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_payment_controller_parent_connectable_interface;

static guint jukebox_payment_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_payment_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_payment_controller = 0;

    static const GTypeInfo monothek_jukebox_payment_controller_info = {
      sizeof (MonothekJukeboxPaymentControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_payment_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPaymentController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_payment_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_payment_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_payment_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								      "MonothekJukeboxPaymentController", &monothek_jukebox_payment_controller_info,
								      0);

    g_type_add_interface_static(monothek_type_jukebox_payment_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_payment_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_payment_controller_class_init(MonothekJukeboxPaymentControllerClass *jukebox_payment_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_jukebox_payment_controller_parent_class = g_type_class_peek_parent(jukebox_payment_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_payment_controller;

  gobject->finalize = monothek_jukebox_payment_controller_finalize;

  /* MonothekJukeboxPaymentController */
  jukebox_payment_controller->do_transaction = monothek_jukebox_payment_controller_real_do_transaction;

  jukebox_payment_controller->transaction_completed = monothek_jukebox_payment_controller_real_transaction_completed;
  jukebox_payment_controller->transaction_failed = monothek_jukebox_payment_controller_real_transaction_failed;

  /* signals */
  /**
   * MonothekJukeboxPaymentController::do-transaction:
   * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
   *
   * The ::do-transaction signal notifies about running transaction.
   *
   * Since: 1.0.0
   */
  jukebox_payment_controller_signals[DO_TRANSACTION] =
    g_signal_new("do-transaction",
		 G_TYPE_FROM_CLASS(jukebox_payment_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxPaymentControllerClass, do_transaction),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxPaymentController::transaction-completed:
   * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
   *
   * The ::transaction-completed signal notifies about completed transaction.
   *
   * Since: 1.0.0
   */
  jukebox_payment_controller_signals[TRANSACTION_COMPLETED] =
    g_signal_new("transaction-completed",
		 G_TYPE_FROM_CLASS(jukebox_payment_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxPaymentControllerClass, transaction_completed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxPaymentController::transaction-failed:
   * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
   *
   * The ::transaction-failed signal notifies about failed transaction.
   *
   * Since: 1.0.0
   */
  jukebox_payment_controller_signals[TRANSACTION_FAILED] =
    g_signal_new("transaction-failed",
		 G_TYPE_FROM_CLASS(jukebox_payment_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxPaymentControllerClass, transaction_failed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_payment_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_payment_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_payment_controller_connect;
  connectable->disconnect = monothek_jukebox_payment_controller_disconnect;
}

void
monothek_jukebox_payment_controller_init(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  //empty
}

void
monothek_jukebox_payment_controller_finalize(GObject *gobject)
{
  MonothekJukeboxPaymentController *jukebox_payment_controller;

  jukebox_payment_controller = (MonothekJukeboxPaymentController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_payment_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_payment_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxPaymentController *jukebox_payment_controller;

  jukebox_payment_controller = MONOTHEK_JUKEBOX_PAYMENT_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_payment_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_payment_controller_parent_connectable_interface->connect(connectable);

  //empty

#ifdef MONOTHEK_DEVEL_MODE
  {
    MonothekView *view;
    
    g_object_get(jukebox_payment_controller,
		 "view", &view,
		 NULL);

    gtk_widget_grab_focus(view);
    g_signal_connect(G_OBJECT(view), "key-press-event",
		     G_CALLBACK(monothek_jukebox_payment_controller_key_press_event_callback), jukebox_payment_controller);
  }
#endif
}

void
monothek_jukebox_payment_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxPaymentController *jukebox_payment_controller;

  jukebox_payment_controller = MONOTHEK_JUKEBOX_PAYMENT_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_payment_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_payment_controller_parent_connectable_interface->disconnect(connectable);

  //empty

#ifdef MONOTHEK_DEVEL_MODE
  {
    MonothekView *view;
    
    g_object_get(jukebox_payment_controller,
		 "view", &view,
		 NULL);
    
    g_object_disconnect(G_OBJECT(view),
			"any_signal::key-press-event",
			G_CALLBACK(monothek_jukebox_payment_controller_key_press_event_callback),
			jukebox_payment_controller,
			NULL);
  }
#endif
}

#ifdef MONOTHEK_DEVEL_MODE
gboolean
monothek_jukebox_payment_controller_key_press_event_callback(GtkWidget *widget,
							     GdkEvent *event,
							     MonothekJukeboxPaymentController *jukebox_payment_controller)
{  
  monothek_jukebox_payment_controller_transaction_completed(jukebox_payment_controller);
  
  return(FALSE);
}
#endif

void
monothek_jukebox_payment_controller_real_do_transaction(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  //TODO:JK: implement me
}

/**
 * monothek_jukebox_payment_controller_do_transaction:
 * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
 * 
 * Payment transaction completed.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_payment_controller_do_transaction(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_PAYMENT_CONTROLLER(jukebox_payment_controller));
  
  g_object_ref((GObject *) jukebox_payment_controller);
  g_signal_emit(G_OBJECT(jukebox_payment_controller),
		jukebox_payment_controller_signals[DO_TRANSACTION], 0);
  g_object_unref((GObject *) jukebox_payment_controller);
}

void
monothek_jukebox_payment_controller_real_transaction_completed(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  MonothekWindow *window;
  MonothekJukeboxPaymentView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *preserve_jukebox;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  preserve_jukebox = g_hash_table_lookup(session->value,
					 "preserve-jukebox");

  g_value_set_boolean(preserve_jukebox, TRUE);

  /* change view */
  g_object_get(jukebox_payment_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_MODE_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_payment_controller_transaction_completed:
 * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
 * 
 * Payment transaction completed.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_payment_controller_transaction_completed(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_PAYMENT_CONTROLLER(jukebox_payment_controller));
  
  g_object_ref((GObject *) jukebox_payment_controller);
  g_signal_emit(G_OBJECT(jukebox_payment_controller),
		jukebox_payment_controller_signals[TRANSACTION_COMPLETED], 0);
  g_object_unref((GObject *) jukebox_payment_controller);
}

void
monothek_jukebox_payment_controller_real_transaction_failed(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  MonothekWindow *window;
  MonothekJukeboxPaymentView *view;

  /* change view */
  g_object_get(jukebox_payment_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_TRANSACTION_FAILED_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_payment_controller_transaction_failed:
 * @jukebox_payment_controller: the #MonothekJukeboxPaymentController
 * 
 * Payment transaction failed.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_payment_controller_transaction_failed(MonothekJukeboxPaymentController *jukebox_payment_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_PAYMENT_CONTROLLER(jukebox_payment_controller));
  
  g_object_ref((GObject *) jukebox_payment_controller);
  g_signal_emit(G_OBJECT(jukebox_payment_controller),
		jukebox_payment_controller_signals[TRANSACTION_FAILED], 0);
  g_object_unref((GObject *) jukebox_payment_controller);
}

/**
 * monothek_jukebox_payment_controller_new:
 *
 * Creates an #MonothekJukeboxPaymentController
 *
 * Returns: a new #MonothekJukeboxPaymentController
 *
 * Since: 1.0.0
 */
MonothekJukeboxPaymentController*
monothek_jukebox_payment_controller_new()
{
  MonothekJukeboxPaymentController *jukebox_payment_controller;

  jukebox_payment_controller = (MonothekJukeboxPaymentController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PAYMENT_CONTROLLER,
										 NULL);
  
  return(jukebox_payment_controller);
}
