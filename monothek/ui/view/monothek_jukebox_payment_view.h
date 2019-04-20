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

#ifndef __MONOTHEK_JUKEBOX_PAYMENT_VIEW_H__
#define __MONOTHEK_JUKEBOX_PAYMENT_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW                (monothek_jukebox_payment_view_get_type())
#define MONOTHEK_JUKEBOX_PAYMENT_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW, MonothekJukeboxPaymentView))
#define MONOTHEK_JUKEBOX_PAYMENT_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW, MonothekJukeboxPaymentViewClass))
#define MONOTHEK_IS_JUKEBOX_PAYMENT_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW))
#define MONOTHEK_IS_JUKEBOX_PAYMENT_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW))
#define MONOTHEK_JUKEBOX_PAYMENT_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW, MonothekJukeboxPaymentViewClass))

#define MONOTHEK_JUKEBOX_PAYMENT_VIEW_CARDS_PNG "/usr/share/monothek/images/jukebox-payment-cards.png"
#define MONOTHEK_JUKEBOX_PAYMENT_VIEW_NFC_PNG "/usr/share/monothek/images/jukebox-payment-nfc.png"

typedef struct _MonothekJukeboxPaymentView MonothekJukeboxPaymentView;
typedef struct _MonothekJukeboxPaymentViewClass MonothekJukeboxPaymentViewClass;

struct _MonothekJukeboxPaymentView
{
  MonothekView monothek_view;

  //pay
  gdouble pay_box_line_width;

  gdouble pay_box_x0;
  gdouble pay_box_y0;

  gdouble pay_box_width;
  gdouble pay_box_height;

  //arrow
  gdouble arrow_box_x0;
  gdouble arrow_box_y0;

  gdouble arrow_box_width;
  gdouble arrow_box_height;

  gdouble arrow_top_x0;
  gdouble arrow_top_y0;

  gdouble arrow_top_width;
  gdouble arrow_top_height;

  gchar *cards_png_filename;
  gchar *nfc_png_filename;
};

struct _MonothekJukeboxPaymentViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_payment_view_get_type(void);

MonothekJukeboxPaymentView* monothek_jukebox_payment_view_new();

#endif /*__MONOTHEK_JUKEBOX_PAYMENT_VIEW_H__*/
