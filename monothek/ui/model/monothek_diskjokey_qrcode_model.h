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

#ifndef __MONOTHEK_DISKJOKEY_QRCODE_MODEL_H__
#define __MONOTHEK_DISKJOKEY_QRCODE_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL                (monothek_diskjokey_qrcode_model_get_type())
#define MONOTHEK_DISKJOKEY_QRCODE_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL, MonothekDiskjokeyQrcodeModel))
#define MONOTHEK_DISKJOKEY_QRCODE_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL, MonothekDiskjokeyQrcodeModelClass))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL))
#define MONOTHEK_DISKJOKEY_QRCODE_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_MODEL, MonothekDiskjokeyQrcodeModelClass))

#define MONOTHEK_DISKJOKEY_QRCODE_MODEL_DEFAULT_DURATION_SEC (120)

typedef struct _MonothekDiskjokeyQrcodeModel MonothekDiskjokeyQrcodeModel;
typedef struct _MonothekDiskjokeyQrcodeModelClass MonothekDiskjokeyQrcodeModelClass;

struct _MonothekDiskjokeyQrcodeModel
{
  GObject gobject;

  gboolean quit_active;
  
  struct timespec *duration;
};

struct _MonothekDiskjokeyQrcodeModelClass
{
  GObjectClass gobject;
};

GType monothek_diskjokey_qrcode_model_get_type(void);

MonothekDiskjokeyQrcodeModel* monothek_diskjokey_qrcode_model_new();

#endif /*__MONOTHEK_DISKJOKEY_QRCODE_MODEL_H__*/
