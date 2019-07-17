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

#ifndef __MONOTHEK_SCREENSAVER_MODEL_H__
#define __MONOTHEK_SCREENSAVER_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_SCREENSAVER_MODEL                (monothek_screensaver_model_get_type())
#define MONOTHEK_SCREENSAVER_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_SCREENSAVER_MODEL, MonothekScreensaverModel))
#define MONOTHEK_SCREENSAVER_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_SCREENSAVER_MODEL, MonothekScreensaverModelClass))
#define MONOTHEK_IS_SCREENSAVER_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_SCREENSAVER_MODEL))
#define MONOTHEK_IS_SCREENSAVER_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_SCREENSAVER_MODEL))
#define MONOTHEK_SCREENSAVER_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_SCREENSAVER_MODEL, MonothekScreensaverModelClass))

typedef struct _MonothekScreensaverModel MonothekScreensaverModel;
typedef struct _MonothekScreensaverModelClass MonothekScreensaverModelClass;

struct _MonothekScreensaverModel
{
  GObject gobject;
};

struct _MonothekScreensaverModelClass
{
  GObjectClass gobject;
};

GType monothek_screensaver_model_get_type(void);

MonothekScreensaverModel* monothek_screensaver_model_new();

#endif /*__MONOTHEK_SCREENSAVER_MODEL_H__*/
