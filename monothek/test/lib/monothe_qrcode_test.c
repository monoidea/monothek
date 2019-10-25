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

#include <glib.h>
#include <glib-object.h>

#include <monothek/lib/monothek_qrcode.h>

#define MONOTHEK_QRCODE_TEST_PNG_FILENAME "monothek-qrcode.png"

int
main(int argc, char **argv)
{
  MonothekQrcode *qrcode;

  qrcode = monothek_qrcode_new();
  monothek_qrcode_encode(qrcode);

  monothek_qrcode_export_png(qrcode,
			     MONOTHEK_QRCODE_TEST_PNG_FILENAME);
  
  return(0);
}
