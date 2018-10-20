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

#include <monothek/ui/controller/monothek_action_box.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_action_box_class_init(MonothekActionBoxClass *action_box);
void monothek_action_box_init(MonothekActionBox *action_box);
void monothek_action_box_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec);
void monothek_action_box_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec);
void monothek_action_box_finalize(GObject *gobject);

/**
 * SECTION:monothek_action_box
 * @short_description: The  action_box object.
 * @title: MonothekActionBox
 * @section_id:
 * @include: monothek/ui/action_box/monothek_action_box.h
 *
 * #MonothekActionBox does listen do view's mouse events and synthesises
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
  ACTIVATE,
  CLICKED,
  ENTER,
  LEAVE,
  PRESSED,
  RELEASED,
  LAST_SIGNAL,
};

static gpointer monothek_action_box_parent_class = NULL;

static guint action_box_signals[LAST_SIGNAL];

GType
monothek_action_box_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_action_box = 0;

    static const GTypeInfo monothek_action_box_info = {
      sizeof (MonothekActionBoxClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_action_box_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekActionBox),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_action_box_init,
    };

    monothek_type_action_box = g_type_register_static(G_TYPE_OBJECT,
						      "MonothekActionBox", &monothek_action_box_info,
						      0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_action_box);
  }

  return g_define_type_id__volatile;
}

void
monothek_action_box_class_init(MonothekActionBoxClass *action_box)
{
  GObjectClass *gobject;

  GParamSpec *param_spec;

  monothek_action_box_parent_class = g_type_class_peek_parent(action_box);

  /* GObjectClass */
  gobject = (GObjectClass *) action_box;

  gobject->set_property = monothek_action_box_set_property;
  gobject->get_property = monothek_action_box_get_property;

  gobject->finalize = monothek_action_box_finalize;

  /* properties */
  /**
   * MonothekActionBox:action-identifier:
   *
   * The action box's action identifier.
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
   * MonothekActionBox:x0:
   *
   * The action box's x0 of listening region.
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
   * MonothekActionBox:y0:
   *
   * The action box's y0 of listening region.
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
   * MonothekActionBox:width:
   *
   * The action box's width of listening region.
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
   * MonothekActionBox:height:
   *
   * The action box's height of listening region.
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
  action_box->activate = NULL;
  action_box->clicked = NULL;

  action_box->enter = NULL;
  action_box->leave = NULL;

  action_box->pressed = NULL;
  action_box->released = NULL;

  /* signals */
  /**
   * MonothekActionBox::activate:
   * @action_box: the #MonothekActionBox
   *
   * The ::activate signal notifies about activate event.
   *
   * Since: 1.0.0
   */
  action_box_signals[ACTIVATE] =
    g_signal_new("activate",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, activate),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionBox::clicked:
   * @action_box: the #MonothekActionBox
   *
   * The ::clicked signal notifies about click event.
   *
   * Since: 1.0.0
   */
  action_box_signals[CLICKED] =
    g_signal_new("clicked",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, clicked),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionBox::enter:
   * @action_box: the #MonothekActionBox
   *
   * The ::enter signal notifies about enter event.
   *
   * Since: 1.0.0
   */
  action_box_signals[ENTER] =
    g_signal_new("enter",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, enter),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionBox::leave:
   * @action_box: the #MonothekActionBox
   *
   * The ::leave signal notifies about leave event.
   *
   * Since: 1.0.0
   */
  action_box_signals[LEAVE] =
    g_signal_new("leave",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, leave),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionBox::pressed:
   * @action_box: the #MonothekActionBox
   *
   * The ::pressed signal notifies about pressed event.
   *
   * Since: 1.0.0
   */
  action_box_signals[PRESSED] =
    g_signal_new("pressed",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, pressed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekActionBox::released:
   * @action_box: the #MonothekActionBox
   *
   * The ::released signal notifies about released event.
   *
   * Since: 1.0.0
   */
  action_box_signals[RELEASED] =
    g_signal_new("released",
		 G_TYPE_FROM_CLASS(action_box),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekActionBoxClass, released),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_action_box_init(MonothekActionBox *action_box)
{
  action_box->action_identifier = NULL;
  
  action_box->x0 = 0;
  action_box->y0 = 0;

  action_box->width = 0;
  action_box->height = 0;
}


void
monothek_action_box_set_property(GObject *gobject,
				 guint prop_id,
				 const GValue *value,
				 GParamSpec *param_spec)
{
  MonothekActionBox *action_box;

  action_box = MONOTHEK_ACTION_BOX(gobject);

  switch(prop_id){
  case PROP_ACTION_IDENTIFIER:
    {
      gchar *action_identifier;

      action_identifier = g_value_get_string(value);
      
      if(action_identifier == action_box->action_identifier){
	return;
      }

      g_free(action_box->action_identifier);
      
      action_box->action_identifier = g_strdup(action_identifier);
    }
    break;
  case PROP_X0:
    {
      action_box->x0 = g_value_get_uint(value);
    }
    break;
  case PROP_Y0:
    {
      action_box->y0 = g_value_get_uint(value);
    }
    break;
  case PROP_WIDTH:
    {
      action_box->width = g_value_get_uint(value);
    }
    break;
  case PROP_HEIGHT:
    {
      action_box->height = g_value_get_uint(value);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_action_box_get_property(GObject *gobject,
				 guint prop_id,
				 GValue *value,
				 GParamSpec *param_spec)
{
  MonothekActionBox *action_box;

  action_box = MONOTHEK_ACTION_BOX(gobject);

  switch(prop_id){
  case PROP_ACTION_IDENTIFIER:
    {
      g_value_set_string(value,
			 action_box->action_identifier);
    }
    break;
  case PROP_X0:
    {
      g_value_set_uint(value,
		       action_box->x0);
    }
    break;
  case PROP_Y0:
    {
      g_value_set_uint(value,
		       action_box->y0);
    }
    break;
  case PROP_WIDTH:
    {
      g_value_set_uint(value,
		       action_box->width);
    }
    break;
  case PROP_HEIGHT:
    {
      g_value_set_uint(value,
		       action_box->height);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_action_box_finalize(GObject *gobject)
{
  MonothekActionBox *action_box;

  action_box = (MonothekActionBox *) gobject;

  g_free(action_box->action_identifier);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_action_box_parent_class)->finalize(gobject);
}

/**
 * monothek_action_box_activate:
 * @action_box: the #MonothekActionBox
 * 
 * Activate event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_activate(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[ACTIVATE], 0);
  g_object_unref((GObject *) action_box);
}

/**
 * monothek_action_box_clicked:
 * @action_box: the #MonothekActionBox
 * 
 * Clicked event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_clicked(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[CLICKED], 0);
  g_object_unref((GObject *) action_box);
}

/**
 * monothek_action_box_enter:
 * @action_box: the #MonothekActionBox
 * 
 * Enter event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_enter(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[ENTER], 0);
  g_object_unref((GObject *) action_box);
}

/**
 * monothek_action_box_leave:
 * @action_box: the #MonothekActionBox
 * 
 * Leave event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_leave(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[LEAVE], 0);
  g_object_unref((GObject *) action_box);
}
  
/**
 * monothek_action_box_pressed:
 * @action_box: the #MonothekActionBox
 * 
 * Pressed event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_pressed(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[PRESSED], 0);
  g_object_unref((GObject *) action_box);
}

/**
 * monothek_action_box_released:
 * @action_box: the #MonothekActionBox
 * 
 * Released event.
 * 
 * Since: 1.0.0
 */
void
monothek_action_box_released(MonothekActionBox *action_box)
{
  g_return_if_fail(MONOTHEK_IS_ACTION_BOX(action_box));
  
  g_object_ref((GObject *) action_box);
  g_signal_emit(G_OBJECT(action_box),
		action_box_signals[RELEASED], 0);
  g_object_unref((GObject *) action_box);
}

/**
 * monothek_action_box_new:
 *
 * Creates an #MonothekActionBox
 *
 * Returns: a new #MonothekActionBox
 *
 * Since: 1.0.0
 */
MonothekActionBox*
monothek_action_box_new()
{
  MonothekActionBox *action_box;

  action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						  NULL);
  
  return(action_box);
}
