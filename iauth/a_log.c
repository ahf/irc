/************************************************************************
 *   IRC - Internet Relay Chat, iauth/a_log.c
 *   Copyright (C) 1998 Christophe Kalt
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef lint
static  char rcsid[] = "@(#)$Id$";
#endif

#include "os.h"
#include "a_defines.h"
#define A_LOG_C
#include "a_externs.h"
#undef A_LOG_C

static FILE	*debug, *authlog;

void
init_log()
{
#if defined(USE_SYSLOG)
	openlog("iauth", LOG_PID|LOG_NDELAY, LOG_FACILITY);
#endif
#if defined(IAUTH_DEBUG)
	debug = fopen(OPATH, "w");
# if defined(USE_SYSLOG)
	if (!debug)
		syslog(LOG_ERR, "Failed to open \"%s\" for writing", OPATH);
# endif
#endif /* IAUTH_DEBUG */
	authlog = fopen(FNAME_AUTHLOG, "a");
#if defined(USE_SYSLOG)
	if (!authlog)
		syslog(LOG_NOTICE, "Failed to open \"%s\" for writing",
		       FNAME_AUTHLOG);
#endif
}

#if ! USE_STDARG
void
sendto_log(flags, slflag, pattern, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
int flags, slflag;
char    *pattern, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10;
#else
void
vsendto_log(int flags, int slflag, char *pattern, va_list va)
#endif
{
	char	logbuf[4096];

	logbuf[0] = '>';
#if ! USE_STDARG
	sprintf(logbuf+1, pattern, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
#else
	vsprintf(logbuf+1, pattern, va);
#endif

#if defined(USE_SYSLOG)
	if (slflags)
		syslog(slflags, logbuf+1);
#endif

	strcat(logbuf, "\n");

#if defined(IAUTH_DEBUG)
	if ((flags & ALOG_DALL) && (flags & debuglevel) && debug)
	    {
		fprintf(debug, logbuf+1);
		fflush(debug);
	    }
#endif
	if (authlog && (flags & ALOG_FLOG))
	    {
		fprintf(authlog, logbuf+1);
		fflush(authlog);
	    }
	if (flags & ALOG_IRCD)
	    {
		write(0, logbuf, strlen(logbuf));
#if defined(IAUTH_DEBUG)
		if ((ALOG_DSPY & debuglevel) && debug)
		    {
			fprintf(debug, "To ircd: %s", logbuf+1);
			fflush(debug);
		    }
#endif
	    }
}

#if USE_STDARG
void
sendto_log(int flags, int slflags, char *pattern, ...)
{
        va_list va;
        va_start(va, pattern);
        vsendto_log(flags, slflags, pattern, va);
        va_end(va);
}
#endif
