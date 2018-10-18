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

#ifndef __MONOTHEK_JUKEBOX_NO_TEST_VIEW_H__
#define __MONOTHEK_JUKEBOX_NO_TEST_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW                (monothek_jukebox_no_test_view_get_type())
#define MONOTHEK_JUKEBOX_NO_TEST_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW, MonothekJukeboxNoTestView))
#define MONOTHEK_JUKEBOX_NO_TEST_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW, MonothekJukeboxNoTestViewClass))
#define MONOTHEK_IS_JUKEBOX_NO_TEST_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW))
#define MONOTHEK_IS_JUKEBOX_NO_TEST_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW))
#define MONOTHEK_JUKEBOX_NO_TEST_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_NO_TEST_VIEW, MonothekJukeboxNoTestViewClass))

typedef struct _MonothekJukeboxNoTestView MonothekJukeboxNoTestView;
typedef struct _MonothekJukeboxNoTestViewClass MonothekJukeboxNoTestViewClass;

struct _MonothekJukeboxNoTestView
{
  MonothekView monothek_view;

  //play
  gdouble play_box_line_width;

  gdouble play_box_x0;
  gdouble play_box_y0;

  gdouble play_box_width;
  gdouble play_box_height;

  //cancel
  gdouble cancel_box_line_width;

  gdouble cancel_box_x0;
  gdouble cancel_box_y0;

  gdouble cancel_box_width;
  gdouble cancel_box_height;
};

struct _MonothekJukeboxNoTestViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_no_test_view_get_type(void);

MonothekJukeboxNoTestView* monothek_jukebox_no_test_view_new();

#endif /*__MONOTHEK_JUKEBOX_NO_TEST_VIEW_H__*/
