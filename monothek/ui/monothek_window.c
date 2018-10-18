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

#include <monothek/ui/monothek_application_context.h>

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

void monothek_window_show(GtkWidget *widget);
gboolean monothek_window_delete_event(GtkWidget *widget, GdkEventAny *event);

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
  PROP_SOUNDCARD,
  PROP_APPLICATION_CONTEXT,
};

static gpointer monothek_window_parent_class = NULL;

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
   * MonothekWindow:soundcard:
   *
   * The assigned main MonothekSoundcard.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("soundcard",
				   i18n_pspec("assigned soundcard"),
				   i18n_pspec("The soundcard it is assigned with"),
				   G_TYPE_OBJECT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SOUNDCARD,
				  param_spec);

  /**
   * MonothekWindow:application-context:
   *
   * The assigned application context.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("application-context",
				   i18n_pspec("assigned application context"),
				   i18n_pspec("The MonothekApplicationContext it is assigned with"),
				   G_TYPE_OBJECT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_APPLICATION_CONTEXT,
				  param_spec);


  /* GtkWidgetClass */
  widget = (GtkWidgetClass *) window;

  widget->show = monothek_window_show;
  widget->delete_event = monothek_window_delete_event;
}

void
monothek_window_connectable_interface_init(AgsConnectableInterface *connectable)
{
  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_window_connect;
  connectable->disconnect = monothek_window_disconnect;
}

void
monothek_window_init(MonothekWindow *window)
{
  GtkWidget *scrolled_window;
  GtkWidget *viewport;
  GtkVBox *vbox;

  GError *error;

  window->flags = 0;

  window->application_context = NULL;
  window->application_mutex = NULL;
  
  window->soundcard = NULL;

  gtk_window_set_title((GtkWindow *) window,
		       "monoidea's monothek");

  /* scrolled window */
  scrolled_window = (GtkWidget *) gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(window,
		    scrolled_window);


  /* vbox */
  window->view = (GtkVBox *) gtk_vbox_new(FALSE, 0);
  gtk_container_add((GtkContainer *) scrolled_window, (GtkWidget*) window->view);
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
  case PROP_SOUNDCARD:
    {
      GObject *soundcard;

      soundcard = g_value_get_object(value);

      if(window->soundcard == soundcard){
	return;
      }
      
      if(soundcard != NULL){
	g_object_ref(soundcard);
      }
      
      window->soundcard = soundcard;
    }
    break;
  case PROP_APPLICATION_CONTEXT:
    {
      MonothekApplicationContext *application_context;

      application_context = (MonothekApplicationContext *) g_value_get_object(value);

      if((MonothekApplicationContext *) window->application_context == application_context){
	return;
      }

      if(window->application_context != NULL){
	window->application_mutex = NULL;
	g_object_unref(window->application_context);
      }

      if(application_context != NULL){
	window->application_mutex = AGS_APPLICATION_CONTEXT(application_context)->obj_mutex;
	
	g_object_ref(application_context);
      }

      window->application_context = (GObject *) application_context;
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
  case PROP_SOUNDCARD:
    g_value_set_object(value, window->soundcard);
    break;
  case PROP_APPLICATION_CONTEXT:
    g_value_set_object(value, window->application_context);
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

  g_object_unref(G_OBJECT(window->soundcard));
  
  /* call parent */
  G_OBJECT_CLASS(monothek_window_parent_class)->finalize(gobject);
}

void
monothek_window_connect(AgsConnectable *connectable)
{
  MonothekWindow *window;

  GList *list, *list_start;

  window = MONOTHEK_WINDOW(connectable);

  if((MONOTHEK_WINDOW_CONNECTED & (window->flags)) != 0){
    return;
  }

  window->flags |= MONOTHEK_WINDOW_CONNECTED;  
}

void
monothek_window_disconnect(AgsConnectable *connectable)
{
  MonothekWindow *window;

  GList *list, *list_start;

  window = MONOTHEK_WINDOW(connectable);

  if((MONOTHEK_WINDOW_CONNECTED & (window->flags)) == 0){
    return;
  }

  window->flags &= (~MONOTHEK_WINDOW_CONNECTED);
}

void
monothek_window_show(GtkWidget *widget)
{
  MonothekWindow *window;

  window = (MonothekWindow *) widget;

  GTK_WIDGET_CLASS(monothek_window_parent_class)->show(widget);
}

gboolean
monothek_window_delete_event(GtkWidget *widget, GdkEventAny *event)
{
  gtk_widget_destroy(widget);

  GTK_WIDGET_CLASS(monothek_window_parent_class)->delete_event(widget, event);

  return(FALSE);
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
