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

#include <monothek/ui/controller/monothek_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/view/monothek_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_controller_class_init(MonothekControllerClass *controller);
void monothek_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_controller_init(MonothekController *controller);
void monothek_controller_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec);
void monothek_controller_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec);
void monothek_controller_finalize(GObject *gobject);

gboolean monothek_controller_is_ready(AgsConnectable *connectable);
gboolean monothek_controller_is_connected(AgsConnectable *connectable);
void monothek_controller_connect(AgsConnectable *connectable);
void monothek_controller_disconnect(AgsConnectable *connectable);

gboolean monothek_controller_motion_notify_event_callback(GtkWidget *widget,
							  GdkEvent *event,
							  MonothekController *controller);
gboolean monothek_controller_button_press_event_callback(GtkWidget *widget,
							 GdkEvent *event,
							 MonothekController *controller);
gboolean monothek_controller_button_release_event_callback(GtkWidget *widget,
							   GdkEvent *event,
							   MonothekController *controller);

/**
 * SECTION:monothek_controller
 * @short_description: The  controller object.
 * @title: MonothekController
 * @section_id:
 * @include: monothek/ui/controller/monothek_controller.h
 *
 * #MonothekController is the MVC's abstract controller.
 */

enum{
  PROP_0,
  PROP_MODEL,
  PROP_VIEW,
  PROP_ACTION_BOX,
  PROP_ACTION_SLIDER,
};

enum{
  RESET,
  LAST_SIGNAL,
};

static gpointer monothek_controller_parent_class = NULL;
static guint controller_signals[LAST_SIGNAL];

GType
monothek_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_controller = 0;

    static const GTypeInfo monothek_controller_info = {
      sizeof (MonothekControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_controller = g_type_register_static(G_TYPE_OBJECT,
						      "MonothekController", &monothek_controller_info,
						      0);

    g_type_add_interface_static(monothek_type_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_controller_class_init(MonothekControllerClass *controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_controller_parent_class = g_type_class_peek_parent(controller);

  /* GObjectClass */
  gobject = (GObjectClass *) controller;

  gobject->set_property = monothek_controller_set_property;
  gobject->get_property = monothek_controller_get_property;

  gobject->finalize = monothek_controller_finalize;

  /* properties */
  /**
   * MonothekController:model:
   *
   * The assigned #GObject implementing the model.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("model",
				   i18n_pspec("assigned model"),
				   i18n_pspec("The model it is assigned with"),
				   G_TYPE_OBJECT,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_MODEL,
				  param_spec);

  /**
   * MonothekController:view:
   *
   * The assigned #MonothekView.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("view",
				   i18n_pspec("assigned view"),
				   i18n_pspec("The view it is assigned with"),
				   MONOTHEK_TYPE_VIEW,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_VIEW,
				  param_spec);
  
  /**
   * MonothekControlelr:action-box:
   *
   * The assigned #GList-struct containing #MonothekActionBox.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("action-box",
				    i18n_pspec("action box list"),
				    i18n_pspec("The action box list"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ACTION_BOX,
				  param_spec);
  
  /**
   * MonothekControlelr:action-slider:
   *
   * The assigned #GList-struct containing #MonothekActionSlider.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("action-slider",
				    i18n_pspec("action slider list"),
				    i18n_pspec("The action slider list"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ACTION_SLIDER,
				  param_spec);

  /* MonothekControllerClass */
  controller->reset = NULL;
  
  /* signals */
  /**
   * MonothekController::reset:
   * @controller: the #MonothekController
   *
   * The ::reset signal notifies to reset.
   *
   * Since: 1.0.0
   */
  controller_signals[RESET] = 
    g_signal_new("reset",
		 G_TYPE_FROM_CLASS(controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekControllerClass, reset),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{
  connectable->is_ready = monothek_controller_is_ready;
  connectable->is_connected = monothek_controller_is_connected;
  
  connectable->connect = monothek_controller_connect;
  connectable->disconnect = monothek_controller_disconnect;
}

void
monothek_controller_init(MonothekController *controller)
{
  controller->flags = 0;
  
  controller->model = NULL;

  controller->view = NULL;

  controller->action_box = NULL;
  controller->action_slider = NULL;
}

void
monothek_controller_set_property(GObject *gobject,
				 guint prop_id,
				 const GValue *value,
				 GParamSpec *param_spec)
{
  MonothekController *controller;

  controller = MONOTHEK_CONTROLLER(gobject);

  switch(prop_id){
  case PROP_MODEL:
    {
      GObject *model;

      model = g_value_get_object(value);

      if(model == controller->model){
	return;
      }

      if(controller->model != NULL){
	g_object_unref(controller->model);
      }

      if(model != NULL){
	g_object_ref(model);
      }

      controller->model = model;
    }
    break;
  case PROP_VIEW:
    {
      MonothekView *view;

      view = g_value_get_object(value);

      if(view == controller->view){
	return;
      }

      if(controller->view != NULL){
	g_object_unref(controller->view);
      }

      if(view != NULL){
	g_object_ref(view);
      }

      controller->view = view;
    }
    break;
  case PROP_ACTION_BOX:
    {
      MonothekActionBox *action_box;

      action_box = g_value_get_pointer(value);
      
      monothek_controller_add_action_box(controller,
					 action_box);
    }
    break;
  case PROP_ACTION_SLIDER:
    {
      MonothekActionSlider *action_slider;

      action_slider = g_value_get_pointer(value);
      
      monothek_controller_add_action_slider(controller,
					    action_slider);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_controller_get_property(GObject *gobject,
				 guint prop_id,
				 GValue *value,
				 GParamSpec *param_spec)
{
  MonothekController *controller;

  controller = MONOTHEK_CONTROLLER(gobject);

  switch(prop_id){
  case PROP_MODEL:
    {
      g_value_set_object(value,
			 controller->model);
    }
    break;
  case PROP_VIEW:
    {
      g_value_set_object(value,
			 controller->view);
    }
    break;
  case PROP_ACTION_BOX:
    {
      g_value_set_pointer(value,
			  g_list_copy(controller->action_box));
    }
    break;
  case PROP_ACTION_SLIDER:
    {
      g_value_set_pointer(value,
			  g_list_copy(controller->action_slider));
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_controller_finalize(GObject *gobject)
{
  MonothekController *controller;

  controller = (MonothekController *) gobject;

  if(controller->view != NULL){
    g_object_unref(controller->view);
  }
  
  g_list_free_full(controller->action_box,
		   g_object_unref);

  g_list_free_full(controller->action_slider,
		   g_object_unref);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_controller_parent_class)->finalize(gobject);
}

gboolean
monothek_controller_is_ready(AgsConnectable *connectable)
{
  MonothekController *controller;

  gboolean retval;
  
  controller = MONOTHEK_CONTROLLER(connectable);

  retval = ((MONOTHEK_CONTROLLER_ADDED_TO_REGISTRY & (controller->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

gboolean
monothek_controller_is_connected(AgsConnectable *connectable)
{
  MonothekController *controller;

  gboolean retval;
  
  controller = MONOTHEK_CONTROLLER(connectable);

  retval = ((MONOTHEK_CONTROLLER_CONNECTED & (controller->flags)) != 0) ? TRUE: FALSE;
  
  return(retval);
}

void
monothek_controller_connect(AgsConnectable *connectable)
{
  MonothekView *view;
  MonothekController *controller;

  controller = MONOTHEK_CONTROLLER(connectable);

  if(monothek_controller_test_flags(controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_controller_set_flags(controller, MONOTHEK_CONTROLLER_CONNECTED);

  g_object_get(controller,
	       "view", &view,
	       NULL);

  if(view != NULL){
    g_signal_connect(G_OBJECT(view), "motion-notify-event",
		     G_CALLBACK(monothek_controller_motion_notify_event_callback), controller);

    g_signal_connect(G_OBJECT(view), "button-press-event",
		     G_CALLBACK(monothek_controller_button_press_event_callback), controller);
    
    g_signal_connect(G_OBJECT(view), "button-release-event",
		     G_CALLBACK(monothek_controller_button_release_event_callback), controller);
  }
}

void
monothek_controller_disconnect(AgsConnectable *connectable)
{
  MonothekView *view;
  MonothekController *controller;

  controller = MONOTHEK_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_controller_unset_flags(controller, MONOTHEK_CONTROLLER_CONNECTED);

  g_object_get(controller,
	       "view", &view,
	       NULL);

  if(view != NULL){
    g_object_disconnect(G_OBJECT(view),
			"any_signal::motion-notify-event",
			G_CALLBACK(monothek_controller_motion_notify_event_callback),
			controller,
			"any_signal::button-press-event",
			G_CALLBACK(monothek_controller_button_press_event_callback),
			controller,
			"any_signal::button-release-event",
			G_CALLBACK(monothek_controller_button_release_event_callback),
			controller,
			NULL);
  }
}

gboolean
monothek_controller_motion_notify_event_callback(GtkWidget *widget,
						 GdkEvent *event,
						 MonothekController *controller)
{
  GList *list_start, *list;
  
  /* action box */
  g_object_get(controller,
	       "action-box", &list_start,
	       NULL);

  list = list_start;

  while(list != NULL){
    guint x0, y0;
    guint width, height;

    if(!MONOTHEK_ACTION_BOX(list->data)->enabled){
      list = list->next;

      continue;
    }
    
    g_object_get(G_OBJECT(list->data),
		 "x0", &x0,
		 "y0", &y0,
		 "width", &width,
		 "height", &height,
		 NULL);

    if(x0 <= ((GdkEventMotion *) event)->x &&
       x0 + width > ((GdkEventMotion *) event)->x &&
       y0 <= ((GdkEventMotion *) event)->y &&
       y0 + height > ((GdkEventMotion *) event)->y){
      if(!monothek_action_box_get_active(list->data)){
	monothek_action_box_enter(list->data);
      }
    }else{
      if(monothek_action_box_get_active(list->data)){
	monothek_action_box_leave(list->data);
      }
    }
    
    list = list->next;
  }

  g_list_free(list_start);
  
  /* action slider */
  g_object_get(controller,
	       "action-slider", &list_start,
	       NULL);

  list = list_start;

  while(list != NULL){
    guint x0, y0;
    guint width, height;

    g_object_get(G_OBJECT(list->data),
		 "x0", &x0,
		 "y0", &y0,
		 "width", &width,
		 "height", &height,
		 NULL);

    if(x0 <= ((GdkEventMotion *) event)->x &&
       x0 + width > ((GdkEventMotion *) event)->x &&
       y0 <= ((GdkEventMotion *) event)->y &&
       y0 + height > ((GdkEventMotion *) event)->y){      
      GtkAdjustment *adjustment;

      gdouble event_x, event_y;
      gdouble new_value;

      adjustment = MONOTHEK_ACTION_SLIDER(list->data)->adjustment;
      new_value = 0.0;

      event_x = ((GdkEventButton *) event)->x;
      event_y = ((GdkEventButton *) event)->y;
      
      if(MONOTHEK_ACTION_SLIDER(list->data)->orientation == GTK_ORIENTATION_HORIZONTAL){
	new_value = adjustment->lower + (adjustment->upper - adjustment->lower) / width * (event_x - x0);
      }else if(MONOTHEK_ACTION_SLIDER(list->data)->orientation == GTK_ORIENTATION_VERTICAL){
	new_value = adjustment->lower + (adjustment->upper - adjustment->lower) / height * (event_y - y0);
      }
      
      monothek_action_slider_change_value(list->data,
					  new_value);
      monothek_action_slider_move_slider(list->data);
      monothek_action_slider_value_changed(list->data);
    }
    
    list = list->next;
  }

  g_list_free(list_start);

  return(FALSE);
}

gboolean
monothek_controller_button_press_event_callback(GtkWidget *widget,
						GdkEvent *event,
						MonothekController *controller)
{
  GList *list_start, *list;

  /* action box */
  g_object_get(controller,
	       "action-box", &list_start,
	       NULL);

  list = list_start;

  while(list != NULL){
    guint x0, y0;
    guint width, height;

    if(!MONOTHEK_ACTION_BOX(list->data)->enabled){
      list = list->next;

      continue;
    }

    g_object_get(G_OBJECT(list->data),
		 "x0", &x0,
		 "y0", &y0,
		 "width", &width,
		 "height", &height,
		 NULL);

    if(x0 <= ((GdkEventButton *) event)->x &&
       x0 + width > ((GdkEventButton *) event)->x &&
       y0 <= ((GdkEventButton *) event)->y &&
       y0 + height > ((GdkEventButton *) event)->y){
      monothek_action_box_pressed(list->data);
    }else{
      if(monothek_action_box_get_active(list->data)){
	monothek_action_box_leave(list->data);
      }
    }
    
    list = list->next;
  }

  g_list_free(list_start);

  /* action slider */
  g_object_get(controller,
	       "action-slider", &list_start,
	       NULL);

  list = list_start;

  while(list != NULL){
    guint x0, y0;
    guint width, height;

    g_object_get(G_OBJECT(list->data),
		 "x0", &x0,
		 "y0", &y0,
		 "width", &width,
		 "height", &height,
		 NULL);

    if(x0 <= ((GdkEventButton *) event)->x &&
       x0 + width > ((GdkEventButton *) event)->x &&
       y0 <= ((GdkEventButton *) event)->y &&
       y0 + height > ((GdkEventButton *) event)->y){
      GtkAdjustment *adjustment;

      gdouble event_x, event_y;
      gdouble new_value;

      adjustment = MONOTHEK_ACTION_SLIDER(list->data)->adjustment;
      new_value = 0.0;

      event_x = ((GdkEventButton *) event)->x;
      event_y = ((GdkEventButton *) event)->y;
      
      if(MONOTHEK_ACTION_SLIDER(list->data)->orientation == GTK_ORIENTATION_HORIZONTAL){
	new_value = adjustment->lower + (adjustment->upper - adjustment->lower) / width * (event_x - x0);
      }else if(MONOTHEK_ACTION_SLIDER(list->data)->orientation == GTK_ORIENTATION_VERTICAL){
	new_value = adjustment->lower + (adjustment->upper - adjustment->lower) / height * (event_y - y0);
      }
      
      monothek_action_slider_change_value(list->data,
					  new_value);
      monothek_action_slider_move_slider(list->data);
      monothek_action_slider_value_changed(list->data);
    }
    
    list = list->next;
  }

  g_list_free(list_start);
  
  return(FALSE);
}

gboolean
monothek_controller_button_release_event_callback(GtkWidget *widget,
						  GdkEvent *event,
						  MonothekController *controller)
{
  GList *list_start, *list;
  
  g_object_get(controller,
	       "action-box", &list_start,
	       NULL);

  list = list_start;

  while(list != NULL){
    guint x0, y0;
    guint width, height;

    if(!MONOTHEK_ACTION_BOX(list->data)->enabled){
      list = list->next;

      continue;
    }

    g_object_get(G_OBJECT(list->data),
		 "x0", &x0,
		 "y0", &y0,
		 "width", &width,
		 "height", &height,
		 NULL);

    if(x0 <= ((GdkEventButton *) event)->x &&
       x0 + width > ((GdkEventButton *) event)->x &&
       y0 <= ((GdkEventButton *) event)->y &&
       y0 + height > ((GdkEventButton *) event)->y){
      monothek_action_box_released(list->data);
    }else{
      if(monothek_action_box_get_active(list->data)){
	monothek_action_box_leave(list->data);
      }
    }
    
    list = list->next;
  }

  g_list_free(list_start);

  return(FALSE);
}

/**
 * monothek_controller_test_flags:
 * @controller: the #MonothekController
 * @flags: the flags
 * 
 * Test @controller to have @flags set. 
 * 
 * Returns: %TRUE on success, otherwise %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_controller_test_flags(MonothekController *controller, guint flags)
{
  gboolean retval;

  if(!MONOTHEK_IS_CONTROLLER(controller)){
    return(FALSE);
  }

  retval = (flags & (controller->flags)) ? TRUE: FALSE;

  return(retval);
}

/**
 * monothek_controller_set_flags:
 * @controller: the #MonothekController
 * @flags: the flags
 * 
 * Set @flags for  @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_set_flags(MonothekController *controller, guint flags)
{
  if(!MONOTHEK_IS_CONTROLLER(controller)){
    return;
  }

  controller->flags |= flags;
}

/**
 * monothek_controller_unset_flags:
 * @controller: the #MonothekController
 * @flags: the flags
 * 
 * Unset @flags for  @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_unset_flags(MonothekController *controller, guint flags)
{
  if(!MONOTHEK_IS_CONTROLLER(controller)){
    return;
  }

  controller->flags &= (~flags);
}

/**
 * monothek_controller_add_action_box:
 * @controller: the #MonothekController
 * @action_box: the #MonothekActionBox
 * 
 * Add @action_box to @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_add_action_box(MonothekController *controller,
				   MonothekActionBox *action_box)
{
  if(!MONOTHEK_IS_CONTROLLER(controller) ||
     !MONOTHEK_IS_ACTION_BOX(action_box)){
    return;
  }

  if(g_list_find(controller->action_box, action_box) == NULL){
    g_object_ref(action_box);
    controller->action_box = g_list_prepend(controller->action_box,
					    action_box);
  }
}

/**
 * monothek_controller_remove_action_box:
 * @controller: the #MonothekController
 * @action_box: the #MonothekActionBox
 * 
 * Remove @action_box from @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_remove_action_box(MonothekController *controller,
				      MonothekActionBox *action_box)
{
  if(!MONOTHEK_IS_CONTROLLER(controller) ||
     !MONOTHEK_IS_ACTION_BOX(action_box)){
    return;
  }

  if(g_list_find(controller->action_box, action_box) != NULL){
    g_object_unref(action_box);
    controller->action_box = g_list_remove(controller->action_box,
					   action_box);
  }
}

/**
 * monothek_controller_add_action_slider:
 * @controller: the #MonothekController
 * @action_slider: the #MonothekActionSlider
 * 
 * Add @action_slider to @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_add_action_slider(MonothekController *controller,
				      MonothekActionSlider *action_slider)
{
  if(!MONOTHEK_IS_CONTROLLER(controller) ||
     !MONOTHEK_IS_ACTION_SLIDER(action_slider)){
    return;
  }

  if(g_list_find(controller->action_slider, action_slider) == NULL){
    g_object_ref(action_slider);
    controller->action_slider = g_list_prepend(controller->action_slider,
					       action_slider);
  }
}

/**
 * monothek_controller_remove_action_slider:
 * @controller: the #MonothekController
 * @action_slider: the #MonothekActionSlider
 * 
 * Remove @action_slider from @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_remove_action_slider(MonothekController *controller,
					 MonothekActionSlider *action_slider)
{
  if(!MONOTHEK_IS_CONTROLLER(controller) ||
     !MONOTHEK_IS_ACTION_SLIDER(action_slider)){
    return;
  }

  if(g_list_find(controller->action_slider, action_slider) != NULL){
    g_object_unref(action_slider);
    controller->action_slider = g_list_remove(controller->action_slider,
					      action_slider);
  }
}

/**
 * monothek_controller_find_view_type:
 * @list: the #GList-struct containing #MonothekController
 * @view_type: the view type to match
 * 
 * Find @view_type in @list.
 * 
 * Returns: the matching #GList-struct if found, else %NULL
 * 
 * Since: 1.0.0
 */
GList*
monothek_controller_find_view_type(GList *list,
				   GType view_type)
{
  while(list != NULL){
    if(MONOTHEK_IS_CONTROLLER(list->data) &&
       G_OBJECT_TYPE(MONOTHEK_CONTROLLER(list->data)->view) == view_type){
      return(list);
    }

    list = list->next;
  }

  return(NULL);
}

/**
 * monothek_controller_reset:
 * @controller: the #MonothekController
 * 
 * Reset @controller.
 * 
 * Since: 1.0.0
 */
void
monothek_controller_reset(MonothekController *controller)
{
  g_return_if_fail(MONOTHEK_IS_CONTROLLER(controller));

  /* emit */
  g_object_ref((GObject *) controller);
  g_signal_emit(G_OBJECT(controller),
		controller_signals[RESET], 0);
  g_object_unref((GObject *) controller);  
}

/**
 * monothek_controller_new:
 *
 * Creates an #MonothekController
 *
 * Returns: a new #MonothekController
 *
 * Since: 1.0.0
 */
MonothekController*
monothek_controller_new()
{
  MonothekController *controller;

  controller = (MonothekController *) g_object_new(MONOTHEK_TYPE_CONTROLLER,
						   NULL);
  
  return(controller);
}
