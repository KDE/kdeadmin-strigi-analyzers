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

#include <config.h>

#include <kgenericfactory.h>
#include <kdebug.h>
#include <qfile.h>

#if !defined(__osf__)
#include <inttypes.h>
#else
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#endif

#include "kfile_rpm.h"

typedef KGenericFactory<KRpmPlugin> RpmFactory;

K_EXPORT_COMPONENT_FACTORY(kfile_rpm, RpmFactory( "kfile_rpm" ))

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
    item = addItemInfo(group, "Release", i18n("Release"), QVariant::Int);
    item = addItemInfo(group, "Summary", i18n("Summary"), QVariant::String);
    setAttributes ( item, KFileMimeTypeInfo::Description );
    item = addItemInfo(group, "Group", i18n("Group"), QVariant::String);
    item = addItemInfo(group, "Size", i18n("Size"), QVariant::Int);
    setUnit ( item, KFileMimeTypeInfo::Bytes );
    item = addItemInfo(group, "Vendor", i18n("Vendor"), QVariant::String );
    item = addItemInfo(group, "Packager", i18n("Packager"), QVariant::String );
    item = addItemInfo(group, "Archive Offset", i18n("Archive Offset"), QVariant::Int);
    item = addItemInfo(group, "Comment", i18n("Comment"), QVariant::String);
    setAttributes( item, KFileMimeTypeInfo::MultiLine );

    group = addGroupInfo(info, "All tags", i18n("All tags"));
    addVariableInfo(group, QVariant::String, 0);
}

bool KRpmPlugin::readInfo( KFileMetaInfo& info, uint what)
{
    QFile file(info.path());
    int pass;
    KFileMetaInfoGroup general, all;

    if (!file.open(IO_ReadOnly))
    {
        kdDebug(7034) << "Couldn't open " << QFile::encodeName(info.path()) << endl;
        return false;
    }

    QDataStream dstream(&file);
    dstream.setByteOrder(QDataStream::BigEndian);
    general = appendGroup(info, "General");
    if (what == KFileMetaInfo::Everything) all = appendGroup(info, "All tags");

    file.at(96); // Seek past old lead

    for (pass = 0; pass < 2; pass++) { // RPMs have two headers
	uint32_t storepos, entries, size, reserved;
	unsigned char version;
	char magic[3];

	dstream.readRawBytes(magic, 3);
	dstream >> version >> reserved >> entries >> size;
	if (memcmp(magic, RPM_HEADER_MAGIC, 3)) return false;
	if (version != 1) return false; // Only v1 headers supported

	storepos = file.at() + entries * 16;
	if (pass == 0) { // Don't need the first batch of tags - pgp etc
		file.at(storepos + size);
		file.at(file.at() + (8 - (file.at() % 8)) % 8); // Skip padding
		continue;
	}

	if (entries < 500) while (entries--) { // Just in case something goes wrong, limit to 500
		uint32_t tag, type, offset, count;
		QString tagname;
		dstream >> tag >> type >> offset >> count;
		offset += storepos;

		switch (tag) {
			case RPMTAG_NAME: tagname = "Name"; break;
			case RPMTAG_VERSION: tagname = "Version"; break;
			case RPMTAG_SUMMARY: tagname = "Summary"; break;
			case RPMTAG_GROUP: tagname = "Group"; break;
			case RPMTAG_SIZE: tagname = "Size"; break;
			case RPMTAG_RELEASE: tagname = "Release"; break;
			case RPMTAG_VENDOR: tagname = "Vendor"; break;
			case RPMTAG_PACKAGER: tagname = "Packager"; break;
			case RPMTAG_DESCRIPTION: tagname = "Comment"; break;
		}

		if ( !tagname.isEmpty() || all.isValid() ) {
			// kdDebug(7034) << "Tag number: " << tag << " Type: " << type << endl;
			int oldPos = file.at();
			file.at(offset); // Set file position to correct place in store
			switch (type) {
				case RPM_INT32_TYPE:	uint32_t int32tag;
							dstream >> int32tag;
							if ( !tagname.isEmpty() ) appendItem(general, tagname, int(int32tag));
							if ( all.isValid() ) appendItem(all, QString("%1").arg( tag ), QString("%1").arg( int32tag ));
							break;
				case RPM_INT16_TYPE:	uint16_t int16tag;
							dstream >> int16tag;
							if ( !tagname.isEmpty() ) appendItem(general, tagname, int(int16tag));
							if ( all.isValid() ) appendItem(all, QString("%1").arg( tag ), QString("%1").arg( int16tag ));
							break;
				case RPM_I18NSTRING_TYPE: // Fallthru
				case RPM_STRING_TYPE:   QString strtag; char in;
							while ( ( in = file.getch() ) != '\0' ) strtag += in;
							if ( !tagname.isEmpty() ) appendItem(general, tagname, strtag);
							if( all.isValid() ) appendItem(all, QString("%1").arg( tag ), strtag);
							break;
			}
			file.at(oldPos); // Restore old position
		}
	}
	appendItem(general, "Archive Offset", (storepos + size) );
    }

    return true;
}

#include "kfile_rpm.moc"
