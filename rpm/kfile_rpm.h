/* This file is part of the KDE project
 * Copyright (C) 2002 Laurence Anderson <l.d.anderson@warwick.ac.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef __KFILE_RPM_H__
#define __KFILE_RPM_H__

#include <kfilemetainfo.h>

class QStringList;

class KRpmPlugin: public KFilePlugin
{
    Q_OBJECT
    
public:
    KRpmPlugin( QObject *parent, const char *name, const QStringList& args );
    
    virtual bool readInfo( KFileMetaInfo& info, uint what);

};
		
#define RPM_HEADER_MAGIC "\216\255\350"
#define	RPMTAG_NAME  			1000
#define	RPMTAG_VERSION			1001
#define RPMTAG_RELEASE                  1002
#define	RPMTAG_SUMMARY			1004
#define RPMTAG_DESCRIPTION		1005
#define	RPMTAG_SIZE			1009
#define RPMTAG_VENDOR                   1011
#define RPMTAG_PACKAGER                 1015
#define	RPMTAG_GROUP			1016
#define RPM_INT16_TYPE		3
#define RPM_INT32_TYPE		4
#define RPM_STRING_TYPE		6
#define RPM_I18NSTRING_TYPE     9

#endif
