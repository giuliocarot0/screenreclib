#ifndef QTRECORDER_H
#define QTRECORDER_H

#include <QMainWindow>
#include <QRubberBand>
#include <QMouseEvent>
#include <QCheckBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QPixmap>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class QtRecorder; }
QT_END_NAMESPACE

class QtRecorder : public QMainWindow
{
    Q_OBJECT

public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QtRecorder(QWidget *parent = nullptr);
    ~QtRecorder();
    void welcomePage();


private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_nav_btn_clicked();

private:
    QLabel *filename_label;
    QLabel *w_title;
    QLabel *w_description;
    QLabel *screenshot_label;
    QLineEdit *filename_box;
    QCheckBox *audio_check;
    QCheckBox *video_check;
    QCheckBox *crop_check;
    QPoint origin;
    QPushButton *next_button, *start_button, *pause_button, *stop_button;
    QRubberBand *rubberband;
    Ui::QtRecorder *ui;
    void closeWelcomePage();
    void startWizard();
    void crop_procedure();
    void end_crop_procedure();
    void ready_to_record();
    int w_state; //should be 0 for welcome, 1 for settings, 2 for cropping
    qreal size_x;
    qreal size_y;
    qreal off_x;
    qreal off_y;
    //settings
    bool rec_video;
    bool rec_audio;
    bool crop;
    QString filename;
    QSize last_size;

};
#endif // QTRECORDER_H
