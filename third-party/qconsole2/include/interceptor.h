//
// Author: Kuba Ober <kuba@mareimbrium.org>
// Downloaded from: http://www.ibib.waw.pl/~winnie
//
// License:  Public domain
//

#if ! defined(INLUDED_INTERCEPTOR_H)
#define INCLUDED_INTERCEPTOR_H

#include <qobject.h>

class QTextIStream;
class QSocketNotifier;

class Interceptor : public QObject
{
    Q_OBJECT
    public:
    Interceptor(QObject * p = 0);
    ~Interceptor();
    public slots:
    void initialize(int outFd = 1); // defaults to standard output
    void received();
    void finish();
    QTextIStream *textIStream() {return m_stream;};
    signals:
    void received(QTextIStream *);
    private:
    QTextIStream * m_stream;
    QSocketNotifier * m_notifier;
    int m_pipeFd[2];
    int m_origFd;
    int m_origFdCopy;
};

#endif
