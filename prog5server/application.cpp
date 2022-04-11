#include "application.h"
#include "polinom.h"
#include "rational.h"

#include "common.h"
QString& operator<<(QString& s, int c){
    s += QString().setNum(c);

    return s;
}
QByteArray& operator>>(QByteArray& arr, int& c)
{
    int p = arr.indexOf(separator.toLatin1());
    //p = arr.indexOf(separator.toLatin1(), p+1);
    if (p > 0)
    {
        c = atoi(arr.left(p));
        arr = arr.right(arr.length()-p-1);
    }
    return arr;
}

TApplication::TApplication(int argc, char *argv[])
            : QCoreApplication(argc,argv)
{
    TCommParams pars = { QHostAddress("127.0.0.1"), 10000,
                         QHostAddress("127.0.0.1"), 10001};
    comm = new TCommunicator(pars, this);

    connect(comm,SIGNAL(recieved(QByteArray)),this,SLOT(recieve(QByteArray)));
}

void TApplication::recieve(QByteArray msg)
{
    QString answer, s;
    int pos = msg.indexOf(separator.toLatin1());
    int mode = msg.left(pos).toInt();
    msg = msg.right(msg.length()-pos-1);
    if (mode == R_MODE){
        TRational  a, b, c, x, v;
        msg>>a>>b>>c;
        TPolinom<TRational> p(a,b,c);
        pos = msg.indexOf(separator.toLatin1());
        int t = msg.left(pos).toInt();
        switch (t)
        {
            case VALUE_REQUEST:
                msg = msg.right(msg.length()-pos-1);
                msg>>x;
                v = p.value(x);
                s<<(QString)x<<(QString)v;
                answer<<QString().setNum(VALUE_ANSWER);
                answer += s;
                break;
            case PRINT_CLASSIC_REQUEST:
                p.setPrintMode(EPrintModeClassic);
                s<<p;
                answer<<QString().setNum(PRINT_ANSWER)<<s;
                break;

            case PRINT_CANONIC_REQUEST:
                p.setPrintMode(EPrintModeCanonic);
                s<<p;
                answer<<QString().setNum(PRINT_ANSWER)<<s;
                break;

            case ROOT_REQUEST:
                //p.findRoot();
                if (p.get_D() < 0){
                    s+=QString("No roots");
                }
                else{
                    s+=QString("x1 = " + p.get_first_root() + " x2 = " + p.get_second_root());
                }

                answer<<QString().setNum(PRINT_ROOT)<<s;
                break;
            default: return;
            }
    }
    else if (mode == I_MODE){
        int  a, b, c, x, v;
        msg>>a>>b>>c;
        TPolinom<int> p(a,b,c);
        pos = msg.indexOf(separator.toLatin1());
        int t = msg.left(pos).toInt();
        switch (t)
        {
            case VALUE_REQUEST:
                msg = msg.right(msg.length()-pos-1);
                msg>>x;
                v = p.value(x);
                s<<QString().setNum(x)<<QString().setNum(v);
                answer<<QString().setNum(VALUE_ANSWER);
                answer += s;
                break;
            case PRINT_CLASSIC_REQUEST:
                p.setPrintMode(EPrintModeClassic);
                s<<p;
                answer<<QString().setNum(PRINT_ANSWER)<<s;
                break;

            case PRINT_CANONIC_REQUEST:
                p.setPrintMode(EPrintModeCanonic);
                s<<p;
                answer<<QString().setNum(PRINT_ANSWER)<<s;
                break;

            case ROOT_REQUEST:
                //p.findRoot();
                if (p.get_D() < 0){
                    s+=QString("No roots");
                }
                else{

                    s+=QString("x1 = " + QString().setNum(p.get_first_root()) +
                               " x2 = " + QString().setNum(p.get_second_root()));
                }

                answer<<QString().setNum(PRINT_ROOT)<<s;
                break;
            default: return;
            }
    }
    comm->send(answer.toUtf8());
}
