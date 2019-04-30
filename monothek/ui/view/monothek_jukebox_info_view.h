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

#ifndef __MONOTHEK_JUKEBOX_INFO_VIEW_H__
#define __MONOTHEK_JUKEBOX_INFO_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_INFO_VIEW                (monothek_jukebox_info_view_get_type())
#define MONOTHEK_JUKEBOX_INFO_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_INFO_VIEW, MonothekJukeboxInfoView))
#define MONOTHEK_JUKEBOX_INFO_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_INFO_VIEW, MonothekJukeboxInfoViewClass))
#define MONOTHEK_IS_JUKEBOX_INFO_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_INFO_VIEW))
#define MONOTHEK_IS_JUKEBOX_INFO_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_INFO_VIEW))
#define MONOTHEK_JUKEBOX_INFO_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_INFO_VIEW, MonothekJukeboxInfoViewClass))

typedef struct _MonothekJukeboxInfoView MonothekJukeboxInfoView;
typedef struct _MonothekJukeboxInfoViewClass MonothekJukeboxInfoViewClass;

struct _MonothekJukeboxInfoView
{
  MonothekView monothek_view;

  // ok
  gdouble ok_box_line_width;

  gdouble ok_box_x0;
  gdouble ok_box_y0;

  gdouble ok_box_width;
  gdouble ok_box_height;
};

struct _MonothekJukeboxInfoViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_info_view_get_type(void);

MonothekJukeboxInfoView* monothek_jukebox_info_view_new();

#endif /*__MONOTHEK_JUKEBOX_INFO_VIEW_H__*/
