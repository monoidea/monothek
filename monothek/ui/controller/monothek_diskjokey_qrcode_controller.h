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

#ifndef __MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER_H__
#define __MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#define MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER                (monothek_diskjokey_qrcode_controller_get_type())
#define MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER, MonothekDiskjokeyQrcodeController))
#define MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER, MonothekDiskjokeyQrcodeControllerClass))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER))
#define MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_CONTROLLER, MonothekDiskjokeyQrcodeControllerClass))

typedef struct _MonothekDiskjokeyQrcodeController MonothekDiskjokeyQrcodeController;
typedef struct _MonothekDiskjokeyQrcodeControllerClass MonothekDiskjokeyQrcodeControllerClass;

struct _MonothekDiskjokeyQrcodeController
{
  MonothekController controller;
};

struct _MonothekDiskjokeyQrcodeControllerClass
{
  MonothekControllerClass controller;

  void (*timeout)(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);
};

GType monothek_diskjokey_qrcode_controller_get_type(void);

void monothek_diskjokey_qrcode_controller_timeout(MonothekDiskjokeyQrcodeController *diskjokey_qrcode_controller);

MonothekDiskjokeyQrcodeController* monothek_diskjokey_qrcode_controller_new();

#endif /*__MONOTHEK_DISKJOKEY_QRCODE_CONTROLLER_H__*/
