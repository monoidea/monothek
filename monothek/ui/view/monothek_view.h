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

#ifndef __MONOTHEK_VIEW_H__
#define __MONOTHEK_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_VIEW                (monothek_view_get_type())
#define MONOTHEK_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_VIEW, MonothekView))
#define MONOTHEK_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_VIEW, MonothekViewClass))
#define MONOTHEK_IS_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_VIEW))
#define MONOTHEK_IS_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_VIEW))
#define MONOTHEK_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_VIEW, MonothekViewClass))

#define MONOTHEK_VIEW_DEFAULT_JUKEBOX_GC ((guint32) 0xffff)
#define MONOTHEK_VIEW_DEFAULT_DISKJOKEY_GC ((guint32) 0xff00ff)

#define MONOTHEK_VIEW_DEFAULT_FONT "Eurostile"

typedef struct _MonothekView MonothekView;
typedef struct _MonothekViewClass MonothekViewClass;

typedef enum{
  MONOTHEK_VIEW_ADDED_TO_REGISTRY           = 1,
  MONOTHEK_VIEW_CONNECTED                   = 1 <<  1,
}MonothekViewFlags;

typedef enum{
  MONOTHEK_VIEW_BUTTON_1_PRESSED     = 1,
}MonothekViewButtonState;

struct _MonothekView
{
  GtkWidget widget;

  guint flags;

  guint button_state;

  guint32 jukebox_gc;
  guint32 diskjokey_gc;

  gchar *font;

  GObject *model;
};

struct _MonothekViewClass
{
  GtkWidgetClass widget;

  void (*draw)(MonothekView *view);
};

GType monothek_view_get_type(void);

void monothek_view_draw(MonothekView *view);

MonothekView* monothek_view_new();

#endif /*__MONOTHEK_VIEW_H__*/
