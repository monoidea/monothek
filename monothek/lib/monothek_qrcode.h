/* Monothek - monoidea's monothek service
 * Copyright (C) 2019 Joël Krähemann
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

#ifndef __MONOTHEK_QRCODE_H__
#define __MONOTHEK_QRCODE_H__

#include <glib.h>
#include <glib-object.h>

#include <qrencode.h>

#define MONOTHEK_TYPE_QRCODE                (monothek_qrcode_get_type())
#define MONOTHEK_QRCODE(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_QRCODE, MonothekQrcode))
#define MONOTHEK_QRCODE_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_QRCODE, MonothekQrcodeClass))
#define MONOTHEK_IS_QRCODE(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_QRCODE))
#define MONOTHEK_IS_QRCODE_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_QRCODE))
#define MONOTHEK_QRCODE_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_QRCODE, MonothekQrcodeClass))

#define MONOTHEK_QRCODE_DEFAULT_URL "http://monothek.ch"
#define MONOTHEK_QRCODE_DEFAULT_FILENAME SRCDIR "/monothek.share/monothek/qrcode/qrcode.xpm"

#define MONOTHEK_QRCODE_DEFAULT_SVG_WIDTH (320)
#define MONOTHEK_QRCODE_DEFAULT_SVG_HEIGHT (320)

#define MONOTHEK_QRCODE_DEFAULT_PNG_WIDTH (320)
#define MONOTHEK_QRCODE_DEFAULT_PNG_HEIGHT (320)

typedef struct _MonothekQrcode MonothekQrcode;
typedef struct _MonothekQrcodeClass MonothekQrcodeClass;

struct _MonothekQrcode
{
  GObject gobject;
  
  guint flags;
  
  gchar *url;
  gchar *filename;
  
  QRcode *qrcode;
};

struct _MonothekQrcodeClass
{
  GObjectClass gobject;

  void (*encode)(MonothekQrcode *qrcode);
  
  void (*write)(MonothekQrcode *qrcode);
};

GType monothek_qrcode_get_type();

void monothek_qrcode_encode(MonothekQrcode *qrcode);

void monothek_qrcode_write(MonothekQrcode *qrcode);

void monothek_qrcode_export_svg(MonothekQrcode *qrcode,
				gchar *svg_filename);
void monothek_qrcode_export_png(MonothekQrcode *qrcode,
				gchar *png_filename);

MonothekQrcode* monothek_qrcode_new();

#endif /*__MONOTHEK_QRCODE_H__*/
