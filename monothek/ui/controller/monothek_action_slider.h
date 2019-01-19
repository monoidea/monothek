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

#ifndef __MONOTHEK_ACTION_SLIDER_H__
#define __MONOTHEK_ACTION_SLIDER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_ACTION_SLIDER                (monothek_action_slider_get_type())
#define MONOTHEK_ACTION_SLIDER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_ACTION_SLIDER, MonothekActionSlider))
#define MONOTHEK_ACTION_SLIDER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_ACTION_SLIDER, MonothekActionSliderClass))
#define MONOTHEK_IS_ACTION_SLIDER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_ACTION_SLIDER))
#define MONOTHEK_IS_ACTION_SLIDER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_ACTION_SLIDER))
#define MONOTHEK_ACTION_SLIDER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_ACTION_SLIDER, MonothekActionSliderClass))

typedef struct _MonothekActionSlider MonothekActionSlider;
typedef struct _MonothekActionSliderClass MonothekActionSliderClass;

typedef enum{
  MONOTHEK_ACTION_SLIDER_BUTTON_1            = 1,
}MonothekActionSliderButtonMask;

struct _MonothekActionSlider
{
  GObject gobject;

  guint button_mask;
  guint orientation;
  
  GtkAdjustment *adjustment;
  
  gchar *action_identifier;
  
  guint x0;
  guint y0;

  guint width;
  guint height;
};

struct _MonothekActionSliderClass
{
  GObjectClass gobject;

  gboolean (*change_value)(MonothekActionSlider *action_slider,
			   gdouble new_value);
  void (*move_slider)(MonothekActionSlider *action_slider);

  void (*value_changed)(MonothekActionSlider *action_slider);
};

GType monothek_action_slider_get_type(void);

gboolean monothek_action_slider_change_value(MonothekActionSlider *action_slider,
					     gdouble new_value);  
void monothek_action_slider_move_slider(MonothekActionSlider *action_slider);

void monothek_action_slider_value_changed(MonothekActionSlider *action_slider);

MonothekActionSlider* monothek_action_slider_new();

#endif /*__MONOTHEK_ACTION_SLIDER_H__*/
