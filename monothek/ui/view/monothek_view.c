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

#include "monothek_view.h"

#include <atk/atk.h>

#include <gdk/gdkkeysyms.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>
#include <math.h>

#include <monothek/i18n.h>

void monothek_view_class_init(MonothekViewClass *view);
void monothek_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_view_init(MonothekView *view);
void monothek_view_set_property(GObject *gobject,
				guint prop_id,
				const GValue *value,
				GParamSpec *param_spec);
void monothek_view_get_property(GObject *gobject,
				guint prop_id,
				GValue *value,
				GParamSpec *param_spec);
void monothek_view_finalize(GObject *gobject);

gboolean monothek_view_is_ready(AgsConnectable *connectable);
gboolean monothek_view_is_connected(AgsConnectable *connectable);
void monothek_view_connect(AgsConnectable *connectable);
void monothek_view_disconnect(AgsConnectable *connectable);

void monothek_view_map(GtkWidget *widget);
void monothek_view_realize(GtkWidget *widget);
void monothek_view_size_request(GtkWidget *widget,
				GtkRequisition   *requisition);
void monothek_view_size_allocate(GtkWidget *widget,
				 GtkAllocation *allocation);
gboolean monothek_view_expose(GtkWidget *widget,
			      GdkEventExpose *event);
gboolean monothek_view_button_press(GtkWidget *widget,
				    GdkEventButton *event);
gboolean monothek_view_button_release(GtkWidget *widget,
				      GdkEventButton *event);
gboolean monothek_view_key_press(GtkWidget *widget,
				 GdkEventKey *event);
gboolean monothek_view_key_release(GtkWidget *widget,
				   GdkEventKey *event);
gboolean monothek_view_motion_notify(GtkWidget *widget,
				     GdkEventMotion *event);

void monothek_view_real_draw(MonothekView *view);

/**
 * SECTION:monothek_view
 * @short_description: The view object.
 * @title: MonothekView
 * @section_id:
 * @include: monothek/ui/view/monothek_view.h
 *
 * #MonothekView is a base class of a MVC widget's view.
 */

enum{
  DRAW,
  RESET,
  CLEAR,
  LAST_SIGNAL,
};

enum{
  PROP_0,
  PROP_JUKEBOX_GC,
  PROP_DISKJOKEY_GC,
  PROP_FONT,
  PROP_MODEL,
};

static gpointer monothek_view_parent_class = NULL;

static guint view_signals[LAST_SIGNAL];

GType
monothek_view_get_type(void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_view = 0;

    static const GTypeInfo monothek_view_info = {
      sizeof(MonothekViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof(MonothekView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_view = g_type_register_static(GTK_TYPE_WIDGET,
						"MonothekView", &monothek_view_info,
						0);

    g_type_add_interface_static(monothek_type_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_view_class_init(MonothekViewClass *view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_view_parent_class = g_type_class_peek_parent(view);

  /* GObjectClass */
  gobject = (GObjectClass *) view;

  gobject->set_property = monothek_view_set_property;
  gobject->get_property = monothek_view_get_property;

  gobject->finalize = monothek_view_finalize;

  /* properties */
  /**
   * MonothekView:jukebox-gc:
   *
   * The jukebox ground color used for drawing strings.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("jukebox-gc",
				 i18n("jukebox ground color"),
				 i18n("The jukebox's ground color"),
				 0,
				 0xffffff,
				 MONOTHEK_VIEW_DEFAULT_JUKEBOX_GC,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_JUKEBOX_GC,
				  param_spec);

  /**
   * MonothekView:diskjokey-gc:
   *
   * The diskjokey ground color used for drawing strings.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("diskjokey-gc",
				 i18n("diskjokey ground color"),
				 i18n("The diskjokey's ground color"),
				 0,
				 0xffffff,
				 MONOTHEK_VIEW_DEFAULT_DISKJOKEY_GC,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_DISKJOKEY_GC,
				  param_spec);

  /**
   * MonothekView:font:
   *
   * The font used for drawing strings.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("font",
				   i18n("font"),
				   i18n("The font to be used for drawing strings"),
				   MONOTHEK_VIEW_DEFAULT_FONT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_FONT,
				  param_spec);

  /**
   * MonothekView:model:
   *
   * The model assigned to the view.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("model",
				   i18n("model"),
				   i18n("The model of view"),
				   G_TYPE_OBJECT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_MODEL,
				  param_spec);
  
  /* GtkWidgetClass */
  widget = (GtkWidgetClass *) view;

  //  widget->map = monothek_view_map;
  widget->realize = monothek_view_realize;
  widget->expose_event = monothek_view_expose;
  widget->size_request = monothek_view_size_request;
  widget->size_allocate = monothek_view_size_allocate;
  widget->button_press_event = monothek_view_button_press;
  widget->button_release_event = monothek_view_button_release;
  widget->key_press_event = monothek_view_key_press;
  widget->key_release_event = monothek_view_key_release;
  widget->motion_notify_event = monothek_view_motion_notify;
  
  /* MonothekViewClass */
  view->draw = monothek_view_real_draw;

  view->reset = NULL;
  view->clear = NULL;
  
  /* signals */
  /**
   * MonothekView::draw:
   * @view: the #MonothekView
   *
   * The ::draw signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  view_signals[DRAW] =
    g_signal_new("draw",
		 G_TYPE_FROM_CLASS(view),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekViewClass, draw),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekView::reset:
   * @view: the #MonothekView
   *
   * The ::reset signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  view_signals[RESET] =
    g_signal_new("reset",
		 G_TYPE_FROM_CLASS(view),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekViewClass, reset),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__BOOLEAN_BOOLEAN,
		 G_TYPE_NONE, 2,
		 G_TYPE_BOOLEAN,
		 G_TYPE_BOOLEAN);

  /**
   * MonothekView::clear:
   * @view: the #MonothekView
   *
   * The ::clear signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  view_signals[CLEAR] =
    g_signal_new("clear",
		 G_TYPE_FROM_CLASS(view),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekViewClass, clear),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__BOOLEAN_BOOLEAN,
		 G_TYPE_NONE, 2,
		 G_TYPE_BOOLEAN,
		 G_TYPE_BOOLEAN);
}

void
monothek_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  connectable->is_ready = monothek_view_is_ready;
  connectable->is_connected = monothek_view_is_connected;
  
  connectable->connect = monothek_view_connect;
  connectable->disconnect = monothek_view_disconnect;
}

void
monothek_view_init(MonothekView *view)
{
  AtkObject *accessible;

  g_object_set(G_OBJECT(view),
	       "app-paintable", TRUE,
	       "can-focus", TRUE,
	       NULL);

  view->flags = 0;

  view->button_state = 0;

  view->jukebox_gc = MONOTHEK_VIEW_DEFAULT_JUKEBOX_GC;
  view->diskjokey_gc = MONOTHEK_VIEW_DEFAULT_DISKJOKEY_GC;
  
  view->font = MONOTHEK_VIEW_DEFAULT_FONT;

  view->model = NULL;
}

void
monothek_view_set_property(GObject *gobject,
			   guint prop_id,
			   const GValue *value,
			   GParamSpec *param_spec)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(gobject);

  switch(prop_id){
  case PROP_JUKEBOX_GC:
    {
      view->jukebox_gc = g_value_get_uint(value);
    }
    break;
  case PROP_DISKJOKEY_GC:
    {
      view->diskjokey_gc = g_value_get_uint(value);
    }
    break;
  case PROP_FONT:
    {
      gchar *font;

      font = g_value_get_string(value);
      
      view->font = g_strdup(font);
    }
    break;
  case PROP_MODEL:
    {
      GObject *model;

      model = g_value_get_object(value);

      if(view->model == model){
	return;
      }

      if(view->model != NULL){
	g_object_unref(view->model);
      }

      if(model != NULL){
	g_object_ref(model);
      }

      view->model = model;
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_view_get_property(GObject *gobject,
			   guint prop_id,
			   GValue *value,
			   GParamSpec *param_spec)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(gobject);

  switch(prop_id){
  case PROP_JUKEBOX_GC:
    {
      g_value_set_uint(value,
		       view->jukebox_gc);
    }
    break;
  case PROP_DISKJOKEY_GC:
    {
      g_value_set_uint(value,
		       view->diskjokey_gc);
    }
    break;
  case PROP_FONT:
    {
      g_value_set_string(value,
			 view->font);
    }
    break;
  case PROP_MODEL:
    {
      g_value_set_object(value,
			 view->model);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_view_finalize(GObject *gobject)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(gobject);

  g_free(view->font);
  
  if(view->model != NULL){
    g_object_unref(view->model);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_view_parent_class)->finalize(gobject);
}

gboolean
monothek_view_is_ready(AgsConnectable *connectable)
{
  MonothekView *view;

  gboolean retval;
  
  view = MONOTHEK_VIEW(connectable);

  retval = ((MONOTHEK_VIEW_ADDED_TO_REGISTRY & (view->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

gboolean
monothek_view_is_connected(AgsConnectable *connectable)
{
  MonothekView *view;

  gboolean retval;
  
  view = MONOTHEK_VIEW(connectable);

  retval = ((MONOTHEK_VIEW_CONNECTED & (view->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

void
monothek_view_connect(AgsConnectable *connectable)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(connectable);

  if(monothek_view_test_flags(view, MONOTHEK_VIEW_CONNECTED)){
    return;
  }

  monothek_view_set_flags(view, MONOTHEK_VIEW_CONNECTED);
}

void
monothek_view_disconnect(AgsConnectable *connectable)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(connectable);

  if(!monothek_view_test_flags(view, MONOTHEK_VIEW_CONNECTED)){
    return;
  }

  monothek_view_unset_flags(view, MONOTHEK_VIEW_CONNECTED);
}

void
monothek_view_map(GtkWidget *widget)
{
  if(gtk_widget_get_realized (widget) && !gtk_widget_get_mapped(widget)){
    GTK_WIDGET_CLASS(monothek_view_parent_class)->map(widget);
    
    gdk_window_show(widget->window);
    monothek_view_draw((MonothekView *) widget);
  }
}

void
monothek_view_realize(GtkWidget *widget)
{
  MonothekView *view;

  GdkWindowAttr attributes;

  gint attributes_mask;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(MONOTHEK_IS_VIEW(widget));

  view = MONOTHEK_VIEW(widget);

  gtk_widget_set_realized(widget, TRUE);

  /*  */
  attributes.window_type = GDK_WINDOW_CHILD;
  
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);
  attributes.event_mask = gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_EXPOSURE_MASK |
                            GDK_BUTTON_PRESS_MASK |
                            GDK_BUTTON_RELEASE_MASK |
#ifdef MONOTHEK_DEVEL_MODE
			    GDK_KEY_PRESS_MASK |
			    GDK_KEY_RELEASE_MASK |
#endif
                            GDK_BUTTON1_MOTION_MASK |
                            GDK_BUTTON3_MOTION_MASK |
                            GDK_POINTER_MOTION_HINT_MASK |
                            GDK_POINTER_MOTION_MASK |
                            GDK_ENTER_NOTIFY_MASK |
                            GDK_LEAVE_NOTIFY_MASK);

  widget->window = gdk_window_new(gtk_widget_get_parent_window (widget),
				  &attributes, attributes_mask);
  gdk_window_set_user_data(widget->window, view);

  widget->style = gtk_style_attach(widget->style,
				   widget->window);
  gtk_style_set_background(widget->style,
			   widget->window,
			   GTK_STATE_NORMAL);

  gtk_widget_queue_resize(widget);
}

void
monothek_view_size_request(GtkWidget *widget,
			   GtkRequisition *requisition)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(widget);

  requisition->width = 1920;
  requisition->height = 1080;
}

void
monothek_view_size_allocate(GtkWidget *widget,
			    GtkAllocation *allocation)
{
  MonothekView *view;

  view = MONOTHEK_VIEW(widget);
  
  widget->allocation = *allocation;

  widget->allocation.x = 0;
  widget->allocation.y = 0;

  widget->allocation.width = 1920;
  widget->allocation.height = 1080;
}

gboolean
monothek_view_expose(GtkWidget *widget,
		     GdkEventExpose *event)
{
  monothek_view_draw(MONOTHEK_VIEW(widget));

  return(FALSE);
}

gboolean
monothek_view_button_press(GtkWidget *widget,
			   GdkEventButton *event)
{
  MonothekView *view;

  guint width, height;
  guint x_start, y_start;

  view = MONOTHEK_VIEW(widget);

  width = widget->allocation.width;
  height = widget->allocation.height;

  x_start = 0;
  y_start = 0;

  if(event->x >= x_start &&
     event->x < width &&
     event->y >= y_start &&
     event->y < height){
    if(event->button == 1){      
      view->button_state |= MONOTHEK_VIEW_BUTTON_1_PRESSED;

      //TODO:JK: implement me
    }
  }
  
  return(FALSE);
}

gboolean
monothek_view_button_release(GtkWidget *widget,
			     GdkEventButton *event)
{
  MonothekView *view;

  gtk_widget_grab_focus(widget);

  view = MONOTHEK_VIEW(widget);
  
  if(event->button == 1){
    if((MONOTHEK_VIEW_BUTTON_1_PRESSED & (view->button_state)) != 0){
      //TODO:JK: implement me
    }
    
    view->button_state &= (~MONOTHEK_VIEW_BUTTON_1_PRESSED);
  }

  return(FALSE);
}

gboolean
monothek_view_key_press(GtkWidget *widget,
			GdkEventKey *event)
{
  if(event->keyval == GDK_KEY_Tab ||
     event->keyval == GDK_ISO_Left_Tab ||
     event->keyval == GDK_KEY_Shift_L ||
     event->keyval == GDK_KEY_Shift_R ||
     event->keyval == GDK_KEY_Alt_L ||
     event->keyval == GDK_KEY_Alt_R ||
     event->keyval == GDK_KEY_Control_L ||
     event->keyval == GDK_KEY_Control_R ){
    return(GTK_WIDGET_CLASS(monothek_view_parent_class)->key_press_event(widget, event));
  }

  return(TRUE);
}

gboolean
monothek_view_key_release(GtkWidget *widget,
			  GdkEventKey *event)
{
  MonothekView *view;

  //TODO:JK: implement me
  
  if(event->keyval == GDK_KEY_Tab ||
     event->keyval == GDK_ISO_Left_Tab ||
     event->keyval == GDK_KEY_Shift_L ||
     event->keyval == GDK_KEY_Shift_R ||
     event->keyval == GDK_KEY_Alt_L ||
     event->keyval == GDK_KEY_Alt_R ||
     event->keyval == GDK_KEY_Control_L ||
     event->keyval == GDK_KEY_Control_R ){
    return(GTK_WIDGET_CLASS(monothek_view_parent_class)->key_release_event(widget, event));
  }

  view = MONOTHEK_VIEW(widget);  
  
  return(TRUE);
}

gboolean
monothek_view_motion_notify(GtkWidget *widget,
			    GdkEventMotion *event)
{
  MonothekView *view;

  guint width, height;
  
  view = MONOTHEK_VIEW(widget);

  width = widget->allocation.width;
  height = widget->allocation.height;

  //TODO:JK: implement me
  
  return(FALSE);
}

void
monothek_view_real_draw(MonothekView *view)
{
  cairo_t *cr;

  guint width, height;
  guint x_start, y_start;

  static const gdouble white_gc = 65535.0;

  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_start = 0;
  y_start = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  /* fill bg */
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_rectangle(cr,
		  (double) x_start, (double) y_start,
		  (double) width, (double) height);
  cairo_fill(cr);

  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);
}

/**
 * monothek_view_test_flags:
 * @view: the #MonothekView
 * @flags: the flags
 * 
 * Test @view to have @flags set. 
 * 
 * Returns: %TRUE on success, otherwise %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_view_test_flags(MonothekView *view, guint flags)
{
  gboolean retval;

  if(!MONOTHEK_IS_VIEW(view)){
    return(FALSE);
  }

  retval = (flags & (view->flags)) ? TRUE: FALSE;

  return(retval);
}

/**
 * monothek_view_set_flags:
 * @view: the #MonothekView
 * @flags: the flags
 * 
 * Set @flags for  @view.
 * 
 * Since: 1.0.0
 */
void
monothek_view_set_flags(MonothekView *view, guint flags)
{
  if(!MONOTHEK_IS_VIEW(view)){
    return;
  }

  view->flags |= flags;
}

/**
 * monothek_view_unset_flags:
 * @view: the #MonothekView
 * @flags: the flags
 * 
 * Unset @flags for  @view.
 * 
 * Since: 1.0.0
 */
void
monothek_view_unset_flags(MonothekView *view, guint flags)
{
  if(!MONOTHEK_IS_VIEW(view)){
    return;
  }

  view->flags &= (~flags);
}

/**
 * monothek_view_draw:
 * @view: the #MonothekView
 * 
 * Draw view.
 * 
 * Since: 1.0.0
 */
void
monothek_view_draw(MonothekView *view)
{
  g_return_if_fail(MONOTHEK_IS_VIEW(view));
  
  g_object_ref((GObject *) view);
  g_signal_emit(G_OBJECT(view),
		view_signals[DRAW], 0);
  g_object_unref((GObject *) view);
}

/**
 * monothek_view_new:
 * 
 * Create a new instance of #MonothekView.
 * 
 * Returns: the new #MonothekView instance
 * 
 * Since: 1.0.0
 */
MonothekView*
monothek_view_new()
{
  MonothekView *view;

  view = (MonothekView *) g_object_new(MONOTHEK_TYPE_VIEW,
				       NULL);
  
  return(view);
}
