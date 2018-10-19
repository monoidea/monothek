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

#ifndef __MONOTHEK_JUKEBOX_END_VIEW_H__
#define __MONOTHEK_JUKEBOX_END_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_END_VIEW                (monothek_jukebox_end_view_get_type())
#define MONOTHEK_JUKEBOX_END_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_END_VIEW, MonothekJukeboxEndView))
#define MONOTHEK_JUKEBOX_END_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_END_VIEW, MonothekJukeboxEndViewClass))
#define MONOTHEK_IS_JUKEBOX_END_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_END_VIEW))
#define MONOTHEK_IS_JUKEBOX_END_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_END_VIEW))
#define MONOTHEK_JUKEBOX_END_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_END_VIEW, MonothekJukeboxEndViewClass))

typedef struct _MonothekJukeboxEndView MonothekJukeboxEndView;
typedef struct _MonothekJukeboxEndViewClass MonothekJukeboxEndViewClass;

struct _MonothekJukeboxEndView
{
  MonothekView monothek_view;

  //continue
  gdouble continue_box_line_width;

  gdouble continue_box_x0;
  gdouble continue_box_y0;

  gdouble continue_box_width;
  gdouble continue_box_height;

  //quit
  gdouble quit_box_line_width;

  gdouble quit_box_x0;
  gdouble quit_box_y0;

  gdouble quit_box_width;
  gdouble quit_box_height;
};

struct _MonothekJukeboxEndViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_end_view_get_type(void);

MonothekJukeboxEndView* monothek_jukebox_end_view_new();

#endif /*__MONOTHEK_JUKEBOX_END_VIEW_H__*/
