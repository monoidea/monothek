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

#ifndef __MONOTHEK_DISKJOKEY_PAYMENT_MODEL_H__
#define __MONOTHEK_DISKJOKEY_PAYMENT_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL                (monothek_diskjokey_payment_model_get_type())
#define MONOTHEK_DISKJOKEY_PAYMENT_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL, MonothekDiskjokeyPaymentModel))
#define MONOTHEK_DISKJOKEY_PAYMENT_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL, MonothekDiskjokeyPaymentModelClass))
#define MONOTHEK_IS_DISKJOKEY_PAYMENT_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL))
#define MONOTHEK_IS_DISKJOKEY_PAYMENT_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL))
#define MONOTHEK_DISKJOKEY_PAYMENT_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_MODEL, MonothekDiskjokeyPaymentModelClass))

typedef struct _MonothekDiskjokeyPaymentModel MonothekDiskjokeyPaymentModel;
typedef struct _MonothekDiskjokeyPaymentModelClass MonothekDiskjokeyPaymentModelClass;

struct _MonothekDiskjokeyPaymentModel
{
  GObject gobject;
};

struct _MonothekDiskjokeyPaymentModelClass
{
  GObjectClass gobject;
};

GType monothek_diskjokey_payment_model_get_type(void);

MonothekDiskjokeyPaymentModel* monothek_diskjokey_payment_model_new();

#endif /*__MONOTHEK_DISKJOKEY_PAYMENT_MODEL_H__*/
