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

#ifndef __MONOTHEK_START_CONTROLLER_H__
#define __MONOTHEK_START_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_START_CONTROLLER                (monothek_start_controller_get_type())
#define MONOTHEK_START_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_START_CONTROLLER, MonothekStartController))
#define MONOTHEK_START_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_START_CONTROLLER, MonothekStartControllerClass))
#define MONOTHEK_IS_START_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_START_CONTROLLER))
#define MONOTHEK_IS_START_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_START_CONTROLLER))
#define MONOTHEK_START_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_START_CONTROLLER, MonothekStartControllerClass))

typedef struct _MonothekStartController MonothekStartController;
typedef struct _MonothekStartControllerClass MonothekStartControllerClass;

struct _MonothekStartController
{
  GObject gobject;
};

struct _MonothekStartControllerClass
{
  GObjectClass gobject;

  void (*launch_jukebox)(MonothekStartController *start_controller);
  void (*launch_diskjokey)(MonothekStartController *start_controller);
};

GType monothek_start_controller_get_type(void);

void monothek_start_controller_launch_jukebox(MonothekStartController *start_controller);
void monothek_start_controller_launch_diskjokey(MonothekStartController *start_controller);

MonothekStartController* monothek_start_controller_new();

#endif /*__MONOTHEK_START_CONTROLLER_H__*/
