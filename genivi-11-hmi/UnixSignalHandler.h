#ifndef UNIXSIGNALHANDLER_H
#define UNIXSIGNALHANDLER_H

#include <QObject>

class UnixSignalHandler : public QObject
{
    Q_OBJECT
public:
    UnixSignalHandler();

signals:
    void interruptRequested();
    void terminateRequested();
};

#endif // UNIXSIGNALHANDLER_H
