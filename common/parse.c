/************************************************************************
 *   IRC - Internet Relay Chat, common/parse.c
 *   Copyright (C) 1990 Jarkko Oikarinen and
 *                      University of Oulu, Computing Center
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
#ifndef CLIENT_COMPILE
# include "s_defines.h"
#else
# include "c_defines.h"
#endif
#define PARSE_C
#ifndef CLIENT_COMPILE
# include "s_externs.h"
#else
# include "c_externs.h"
#endif
#undef PARSE_C

struct Message msgtab[] = {
  { MSG_PRIVATE, m_private,  MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
#ifndef CLIENT_COMPILE
  { MSG_NJOIN,   m_njoin,    MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
#endif
  { MSG_JOIN,    m_join,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_MODE,    m_mode,     MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
#ifndef CLIENT_COMPILE
  { MSG_UNICK,   m_unick,    MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
#endif
  { MSG_NICK,    m_nick,     MAXPARA, MSG_LAG, 0, 0, 0L},
  { MSG_PART,    m_part,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_QUIT,    m_quit,     MAXPARA, MSG_LAG, 0, 0, 0L},
  { MSG_NOTICE,  m_notice,   MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_KICK,    m_kick,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_SERVER,  m_server,   MAXPARA, MSG_LAG|MSG_NOU, 0, 0, 0L},
#ifndef CLIENT_COMPILE
  { MSG_SMASK,   m_smask,    MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
  { MSG_TRACE,   m_trace,    MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
#endif
  { MSG_TOPIC,   m_topic,    MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_INVITE,  m_invite,   MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_WALLOPS, m_wallops,  MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
  { MSG_PING,    m_ping,     MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_PONG,    m_pong,     MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_ERROR,   m_error,    MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
#ifdef	OPER_KILL
  { MSG_KILL,    m_kill,     MAXPARA, MSG_LAG|MSG_REG|MSG_OP|MSG_LOP, 0,0, 0L},
#else
  { MSG_KILL,    m_kill,     MAXPARA, MSG_LAG|MSG_REG|MSG_NOU, 0, 0, 0L},
#endif
#ifndef CLIENT_COMPILE
  { MSG_SAVE,    m_save,     MAXPARA, MSG_LAG|MSG_NOU, 0, 0, 0L},
  { MSG_USER,    m_user,     MAXPARA, MSG_LAG|MSG_NOU, 0, 0, 0L},
  { MSG_AWAY,    m_away,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_UMODE,   m_umode,    MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_ISON,    m_ison,     1,	 MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_SQUIT,   m_squit,    MAXPARA, MSG_LAG|MSG_REG|MSG_OP|MSG_LOP, 0,0, 0L},
  { MSG_WHOIS,   m_whois,    MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_WHO,     m_who,      MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_WHOWAS,  m_whowas,   MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_LIST,    m_list,     MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_NAMES,   m_names,    MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_USERHOST,m_userhost, MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_PASS,    m_pass,     MAXPARA, MSG_LAG|MSG_NOU, 0, 0, 0L},
  { MSG_LUSERS,  m_lusers,   MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_TIME,    m_time,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_OPER,    m_oper,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_CONNECT, m_connect,  MAXPARA,
				MSG_LAG|MSG_REGU|MSG_OP|MSG_LOP, 0, 0, 0L},
  { MSG_VERSION, m_version,  MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_STATS,   m_stats,    MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_LINKS,   m_links,    MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_ADMIN,   m_admin,    MAXPARA, MSG_LAG, 0, 0, 0L},
  { MSG_USERS,   m_users,    MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_SUMMON,  m_summon,   MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_HELP,    m_help,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_INFO,    m_info,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_MOTD,    m_motd,     MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_CLOSE,   m_close,    MAXPARA, MSG_LAG|MSG_REGU|MSG_OP, 0, 0, 0L},
  { MSG_SERVICE, m_service,  MAXPARA, MSG_LAG|MSG_NOU, 0, 0, 0L},
  { MSG_EOB,     m_eob,      MAXPARA, MSG_LAG|MSG_NOU|MSG_REG, 0, 0, 0L},
#ifdef	USE_SERVICES
  { MSG_SERVSET, m_servset,  MAXPARA, MSG_LAG|MSG_SVC, 0, 0, 0L},
#endif
  { MSG_SQUERY,  m_squery,   MAXPARA, MSG_LAG|MSG_REGU, 0, 0, 0L},
  { MSG_SERVLIST,m_servlist, MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_HASH,    m_hash,     MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
  { MSG_DNS,     m_dns,      MAXPARA, MSG_LAG|MSG_REG, 0, 0, 0L},
#ifdef	OPER_REHASH
  { MSG_REHASH,  m_rehash,   MAXPARA, MSG_REGU|MSG_OP
# ifdef	LOCOP_REHASH
					 |MSG_LOP
# endif
					, 0, 0, 0L},
#endif
#ifdef	OPER_RESTART
  { MSG_RESTART,  m_restart,   MAXPARA, MSG_REGU|MSG_OP
# ifdef	LOCOP_RESTART
					 |MSG_LOP
# endif
					, 0, 0, 0L},
#endif
#ifdef	OPER_DIE
  { MSG_DIE,  m_die,   MAXPARA, MSG_REGU|MSG_OP
# ifdef	LOCOP_DIE
					 |MSG_LOP
# endif
					, 0, 0, 0L},
#endif
#ifdef OPER_SET
  { MSG_SET,  m_set,   MAXPARA, MSG_REGU|MSG_OP
#ifdef LOCOP_SET
					| MSG_LOP
#endif
					, 0, 0, 0L},
#endif /* OPER_SET */
  { MSG_MAP,  m_map,   MAXPARA, MSG_LAG | MSG_REG, 0, 0, 0L},
  { MSG_POST,    m_post,     MAXPARA, MSG_NOU, 0, 0, 0L},
#endif /* !CLIENT_COMPILE */
  { NULL, NULL, 0, 0, 0, 0, 0L}
};

/*
 * NOTE: parse() should not be called recursively by other functions!
 */
static	char	*para[MAXPARA+1];

#ifdef	CLIENT_COMPILE
static	char	sender[NICKLEN+USERLEN+HOSTLEN+3];
char	userhost[USERLEN+HOSTLEN+2];
#define	timeofday	time(NULL)
#else
static	char	sender[HOSTLEN+1];
static	int	cancel_clients (aClient *, aClient *, char *);
static	void	remove_unknown (aClient *, char *);
#endif

static	int	find_sender (aClient *cptr, aClient **sptr, char *sender,
			char *buffer);

/*
**  Find a client (server or user) by name.
**
**  *Note*
**	Semantics of this function has been changed from
**	the old. 'name' is now assumed to be a null terminated
**	string and the search is the for server and user.
*/
#ifndef CLIENT_COMPILE
aClient	*find_client(char *name, aClient *cptr)
{
	aClient *acptr = cptr;

	if (name && *name)
		acptr = hash_find_client(name, cptr);

	return acptr;
}

aClient	*find_uid(char *uid, aClient *cptr)
{
	aClient *acptr = cptr;

	if (uid && isdigit(*uid))
		acptr = hash_find_uid(uid, cptr);

	return acptr;
}

aClient	*find_sid(char *sid, aClient *cptr)
{
	if (sid && isdigit(*sid))
	{
		cptr = hash_find_sid(sid, cptr);
	}

	return cptr;
}

aClient	*find_service(char *name, aClient *cptr)
{
	aClient *acptr = cptr;

	if (index(name, '@'))
		acptr = hash_find_client(name, cptr);
	return acptr;
}

aClient	*find_matching_client(char *mask)
{
	aClient *acptr;
	aServer *asptr;
	aService *sp;
	char *ch;
	int wild = 0, dot = 0;
	
	/* try to find exact match */	
	acptr = find_client(mask, NULL);
	
	if (acptr)
	{
		return acptr;
	}
	/* check if we should check against wilds */
	for (ch = mask; *ch; ch++)
	{
		if (*ch == '*' || *ch == '?')
		{
			wild = 1;
			break;
		}
		if (*ch == '.')
		{
			dot = 1;
			break;
		}
	}
	
	if (!wild && !dot)
	{
		return NULL;
	}
	
	(void) collapse(mask);
	
	/* try to match some servername against mask */
	/* start from bottom, from ME, to return ourselves first */
	for (asptr = me.serv; asptr; asptr = asptr->prevs)
	{
		if (!match(asptr->bcptr->name, mask) ||
		    !match(mask, asptr->bcptr->name))
		{
			acptr = asptr->bcptr;
			return acptr->serv->maskedby;
		}
	}
	/* no match, try services */
	for (sp = svctop; sp; sp = sp->nexts)
	{
		if (!match(sp->bcptr->name, mask) ||
		    !match(mask, sp->bcptr->name))
		{
			acptr = sp->bcptr;
			return acptr;	
		}
	}
	return NULL;
}
#else /* CLIENT_COMPILE */

aClient	*find_client(char *name, aClient *cptr)
{
	Reg	aClient	*c2ptr = cptr;

	if (!name || !*name)
		return c2ptr;

	for (c2ptr = client; c2ptr; c2ptr = c2ptr->next) 
		if (mycmp(name, c2ptr->name) == 0)
			return c2ptr;
	return cptr;
}
#endif /* CLIENT_COMPILE */
#ifndef CLIENT_COMPILE
/*
**  Find a user@host (server or user).
**
**  *Note*
**	Semantics of this function has been changed from
**	the old. 'name' is now assumed to be a null terminated
**	string and the search is the for server and user.
*/
aClient	*find_userhost(char *user, char *host, aClient *cptr, int *count)
{
	Reg	aClient	*c2ptr;
	Reg	aClient	*res = cptr;

	*count = 0;
	if (user)
	{
		if (host)
		{
			anUser *auptr;
			for (auptr = hash_find_hostname(host, NULL); auptr;
					auptr = auptr->hhnext)
			{
				if (MyConnect(auptr->bcptr)
				    && !mycmp(user, auptr->username))
				{
					if (++(*count) > 1)
					{
						/* We already failed
						 * - just return */
						return res;
					}
					res = auptr->bcptr;
				}
			}
		}
		else
		{
			int i;
			for (i = 0; i <= highest_fd; i++)
			{
				if (!(c2ptr = local[i])
				      || !IsRegisteredUser(c2ptr))
				{
					continue;
				}
				if (!mycmp(user, c2ptr->user->username))
				{
					if (++(*count) > 1)
					{
					/* Already failed, just return */
						return res;
					}
					res = c2ptr;
				}
			}
	    }
	}
	return res;
}


/*
**  Find server by name.
**
**	This implementation assumes that server and user names
**	are unique, no user can have a server name and vice versa.
**	One should maintain separate lists for users and servers,
**	if this restriction is removed.
**
**  *Note*
**	Semantics of this function has been changed from
**	the old. 'name' is now assumed to be a null terminated
**	string.
*/

/*
** Find a server from hash table, given its name
*/
aClient	*find_server(char *name, aClient *cptr)
{
	aClient *acptr = cptr;

	if (name && *name)
		acptr = hash_find_server(name, cptr);
	return acptr;
}

/*
** Given a server name, find the server mask behind which the server
** is hidden.
*/
aClient	*find_mask(char *name, aClient *cptr)
{
	static	char	servermask[HOSTLEN+1];
	Reg	aClient	*c2ptr = cptr;
	Reg	char	*mask = servermask;

	if (!name || !*name)
		return c2ptr;
	if ((c2ptr = hash_find_server(name, cptr)))
		return (c2ptr);
	if (index(name, '*'))
		return c2ptr;
	strcpy (servermask, name);
	while (*mask)
	{
		if (*(mask+1) == '.')
		{
			*mask = '*';
			if ((c2ptr = hash_find_server(mask, cptr)))
				return (c2ptr);
		}
		mask++;
	}
	return (c2ptr ? c2ptr : cptr);
}

/*
** Find a server from hash table, given its token
*/
aServer	*find_tokserver(int token, aClient *cptr, aClient *c2ptr)
{
	return hash_find_stoken(token, cptr, c2ptr);
}

/*
** Find a server, given its name (which might contain *'s, in which case
** the first match will be return [not the best one])
*/
aClient	*find_name(char *name, aClient *cptr)
{
	Reg	aClient	*c2ptr = cptr;
	Reg	aServer	*sp = NULL;

	if (!name || !*name)
		return c2ptr;

	if ((c2ptr = hash_find_server(name, cptr)))
		return (c2ptr);
	if (!index(name, '*'))
		return c2ptr;
	for (sp = svrtop; sp; sp = sp->nexts)
	    {
		/*
		** A server present in the list necessarily has a non NULL
		** bcptr pointer.
		*/
		if (match(name, sp->bcptr->name) == 0)
			break;
		if (index(sp->bcptr->name, '*'))
			if (match(sp->bcptr->name, name) == 0)
					break;
	    }
	return (sp ? sp->bcptr : cptr);
}
#else
aClient	*find_server(char *name, aClient *cptr)
{
	Reg	aClient *c2ptr = cptr;

	if (!name || !*name)
		return c2ptr;

	for (c2ptr = client; c2ptr; c2ptr = c2ptr->next)
	    {
		if (!IsServer(c2ptr) && !IsMe(c2ptr))
			continue;
		if (match(c2ptr->name, name) == 0 ||
		    match(name, c2ptr->name) == 0)
			break;
	    }
	return (c2ptr ? c2ptr : cptr);
}
#endif /* CLIENT_COMPILE */

/*
**  Find person by (nick)name.
*/
aClient	*find_person(char *name, aClient *cptr)
{
	Reg	aClient	*c2ptr = cptr;

	c2ptr = find_client(name, c2ptr);

	if (c2ptr && IsClient(c2ptr) && c2ptr->user)
		return c2ptr;
	else
		return cptr;
}

/*
** find_sender(): 
** Find the client structure for the sender of the message we got from cptr
** and checks it to be valid.
** Stores the result in *sptr.
** Returns:
**	 1 on success.
**	 0 when we removed a remote client.
**	-1 when coming from a wrong server (wrong direction).
**	-2 (FLUSH_BUFFER) when we removed a local client (server).
**	-3 when client not found.
*/
static	int	find_sender(aClient *cptr, aClient **sptr, char *sender,
			char *buffer)
{
	aClient *from = NULL;

#ifndef	CLIENT_COMPILE
	if (ST_UID(cptr))
	{
		if (isdigit(*sender))
		{
			if (strlen(sender) == SIDLEN)
			{
				/* SID */
				from = find_sid(sender, NULL);
			}
			else
			{
				/* UID */
				from = find_uid(sender, NULL);
			}
		}
		else if (*sender == '$' && strlen(sender) == SIDLEN)
		{
			/* Compatibility SID. */
			aServer *servptr;

			servptr = find_tokserver(idtol(sender + 1, SIDLEN - 1),
				cptr, NULL);
			if (servptr)
			{
				from = servptr->bcptr;
			}
		}
	}
#endif
	if (!from)
	{
		from = find_client(sender, (aClient *) NULL);
		if (!from ||
			/*
			** I really believe that the followin line is 
			** useless.  What a waste, especially with 2.9
			** hostmasks.. at least the test on from->name
			** will make it a bit better. -krys
			*/
			(*from->name == '*' && match(from->name, sender)))
		{
			from = find_server(sender, (aClient *)NULL);
		}
	}
#ifndef	CLIENT_COMPILE
	/* Is there svc@server prefix ever? -Vesa */
	/* every time a service talks -krys */
	if (!from && index(sender, '@'))
	{
		from = find_service(sender, (aClient *)NULL);
	}
	if (!from)
	{
		from = find_mask(sender, (aClient *) NULL);
	}
	if (isdigit(sender[0]) || sender[0] == '$')
	{
		para[0] = from->name;
	}
	else
#endif
	{
		para[0] = sender;
	}

	/* Hmm! If the client corresponding to the
	** prefix is not found--what is the correct
	** action??? Now, I will ignore the message
	** (old IRC just let it through as if the
	** prefix just wasn't there...) --msa
	** Since 2.9 we pick them up and .. --Vesa
	*/
	if (!from)
	{
		Debug((DEBUG_ERROR,
			"Unknown prefix (%s)(%s) from (%s)",
			sender, buffer, cptr->name));
		ircstp->is_unpf++;
#ifndef	CLIENT_COMPILE
		remove_unknown(cptr, sender);
#endif
		return -3;	/* Grab it in read_message() */
	}
	if (from->from != cptr)
	{
		ircstp->is_wrdi++;
		Debug((DEBUG_ERROR,
			"Message (%s) coming from (%s)",
			buffer, cptr->name));
#ifndef	CLIENT_COMPILE
		return cancel_clients(cptr, from, buffer);
#else
		return -1;
#endif
	}

	*sptr = from;
	return 1;
}
#ifndef CLIENT_COMPILE
/* find target.
**  name - name of the client to be searched
**  cptr - originating socket
*/
aClient	*find_target(char *name, aClient *cptr)
{
	aClient *acptr = NULL;
	
	if (ST_UID(cptr))
	{
		if (isdigit(name[0]))
		{
			if (name[SIDLEN] == '\0')
			{
				acptr = find_sid(name, NULL);
			}
			else
			{
				acptr = find_uid(name, NULL);
			}
		}
		else if (name[0] == '$' && name[SIDLEN] == '\0')
		{
			aServer *asptr;
			asptr = find_tokserver(idtol(name + 1, SIDLEN - 1),
				cptr, NULL);
			if (acptr)
			{
				acptr = asptr->bcptr;
			}
		}
	}
	if (!acptr)
	{
		acptr = find_client(name, NULL);
		if (!acptr)
		{
			acptr = find_server(name, NULL);
		}
		if (!acptr && !match(name, ME))
		{
			/* Matches when the target is "*.ourmask" which
			 * is not handled by above functions.
			 */
			acptr = &me;
		}
	}
	return acptr;
}
#endif

/*
 * parse a buffer.
 * Return values:
 *  errors: -3 for unknown origin/sender, -2 for FLUSH_BUFFER, -1 for bad cptr
 *
 * NOTE: parse() should not be called recusively by any other fucntions!
 */
int	parse(aClient *cptr, char *buffer, char *bufend)
{
	aClient *from = cptr;
	Reg	char	*ch, *s;
	Reg	int	len, i, numeric = 0, paramcount;
	Reg	struct	Message *mptr = NULL;
	int	ret;

#ifndef	CLIENT_COMPILE
	Debug((DEBUG_DEBUG, "Parsing %s: %s",
		get_client_name(cptr, FALSE), buffer));
	if (IsDead(cptr))
		return -1;
#endif

	s = sender;
	*s = '\0';
	for (ch = buffer; *ch == ' '; ch++)
		;
	para[0] = from->name;
	if (*ch == ':')
	    {
		/*
		** Copy the prefix to 'sender' assuming it terminates
		** with SPACE (or NULL, which is an error, though).
		*/
		for (++ch; *ch && *ch != ' '; ++ch )
			if (s < (sender + sizeof(sender)-1))
				*s++ = *ch; /* leave room for NULL */
		*s = '\0';
#ifdef CLIENT_COMPILE
		if ((s = index(sender, '!')))
		    {
			*s++ = '\0';
			strncpyzt(userhost, s, sizeof(userhost));
		    }
		else if ((s = index(sender, '@')))
		    {
			*s++ = '\0';
			strncpyzt(userhost, s, sizeof(userhost));
		    }
#endif
		/*
		** Actually, only messages coming from servers can have
		** the prefix--prefix silently ignored, if coming from
		** a user client...
		**
		** ...sigh, the current release "v2.2PL1" generates also
		** null prefixes, at least to NOTIFY messages (e.g. it
		** puts "sptr->nickname" as prefix from server structures
		** where it's null--the following will handle this case
		** as "no prefix" at all --msa  (": NOTICE nick ...")
		*/
		if (*sender && IsServer(cptr))
		{
			i = find_sender(cptr, &from, sender, buffer);
			if (i <= 0)
			{
				return i;
			}
		}
		while (*ch == ' ')
			ch++;
	    }
	if (*ch == '\0')
	    {
		ircstp->is_empt++;
		Debug((DEBUG_NOTICE, "Empty message from host %s:%s",
		      cptr->name, from->name));
		return -1;
	    }
	/*
	** Extract the command code from the packet.  Point s to the end
	** of the command code and calculate the length using pointer
	** arithmetic.  Note: only need length for numerics and *all*
	** numerics must have paramters and thus a space after the command
	** code. -avalon
	*/
	s = (char *)index(ch, ' '); /* s -> End of the command code */
	len = (s) ? (s - ch) : 0;
	if (len == 3 &&
	    isdigit(*ch) && isdigit(*(ch + 1)) && isdigit(*(ch + 2)))
	    {
		numeric = (*ch - '0') * 100 + (*(ch + 1) - '0') * 10
			+ (*(ch + 2) - '0');
		paramcount = MAXPARA;
		ircstp->is_num++;
	    }
	else
	    {
		if (s)
			*s++ = '\0';
		for (mptr = msgtab; mptr->cmd; mptr++) 
			if (mycmp(mptr->cmd, ch) == 0)
				break;

		if (!mptr->cmd)
		    {
			/*
			** Note: Give error message *only* to recognized
			** persons. It's a nightmare situation to have
			** two programs sending "Unknown command"'s or
			** equivalent to each other at full blast....
			** If it has got to person state, it at least
			** seems to be well behaving. Perhaps this message
			** should never be generated, though...  --msa
			** Hm, when is the buffer empty -- if a command
			** code has been found ?? -Armin
			*/
			if (buffer[0] != '\0')
			    {
				cptr->flags |= FLAGS_UNKCMD;
				if (IsPerson(from))
					sendto_one(from,
					    ":%s %d %s %s :Unknown command",
					    me.name, ERR_UNKNOWNCOMMAND,
					    from->name, ch);
#ifdef	CLIENT_COMPILE
				Debug((DEBUG_ERROR,"Unknown (%s) from %s[%s]",
					ch, cptr->name, cptr->sockhost));
#else
				else if (IsServer(cptr))
					sendto_flag(SCH_ERROR,
					    "Unknown command from %s:%s",
					    get_client_name(cptr, TRUE), ch);
				Debug((DEBUG_ERROR,"Unknown (%s) from %s",
					ch, get_client_name(cptr, TRUE)));
#endif
			    }
			ircstp->is_unco++;
			return -1;
		    }
		paramcount = mptr->parameters;
		i = bufend - ((s) ? s : ch);
		mptr->bytes += i;
#ifndef	CLIENT_COMPILE
		if ((mptr->flags & MSG_LAG) &&
		    !(IsServer(cptr) || IsService(cptr)))
		    {	/* Flood control partly migrated into penalty */
			if (bootopt & BOOT_PROT)
				cptr->since += (1 + i / 100);
			else
				cptr->since = timeofday;
			/* Allow only 1 msg per 2 seconds
			 * (on average) to prevent dumping.
			 * to keep the response rate up,
			 * bursts of up to 5 msgs are allowed
			 * -SRB
			 */
		    }
#endif
	    }
	/*
	** Must the following loop really be so devious? On
	** surface it splits the message to parameters from
	** blank spaces. But, if paramcount has been reached,
	** the rest of the message goes into this last parameter
	** (about same effect as ":" has...) --msa
	*/

	/* Note initially true: s==NULL || *(s-1) == '\0' !! */

#ifdef	CLIENT_COMPILE
	if (me.user)
		para[0] = sender;
#endif
	i = 0;
	if (s)
	    {
		if (paramcount > MAXPARA)
			paramcount = MAXPARA;
		for (;;)
		    {
			/*
			** Never "FRANCE " again!! ;-) Clean
			** out *all* blanks.. --msa
			*/
			while (*s == ' ')
				*s++ = '\0';

			if (*s == '\0')
				break;
			if (*s == ':')
			    {
				/*
				** The rest is single parameter--can
				** include blanks also.
				*/
				para[++i] = s + 1;
				break;
			    }
			para[++i] = s;
			if (i >= paramcount-1)
				break;
			for (; *s != ' ' && *s; s++)
				;
		    }
	    }
	para[++i] = NULL; /* at worst, ++i is paramcount (MAXPARA) */
	if (mptr == NULL)
		return (do_numeric(numeric, cptr, from, i, para));
	mptr->count++;
	if (!MyConnect(from))
		mptr->rcount++;
	if (IsRegisteredUser(cptr) &&
#ifdef	IDLE_FROM_MSG
	    mptr->func == m_private)
#else
	    mptr->func != m_ping && mptr->func != m_pong)
#endif
		from->user->last = timeofday;
	Debug((DEBUG_DEBUG, "Function: %#x = %s parc %d parv %#x",
		mptr->func, mptr->cmd, i, para));
#ifndef	CLIENT_COMPILE
	if ((mptr->flags & MSG_REGU) && check_registered_user(from))
		return -1;
	if ((mptr->flags & MSG_SVC) && check_registered_service(from))
		return -1;
	if ((mptr->flags & MSG_REG) && check_registered(from))
		return -1;
	if ((mptr->flags & MSG_NOU) && (MyPerson(from) || MyService(from)))
	    {
		sendto_one(from, replies[ERR_ALREADYREGISTRED], ME, BadTo(para[0]));
		return-1;
	    }
	if (MyConnect(from) && !IsServer(from) &&
	    (mptr->flags & (MSG_LOP|MSG_OP)) &&
	    !((mptr->flags & MSG_OP) && (IsOper(from))) &&
	    !((mptr->flags & MSG_LOP) && (IsLocOp(from))))
		    {
			sendto_one(from, replies[ERR_NOPRIVILEGES], ME, BadTo(para[0]));
			return -1;
		    }
#endif
	/*
	** ALL m_functions return now UNIFORMLY:
	**   -2  old FLUSH_BUFFER return value (unchanged).
	**   -1  if parsing of a protocol message leads in a syntactic/semantic
	**       error and NO penalty scoring should be applied.
	**   >=0 if protocol message processing was successful. The return
	**       value indicates the penalty score.
	*/
	ret = (*mptr->func)(cptr, from, i, para);

#ifndef       CLIENT_COMPILE
	/*
        ** Add penalty score for sucessfully parsed command if issued by
	** a LOCAL user client.
	*/
	if ((ret > 0) && IsRegisteredUser(cptr) && (bootopt & BOOT_PROT))
	    {
		cptr->since += ret;
/* only to lurk
		sendto_one(cptr,
			   ":%s NOTICE %s :*** Penalty INCR [%s] +%d",
			   me.name, cptr->name, ch, ret);
*/
	    }
#endif
	return (ret != FLUSH_BUFFER) ? 2 : FLUSH_BUFFER;
}

/*
 * field breakup for ircd.conf file.
 */
char	*getfield(char *irc_newline)
{
	static	char *line = NULL;
	char	*end, *field;
	
	if (irc_newline)
		line = irc_newline;
	if (line == NULL)
		return(NULL);

	field = line;

	end = index(line, IRCDCONF_DELIMITER);
	if (end == line)
	{ /* empty */
		line++;
	}
	else
	{
		for (;;)
		{
			if (!end)
			{
				/* we can't find delimiter at the end of
				 * this field. (probably last one)
				 */
				break;
			}
			if (*(end - 1) != '\\')
			{ /* not escaped delimiter */
				break;
			}
			else
			{ /* escaped one, dequote */
				char *s;
				if (*(end+1) == '\0')
					break;
				for (s = (end - 1); (*s = *(s+1)) ;s++);
				end++;
				end = index(end, IRCDCONF_DELIMITER);
			}
		}
		if (!end)
		{
			line = NULL;
			if ((end = (char *)index(field, '\n')) == NULL)
				end = field + strlen(field);
		}
		else
		{
			line = end + 1;
		}
	}
	*end = '\0';
	return(field);
}

#ifndef	CLIENT_COMPILE
static	int	cancel_clients(aClient *cptr, aClient *sptr, char *cmd)
{
	/*
	 * kill all possible points that are causing confusion here,
	 * I'm not sure I've got this all right...
	 * - avalon
	 */
	sendto_flag(SCH_NOTICE, "Message (%s) for %s[%s!%s@%s] from %s",
		    cmd, sptr->name, sptr->from->name, sptr->from->username,
		    sptr->from->sockhost, get_client_name(cptr, TRUE));
	/*
	 * Incorrect prefix for a server from some connection.  If it is a
	 * client trying to be annoying, just QUIT them, if it is a server
	 * then the same deal.
	 */
	if (IsServer(sptr) || IsMe(sptr))
	    {
		sendto_flag(SCH_NOTICE, "Dropping server %s",cptr->name);
		return exit_client(cptr, cptr, &me, "Fake Direction");
	    }
	/*
	 * Ok, someone is trying to impose as a client and things are
	 * confused.  If we got the wrong prefix from a server, send out a
	 * kill, else just exit the lame client.
	 */
	if (IsServer(cptr))
	    {
		sendto_serv_butone(NULL, ":%s KILL %s :%s (%s[%s] != %s)",
				   me.name, sptr->name, me.name,
				   sptr->name, sptr->from->name,
				   get_client_name(cptr, TRUE));
		sptr->flags |= FLAGS_KILLED;
		return exit_client(cptr, sptr, &me, "Fake Prefix");
	    }
	return exit_client(cptr, cptr, &me, "Fake prefix");
}

static	void	remove_unknown(aClient *cptr, char *sender)
{
	if (!IsRegistered(cptr) || IsClient(cptr))
		return;
	/*
	 * Not from a server so don't need to worry about it.
	 */
	if (!IsServer(cptr))
		return;
	/*
	 * squit if it is a server because it means something is really
	 * wrong.
	 */
	if (index(sender, '.') /* <- buggy, it could be a service! */
	    && !index(sender, '@')) /* better.. */
	    {
		sendto_flag(SCH_NOTICE, "Squitting unknown %s brought by %s.",
			    sender, get_client_name(cptr, FALSE));
		sendto_one(cptr, ":%s SQUIT %s :(Unknown from %s)",
			   me.name, sender, get_client_name(cptr, FALSE));
	    }
	else
	/*
	 * Do kill if it came from a server because it means there is a ghost
	 * user on the other server which needs to be removed. -avalon
	 * it can simply be caused by lag (among other things), so just
	 * drop it if it is not a server. -krys
	 * services aren't prone to collisions, so lag shouldn't be responsible
	 * if we get here and sender is a service, we should probably issue
	 * a kill in this case! -krys
	 */
		sendto_flag(SCH_NOTICE, "Dropping unknown %s brought by %s.",
			    sender, get_client_name(cptr, FALSE));
}
#endif

