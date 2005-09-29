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
#include <qbuffer.h>
#include <kfilterdev.h>
#include <qregexp.h>
#include <karchive.h>
#include <ktar.h>
#include <kar.h>

#include "kfile_deb.h"

typedef KGenericFactory<KDebPlugin> DebFactory;

K_EXPORT_COMPONENT_FACTORY(kfile_deb, DebFactory( "kfile_deb" ))

KDebPlugin::KDebPlugin(QObject *parent, const char *name,
                       const QStringList &args)
    
    : KFilePlugin(parent, name, args)
{
    KFileMimeTypeInfo* info = addMimeTypeInfo( "application/x-deb" );
    KFileMimeTypeInfo::GroupInfo* group = 0L;
    group = addGroupInfo(info, "General", i18n("General"));
    KFileMimeTypeInfo::ItemInfo* item;

    item = addItemInfo(group, "Name", i18n("Name"), QVariant::String);
    item = addItemInfo(group, "Version", i18n("Version"), QVariant::String);
    item = addItemInfo(group, "Summary", i18n("Summary"), QVariant::String);
    item = addItemInfo(group, "Size", i18n("Size"), QVariant::Int);
}

bool KDebPlugin::readInfo( KFileMetaInfo& info, uint /*what*/)
{
    KAr debfile (info.path());
  
    if ( !debfile.open( IO_ReadOnly ) ) {
        kdDebug(7034) << "Couldn't open " << QFile::encodeName(info.path()) << endl;
        return false;    
    }

    const KArchiveDirectory* debdir = debfile.directory();
    const KArchiveEntry* controlentry = debdir->entry( "control.tar.gz" );
    if ( !controlentry || !controlentry->isFile() ) {
        kdWarning(7034) << "control.tar.gz not found" << endl;
        return false;
    }

    QIODevice* filterDev = KFilterDev::device( static_cast<const KArchiveFile *>( controlentry )->device(), "application/x-gzip" );
    if ( !filterDev ) {
        kdWarning(7034) << "Couldn't create filter device for control.tar.gz" << endl;
        return false;
    }
    KTar tarfile( filterDev );

    if ( !tarfile.open( IO_ReadOnly ) ) {
        kdWarning(7034) << "Couldn't open control.tar.gz" << endl;
        return false;
    }

    const KArchiveDirectory* controldir = tarfile.directory();
    Q_ASSERT( controldir );
    
    const KArchiveEntry* controlfile = controldir->entry( "control" );
    
    Q_ASSERT( controlfile );
    if (controlfile) {
        KFileMetaInfoGroup group = appendGroup(info, "General");	
        QByteArray control( static_cast<const KArchiveFile *>(controlfile)->data() );
    
        // Now process control array
        QBuffer controldev(control);
        controldev.open( IO_ReadOnly );
        while (!controldev.atEnd()) {
            char linebuf[100];
            controldev.readLine(linebuf, sizeof( linebuf ));
            QString line(linebuf);
            int fieldstart = line.find(QRegExp(":"), 0) + 2;
            if (fieldstart == 1) break;
            QString fieldname = line.mid(0, fieldstart - 2);
            QString fielddata = line.mid(fieldstart, line.length() - fieldstart - 1);

            if (fieldname == "Package") appendItem(group, "Name", fielddata);
            else if (fieldname == "Version") appendItem(group, "Version", fielddata);
            else if (fieldname == "Description") appendItem(group, "Summary", fielddata);
            else if (fieldname == "Installed-Size") appendItem(group, "Size", int(fielddata.toInt()));

            kdDebug(7034) << "Name: [" << fieldname << "] Data: [" << fielddata << "]" << endl;
        }
    } else {
        kdDebug(7034) << "Couldn't read control file" << endl;
        return false;
    }
    
    tarfile.close();    
    debfile.close();

    return true;
}

#include "kfile_deb.moc"
