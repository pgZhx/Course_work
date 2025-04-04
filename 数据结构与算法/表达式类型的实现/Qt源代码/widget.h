#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<iostream>
#include<string>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
struct Bitree{
    char data;
    Bitree* left;
    Bitree* right;
    Bitree(char a):data(a),left(nullptr),right(nullptr){}
    ~Bitree();
};
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_one_clicked();

    void on_two_clicked();

    void on_three_clicked();

    void on_four_clicked();

    void on_five_clicked();

    void on_six_clicked();

    void on_seven_clicked();

    void on_eight_clicked();

    void on_nine_clicked();

    void on_plus_clicked();

    void on_sub_clicked();

    void on_mult_clicked();

    void on_div_clicked();

    void on_zero_clicked();

    void on_eq_clicked();

    void on_del_clicked();

    void on_clear_clicked();

    void on_miyunsuan_clicked();

    void on_Q1_clicked();

    void on_W1_clicked();

    void on_E1_clicked();

    void on_R1_clicked();

    void on_T1_clicked();

    void on_Y1_clicked();

    void on_U1_clicked();

    void on_zero_9_clicked();

    void on_O1_clicked();

    void on_P1_clicked();

    void on_A1_2_clicked();

    void on_S1_clicked();

    void on_D1_clicked();

    void on_F1_clicked();

    void on_G1_clicked();

    void on_H1_clicked();

    void on_J1_clicked();

    void on_K1_clicked();

    void on_L1_clicked();

    void on_Z1_clicked();

    void on_X1_clicked();

    void on_C1_clicked();

    void on_V1_clicked();

    void on_B1_clicked();

    void on_N1_clicked();

    void on_M1_clicked();

    void on_fun1_clicked();



    void on_fun3_clicked();

    void on_fun2_clicked();

    void on_fun4_clicked();

    void on_SPace_clicked();

private:
    Ui::Widget *ui;
    QString expression;
    Bitree* build(string& expr, int&index);
    int getOperPri(char& op);
    bool isdigit(char& a);
    void printorder(Bitree* &root);
    bool hasDegreeOneNode(Bitree* root);
    Bitree* Root;
};
#endif // WIDGET_H
