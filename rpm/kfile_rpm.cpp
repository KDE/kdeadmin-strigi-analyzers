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
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include <config.h>

#include <kprocess.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <kstringvalidator.h>
#include <kdebug.h>

#include <qdict.h>
#include <qvalidator.h>
#include <qcstring.h>
#include <qfile.h>
#include <qdatetime.h>

#if !defined(__osf__)
#include <inttypes.h>
#else
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#endif

#include "kfile_rpm.h"
#include <netinet/in.h>

typedef KGenericFactory<KRpmPlugin> RpmFactory;

K_EXPORT_COMPONENT_FACTORY(kfile_rpm, RpmFactory( "kfile_rpm" ));

KRpmPlugin::KRpmPlugin(QObject *parent, const char *name,
                       const QStringList &args)
    
    : KFilePlugin(parent, name, args)
{
    KFileMimeTypeInfo* info = addMimeTypeInfo( "application/x-rpm" );
    KFileMimeTypeInfo::GroupInfo* group = 0L;
    group = addGroupInfo(info, "General", i18n("General"));
    KFileMimeTypeInfo::ItemInfo* item;

    item = addItemInfo(group, "Name", i18n("Name"), QVariant::String);
    item = addItemInfo(group, "Version", i18n("Version"), QVariant::String);
    item = addItemInfo(group, "Summary", i18n("Summary"), QVariant::String);
    item = addItemInfo(group, "Group", i18n("Group"), QVariant::String);
    item = addItemInfo(group, "Size", i18n("Size"), QVariant::Int);
}

QString KRpmPlugin::getStringTag( QFile& file, uint32_t offset, uint32_t type )
{
    int old = file.at();
    QString result = "";
    char in;
    if (type != RPM_STRING_TYPE && type != RPM_I18NSTRING_TYPE) return result;
    
    file.at(offset);
    while ((in = file.getch()) != '\0') result += in;
    file.at(old);
    return result;
}

uint32_t KRpmPlugin::getInt32Tag( QFile& file, QDataStream& dfile, uint32_t offset, uint32_t type )
{
    int old = file.at();
    uint32_t num = 0;
    if (type != RPM_INT32_TYPE) return num;
    
    file.at(offset);
    dfile >> num;
    file.at(old);
    return num;

}

bool KRpmPlugin::readInfo( KFileMetaInfo& info, uint /*what*/)
{
    QFile file(info.path());
    int pass;

    if (!file.open(IO_ReadOnly))
    {
        kdDebug(7034) << "Couldn't open " << QFile::encodeName(info.path()) << endl;
        return false;
    }    

    QDataStream dstream(&file);
    dstream.setByteOrder(QDataStream::BigEndian);
    KFileMetaInfoGroup group = appendGroup(info, "General");
    
    file.at(96); // Seek past old lead
    
    for (pass = 0; pass < 2; pass++) {
	uint32_t storepos, entries, size, reserved;
	uint8_t version;
	char magic[3];
	
	dstream.readRawBytes(magic, 3);	
	dstream >> version >> reserved >> entries >> size;
	if (memcmp(magic, RPM_HEADER_MAGIC, 3)) return false;	
	if (version != 1) return false; // Only v1 headers supported

	storepos = file.at() + entries * 16;
	
	if (entries < 500 && pass != 0) while (entries--) { // Just in case something goes wrong, limit to 500
		uint32_t tag, type, offset, count;
		dstream >> tag >> type >> offset >> count;
		offset += storepos;
		
		switch (tag) {
			case RPMTAG_NAME: appendItem(group, "Name", getStringTag(file, offset, type ) ); break;
			case RPMTAG_VERSION: appendItem(group, "Version", getStringTag(file, offset, type ) ); break;
			case RPMTAG_SUMMARY: appendItem(group, "Summary", getStringTag(file, offset, type ) ); break;
			case RPMTAG_GROUP: appendItem(group, "Group", getStringTag(file, offset, type ) ); break;
			case RPMTAG_SIZE: appendItem(group, "Size", int( getInt32Tag(file, dstream, offset, type ) ) ); break;
		}
	}

	file.at(storepos + size);
	if (pass == 0) file.at(file.at() + (8 - (file.at() % 8)) % 8); // Skip padding
    }
    
    return true;
}

#include "kfile_rpm.moc"
