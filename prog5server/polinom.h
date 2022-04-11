#ifndef TPOLINOM_H
#define TPOLINOM_H

#include <QWidget>
#include <iostream>
#include <math.h>       /* sqrt */

//#include "rational.h"
//typedef TRational number;

using namespace std;

enum EPrintMode{
    EPrintModeClassic,
    EPrintModeCanonic,
};
template <class number>
class TPolinom
{
    EPrintMode printMode;
    number a,b,c;
    string first_sign, second_sign;
    number D, first_root, second_root;
public:

    TPolinom(number, number, number);
    template <class T>
    friend ostream& operator<<(ostream&, TPolinom<T>&);
    template <class T>
    friend QString& operator<<(QString&, TPolinom<T>&);

    number value(number);
    void setPrintMode(EPrintMode);
    void findRoot();
    number get_first_root();
    number get_second_root();
    number get_D();

};

#endif // TPOLINOM_H
template <class number>
TPolinom<number>::TPolinom(number x, number y, number z)
{
    printMode = EPrintModeClassic;
    a = x;
    b = y;
    c = z;
    if (b >= 0){
            first_sign = '+';
        }
        else{
            first_sign = "";
        }
        if (c >=  0){
            second_sign = '+';
        }
        else{
            second_sign = "";
        }
    this->findRoot();
}
template <class number>
ostream& operator<< (ostream& os, TPolinom<number>& p){
    if (p.printMode == EPrintModeClassic){
        //os<<p.a<<"x^2+"<<p.b<<"x+"<<p.c;
        os<<p.a<<"x^2"<<p.first_sign<<p.b<<"x"<<p.second_sign<<p.c;

    }
    else{
        //p.findRoot();
        if (p.D < 0){
            os<<"No roots";
        }
        else{
            os<<p.a<<"(x-"<<p.first_root<<")"<<"(x-"<<p.second_root<<")";
        }
    }
    return os;
}
template <class number>
QString& operator<< (QString& os, TPolinom<number>& p){
    if (p.printMode == EPrintModeClassic){
        //os<<p.a<<"x^2+"<<p.b<<"x+"<<p.c;
        os<<p.a;
        os+="x^2";
        os+= QString::fromStdString(p.first_sign);
        os<<p.b;
        os+="x";
        os+= QString::fromStdString(p.second_sign);
        os<<p.c;

    }
    else{
        p.findRoot();
        if (p.D < 0){
            os += "No roots";
        }
        else{
            os<<p.a;
            os+="(x-";
            os<<p.first_root;
            os+=")(x-";
            os<<p.second_root;
            os+=")";
        }
    }
    return os;
}
template <class number>
number TPolinom<number>::value(number x){
    return a*x*x + b*x + c;
}
template <class number>
void  TPolinom<number>::setPrintMode(EPrintMode m){
   printMode = m;
}
template <class number>
void TPolinom<number>::findRoot(){
    D = b*b-a*c*4;
    if (D >= 0) {
        first_root = (-b-sqrt(D))/(a*2);
        second_root = (-b+sqrt(D))/(a*2);
        if (!(a*first_root*first_root+b*first_root+c == 0 && a*first_root*first_root+b*first_root+c == 0)){
            D = -1; //На случай если истинные корни не принадлежат заданному множеству
        }
    }

}
template <class number>

number TPolinom<number>::get_D(){
    return D;
}
template <class number>
number TPolinom<number>::get_first_root(){
    return first_root;
}
template <class number>
number TPolinom<number>::get_second_root(){
    return second_root;
}
