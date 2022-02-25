#include "qtrecorder.h"
#include "ui_qtrecorder.h"

QtRecorder::QtRecorder(QWidget *parent)
    : QMainWindow(parent)
    , origin(0,0)
    , rubberband(nullptr)
    , ui(new Ui::QtRecorder)
    , w_state(0)
    , size_x(0)
    , size_y(0)
    , off_x(0)
    , off_y(0)
    , recorder(nullptr)
    , rec_video(false)
    , rec_audio(false)
    , crop(false)
{
    ui->setupUi(this);
    w_title = this->findChild<QLabel *>("t_label");
    w_description = this->findChild<QLabel *>("d_label");
    video_check = this->findChild<QCheckBox *>("checkBox");
    audio_check = this->findChild<QCheckBox *>("checkBox_2");
    crop_check = this->findChild<QCheckBox *>("checkBox_3");
    next_button = this->findChild<QPushButton *>("nav_btn");
    start_button = this->findChild<QPushButton *>("start_btn");
    pause_button = this->findChild<QPushButton *>("pause_btn");
    stop_button = this->findChild<QPushButton *>("stop_btn");

    screenshot_label = this->findChild<QLabel *>("display_screen");
    filename_label = this->findChild<QLabel *>("label_3");
    filename_box = this->findChild<QLineEdit *>("lineEdit");
    screenshot_label->hide();
    start_button->hide();
    pause_button ->hide();
    stop_button ->hide();
    video_check->hide();
    audio_check->hide();
    crop_check->hide();
    crop_check->setEnabled(false);
    crop_check->setChecked(false);
    filename_label->hide();
    filename_box->hide();
}

void QtRecorder::startWizard(){
    this->setWindowTitle("QtRecorder - capture settings");
    w_title->show();
    w_description->show();
    next_button->show();
    next_button->setText("Next >");

    if((rec_audio || rec_video) && filename_box->text() != "") next_button->setEnabled(true);
    else next_button->setEnabled(false);   w_title->setText("Quick Settings");
     w_description->setText("These few options will allow you to customize your recording session in a very simple way!");
     video_check->show();
   audio_check->show();
   crop_check->show();
   start_button->hide();
   pause_button ->hide();
   stop_button ->hide();
   filename_label->show();
   filename_box->show();
   if(crop) crop_check->setText(crop_check->text() + " "+ QString::number(size_x) + ":" + QString::number(size_y) + "+(offset)" +  QString::number(off_x) + ":" + QString::number(off_y));
   screenshot_label->hide();
   if(rubberband!=nullptr) rubberband->hide();
   video_check->setChecked(rec_video);
   audio_check->setChecked(rec_audio);
   crop_check->setChecked(crop);
   filename_box->setText(filename);
   w_state = 1;
}

void QtRecorder::mousePressEvent(QMouseEvent *event){
    if(w_state != 2) return;
    origin = event->pos();
    QPoint last;
    //check the starting point is inside the picture
    if(event->pos().x() < screenshot_label->pos().x())
        last.setX( screenshot_label->pos().x());
    else if(event->pos().x() > screenshot_label->pos().x() + screenshot_label->width())
        last.setX( screenshot_label->pos().x()+ screenshot_label->width());
    else
        last.setX(event->pos().x());

    if(event->pos().y() < screenshot_label->pos().y())
        last.setY( screenshot_label->pos().y());
    else if( event->pos().y() > screenshot_label->pos().y() + screenshot_label->height())
        last.setY( screenshot_label->pos().y()+ screenshot_label->height());
    else
        last.setY(event->pos().y());
    origin = last;
    if (!rubberband)
        rubberband = new QRubberBand(QRubberBand::Rectangle, this);
    rubberband->setGeometry(QRect(last, QSize()));
    rubberband->show();
}

void QtRecorder::mouseMoveEvent(QMouseEvent *event)
{
    if(w_state != 2 || rubberband == nullptr) return;

    QPoint last;
    //limit the rubberband max size to do not exceed the image
    if(event->pos().x() < screenshot_label->pos().x())
        last.setX( screenshot_label->pos().x()-1);
    else if(event->pos().x() > screenshot_label->pos().x() + screenshot_label->width())
        last.setX( screenshot_label->pos().x()+ screenshot_label->width()-1);
    else
        last.setX(event->pos().x()-1);

    if(event->pos().y() < screenshot_label->pos().y())
        last.setY( screenshot_label->pos().y()-1);
    else if( event->pos().y() > screenshot_label->pos().y() + screenshot_label->height())
        last.setY( screenshot_label->pos().y()+ screenshot_label->height()-1);
    else
        last.setY(event->pos().y()-1);
    rubberband->setGeometry(QRect(origin, last).normalized());
}

void QtRecorder::mouseReleaseEvent(QMouseEvent *event)
{
    if(w_state!=2 || rubberband == nullptr) return;

    //compute the relative size
    size_x = (qreal)rubberband->width() / screenshot_label->width();
    size_y = (qreal)rubberband->height() / screenshot_label->height();

    //compute the relative offset
    off_x = abs((rubberband->pos().x()-(qreal)screenshot_label->pos().x())/(qreal)screenshot_label->width());
    off_y = abs((rubberband->pos().y()-(qreal)screenshot_label->pos().y())/(qreal)screenshot_label->height());
    startWizard();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}

QtRecorder::~QtRecorder()
{
    delete ui;
}

void QtRecorder::ready_to_record(){
    //set ready to record state
    w_state = 3;
    //reuse next_button to go back to settings
    next_button->setText("< Back");
    w_title->setText("Ready to record!");
    w_description->setText("Please use the following buttons to control the screen capture");
    video_check->hide();
    audio_check->hide();
    crop_check->hide();
    filename_label->hide();
    filename_box->hide();
    start_button->show();
    pause_button->show();
    stop_button->show();
    pause_button->setEnabled(false);
    stop_button->setEnabled(false);
}

void QtRecorder::on_nav_btn_clicked()
{
 switch(w_state){
    case 0:
        startWizard();
     break;
    case 1:
        //if button clickable, then settings have been validated => no need for validation here
        ready_to_record();
     break;
     case 3:
        //start wizard again;
        startWizard();
     break;
    default:
     return;
 }
}

void QtRecorder::crop_procedure(){
    //crop procedure
    //hide the previous elements
    w_title->hide();
    w_description->hide();
    video_check->hide();
    audio_check->hide();
    crop_check->hide();
    next_button->hide();
    filename_label->hide();
    filename_box->hide();

    w_state = 2;
    //screenshot the screen
    QScreen *screen;
    QPixmap qpx_pixmap;

    screenshot_label->setAlignment(Qt::AlignCenter);
    screen = this->screen();
    last_size = this->size();
    //this->showMinimized();
    qpx_pixmap = screen->grabWindow(0);
    this->setWindowTitle("QtRecorder - select which area to capture");

    screenshot_label->setFixedSize(this->size()*0.95);
    screenshot_label->setPixmap(qpx_pixmap.scaled(screenshot_label->size(), Qt::KeepAspectRatioByExpanding));
    screenshot_label->show();

   // this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    //this->setParent(0);
    //this->setAttribute(Qt::WA_NoSystemBackground, true);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void QtRecorder::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0){ //disabled
        crop_check->setEnabled(false);
        crop_check->setChecked(false);
        rec_video = false;
    }
    else{
        crop_check->setEnabled(true);
        rec_video = true;
    }
    if((rec_audio || rec_video) && filename_box->text() != "") next_button->setEnabled(true);
    else next_button->setEnabled(false);

}

void QtRecorder::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == 0){ //disabled
        rec_audio = false;
    }
    else{
        rec_audio = true;
    }
    if((rec_audio || rec_video) && filename_box->text() != "") next_button->setEnabled(true);
    else next_button->setEnabled(false);
}

void QtRecorder::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1 == 0){ //disabled
        crop = false;
        crop_check->setText("Record only a portion of the screen");
    }
    else{
        crop = true;
        crop_procedure();
    }
}

void QtRecorder::on_lineEdit_textChanged(const QString &arg1)
{
    QString fil = arg1;
    fil.replace(" ", "_");
    filename_box->setText(fil);
    filename = fil;

    if((rec_audio || rec_video) && arg1!= ""){
        next_button->setEnabled(true);
    }
    else next_button->setEnabled(false);
}

void QtRecorder::on_start_btn_clicked() {
    //instantiate the recorder
    SRConfiguration config;
    config.enable_audio = rec_audio;
    config.enable_video = rec_video;
    filename += ".mp4";
    config.filename = filename.toStdString();
    config.enable_crop = crop;
    if(crop) {
        config.crop_info.dimension.height.den = 1;
        config.crop_info.dimension.height.num = (double)size_y;
        config.crop_info.dimension.width.den = 1;
        config.crop_info.dimension.width.num =(double)size_x;
        config.crop_info.offset.x.den = 1;
        config.crop_info.offset.x.num = (double)off_x;
        config.crop_info.offset.y.den = 1;
        config.crop_info.offset.y.num = (double)off_y;
    }
    try {
        if (recorder == nullptr)
            recorder = new SRRecorder(config);
        recorder->initCapture();
        recorder->startCapture();
        start_button->setEnabled(false);
        pause_button->setEnabled(true);
        stop_button->setEnabled(true);
        next_button->setEnabled(false);
    }catch(SRException &e_){
        showError(e_);
    }


}

void QtRecorder::on_stop_btn_clicked() {
    try {
        recorder->stopCaputure();
        delete recorder;
        recorder = nullptr;
        start_button->setEnabled(true);
        pause_button->setEnabled(false);
        stop_button->setEnabled(false);
        next_button->setEnabled(true);
    }catch(SRException &e_){
        showError(e_);
    }

}

void QtRecorder::on_pause_btn_clicked() {
    try {
        stop_button->setEnabled(true);
        start_button->setEnabled(false);
        if(recorder->isRecording()){
            pause_button->setText("RESUME CAPTURE");
            recorder->pauseCapture();
        }
        else if(recorder->isPaused()){
            pause_button->setText("PAUSE CAPTURE");
            recorder->startCapture();
        }

    }catch (SRException &e_){
        showError(e_);
    }
}

void QtRecorder::showError(SRException &e){
    auto *err = new QErrorMessage();
    err->showMessage("An error has been encountered! Please verify your settings before trying again.\n" + QString(e.what()));
}
