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

#include <monothek/ui/monothek_window.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/object/monothek_marshal.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_application_context.h>

#include <monothek/ui/controller/monothek_controller.h>
#include <monothek/ui/controller/monothek_load_controller.h>
#include <monothek/ui/controller/monothek_start_controller.h>
#include <monothek/ui/controller/monothek_jukebox_payment_controller.h>
#include <monothek/ui/controller/monothek_diskjokey_payment_controller.h>
#include <monothek/ui/controller/monothek_jukebox_mode_controller.h>
#include <monothek/ui/controller/monothek_jukebox_no_test_controller.h>
#include <monothek/ui/controller/monothek_jukebox_playlist_controller.h>
#include <monothek/ui/controller/monothek_jukebox_track_controller.h>
#include <monothek/ui/controller/monothek_jukebox_end_controller.h>
#include <monothek/ui/controller/monothek_jukebox_qrcode_controller.h>
#include <monothek/ui/controller/monothek_diskjokey_sequencer_controller.h>
#include <monothek/ui/controller/monothek_diskjokey_end_controller.h>
#include <monothek/ui/controller/monothek_diskjokey_qrcode_controller.h>

#include <monothek/ui/model/monothek_load_model.h>
#include <monothek/ui/model/monothek_start_model.h>
#include <monothek/ui/model/monothek_closed_model.h>
#include <monothek/ui/model/monothek_outage_model.h>
#include <monothek/ui/model/monothek_jukebox_payment_model.h>
#include <monothek/ui/model/monothek_diskjokey_payment_model.h>
#include <monothek/ui/model/monothek_jukebox_mode_model.h>
#include <monothek/ui/model/monothek_jukebox_no_test_model.h>
#include <monothek/ui/model/monothek_jukebox_playlist_model.h>
#include <monothek/ui/model/monothek_jukebox_track_model.h>
#include <monothek/ui/model/monothek_jukebox_end_model.h>
#include <monothek/ui/model/monothek_jukebox_qrcode_model.h>
#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>
#include <monothek/ui/model/monothek_diskjokey_end_model.h>
#include <monothek/ui/model/monothek_diskjokey_qrcode_model.h>

#include <monothek/ui/view/monothek_load_view.h>
#include <monothek/ui/view/monothek_start_view.h>
#include <monothek/ui/view/monothek_closed_view.h>
#include <monothek/ui/view/monothek_outage_view.h>
#include <monothek/ui/view/monothek_jukebox_payment_view.h>
#include <monothek/ui/view/monothek_diskjokey_payment_view.h>
#include <monothek/ui/view/monothek_jukebox_mode_view.h>
#include <monothek/ui/view/monothek_jukebox_no_test_view.h>
#include <monothek/ui/view/monothek_jukebox_playlist_view.h>
#include <monothek/ui/view/monothek_jukebox_track_view.h>
#include <monothek/ui/view/monothek_jukebox_end_view.h>
#include <monothek/ui/view/monothek_jukebox_qrcode_view.h>
#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>
#include <monothek/ui/view/monothek_diskjokey_end_view.h>
#include <monothek/ui/view/monothek_diskjokey_qrcode_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_window_class_init(MonothekWindowClass *window);
void monothek_window_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_window_init(MonothekWindow *window);
void monothek_window_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec);
void monothek_window_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec);
void monothek_window_finalize(GObject *gobject);

void monothek_window_connect(AgsConnectable *connectable);
void monothek_window_disconnect(AgsConnectable *connectable);

gboolean monothek_window_is_ready(AgsConnectable *connectable);
gboolean monothek_window_is_connected(AgsConnectable *connectable);
void monothek_window_show(GtkWidget *widget);
void monothek_window_show_all(GtkWidget *widget);
gboolean monothek_window_delete_event(GtkWidget *widget, GdkEventAny *event);

void monothek_window_real_change_view(MonothekWindow *window,
				      GType view_type, GType view_type_old);

/**
 * SECTION:monothek_window
 * @short_description: The window object.
 * @title: MonothekWindow
 * @section_id:
 * @include: monothek/ui/monothek_window.h
 *
 * #MonothekWindow is a composite toplevel widget. It contains the
 * menubar, the machine rack and the notation editor.
 */

enum{
  PROP_0,
  PROP_CURRENT_VIEW_TYPE,
};

enum{
  CHANGE_VIEW,
  LAST_SIGNAL,
};

static gpointer monothek_window_parent_class = NULL;
static guint window_signals[LAST_SIGNAL];

GType
monothek_window_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_window = 0;

    static const GTypeInfo monothek_window_info = {
      sizeof (MonothekWindowClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_window_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekWindow),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_window_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_window_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_window = g_type_register_static(GTK_TYPE_WINDOW,
						  "MonothekWindow", &monothek_window_info,
						  0);
    
    g_type_add_interface_static(monothek_type_window,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_window);
  }

  return g_define_type_id__volatile;
}

void
monothek_window_class_init(MonothekWindowClass *window)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_window_parent_class = g_type_class_peek_parent(window);

  /* GObjectClass */
  gobject = (GObjectClass *) window;

  gobject->set_property = monothek_window_set_property;
  gobject->get_property = monothek_window_get_property;

  gobject->finalize = monothek_window_finalize;

  /* properties */
  /**
   * MonothekWindow:current-view-type:
   *
   * The current view's #GType.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_gtype("current-view-type",
				   i18n_pspec("current view type"),
				   i18n_pspec("The type of the current view"),
				   G_TYPE_NONE,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_CURRENT_VIEW_TYPE,
				  param_spec);

  /* GtkWidgetClass */
  widget = (GtkWidgetClass *) window;

  widget->show = monothek_window_show;
  widget->show_all = monothek_window_show_all;
  widget->delete_event = monothek_window_delete_event;

  /* MonothekWindowClass */
  window->change_view = monothek_window_real_change_view;
  
  /* signals */
  /**
   * MonothekWindow::change-view:
   * @window: the #MonothekWindow
   * @view_type: the view's type to set as current
   * @view_type_old: the previous view's type was set as current
   *
   * The ::change-view signal modifies the current view.
   *
   * Since: 1.0.0
   */
  window_signals[CHANGE_VIEW] = 
    g_signal_new("change-view",
		 G_TYPE_FROM_CLASS(window),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekWindowClass, change_view),
		 NULL, NULL,
		 monothek_cclosure_marshal_VOID__ULONG_ULONG,
		 G_TYPE_NONE, 2,
		 G_TYPE_ULONG, G_TYPE_ULONG);
}

void
monothek_window_connectable_interface_init(AgsConnectableInterface *connectable)
{
  connectable->is_ready = monothek_window_is_ready;
  connectable->is_connected = monothek_window_is_connected;
								 
  connectable->connect = monothek_window_connect;
  connectable->disconnect = monothek_window_disconnect;
}

void
monothek_window_init(MonothekWindow *window)
{
  GtkWidget *scrolled_window;
  MonothekView *view;
  
  MonothekController *controller;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GObject *model;

  GError *error;

  window->flags = 0;

  gtk_widget_set_size_request(window,
			      1920, 1080);
  
  gtk_window_set_title((GtkWindow *) window,
		       "monoidea's monothek");

  session_manager = monothek_session_manager_get_instance();

  session = monothek_session_new();
  g_object_set(session,
	       "session-id", MONOTHEK_SESSION_DEFAULT_SESSION,
	       NULL);
  monothek_session_manager_add_session(session_manager,
				       session);

#if 0
  /* scrolled window */
  scrolled_window = (GtkWidget *) gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(window,
		    scrolled_window);
#endif

  window->model = NULL;
  window->controller = NULL;  
  
  /* view - vbox */
  window->current_view_type = G_TYPE_NONE;
  
  window->view = (GtkVBox *) gtk_vbox_new(FALSE,
					  0);
#if 0
  gtk_scrolled_window_add_with_viewport((GtkContainer *) scrolled_window, (GtkWidget*) window->view);
#else
  gtk_container_add(window,
		    window->view);
#endif
  
  /* load view */
  model = monothek_load_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_load_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_load_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* start view */
  model = monothek_start_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);
  
  view = monothek_start_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_start_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);
  
  /* closed view */
  model = monothek_closed_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_closed_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  /* outage view */
  model = monothek_outage_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_outage_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  /* jukebox payment view */
  model = monothek_jukebox_payment_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_payment_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_payment_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* diskjokey payment view */
  model = monothek_diskjokey_payment_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_diskjokey_payment_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_diskjokey_payment_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* jukebox mode view */
  model = monothek_jukebox_mode_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_mode_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_mode_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* jukebox no test view */
  view = monothek_jukebox_no_test_view_new();
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  /* jukebox playlist view */
  model = monothek_jukebox_playlist_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_playlist_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_playlist_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* jukebox track view */
  model = monothek_jukebox_track_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_track_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_track_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* jukebox end view */
  model = monothek_jukebox_end_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_end_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_end_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* jukebox qrcode view */
  model = monothek_jukebox_qrcode_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_jukebox_qrcode_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_jukebox_qrcode_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* diskjokey sequencer view */
  model = monothek_diskjokey_sequencer_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_diskjokey_sequencer_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_diskjokey_sequencer_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* diskjokey end view */
  model = monothek_diskjokey_end_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_diskjokey_end_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_diskjokey_end_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);

  /* diskjokey qrcode view */
  model = monothek_diskjokey_qrcode_model_new();
  g_object_ref(model);
  window->model = g_list_prepend(window->model,
				 model);

  view = monothek_diskjokey_qrcode_view_new();
  g_object_set(view,
	       "model", model,
	       NULL);
  gtk_box_pack_start(window->view,
		     view,
		     FALSE, FALSE,
		     0);

  controller = monothek_diskjokey_qrcode_controller_new();
  g_object_ref(controller);
  window->controller = g_list_prepend(window->controller,
				      controller);
  
  g_object_set(controller,
	       "view", view,
	       "model", model,
	       NULL);
}

void
monothek_window_set_property(GObject *gobject,
			     guint prop_id,
			     const GValue *value,
			     GParamSpec *param_spec)
{
  MonothekWindow *window;

  window = MONOTHEK_WINDOW(gobject);

  switch(prop_id){
  case PROP_CURRENT_VIEW_TYPE:
    {
      window->current_view_type = g_value_get_gtype(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_window_get_property(GObject *gobject,
			     guint prop_id,
			     GValue *value,
			     GParamSpec *param_spec)
{
  MonothekWindow *window;

  window = MONOTHEK_WINDOW(gobject);

  switch(prop_id){
  case PROP_CURRENT_VIEW_TYPE:
    {
      g_value_set_gtype(value,
			window->current_view_type);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_window_finalize(GObject *gobject)
{
  MonothekWindow *window;

  window = (MonothekWindow *) gobject;

  g_list_free_full(window->model,
		   g_object_unref);
  
  g_list_free_full(window->controller,
		   g_object_unref);

  /* call parent */
  G_OBJECT_CLASS(monothek_window_parent_class)->finalize(gobject);
}

gboolean
monothek_window_is_ready(AgsConnectable *connectable)
{
  MonothekWindow *window;

  gboolean retval;
  
  window = MONOTHEK_WINDOW(connectable);

  retval = ((MONOTHEK_WINDOW_ADDED_TO_REGISTRY & (window->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

gboolean
monothek_window_is_connected(AgsConnectable *connectable)
{
  MonothekWindow *window;

  gboolean retval;
  
  window = MONOTHEK_WINDOW(connectable);

  retval = ((MONOTHEK_WINDOW_CONNECTED & (window->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

void
monothek_window_connect(AgsConnectable *connectable)
{
  MonothekWindow *window;

  GList *list, *list_start;

  window = MONOTHEK_WINDOW(connectable);

  if(!monothek_window_test_flags(window, MONOTHEK_WINDOW_CONNECTED)){
    return;
  }

  monothek_window_set_flags(window, MONOTHEK_WINDOW_CONNECTED);

  /* view */
  list =
    list_start = gtk_container_get_children(window->view);

  while(list != NULL){
    ags_connectable_connect(AGS_CONNECTABLE(list->data));

    list = list->next;
  }
  
  g_list_free(list_start);
}

void
monothek_window_disconnect(AgsConnectable *connectable)
{
  MonothekWindow *window;

  GList *list, *list_start;

  window = MONOTHEK_WINDOW(connectable);

  if(monothek_window_test_flags(window, MONOTHEK_WINDOW_CONNECTED)){
    return;
  }

  monothek_window_unset_flags(window, MONOTHEK_WINDOW_CONNECTED);

  /* view */
  list =
    list_start = gtk_container_get_children(window->view);

  while(list != NULL){
    ags_connectable_disconnect(AGS_CONNECTABLE(list->data));

    list = list->next;
  }
  
  g_list_free(list_start);
}

void
monothek_window_show(GtkWidget *widget)
{
  MonothekWindow *window;

  GList *list_start, *list;

  window = (MonothekWindow *) widget;

  GTK_WIDGET_CLASS(monothek_window_parent_class)->show(widget);

  list =
    list_start = gtk_container_get_children(window->view);

  /* hide all */
  while(list != NULL){
    gtk_widget_hide(list->data);
      
    list = list->next;
  }   
}

void
monothek_window_show_all(GtkWidget *widget)
{
  MonothekWindow *window;

  GList *list_start, *list;

  window = (MonothekWindow *) widget;

  GTK_WIDGET_CLASS(monothek_window_parent_class)->show_all(widget);

  list =
    list_start = gtk_container_get_children(window->view);

  /* hide all */
  while(list != NULL){
    gtk_widget_hide(list->data);
      
    list = list->next;
  }   
}

gboolean
monothek_window_delete_event(GtkWidget *widget, GdkEventAny *event)
{
  return(TRUE);
}

/**
 * monothek_window_test_flags:
 * @window: the #MonothekWindow
 * @flags: the flags
 * 
 * Test @window to have @flags set. 
 * 
 * Returns: %TRUE on success, otherwise %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_window_test_flags(MonothekWindow *window, guint flags)
{
  gboolean retval;

  if(!MONOTHEK_IS_WINDOW(window)){
    return(FALSE);
  }

  retval = (flags & (window->flags)) ? TRUE: FALSE;

  return(retval);
}

/**
 * monothek_window_set_flags:
 * @window: the #MonothekWindow
 * @flags: the flags
 * 
 * Set @flags for  @window.
 * 
 * Since: 1.0.0
 */
void
monothek_window_set_flags(MonothekWindow *window, guint flags)
{
  if(!MONOTHEK_IS_WINDOW(window)){
    return;
  }

  window->flags |= flags;
}

/**
 * monothek_window_unset_flags:
 * @window: the #MonothekWindow
 * @flags: the flags
 * 
 * Unset @flags for  @window.
 * 
 * Since: 1.0.0
 */
void
monothek_window_unset_flags(MonothekWindow *window, guint flags)
{
  if(!MONOTHEK_IS_WINDOW(window)){
    return;
  }

  window->flags &= (~flags);
}

void
monothek_window_real_change_view(MonothekWindow *window,
				 GType view_type, GType view_type_old)
{
  GList *list_start, *list;

  if(window->current_view_type == view_type){
    return;
  }
  
  list =
    list_start = gtk_container_get_children(window->view);

  /* hide old view */
  if(view_type_old != G_TYPE_NONE){
    while(list != NULL){
      if(G_OBJECT_TYPE(list->data) == view_type_old){
	GList *controller;

	controller = monothek_controller_find_view_type(window->controller,
							view_type_old);

	if(controller != NULL){
	  ags_connectable_disconnect(AGS_CONNECTABLE(controller->data));
	}
	
	ags_connectable_disconnect(AGS_CONNECTABLE(list->data));
	gtk_widget_hide(list->data);

	break;
      }

      list = list->next;
    }   
  }

  /* show new view */
  if(view_type != G_TYPE_NONE){
    while(list != NULL){      
      if(G_OBJECT_TYPE(list->data) == view_type){
	GList *controller;

	gtk_box_reorder_child(window->view,
			      list->data,
			      0);
	
	gtk_widget_show(list->data);
	gtk_widget_queue_draw(list->data);

	ags_connectable_connect(AGS_CONNECTABLE(list->data));
	
	controller = monothek_controller_find_view_type(window->controller,
							view_type);

	if(controller != NULL){
	  ags_connectable_connect(AGS_CONNECTABLE(controller->data));
	  monothek_controller_reset(controller->data);
	}
	
	break;
      }

      list = list->next;
    }
  }

  window->current_view_type = view_type;
  
  g_list_free(list_start);
}

/**
 * monothek_window_change_view:
 * @window: the #MonothekWindow
 * @view_type: the view's #GType to set as current view
 * @view_type_old: the view's #GType previously set as current view
 * 
 * Change visible view.
 * 
 * Since: 1.0.0
 */
void
monothek_window_change_view(MonothekWindow *window,
			    GType view_type, GType view_type_old)
{
  g_return_if_fail(MONOTHEK_IS_WINDOW(window));

  /* emit */
  g_object_ref((GObject *) window);
  g_signal_emit(G_OBJECT(window),
		window_signals[CHANGE_VIEW], 0,
		view_type, window->current_view_type);
  g_object_unref((GObject *) window);  
}

/**
 * monothek_window_new:
 * @application_context: the application object.
 *
 * Creates an #MonothekWindow
 *
 * Returns: a new #MonothekWindow
 *
 * Since: 1.0.0
 */
MonothekWindow*
monothek_window_new(GObject *application_context)
{
  MonothekWindow *window;

  window = (MonothekWindow *) g_object_new(MONOTHEK_TYPE_WINDOW,
					   "application-context", application_context,
					   NULL);
  
  return(window);
}
