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

#ifndef __MONOTHEK_CONTROLLER_H__
#define __MONOTHEK_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_action_box.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_CONTROLLER                (monothek_controller_get_type())
#define MONOTHEK_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_CONTROLLER, MonothekController))
#define MONOTHEK_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_CONTROLLER, MonothekControllerClass))
#define MONOTHEK_IS_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_CONTROLLER))
#define MONOTHEK_IS_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_CONTROLLER))
#define MONOTHEK_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_CONTROLLER, MonothekControllerClass))

typedef struct _MonothekController MonothekController;
typedef struct _MonothekControllerClass MonothekControllerClass;

/**
 * MonothekControllerFlags:
 * @MONOTHEK_CONTROLLER_ADDED_TO_REGISTRY: the controller was added to registry, see #MonothekConnectable::add_to_registry()
 * @MONOTHEK_CONTROLLER_CONNECTED: the controller was connected by #MonothekConnectable::connect()
 */
typedef enum{
  MONOTHEK_CONTROLLER_ADDED_TO_REGISTRY           = 1,
  MONOTHEK_CONTROLLER_CONNECTED                   = 1 <<  1,
}MonothekControllerFlags;

struct _MonothekController
{
  GObject gobject;

  guint flags;

  GObject *model;
  
  MonothekView *view;

  GList *action_box;
};

struct _MonothekControllerClass
{
  GObjectClass gobject;
};

GType monothek_controller_get_type(void);

gboolean monothek_controller_test_flags(MonothekController *controller, guint flags);
void monothek_controller_set_flags(MonothekController *controller, guint flags);
void monothek_controller_unset_flags(MonothekController *controller, guint flags);

void monothek_controller_add_action_box(MonothekController *controller,
					MonothekActionBox *action_box);
void monothek_controller_remove_action_box(MonothekController *controller,
					   MonothekActionBox *action_box);

GList* monothek_controller_find_view_type(GList *list,
					  GType view_type);

MonothekController* monothek_controller_new();

#endif /*__MONOTHEK_CONTROLLER_H__*/
