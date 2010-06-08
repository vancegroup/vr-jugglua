//
// Author: Kuba Ober <kuba@mareimbrium.org>
// Downloaded from: http://www.ibib.waw.pl/~winnie
//
// License:  Public domain
//

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>

#include <qtextstream.h>
#include <qsocketnotifier.h>
#include "interceptor.h"

using namespace std;

Interceptor::Interceptor(QObject * parent) :
QObject(parent),
    m_stream(0),
    m_notifier(0)
{
}

Interceptor::~Interceptor()
{
    finish();
}

void Interceptor::initialize(int outFd)
{           
    if (m_notifier != 0) finish();

    m_origFd = outFd;
   
    // m_pipefd[0] is the read-end of the pipe and m_pipefd[1] is the write-end.

    // Open a new pipe. We will pipe output file descriptor through the new pipe and
    // read it with a QTextIStream object, using a QSocketNotifier to tell us when
    // something is waiting in the pipe.
    int rc = ::pipe(m_pipeFd);
    assert (rc >= 0);

    // Save the original output descriptor.
    m_origFdCopy = ::dup(m_origFd);
    assert(m_origFdCopy >= 0);
   
    // Make the ouput descriptor a copy of pipe's write end
    rc = ::dup2(m_pipeFd[1], m_origFd);
    assert (rc >= 0);
    ::close(m_pipeFd[1]); // close the write end of the pipe descriptor, it's redundant now

    // Open the pipe's read end non-blocking so that we can reliably get the data
    rc = ::fcntl(m_pipeFd[0], F_GETFL);
    assert(rc != -1);
    rc = ::fcntl(m_pipeFd[0], F_SETFL, rc | O_NONBLOCK); // otherwise atEnd() will block!
    assert(rc != -1);
    FILE * f = fdopen(m_pipeFd[0], "r");
    assert(f != 0);

    if (m_stream != 0) delete m_stream;
    if (m_notifier != 0) delete m_notifier;
    m_stream = new QTextIStream(f);
    m_notifier = new QSocketNotifier(m_pipeFd[0], QSocketNotifier::Read);
    QObject::connect(m_notifier, SIGNAL(activated(int)), SLOT(received()));
}

void Interceptor::received()
{
    emit received(m_stream);
}

void Interceptor::finish()
{
    if (m_notifier == 0) return;
   
    std::cout.flush(); // flush standard output cout file descriptor
    ::fflush(NULL);    // flush all file buffers
    ::fsync(1);        // syncronize standard output buffers -- may be unnessessery
    received();        // process whatever data is left there
   
    // Restore original state
    delete m_notifier;
    delete m_stream;
    m_notifier = 0;
    m_stream = 0;
   
    ::dup2(m_origFdCopy, m_origFd); // restore the output descriptor
    ::close(m_origFdCopy); // close the copy as it's redundant now
    ::close(m_pipeFd[0]);  // close the reading end of the pipe
}
