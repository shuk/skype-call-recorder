/*
	Skype Call Recorder
	Copyright (C) 2008 jlh (jlh at gmx dot ch)

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation; either version 2 of the License, version 3 of
	the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	The GNU General Public License version 2 is included with the source of
	this program under the file name COPYING.  You can also get a copy on
	http://www.fsf.org/
*/

#ifndef CALL_H
#define CALL_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QSet>

class QStringList;
class Skype;
class AudioFileWriter;
class QTcpServer;
class QTcpSocket;

typedef int CallID;

class Call : QObject {
	Q_OBJECT
public:
	Call(Skype *, const QString &, CallID id);
	~Call();
	void startRecording();
	void stopRecording();

private:
	QString getFileName() const;

private:
	Skype *skype;
	QString skypeName;
	CallID id;
	AudioFileWriter *writer;
	bool isRecording;

	QTcpServer *serverLocal, *serverRemote;
	QTcpSocket *socketLocal, *socketRemote;
	QByteArray bufferLocal, bufferRemote;

private slots:
	void acceptLocal();
	void acceptRemote();
	void readLocal();
	void readRemote();
	void checkConnections();
	void tryToWrite(bool = false);

private:
	// disabled
	Call(const Call &);
	Call &operator=(const Call &);
};

class CallHandler {
public:
	CallHandler(Skype *);
	void callCmd(const QStringList &);
	void closeAll();

private:
	QString getObject(const QString &);

private:
	typedef QMap<CallID, Call *> CallMap;
	typedef QSet<CallID> CallSet;

	CallMap calls;
	CallSet ignore;
	Skype *skype;
};

#endif

