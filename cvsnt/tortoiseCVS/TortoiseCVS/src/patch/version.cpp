/* Print the version number.  */

/* $Id: version.cpp,v 1.1 2012/03/04 01:06:58 aliot Exp $ */

#define XTERN extern
#include <common.h>
#undef XTERN
#define XTERN
#include <patchlevel.h>
#include <version.h>

static char const copyright_string[] = "\
Copyright 1984-1988 Larry Wall\n\
Copyright 1989-1999 Free Software Foundation, Inc.";

static char const free_software_msgid[] = "\
This program comes with NO WARRANTY, to the extent permitted by law.\n\
You may redistribute copies of this program\n\
under the terms of the GNU General Public License.\n\
For more information about these matters, see the file named COPYING.";

static char const authorship_msgid[] = "\
written by Larry Wall and Paul Eggert";

void
version (void)
{
  printf ("%s %s\n%s\n\n%s\n\n%s\n", program_name, PATCH_VERSION,
	  copyright_string, free_software_msgid, authorship_msgid);
}
