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

#ifndef __MONOTHEK_BANNER_VIEW_H__
#define __MONOTHEK_BANNER_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_BANNER_VIEW                (monothek_banner_view_get_type())
#define MONOTHEK_BANNER_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_BANNER_VIEW, MonothekBannerView))
#define MONOTHEK_BANNER_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_BANNER_VIEW, MonothekBannerViewClass))
#define MONOTHEK_IS_BANNER_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_BANNER_VIEW))
#define MONOTHEK_IS_BANNER_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_BANNER_VIEW))
#define MONOTHEK_BANNER_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_BANNER_VIEW, MonothekBannerViewClass))

typedef struct _MonothekBannerView MonothekBannerView;
typedef struct _MonothekBannerViewClass MonothekBannerViewClass;

struct _MonothekBannerView
{
  MonothekView monothek_view;
};

struct _MonothekBannerViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_banner_view_get_type(void);

MonothekBannerView* monothek_banner_view_new();

#endif /*__MONOTHEK_BANNER_VIEW_H__*/
