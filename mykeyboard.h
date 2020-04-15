#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QMap>
namespace Ui {
class mykeyboard;
}


class mykeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit mykeyboard(QWidget *parent = nullptr);
    ~mykeyboard();
    void InitTabOrder(void);
    void Init_MAP_Key_value(void);
public:
    QMap<int,QPushButton *>keyboardmap;
    bool keycapsmode = false;
public:
    void set_editTips(QString title = "键入xx的内容：");
    void set_editBox(QString str = "2020");
private:
    void set_capsLockmode(bool enable);
    void writeABC(int index);
signals:
    void editisModifiedok(QString);
private slots:

    void on_key_caps_clicked();

    void on_key_1_clicked();

    void on_key_del_clicked();

    void on_key_ok_clicked();

    void on_key_2_clicked();

    void on_key_3_clicked();

    void on_key_4_clicked();

    void on_key_5_clicked();

    void on_key_6_clicked();

    void on_key_7_clicked();

    void on_key_8_clicked();

    void on_key_9_clicked();

    void on_key_0_clicked();

    void on_key_q_clicked();

    void on_key_w_clicked();

    void on_key_e_clicked();

    void on_key_r_clicked();

    void on_key_t_clicked();

    void on_key_y_clicked();

    void on_key_u_clicked();

    void on_key_i_clicked();

    void on_key_o_clicked();

    void on_key_p_clicked();

    void on_key_a_clicked();

    void on_key_s_clicked();

    void on_key_d_clicked();

    void on_key_f_clicked();

    void on_key_g_clicked();

    void on_key_h_clicked();

    void on_key_j_clicked();

    void on_key_k_clicked();

    void on_key_l_clicked();

    void on_key_z_clicked();

    void on_key_x_clicked();

    void on_key_c_clicked();

    void on_key_v_clicked();

    void on_key_b_clicked();

    void on_key_n_clicked();

    void on_key_m_clicked();

    void on_key_at_clicked();

    void on_key_point_clicked();

private:
    Ui::mykeyboard *ui;

};

#endif // MYKEYBOARD_H
