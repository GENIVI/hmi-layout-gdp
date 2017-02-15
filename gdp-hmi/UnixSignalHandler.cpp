#include "UnixSignalHandler.h"

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <QSocketNotifier>
#include <QDebug>

static int sigIntFd[2];
static int sigTermFd[2];

static void intSignalHandler(int)
{
    char a = 1;
    ::write(sigIntFd[0], &a, sizeof(a));
}

static void termSignalHandler(int)
{
    char a = 1;
    ::write(sigTermFd[0], &a, sizeof(a));
}

UnixSignalHandler::UnixSignalHandler()
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigIntFd))
       qFatal("Couldn't create INT socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigTermFd))
       qFatal("Couldn't create TERM socketpair");

    auto intNotifier = new QSocketNotifier(sigIntFd[1], QSocketNotifier::Read, this);
    connect(intNotifier, SIGNAL(activated(int)), this, SIGNAL(interruptRequested()));

    auto termNotifier = new QSocketNotifier(sigTermFd[1], QSocketNotifier::Read, this);
    connect(termNotifier, SIGNAL(activated(int)), this, SIGNAL(terminateRequested()));

    static struct sigaction interrupt, term;

    interrupt.sa_handler = intSignalHandler;
    sigemptyset(&interrupt.sa_mask);
    interrupt.sa_flags = 0;
    interrupt.sa_flags |= SA_RESTART;
    sigaction(SIGINT, &interrupt, 0);

    term.sa_handler = termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;
    sigaction(SIGTERM, &term, 0);
}
