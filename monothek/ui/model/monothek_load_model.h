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

#ifndef __MONOTHEK_LOAD_MODEL_H__
#define __MONOTHEK_LOAD_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_LOAD_MODEL                (monothek_load_model_get_type())
#define MONOTHEK_LOAD_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_LOAD_MODEL, MonothekLoadModel))
#define MONOTHEK_LOAD_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_LOAD_MODEL, MonothekLoadModelClass))
#define MONOTHEK_IS_LOAD_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_LOAD_MODEL))
#define MONOTHEK_IS_LOAD_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_LOAD_MODEL))
#define MONOTHEK_LOAD_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_LOAD_MODEL, MonothekLoadModelClass))

typedef struct _MonothekLoadModel MonothekLoadModel;
typedef struct _MonothekLoadModelClass MonothekLoadModelClass;

struct _MonothekLoadModel
{
  GObject gobject;
};

struct _MonothekLoadModelClass
{
  GObjectClass gobject;
};

GType monothek_load_model_get_type(void);

MonothekLoadModel* monothek_load_model_new();

#endif /*__MONOTHEK_LOAD_MODEL_H__*/
