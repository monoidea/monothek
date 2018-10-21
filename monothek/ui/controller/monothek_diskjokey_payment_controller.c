/* Monothek - monoidea's monothek
 * Copyright (C) 2018 Joël Krähemann
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

#include <monothek/ui/controller/monothek_diskjokey_payment_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_payment_model.h>

#include <monothek/ui/view/monothek_diskjokey_payment_view.h>
#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_payment_controller_class_init(MonothekDiskjokeyPaymentControllerClass *diskjokey_payment_controller);
void monothek_diskjokey_payment_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_payment_controller_init(MonothekDiskjokeyPaymentController *diskjokey_payment_controller);
void monothek_diskjokey_payment_controller_finalize(GObject *gobject);

void monothek_diskjokey_payment_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_payment_controller_disconnect(AgsConnectable *connectable);

#ifdef MONOTHEK_DEVEL_MODE
gboolean monothek_diskjokey_payment_controller_key_press_event_callback(GtkWidget *widget,
									GdkEvent *event,
									MonothekDiskjokeyPaymentController *diskjokey_payment_controller);
#endif

void monothek_diskjokey_payment_controller_real_transaction_completed(MonothekDiskjokeyPaymentController *diskjokey_payment_controller);

/**
 * SECTION:monothek_diskjokey_payment_controller
 * @short_description: The diskjokey_payment controller object.
 * @title: MonothekDiskjokeyPaymentController
 * @section_id:
 * @include: monothek/ui/controller/monothek_diskjokey_payment_controller.h
 *
 * #MonothekDiskjokeyPaymentController is the MVC's diskjokey_payment controller.
 */

enum{
  TRANSACTION_COMPLETED,
  LAST_SIGNAL,
};

static gpointer monothek_diskjokey_payment_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_payment_controller_parent_connectable_interface;

static guint diskjokey_payment_controller_signals[LAST_SIGNAL];

GType
monothek_diskjokey_payment_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_payment_controller = 0;

    static const GTypeInfo monothek_diskjokey_payment_controller_info = {
      sizeof (MonothekDiskjokeyPaymentControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_payment_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeyPaymentController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_payment_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_diskjokey_payment_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_diskjokey_payment_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
									"MonothekDiskjokeyPaymentController", &monothek_diskjokey_payment_controller_info,
									0);

    g_type_add_interface_static(monothek_type_diskjokey_payment_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_payment_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_payment_controller_class_init(MonothekDiskjokeyPaymentControllerClass *diskjokey_payment_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_diskjokey_payment_controller_parent_class = g_type_class_peek_parent(diskjokey_payment_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_payment_controller;

  gobject->finalize = monothek_diskjokey_payment_controller_finalize;

  /* MonothekDiskjokeyPaymentController */
  diskjokey_payment_controller->transaction_completed = monothek_diskjokey_payment_controller_real_transaction_completed;

  /* signals */
  /**
   * MonothekDiskjokeyPaymentController::transaction-completed:
   * @diskjokey_payment_controller: the #MonothekDiskjokeyPaymentController
   *
   * The ::transaction-completed signal notifies about completed transaction.
   *
   * Since: 1.0.0
   */
  diskjokey_payment_controller_signals[TRANSACTION_COMPLETED] =
    g_signal_new("transaction-completed",
		 G_TYPE_FROM_CLASS(diskjokey_payment_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeyPaymentControllerClass, transaction_completed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_diskjokey_payment_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_diskjokey_payment_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_diskjokey_payment_controller_connect;
  connectable->disconnect = monothek_diskjokey_payment_controller_disconnect;
}

void
monothek_diskjokey_payment_controller_init(MonothekDiskjokeyPaymentController *diskjokey_payment_controller)
{
  //empty
}

void
monothek_diskjokey_payment_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeyPaymentController *diskjokey_payment_controller;

  diskjokey_payment_controller = (MonothekDiskjokeyPaymentController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_payment_controller_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_payment_controller_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeyPaymentController *diskjokey_payment_controller;

  diskjokey_payment_controller = MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER(connectable);

  if(monothek_controller_test_flags(diskjokey_payment_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_payment_controller_parent_connectable_interface->connect(connectable);

  //empty

#ifdef MONOTHEK_DEVEL_MODE
  {
    MonothekView *view;

    g_message("connect");
    
    g_object_get(diskjokey_payment_controller,
		 "view", &view,
		 NULL);

    gtk_widget_grab_focus(view);
    g_signal_connect(G_OBJECT(view), "key-press-event",
		     G_CALLBACK(monothek_diskjokey_payment_controller_key_press_event_callback), diskjokey_payment_controller);
  }
#endif
}

void
monothek_diskjokey_payment_controller_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeyPaymentController *diskjokey_payment_controller;

  diskjokey_payment_controller = MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(diskjokey_payment_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_payment_controller_parent_connectable_interface->disconnect(connectable);

#ifdef MONOTHEK_DEVEL_MODE
  {
    MonothekView *view;
    
    g_object_get(diskjokey_payment_controller,
		 "view", &view,
		 NULL);
    
    g_object_disconnect(G_OBJECT(view),
			"any_signal::key-press-event",
			G_CALLBACK(monothek_diskjokey_payment_controller_key_press_event_callback),
			diskjokey_payment_controller,
			NULL);
  }
#endif
}

#ifdef MONOTHEK_DEVEL_MODE
gboolean
monothek_diskjokey_payment_controller_key_press_event_callback(GtkWidget *widget,
							       GdkEvent *event,
							       MonothekDiskjokeyPaymentController *diskjokey_payment_controller)
{
  g_message("key");
  
  monothek_diskjokey_payment_controller_transaction_completed(diskjokey_payment_controller);
  
  return(FALSE);
}
#endif

void
monothek_diskjokey_payment_controller_real_transaction_completed(MonothekDiskjokeyPaymentController *diskjokey_payment_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeyPaymentView *view;

  g_object_get(diskjokey_payment_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_payment_controller_transaction_completed:
 * @diskjokey_payment_controller: the #MonothekDiskjokeyPaymentController
 * 
 * Payment transaction completed.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_payment_controller_transaction_completed(MonothekDiskjokeyPaymentController *diskjokey_payment_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_PAYMENT_CONTROLLER(diskjokey_payment_controller));
  
  g_object_ref((GObject *) diskjokey_payment_controller);
  g_signal_emit(G_OBJECT(diskjokey_payment_controller),
		diskjokey_payment_controller_signals[TRANSACTION_COMPLETED], 0);
  g_object_unref((GObject *) diskjokey_payment_controller);
}

/**
 * monothek_diskjokey_payment_controller_new:
 *
 * Creates an #MonothekDiskjokeyPaymentController
 *
 * Returns: a new #MonothekDiskjokeyPaymentController
 *
 * Since: 1.0.0
 */
MonothekDiskjokeyPaymentController*
monothek_diskjokey_payment_controller_new()
{
  MonothekDiskjokeyPaymentController *diskjokey_payment_controller;

  diskjokey_payment_controller = (MonothekDiskjokeyPaymentController *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER,
										     NULL);
  
  return(diskjokey_payment_controller);
}
