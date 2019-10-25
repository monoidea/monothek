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

#ifndef __MONOTHEK_DISKJOKEY_QRCODE_VIEW_H__
#define __MONOTHEK_DISKJOKEY_QRCODE_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW                (monothek_diskjokey_qrcode_view_get_type())
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW, MonothekDiskjokeyQrcodeView))
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW, MonothekDiskjokeyQrcodeViewClass))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW))
#define MONOTHEK_IS_DISKJOKEY_QRCODE_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW))
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_QRCODE_VIEW, MonothekDiskjokeyQrcodeViewClass))

#if MONOTHEK_DEVEL_MODE
#if defined(MONOTHEK_W32API)
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_DEFAULT_QRCODE_FILENAME "share\\monothek\\images\\monothek-qrcode.png"
#else
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_DEFAULT_QRCODE_FILENAME SRCDIR "/monothek.share/monothek/images/monothek-qrcode.png"
#endif
#else
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_DEFAULT_QRCODE_FILENAME "/usr/share/monothek/images/monothek-qrcode.png"
#endif

#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_PADDING_TOP (12.9)
#define MONOTHEK_DISKJOKEY_QRCODE_VIEW_PADDING_LEFT (20.0)

typedef struct _MonothekDiskjokeyQrcodeView MonothekDiskjokeyQrcodeView;
typedef struct _MonothekDiskjokeyQrcodeViewClass MonothekDiskjokeyQrcodeViewClass;

struct _MonothekDiskjokeyQrcodeView
{
  MonothekView monothek_view;

  //quit
  gdouble quit_box_line_width;

  gdouble quit_box_x0;
  gdouble quit_box_y0;

  gdouble quit_box_width;
  gdouble quit_box_height;
};

struct _MonothekDiskjokeyQrcodeViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_diskjokey_qrcode_view_get_type(void);

MonothekDiskjokeyQrcodeView* monothek_diskjokey_qrcode_view_new();

#endif /*__MONOTHEK_DISKJOKEY_QRCODE_VIEW_H__*/
