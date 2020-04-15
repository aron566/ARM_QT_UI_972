#include "mykeyboard.h"
#include "ui_mykeyboard.h"
#include <QKeyEvent>
mykeyboard::mykeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mykeyboard)
{
    ui->setupUi(this);
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);
    //聚焦排序
    InitTabOrder();
    //映射按键对象
    Init_MAP_Key_value();
    //设置透明度
//    setWindowOpacity(0.5);
}

void mykeyboard::InitTabOrder(void)
{
#if USE_DEFAULT_FRONT_KEYLAYOUT
    mykeyboard::setTabOrder(ui->key_at, ui->key_point);
    mykeyboard::setTabOrder(ui->key_point, ui->key_caps);
    mykeyboard::setTabOrder(ui->key_caps, ui->key_del);
    mykeyboard::setTabOrder(ui->key_del, ui->key_ok);

    mykeyboard::setTabOrder(ui->key_1, ui->key_2);
    mykeyboard::setTabOrder(ui->key_2, ui->key_3);
    mykeyboard::setTabOrder(ui->key_3, ui->key_4);
    mykeyboard::setTabOrder(ui->key_4, ui->key_5);
    mykeyboard::setTabOrder(ui->key_5, ui->key_6);
    mykeyboard::setTabOrder(ui->key_6, ui->key_7);
    mykeyboard::setTabOrder(ui->key_7, ui->key_8);
    mykeyboard::setTabOrder(ui->key_8, ui->key_9);
    mykeyboard::setTabOrder(ui->key_9, ui->key_0);
    mykeyboard::setTabOrder(ui->key_0, ui->key_q);
    mykeyboard::setTabOrder(ui->key_q, ui->key_w);
    mykeyboard::setTabOrder(ui->key_w, ui->key_e);
    mykeyboard::setTabOrder(ui->key_e, ui->key_r);
    mykeyboard::setTabOrder(ui->key_r, ui->key_t);
    mykeyboard::setTabOrder(ui->key_t, ui->key_y);
    mykeyboard::setTabOrder(ui->key_y, ui->key_u);
    mykeyboard::setTabOrder(ui->key_u, ui->key_i);
    mykeyboard::setTabOrder(ui->key_i, ui->key_o);
    mykeyboard::setTabOrder(ui->key_o, ui->key_p);
    mykeyboard::setTabOrder(ui->key_p, ui->key_a);
    mykeyboard::setTabOrder(ui->key_a, ui->key_s);
    mykeyboard::setTabOrder(ui->key_s, ui->key_d);
    mykeyboard::setTabOrder(ui->key_d, ui->key_f);
    mykeyboard::setTabOrder(ui->key_f, ui->key_g);
    mykeyboard::setTabOrder(ui->key_g, ui->key_h);
    mykeyboard::setTabOrder(ui->key_j, ui->key_k);
    mykeyboard::setTabOrder(ui->key_k, ui->key_l);
    mykeyboard::setTabOrder(ui->key_l, ui->key_z);
    mykeyboard::setTabOrder(ui->key_z, ui->key_x);
    mykeyboard::setTabOrder(ui->key_x, ui->key_c);
    mykeyboard::setTabOrder(ui->key_c, ui->key_v);
    mykeyboard::setTabOrder(ui->key_v, ui->key_b);
    mykeyboard::setTabOrder(ui->key_b, ui->key_n);
    mykeyboard::setTabOrder(ui->key_n, ui->key_m);
#else
    mykeyboard::setTabOrder(ui->key_at, ui->key_point);
    mykeyboard::setTabOrder(ui->key_point, ui->key_1);
    mykeyboard::setTabOrder(ui->key_1, ui->key_2);
    mykeyboard::setTabOrder(ui->key_2, ui->key_3);
    mykeyboard::setTabOrder(ui->key_3, ui->key_4);
    mykeyboard::setTabOrder(ui->key_4, ui->key_5);
    mykeyboard::setTabOrder(ui->key_5, ui->key_6);
    mykeyboard::setTabOrder(ui->key_6, ui->key_7);
    mykeyboard::setTabOrder(ui->key_7, ui->key_8);
    mykeyboard::setTabOrder(ui->key_8, ui->key_9);
    mykeyboard::setTabOrder(ui->key_9, ui->key_0);
    mykeyboard::setTabOrder(ui->key_0, ui->key_q);
    mykeyboard::setTabOrder(ui->key_q, ui->key_w);
    mykeyboard::setTabOrder(ui->key_w, ui->key_e);
    mykeyboard::setTabOrder(ui->key_e, ui->key_r);
    mykeyboard::setTabOrder(ui->key_r, ui->key_t);
    mykeyboard::setTabOrder(ui->key_t, ui->key_y);
    mykeyboard::setTabOrder(ui->key_y, ui->key_u);
    mykeyboard::setTabOrder(ui->key_u, ui->key_i);
    mykeyboard::setTabOrder(ui->key_i, ui->key_o);
    mykeyboard::setTabOrder(ui->key_o, ui->key_p);
    mykeyboard::setTabOrder(ui->key_p, ui->key_a);
    mykeyboard::setTabOrder(ui->key_a, ui->key_s);
    mykeyboard::setTabOrder(ui->key_s, ui->key_d);
    mykeyboard::setTabOrder(ui->key_d, ui->key_f);
    mykeyboard::setTabOrder(ui->key_f, ui->key_g);
    mykeyboard::setTabOrder(ui->key_g, ui->key_h);
    mykeyboard::setTabOrder(ui->key_j, ui->key_k);
    mykeyboard::setTabOrder(ui->key_k, ui->key_l);
    mykeyboard::setTabOrder(ui->key_l, ui->key_z);
    mykeyboard::setTabOrder(ui->key_z, ui->key_x);
    mykeyboard::setTabOrder(ui->key_x, ui->key_c);
    mykeyboard::setTabOrder(ui->key_c, ui->key_v);
    mykeyboard::setTabOrder(ui->key_v, ui->key_b);
    mykeyboard::setTabOrder(ui->key_b, ui->key_n);
    mykeyboard::setTabOrder(ui->key_n, ui->key_m);
    mykeyboard::setTabOrder(ui->key_m, ui->key_caps);
    mykeyboard::setTabOrder(ui->key_caps, ui->key_del);
    mykeyboard::setTabOrder(ui->key_del, ui->key_ok);
#endif
}

mykeyboard::~mykeyboard()
{
    delete ui;
}

void mykeyboard::Init_MAP_Key_value(void)
{
    keyboardmap.insert('a',ui->key_a);
    keyboardmap.insert('b',ui->key_b);
    keyboardmap.insert('c',ui->key_c);
    keyboardmap.insert('d',ui->key_d);
    keyboardmap.insert('e',ui->key_e);
    keyboardmap.insert('f',ui->key_f);
    keyboardmap.insert('g',ui->key_g);
    keyboardmap.insert('h',ui->key_h);
    keyboardmap.insert('i',ui->key_i);
    keyboardmap.insert('j',ui->key_j);
    keyboardmap.insert('k',ui->key_k);
    keyboardmap.insert('l',ui->key_l);
    keyboardmap.insert('m',ui->key_m);
    keyboardmap.insert('n',ui->key_n);
    keyboardmap.insert('o',ui->key_o);
    keyboardmap.insert('p',ui->key_p);
    keyboardmap.insert('q',ui->key_q);
    keyboardmap.insert('r',ui->key_r);
    keyboardmap.insert('s',ui->key_s);
    keyboardmap.insert('t',ui->key_t);
    keyboardmap.insert('u',ui->key_u);
    keyboardmap.insert('v',ui->key_v);
    keyboardmap.insert('w',ui->key_w);
    keyboardmap.insert('x',ui->key_x);
    keyboardmap.insert('y',ui->key_y);
    keyboardmap.insert('z',ui->key_z);
}

void mykeyboard::set_editTips(QString title)
{
    this->ui->keyboardtitlelabel->setText(title);
}
void mykeyboard::set_editBox(QString str)
{
   this->ui->keyboardeditbox->setText(str);
}

void mykeyboard::set_capsLockmode(bool enable)
{
    if(enable == true)
    {
        for(int i = 'a';i <= 'z';i++)
        {
            keyboardmap[i]->setText(keyboardmap[i]->text().toUpper());
        }
    }
    else
    {
        for(int i = 'a';i <= 'z';i++)
        {
            keyboardmap[i]->setText(keyboardmap[i]->text().toLower());
        }
    }
}
void mykeyboard::on_key_caps_clicked()
{
    if(!keycapsmode)
    {
        //转为大写字母模式
        set_capsLockmode(true);
        keycapsmode = true;
    }
    else{
        //转为大写字母模式
        set_capsLockmode(false);
        keycapsmode = false;
    }
}

void mykeyboard::on_key_del_clicked()
{
    ui->keyboardeditbox->backspace();
}

void mykeyboard::on_key_ok_clicked()
{
    this->hide();
    emit editisModifiedok(ui->keyboardeditbox->text());
}
void mykeyboard::on_key_at_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'@');
}

void mykeyboard::on_key_point_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'.');
}
void mykeyboard::on_key_1_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'1');
}
void mykeyboard::on_key_2_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'2');
}

void mykeyboard::on_key_3_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'3');
}

void mykeyboard::on_key_4_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'4');
}

void mykeyboard::on_key_5_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'5');
}

void mykeyboard::on_key_6_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'6');
}

void mykeyboard::on_key_7_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'7');
}

void mykeyboard::on_key_8_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'8');
}

void mykeyboard::on_key_9_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'9');
}

void mykeyboard::on_key_0_clicked()
{
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+'0');
}

void mykeyboard::writeABC(int index)
{
    QString str;
    str = keyboardmap[index]->text();
    ui->keyboardeditbox->setText(ui->keyboardeditbox->text()+str);
}
void mykeyboard::on_key_q_clicked()
{
    writeABC('q');
}

void mykeyboard::on_key_w_clicked()
{
    writeABC('w');
}

void mykeyboard::on_key_e_clicked()
{
    writeABC('e');
}

void mykeyboard::on_key_r_clicked()
{
    writeABC('r');
}

void mykeyboard::on_key_t_clicked()
{
    writeABC('t');
}

void mykeyboard::on_key_y_clicked()
{
    writeABC('y');
}

void mykeyboard::on_key_u_clicked()
{
    writeABC('u');
}

void mykeyboard::on_key_i_clicked()
{
    writeABC('i');
}

void mykeyboard::on_key_o_clicked()
{
    writeABC('o');
}

void mykeyboard::on_key_p_clicked()
{
    writeABC('p');
}

void mykeyboard::on_key_a_clicked()
{
    writeABC('a');
}

void mykeyboard::on_key_s_clicked()
{
    writeABC('s');
}

void mykeyboard::on_key_d_clicked()
{
    writeABC('d');
}

void mykeyboard::on_key_f_clicked()
{
    writeABC('f');
}

void mykeyboard::on_key_g_clicked()
{
    writeABC('g');
}

void mykeyboard::on_key_h_clicked()
{
    writeABC('h');
}

void mykeyboard::on_key_j_clicked()
{
    writeABC('j');
}

void mykeyboard::on_key_k_clicked()
{
    writeABC('k');
}

void mykeyboard::on_key_l_clicked()
{
    writeABC('l');
}

void mykeyboard::on_key_z_clicked()
{
    writeABC('z');
}

void mykeyboard::on_key_x_clicked()
{
    writeABC('x');
}

void mykeyboard::on_key_c_clicked()
{
    writeABC('c');
}

void mykeyboard::on_key_v_clicked()
{
    writeABC('v');
}

void mykeyboard::on_key_b_clicked()
{
    writeABC('b');
}

void mykeyboard::on_key_n_clicked()
{
    writeABC('n');
}

void mykeyboard::on_key_m_clicked()
{
    writeABC('m');
}


