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

#ifndef __MONOTHEK_WINDOW_H__
#define __MONOTHEK_WINDOW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_WINDOW                (monothek_window_get_type())
#define MONOTHEK_WINDOW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_WINDOW, MonothekWindow))
#define MONOTHEK_WINDOW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_WINDOW, MonothekWindowClass))
#define MONOTHEK_IS_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_WINDOW))
#define MONOTHEK_IS_WINDOW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_WINDOW))
#define MONOTHEK_WINDOW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_WINDOW, MonothekWindowClass))

typedef struct _MonothekWindow MonothekWindow;
typedef struct _MonothekWindowClass MonothekWindowClass;

/**
 * MonothekWindowFlags:
 * @MONOTHEK_WINDOW_ADDED_TO_REGISTRY: the window was added to registry, see #MonothekConnectable::add_to_registry()
 * @MONOTHEK_WINDOW_CONNECTED: the window was connected by #MonothekConnectable::connect()
 */
typedef enum{
  MONOTHEK_WINDOW_ADDED_TO_REGISTRY           = 1,
  MONOTHEK_WINDOW_CONNECTED                   = 1 <<  1,
}MonothekWindowFlags;

struct _MonothekWindow
{
  GtkWindow window;

  guint flags;

  GList *model;
  
  GList *controller;
  
  GType current_view_type;
  GtkBox *view;
};

struct _MonothekWindowClass
{
  GtkWindowClass window;

  void (*change_view)(MonothekWindow *window,
		      GType view_type, GType view_type_old);
};

GType monothek_window_get_type(void);

gboolean monothek_window_test_flags(MonothekWindow *window, guint flags);
void monothek_window_set_flags(MonothekWindow *window, guint flags);
void monothek_window_unset_flags(MonothekWindow *window, guint flags);

void monothek_window_change_view(MonothekWindow *window,
				 GType view_type, GType view_type_old);

MonothekWindow* monothek_window_new(GObject *application_context);

#endif /*__MONOTHEK_WINDOW_H__*/
