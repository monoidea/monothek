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

#ifndef __MONOTHEK_ACTION_BOX_H__
#define __MONOTHEK_ACTION_BOX_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_ACTION_BOX                (monothek_action_box_get_type())
#define MONOTHEK_ACTION_BOX(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_ACTION_BOX, MonothekActionBox))
#define MONOTHEK_ACTION_BOX_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_ACTION_BOX, MonothekActionBoxClass))
#define MONOTHEK_IS_ACTION_BOX(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_ACTION_BOX))
#define MONOTHEK_IS_ACTION_BOX_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_ACTION_BOX))
#define MONOTHEK_ACTION_BOX_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_ACTION_BOX, MonothekActionBoxClass))

typedef struct _MonothekActionBox MonothekActionBox;
typedef struct _MonothekActionBoxClass MonothekActionBoxClass;

struct _MonothekActionBox
{
  GObject gobject;

  gchar *action_identifier;
  
  guint x0;
  guint y0;

  guint width;
  guint height;
};

struct _MonothekActionBoxClass
{
  GObjectClass gobject;

  void (*activate)(MonothekActionBox *action_box);
  void (*clicked)(MonothekActionBox *action_box);
  
  void (*enter)(MonothekActionBox *action_box);
  void (*leave)(MonothekActionBox *action_box);
  
  void (*pressed)(MonothekActionBox *action_box);
  void (*released)(MonothekActionBox *action_box);
};

GType monothek_action_box_get_type(void);

void monothek_action_box_activate(MonothekActionBox *action_box);
void monothek_action_box_clicked(MonothekActionBox *action_box);
  
void monothek_action_box_enter(MonothekActionBox *action_box);
void monothek_action_box_leave(MonothekActionBox *action_box);
  
void monothek_action_box_pressed(MonothekActionBox *action_box);
void monothek_action_box_released(MonothekActionBox *action_box);

MonothekActionBox* monothek_action_box_new();

#endif /*__MONOTHEK_ACTION_BOX_H__*/
