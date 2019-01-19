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

#include <monothek/ui/controller/monothek_action_slider.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/object/monothek_marshal.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_action_slider_class_init(MonothekActionSliderClass *action_slider);
void monothek_action_slider_init(MonothekActionSlider *action_slider);
void monothek_action_slider_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec);
void monothek_action_slider_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec);
void monothek_action_slider_finalize(GObject *gobject);

gboolean monothek_action_slider_real_change_value(MonothekActionSlider *action_slider,
						  gdouble new_value);  

/**
 * SECTION:monothek_action_slider
 * @short_description: The  action_slider object.
 * @title: MonothekActionSlider
 * @section_id:
 * @include: monothek/ui/controller/monothek_action_slider.h
 *
 * #MonothekActionSlider does listen do view's mouse events and synthesises
 * its very own events.
 */

enum{
  PROP_0,
  PROP_ACTION_IDENTIFIER,
  PROP_X0,
  PROP_Y0,
  PROP_WIDTH,
  PROP_HEIGHT,
};

enum{
  CHANGE_VALUE,
  MOVE_SLIDER,
  VALUE_CHANGED,
  LAST_SIGNAL,
};

static gpointer monothek_action_slider_parent_class = NULL;

static guint action_slider_signals[LAST_SIGNAL];

GType
monothek_action_slider_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_action_slider = 0;

    static const GTypeInfo monothek_action_slider_info = {
      sizeof (MonothekActionSliderClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_action_slider_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekActionSlider),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_action_slider_init,
    };

    monothek_type_action_slider = g_type_register_static(G_TYPE_OBJECT,
							 "MonothekActionSlider", &monothek_action_slider_info,
							 0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_action_slider);
  }

  return g_define_type_id__volatile;
}

void
monothek_action_slider_class_init(MonothekActionSliderClass *action_slider)
{
  GObjectClass *gobject;

  GParamSpec *param_spec;

  monothek_action_slider_parent_class = g_type_class_peek_parent(action_slider);

  /* GObjectClass */
  gobject = (GObjectClass *) action_slider;

  gobject->set_property = monothek_action_slider_set_property;
  gobject->get_property = monothek_action_slider_get_property;

  gobject->finalize = monothek_action_slider_finalize;

  /* properties */
  /**
   * MonothekActionSlider:action-identifier:
   *
   * The action slider's action identifier.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("action-identifier",
				   i18n_pspec("action identifier"),
				   i18n_pspec("This string is the action identifier"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_ACTION_IDENTIFIER,
				  param_spec);

  /**
   * MonothekActionSlider:x0:
   *
   * The action slider's x0 of listening region.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("x0",
				 i18n_pspec("x0"),
				 i18n_pspec("The x0 of region"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_X0,
				  param_spec);

  /**
   * MonothekActionSlider:y0:
   *
   * The action slider's y0 of listening region.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("y0",
				 i18n_pspec("y0"),
				 i18n_pspec("The y0 of region"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_Y0,
				  param_spec);

  /**
   * MonothekActionSlider:width:
   *
   * The action slider's width of listening region.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("width",
				 i18n_pspec("width"),
				 i18n_pspec("The width of region"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_WIDTH,
				  param_spec);

  /**
   * MonothekActionSlider:height:
   *
   * The action slider's height of listening region.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_uint("height",
				 i18n_pspec("height"),
				 i18n_pspec("The height of region"),
				 0,
				 G_MAXUINT32,
				 0,
				 G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_HEIGHT,
				  param_spec);
  
  /* MonothekStartController */
  action_slider->change_value = monothek_action_slider_real_change_value;
  action_slider->move_slider = NULL;

  action_slider->value_changed = NULL;

  /* signals */
  /**
   * MonothekActionSlider::change-value:
   * @action_slider: the #MonothekActionSlider
   * @new_value: the new value
   *
   * The ::change-value signal notifies about changing value event.
   *
   * Returns: %TRUE if signal emmission shall proceed, else %FALSE
   * 
   * Since: 1.0.0
   */
  action_slider_signals[CHANGE_VALUE] =
    g_signal_new("change-value",
		 G_TYPE_FROM_CLASS(action_slider),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionSliderClass, change_value),
		 NULL, NULL,
		 monothek_cclosure_marshal_BOOLEAN__DOUBLE,
		 G_TYPE_BOOLEAN, 1,
		 G_TYPE_DOUBLE);

  /**
   * MonothekActionSlider::move-slider:
   * @action_slider: the #MonothekActionSlider
   *
   * The ::move-slider signal notifies moving slider event.
   *
   * Since: 1.0.0
   */
  action_slider_signals[MOVE_SLIDER] =
    g_signal_new("move-slider",
		 G_TYPE_FROM_CLASS(action_slider),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionSliderClass, move_slider),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionSlider::value-changed:
   * @action_slider: the #MonothekActionSlider
   *
   * The ::value-changed signal notifies about value-changed event.
   *
   * Since: 1.0.0
   */
  action_slider_signals[VALUE_CHANGED] =
    g_signal_new("value-changed",
		 G_TYPE_FROM_CLASS(action_slider),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionSliderClass, value_changed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_action_slider_init(MonothekActionSlider *action_slider)
{
  action_slider->button_mask = 0;
  action_slider->orientation = GTK_ORIENTATION_HORIZONTAL;

  action_slider->adjustment = gtk_adjustment_new(0.0, 0.0, 1.0, 0.1, 0.25, 0.5);
  g_object_ref(action_slider->adjustment);

  action_slider->action_identifier = NULL;
  
  action_slider->x0 = 0;
  action_slider->y0 = 0;

  action_slider->width = 0;
  action_slider->height = 0;
}

void
monothek_action_slider_set_property(GObject *gobject,
				    guint prop_id,
				    const GValue *value,
				    GParamSpec *param_spec)
{
  MonothekActionSlider *action_slider;

  action_slider = MONOTHEK_ACTION_SLIDER(gobject);

  switch(prop_id){
  case PROP_ACTION_IDENTIFIER:
    {
      gchar *action_identifier;

      action_identifier = g_value_get_string(value);
      
      if(action_identifier == action_slider->action_identifier){
	return;
      }

      g_free(action_slider->action_identifier);
      
      action_slider->action_identifier = g_strdup(action_identifier);
    }
    break;
  case PROP_X0:
    {
      action_slider->x0 = g_value_get_uint(value);
    }
    break;
  case PROP_Y0:
    {
      action_slider->y0 = g_value_get_uint(value);
    }
    break;
  case PROP_WIDTH:
    {
      action_slider->width = g_value_get_uint(value);
    }
    break;
  case PROP_HEIGHT:
    {
      action_slider->height = g_value_get_uint(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_action_slider_get_property(GObject *gobject,
				    guint prop_id,
				    GValue *value,
				    GParamSpec *param_spec)
{
  MonothekActionSlider *action_slider;

  action_slider = MONOTHEK_ACTION_SLIDER(gobject);

  switch(prop_id){
  case PROP_ACTION_IDENTIFIER:
    {
      g_value_set_string(value,
			 action_slider->action_identifier);
    }
    break;
  case PROP_X0:
    {
      g_value_set_uint(value,
		       action_slider->x0);
    }
    break;
  case PROP_Y0:
    {
      g_value_set_uint(value,
		       action_slider->y0);
    }
    break;
  case PROP_WIDTH:
    {
      g_value_set_uint(value,
		       action_slider->width);
    }
    break;
  case PROP_HEIGHT:
    {
      g_value_set_uint(value,
		       action_slider->height);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_action_slider_finalize(GObject *gobject)
{
  MonothekActionSlider *action_slider;

  action_slider = (MonothekActionSlider *) gobject;

  g_free(action_slider->action_identifier);

  g_object_unref(action_slider->adjustment);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_action_slider_parent_class)->finalize(gobject);
}

gboolean
monothek_action_slider_real_change_value(MonothekActionSlider *action_slider,
					 gdouble new_value)
{
  gtk_adjustment_set_value(action_slider->adjustment,
			   new_value);

  return(TRUE);
}

/**
 * monothek_action_slider_change_value:
 * @action_slider: the #MonothekActionSlider
 * @new_value: the value to apply
 *
 * Emits ::change-value event.
 *
 * Returns: %TRUE if signal emmission shall proceed, else %FALSE
 * 
 * Since: 1.0.0
 */
gboolean
monothek_action_slider_change_value(MonothekActionSlider *action_slider,
				    gdouble new_value)
{
  gboolean retval;
  
  g_return_val_if_fail(MONOTHEK_IS_ACTION_SLIDER(action_slider), TRUE);
  
  g_object_ref((GObject *) action_slider);
  g_signal_emit(G_OBJECT(action_slider),
		action_slider_signals[CHANGE_VALUE], 0,
		new_value,
		&retval);
  g_object_unref((GObject *) action_slider);

  return(retval);
}

/**
 * monothek_action_slider_move_slider:
 * @action_slider: the #MonothekActionSlider
 *
 * Emits ::move-slider event.
 *
 * Since: 1.0.0
 */
void
monothek_action_slider_move_slider(MonothekActionSlider *action_slider)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_SLIDER(action_slider));
  
  g_object_ref((GObject *) action_slider);
  g_signal_emit(G_OBJECT(action_slider),
		action_slider_signals[MOVE_SLIDER], 0);
  g_object_unref((GObject *) action_slider);
}

/**
 * monothek_action_slider_value_changed:
 * @action_slider: the #MonothekActionSlider
 *
 * Emits ::value-changed event.
 *
 * Since: 1.0.0
 */
void
monothek_action_slider_value_changed(MonothekActionSlider *action_slider)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_SLIDER(action_slider));
  
  g_object_ref((GObject *) action_slider);
  g_signal_emit(G_OBJECT(action_slider),
		action_slider_signals[VALUE_CHANGED], 0);
  g_object_unref((GObject *) action_slider);
}

/**
 * monothek_action_slider_new:
 *
 * Creates an #MonothekActionSlider
 *
 * Returns: a new #MonothekActionSlider
 *
 * Since: 1.0.0
 */
MonothekActionSlider*
monothek_action_slider_new()
{
  MonothekActionSlider *action_slider;

  action_slider = (MonothekActionSlider *) g_object_new(MONOTHEK_TYPE_ACTION_SLIDER,
							NULL);
  
  return(action_slider);
}
