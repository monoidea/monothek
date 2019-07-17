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

#ifndef __MONOTHEK_OUTAGE_CONTROLLER_H__
#define __MONOTHEK_OUTAGE_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#include <time.h>

#define MONOTHEK_TYPE_OUTAGE_CONTROLLER                (monothek_outage_controller_get_type())
#define MONOTHEK_OUTAGE_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_OUTAGE_CONTROLLER, MonothekOutageController))
#define MONOTHEK_OUTAGE_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_OUTAGE_CONTROLLER, MonothekOutageControllerClass))
#define MONOTHEK_IS_OUTAGE_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_OUTAGE_CONTROLLER))
#define MONOTHEK_IS_OUTAGE_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_OUTAGE_CONTROLLER))
#define MONOTHEK_OUTAGE_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_OUTAGE_CONTROLLER, MonothekOutageControllerClass))

typedef struct _MonothekOutageController MonothekOutageController;
typedef struct _MonothekOutageControllerClass MonothekOutageControllerClass;

struct _MonothekOutageController
{
  MonothekController controller;
};

struct _MonothekOutageControllerClass
{
  MonothekControllerClass controller;
};

GType monothek_outage_controller_get_type(void);

MonothekOutageController* monothek_outage_controller_new();

#endif /*__MONOTHEK_OUTAGE_CONTROLLER_H__*/
