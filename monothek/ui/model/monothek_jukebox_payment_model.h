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

#ifndef __MONOTHEK_JUKEBOX_PAYMENT_MODEL_H__
#define __MONOTHEK_JUKEBOX_PAYMENT_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL                (monothek_jukebox_payment_model_get_type())
#define MONOTHEK_JUKEBOX_PAYMENT_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL, MonothekJukeboxPaymentModel))
#define MONOTHEK_JUKEBOX_PAYMENT_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL, MonothekJukeboxPaymentModelClass))
#define MONOTHEK_IS_JUKEBOX_PAYMENT_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL))
#define MONOTHEK_IS_JUKEBOX_PAYMENT_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL))
#define MONOTHEK_JUKEBOX_PAYMENT_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_MODEL, MonothekJukeboxPaymentModelClass))

typedef struct _MonothekJukeboxPaymentModel MonothekJukeboxPaymentModel;
typedef struct _MonothekJukeboxPaymentModelClass MonothekJukeboxPaymentModelClass;

struct _MonothekJukeboxPaymentModel
{
  GObject gobject;
};

struct _MonothekJukeboxPaymentModelClass
{
  GObjectClass gobject;
};

GType monothek_jukebox_payment_model_get_type(void);

MonothekJukeboxPaymentModel* monothek_jukebox_payment_model_new();

#endif /*__MONOTHEK_JUKEBOX_PAYMENT_MODEL_H__*/
