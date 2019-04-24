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

#ifndef __MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER_H__
#define __MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#define MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER                (monothek_diskjokey_payment_controller_get_type())
#define MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER, MonothekDiskjokeyPaymentController))
#define MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER, MonothekDiskjokeyPaymentControllerClass))
#define MONOTHEK_IS_DISKJOKEY_PAYMENT_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER))
#define MONOTHEK_IS_DISKJOKEY_PAYMENT_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER))
#define MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_PAYMENT_CONTROLLER, MonothekDiskjokeyPaymentControllerClass))

typedef struct _MonothekDiskjokeyPaymentController MonothekDiskjokeyPaymentController;
typedef struct _MonothekDiskjokeyPaymentControllerClass MonothekDiskjokeyPaymentControllerClass;

struct _MonothekDiskjokeyPaymentController
{
  MonothekController controller;
};

struct _MonothekDiskjokeyPaymentControllerClass
{
  MonothekControllerClass controller;

  void (*transaction_completed)(MonothekDiskjokeyPaymentController *diskjokey_payment_controller);
};

GType monothek_diskjokey_payment_controller_get_type(void);

void monothek_diskjokey_payment_controller_transaction_completed(MonothekDiskjokeyPaymentController *diskjokey_payment_controller);

MonothekDiskjokeyPaymentController* monothek_diskjokey_payment_controller_new();

#endif /*__MONOTHEK_DISKJOKEY_PAYMENT_CONTROLLER_H__*/
