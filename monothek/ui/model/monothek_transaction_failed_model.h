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

#ifndef __MONOTHEK_TRANSACTION_FAILED_MODEL_H__
#define __MONOTHEK_TRANSACTION_FAILED_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL                (monothek_transaction_failed_model_get_type())
#define MONOTHEK_TRANSACTION_FAILED_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL, MonothekTransactionFailedModel))
#define MONOTHEK_TRANSACTION_FAILED_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL, MonothekTransactionFailedModelClass))
#define MONOTHEK_IS_TRANSACTION_FAILED_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL))
#define MONOTHEK_IS_TRANSACTION_FAILED_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL))
#define MONOTHEK_TRANSACTION_FAILED_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_TRANSACTION_FAILED_MODEL, MonothekTransactionFailedModelClass))

#define MONOTHEK_TRANSACTION_FAILED_MODEL_DEFAULT_DURATION_SEC (10)

typedef struct _MonothekTransactionFailedModel MonothekTransactionFailedModel;
typedef struct _MonothekTransactionFailedModelClass MonothekTransactionFailedModelClass;

struct _MonothekTransactionFailedModel
{
  GObject gobject;
  
  struct timespec *duration;
};

struct _MonothekTransactionFailedModelClass
{
  GObjectClass gobject;
};

GType monothek_transaction_failed_model_get_type(void);

MonothekTransactionFailedModel* monothek_transaction_failed_model_new();

#endif /*__MONOTHEK_TRANSACTION_FAILED_MODEL_H__*/
