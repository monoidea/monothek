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

#ifndef __MONOTHEK_START_VIEW_H__
#define __MONOTHEK_START_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/monothek_view.h>

#define MONOTHEK_TYPE_START_VIEW                (monothek_start_view_get_type())
#define MONOTHEK_START_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_START_VIEW, MonothekStartView))
#define MONOTHEK_START_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_START_VIEW, MonothekStartViewClass))
#define MONOTHEK_IS_START_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_START_VIEW))
#define MONOTHEK_IS_START_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_START_VIEW))
#define MONOTHEK_START_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_START_VIEW, MonothekStartViewClass))

typedef struct _MonothekStartView MonothekStartView;
typedef struct _MonothekStartViewClass MonothekStartViewClass;

struct _MonothekStartView
{
  MonothekView monothek_view;
};

struct _MonothekStartViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_start_view_get_type(void);

MonothekStartView* monothek_start_view_new();

#endif /*__MONOTHEK_START_VIEW_H__*/
