/***************************************************************************
                          dbg_core.c  -  description
                             -------------------
    begin                : Sun Sep 24 2000
    copyright            : (C) 2001 by Dmitri Dmitrienko
    email                : dd@cron.ru
    www                  : http://dd.cron.ru
    license              : This source file is subject to version 1.00 of 
                           the License,  that is bundled with this package 
                           in the file LICENSE, and is available at through 
                           the world-wide-web at http://dd.cron.ru/license
 ***************************************************************************/

#include "dbg_core.h"
#include <string.h>
#include <stdlib.h>

#ifdef DBG_USE_STDALLOCA
  #define _MALLOC malloc
  #define _FREE free
#else
  #include "zend.h"
  #include "zend_alloc.h"
  #define _MALLOC emalloc
  #define _FREE efree
#endif

DBGAPI int dbg_packet_new(dbg_packet *pack) {
	memset(pack,0,sizeof(dbg_packet));
	return 1;
}

DBGAPI void dbg_packet_free(dbg_packet *pack) {
	if (pack->buf) _FREE(pack->buf);
	memset(pack,0,sizeof(dbg_packet));
}

DBGAPI void dbg_packet_clear(dbg_packet *pack) {
	pack->size = 0;
	pack->lastrawid = 0;
}

int dbg_packet_update_limit(dbg_packet *pack, int space) {
	int alimit;
	void *p;
	
	if (!pack->limit) {
		alimit = PACKET_LIMIT_ROUND(space);
		pack->size = 0;
		pack->buf = _MALLOC(alimit);
		if (!pack->buf) return 0;
		pack->limit = alimit;
	} else if (pack->limit < (pack->size + space)) {
		alimit = PACKET_LIMIT_ROUND(pack->size + space);
		p = _MALLOC(alimit);
		if (!p) return 0;
		memcpy(p, pack->buf, pack->size);
		_FREE(pack->buf);
		pack->buf = p;
		pack->limit = alimit;
	}
	return 1;
}

DBGAPI int dbg_packet_add_frame(dbg_packet *pack, framename frname, void *data, int datasize) {
	dbg_frame *p;	
	int asize;

	asize = datasize + sizeof(dbg_frame);
	if (!dbg_packet_update_limit(pack, asize)) {
		return 0;
	}
	(char *) p = (char *) (pack->buf) + pack->size;
	p->size = datasize;
	memcpy(&(p->name), frname, sizeof(p->name));
	
	if (data) memcpy(FRAME_DATA_PTR(p), data, datasize);
	pack->size += asize;
	return (char*)FRAME_DATA_PTR(p) - (char*)pack->buf;
}

int dbg_sock_read(char *buf, int bufsize, int socket, int timeoutms) {
	fd_set rset;
	struct timeval timeout;
	int ret_val;

	if (socket <= 0) {
		return 0;
	}
	FD_ZERO(&rset);
	FD_SET((unsigned int)socket, &rset);
	if (timeoutms >= 0) {
		timeout.tv_sec = timeoutms / 1000;
		timeout.tv_usec = (timeoutms>1000) ? 0 : timeoutms * 1000;
	} else {
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
	}
	ret_val = select(socket + 1, &rset, NULL, NULL, &timeout);
	if (ret_val != 1) {	
		if (ret_val < 0) return -1;
		return 0;
	}
	ret_val = SREAD(socket, buf, bufsize);	
	return (ret_val) ? (ret_val):(-1);
}

DBGAPI dbg_frame *dbg_packet_firstframe(dbg_packet *pack) {
	return (pack->size ? (pack->buf):(NULL));
}

DBGAPI dbg_frame *dbg_packet_nextframe(dbg_packet *pack, dbg_frame *frame) {
	dbg_frame *p;
	int s,frsize;
	register char is_eq;

	s = pack->size;
	p = pack->buf;
	while (s >= sizeof(dbg_frame)) {
		frsize = p->size + sizeof(dbg_frame);
		is_eq = (p == frame);
		(char *) p += frsize;
		s -= frsize;
		if (is_eq) {
			if (s < sizeof(dbg_frame)) return NULL;
			return p;
		}
	}
	return NULL;
}

DBGAPI dbg_frame* dbg_packet_findfirstframe(dbg_packet *pack, framename frname) {
	dbg_frame *p;
	int s,frsize;

	s = pack->size;
	p = pack->buf;
	while (s >= sizeof(dbg_frame)) {
		if (strncmp(p->name, frname, sizeof(frname)) == 0) {
			return p;
		}
		frsize = p->size + sizeof(dbg_frame);
		(char *) p += frsize;
		s -= frsize;
	}
	return NULL;
}

DBGAPI dbg_frame* dbg_packet_findnextframe(dbg_packet *pack, framename frname, dbg_frame *frame) {
	dbg_frame *p;
	int s,frsize;

	p = dbg_packet_nextframe(pack, frame);

	s = (p) ? pack->size - ((char*)p - (char*)pack->buf) : 0;
	while (s >= sizeof(dbg_frame)) {
		if (strncmp(p->name, frname, sizeof(frname)) == 0) {
			return p;
		}
		frsize = p->size + sizeof(dbg_frame);
		(char *) p += frsize;
		s -= frsize;
	}
	return NULL;
}

DBGAPI int dbg_packet_send(int cmd , dbg_packet *pack, int socket, int flags) {
	dbg_header_struct *packetbuf;
	dbg_frame *p;
	int packsize, frsize, *v, i;
	int ret_val, sz, tosend, chunksz, sentsz;
	char *pchunk;

	if (socket <= 0) {
		return -1;
	}	
	packsize = sizeof(dbg_header_struct) + ((pack!=NULL)? pack->size:0);
	if (!(packetbuf = _MALLOC(packsize))) {
		return 0;
	}
	packetbuf->sync = DBGSYNC;
	packetbuf->cmd = htonl(cmd);
	packetbuf->flags = htonl(flags);
	packetbuf->bodysize = htonl(pack!=NULL?pack->size:0);
	if (pack && pack->size) {
		(char *)p = (char *)packetbuf + sizeof(dbg_header_struct);
		sz = pack->size;
		memcpy(p, pack->buf, pack->size); /* copy data */
		while (sz>0) { /* update all ints to be Big-Endian */
			if (strcmp(p->name, FRAME_RAWDATA) != 0) { 
				(char *)v = (char *)p + sizeof(*p);
				for (i=0; i<p->size/(int)sizeof(*v); i++) {
					*v = htonl(*v);
					v++;
				}
			}
			else {
				dbg_rawdata_body *body = (dbg_rawdata_body *)FRAME_DATA_PTR(p);
				body->datasize = htonl(body->datasize);
				body->rawid = htonl(body->rawid);
			}
			frsize = p->size + sizeof(dbg_frame);
			p->size = htonl(p->size);
			(char *) p += frsize;
			sz-=frsize;
		}
	}
	/* send by CHUNKSIZE chunks */
	tosend = packsize;
	pchunk = (char *)packetbuf;
	sentsz = 0;
	ret_val = 0;
	while  (tosend > 0) {
		chunksz = (tosend > CHUNKSIZE) ? CHUNKSIZE:tosend;
		ret_val = SSEND(socket, pchunk, chunksz);
		if (ret_val <= 0) break;
		sentsz+=ret_val;
		pchunk+=ret_val;
		tosend-=ret_val;
	}	
	_FREE(packetbuf);
	if (ret_val < 0) return -1;
	if (sentsz == packsize) return sentsz;
	return 0;
}

int dbg_packet_recv_body(dbg_packet *pack, int bodysize, int socket, int timeoutms) {
	dbg_frame *p;
	int *v, i;
	int ret_val = 0;
	int restsize, chunksz, recvsz, is_first;
	int frsize;
	char *pchunk;

	if (!dbg_packet_update_limit(pack, bodysize)) {
		return 0;
	}
	pchunk = (char *) (pack->buf) + pack->size;
	restsize = bodysize;
	recvsz = 0;
	is_first = 1;
	while (restsize > 0) {
		if (is_first) {
			chunksz = (restsize>(CHUNKSIZE-sizeof(dbg_header_struct))) ? CHUNKSIZE-sizeof(dbg_header_struct):restsize;
			is_first = 0;
		} 
		else {
			chunksz = (restsize>CHUNKSIZE) ? CHUNKSIZE:restsize;
		}
		ret_val = dbg_sock_read(pchunk, chunksz, socket, timeoutms);
		if (ret_val<=0) break;
		restsize -= ret_val;
		recvsz += ret_val;
		pchunk += ret_val;
	}
	if (recvsz!=bodysize) {
		dbg_packet_clear(pack);
		if (ret_val < 0) return -1;
		return 0;
	}
	(char *) p = (char *) (pack->buf) + pack->size;
	restsize = bodysize;
	while (restsize > 0) {
		p->size = ntohl(p->size);
		if (strcmp(p->name, FRAME_RAWDATA) != 0) { /* make all ints back system-depended Big- or Little- Endian*/
			(char *)v = (char *)p + sizeof(*p);
			for (i=0; i<p->size/(int)sizeof(*v); i++) {
				*v = ntohl(*v);
				v++;
			}
		}
		else {
			dbg_rawdata_body *body = (dbg_rawdata_body *)FRAME_DATA_PTR(p);
			body->datasize = ntohl(body->datasize);
			body->rawid = ntohl(body->rawid);
		}
		frsize = p->size + (int)sizeof(dbg_frame);
		if ((p->size > MAX_PACKET_SIZE) || (frsize > restsize)) {
			dbg_packet_clear(pack);
			return 0;
		}
		(char *) p += frsize;
		restsize -= frsize;
		if (restsize < 0) {
			dbg_packet_clear(pack);
			return 0;
		}
	}
	pack->size += bodysize;
	return bodysize;
}

DBGAPI int dbg_packet_recv(dbg_header_struct *hdr, dbg_packet *pack, int socket, int timeoutms) {
	int ret_val;

	if (!pack || !hdr) return 0;
	dbg_packet_clear(pack);
	ret_val = dbg_sock_read((char *) hdr, sizeof(dbg_header_struct), socket, timeoutms);
	if ((ret_val != sizeof(dbg_header_struct)) || 
		(hdr->sync != DBGSYNC)) {		
		memset(hdr,0,sizeof(dbg_header_struct));
		if (ret_val < 0) return -1;
		return 0;		
	}
	
	hdr->bodysize = ntohl(hdr->bodysize);
	hdr->cmd = ntohl(hdr->cmd);
	hdr->flags = ntohl(hdr->flags);
	
	if ((hdr->bodysize<0) || (hdr->bodysize > MAX_PACKET_SIZE)) {
		memset(hdr,0,sizeof(dbg_header_struct));
		return 0;
	}
	if (hdr->bodysize) {
		if (!dbg_packet_recv_body(pack, hdr->bodysize, socket, timeoutms)) {
			memset(hdr,0,sizeof(dbg_header_struct));
			return 0;
		}
	}
	return hdr->bodysize + sizeof(dbg_header_struct);
}

DBGAPI int dbg_packet_add_rawdata(dbg_packet *pack, const char *data, int datasize) {
	dbg_rawdata_body *body;
	char *p;
	int id, bodyofs;
	
	if (!data && datasize) return 0;
	bodyofs = dbg_packet_add_frame(pack, FRAME_RAWDATA, NULL, datasize + sizeof(dbg_rawdata_body));
	if (!bodyofs) {
		return 0;
	}
	id = ++pack->lastrawid;
	body = (dbg_rawdata_body *)((char *)pack->buf + bodyofs);
	body->rawid = id;
	body->datasize = datasize;
	p = (char*)body + sizeof(dbg_rawdata_body);
	if (data) memcpy(p, data, datasize);

	return id;
}

DBGAPI int dbg_packet_findrawdata(dbg_packet *pack, int rawid, char **data, int *datasize) {
	dbg_frame *fr;
	dbg_rawdata_body *body;

	*datasize = 0;
	*data = NULL;
	if (rawid <= 0) return 0;
	fr = dbg_packet_findfirstframe(pack, FRAME_RAWDATA);
	while (fr) {
		body =(dbg_rawdata_body *) FRAME_DATA_PTR(fr);
		if ((int)body->rawid == rawid) {
			*data = (char*)body + sizeof(dbg_rawdata_body);
			*datasize = body->datasize;
			return sizeof(dbg_rawdata_body) + (*datasize);
		}
		fr = dbg_packet_findnextframe(pack, FRAME_RAWDATA, fr);
	}
	return 0;
}

