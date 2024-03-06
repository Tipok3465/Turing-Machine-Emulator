#include "App.h"

App::App(int width, int height) {
    window_ = new QMainWindow();
    window_->setFixedSize(width, height);
    window_->setStyleSheet("QMainWindow {"
                           "background: #b5a8c2 }");
    bg_ = new QLabel(window_);
    bg_->resize(1200, 700);
    bg_->setPixmap(QPixmap("../Bg.png"));

    tape_data_.resize(2e5, "λ");
    timer = new QTimer(window_);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextStep()));

    stop_timer_ = new QTimer(window_);
    connect(stop_timer_, SIGNAL(timeout()), this, SLOT(Stop()));

    coloring_timer = new QTimer(window_);
    connect(coloring_timer,SIGNAL(timeout()), this, SLOT(updTablet()));

    error_timer_ = new QTimer(window_);
    connect(error_timer_,SIGNAL(timeout()), this, SLOT(hideError()));

    states_["λ"] = 0;

    name_ = new QLabel(window_);
    name_->setText("Turing Machine Emulator");
    name_->resize(1000, 55);
    name_->move(300, 20);
    name_->setStyleSheet("QLabel {"
                         "color: #fff;"
                         "}");
    name_->setFont(QFont("Rockwell", 50));
    for (int i = 0; i < 12; ++i) {
        tape_.push_back(new QLabel(window_));
        tape_[i]->resize(80, 80);
        tape_[i]->move(130+78*i, 100);
        tape_[i]->setFont(QFont("Italic", 40));
        tape_[i]->setText(tape_data_[head_on_ + i]);
        tape_[i]->setAlignment(Qt::AlignCenter);
        if (i == cur_head_) {
            tape_[i]->setStyleSheet("QLabel {"
                                    "background-color: #707070;"
                                    "border: 2px solid #eee;"
                                    "border-radius: 10px;"
                                    "color: #aaaaaa;"
                                    "}");
        } else {
            tape_[i]->setStyleSheet("QLabel {"
                                    "background-color: #404040;"
                                    "border: 2px solid #eee;"
                                    "border-radius: 10px;"
                                    "color: #aaaaaa;"
                                    "}");
        }
    }

    lstephead_ = new QPushButton(window_);
    lstephead_->resize(70, 60);
    lstephead_->move(50, 110);
    lstephead_->setFont(QFont("Times", 48));
    lstephead_->setText("⬅");
    lstephead_->setStyleSheet("QPushButton { background: #9684a8;"
                          "color: #fff;"
                          "border-radius: 5px;"
                          "text-align: bottom;"
                          "}");
    connect(lstephead_, SIGNAL (released()), this, SLOT (stepHeadLeft()));


    rstephead_ = new QPushButton(window_);
    rstephead_->resize(70, 60);
    rstephead_->move(1078, 110);
    rstephead_->setFont(QFont("Times", 46));
    rstephead_->setText("⮕");
    rstephead_->setStyleSheet("QPushButton { background: #9684a8;"
                              "color: #fff;"
                              "border-radius: 5px;"
                              "text-align: bottom;"
                              "}");
    connect(rstephead_, SIGNAL (released()), this, SLOT (stepHeadRight()));

    start_ = new QPushButton(window_);
    start_->resize(60, 60);
    start_->move(465, 190);
    start_->setFont(QFont("Times", 30));
    start_->setText("▶");
    start_->setStyleSheet("QPushButton { background: #9684a8;"
                           "color: #fff;"
                           "border-radius: 5px;"
                           "text-align: center;"
                           "}");
    connect(start_, SIGNAL (released()), this, SLOT (Start()));

    nextstep_ = new QPushButton(window_);
    nextstep_->resize(60, 60);
    nextstep_->move(535, 190);
    nextstep_->setFont(QFont("Times", 50));
    nextstep_->setText("⏯");
    nextstep_->setStyleSheet("QPushButton { background: #9684a8;"
                          "color: #fff;"
                          "border-radius: 5px;"
                          "text-align: bottom;"
                          "}");
    connect(nextstep_, SIGNAL (released()), this, SLOT (nextStep()));

    pause_ = new QPushButton(window_);
    pause_->resize(60, 60);
    pause_->move(605, 190);
    pause_->setFont(QFont("Times", 50));
    pause_->setText("⏸");
    pause_->setStyleSheet("QPushButton { background: #9684a8;"
                             "color: #fff;"
                             "border-radius: 5px;"
                             "text-align: bottom;"
                             "}");
    connect(pause_, SIGNAL (released()), this, SLOT (Pause()));

    stop_ = new QPushButton(window_);
    stop_->resize(60, 60);
    stop_->move(675, 190);
    stop_->setFont(QFont("Times", 47));
    stop_->setText("◼");
    stop_->setStyleSheet("QPushButton { background: #9684a8;"
                          "color: #fff;"
                          "border-radius: 5px;"
                         "text-align: bottom;"
                          "}");
    connect(stop_, SIGNAL (released()), this, SLOT (Stop()));


    tablet_ = new QTableWidget(window_);
    updTablet();

    abcs_ = new QLabel(window_);
    abcs_->setText("Alphabets");
    abcs_->setFont(QFont("Rockwell", 20));
    abcs_->resize(200, 50);
    abcs_->move(760, 270);
    abcs_->setStyleSheet("QLabel {"
                         "color: #eeeeee;}");

    tapeabc_ = new QLineEdit(window_);
    tapeabc_->setPlaceholderText("Tape's Alphabet");
    tapeabc_->setFont(QFont("Italic", 16));
    tapeabc_->resize(200, 30);
    tapeabc_->move(780, 310);
    tapeabc_->setStyleSheet("QLineEdit { background: #9684a8;"
                            "color: #393939;"
                            "border-radius: 3px;"
                            "opacity: 1; }");

    addedabc_ = new QLineEdit(window_);
    addedabc_->setPlaceholderText("Added Alphabet");
    addedabc_->setFont(QFont("Italic", 16));
    addedabc_->resize(200, 30);
    addedabc_->move(780, 350);
    addedabc_->setStyleSheet("QLineEdit { background: #9684a8;"
                            "color: #393939;"
                            "border-radius: 3px;"
                            "opacity: 1; }");

    setabc_ = new QPushButton(window_);
    setabc_->setFont(QFont("Italic", 16));
    setabc_->setText("Set Alphabets");
    setabc_->resize(180, 40);
    setabc_->move(990, 325);
    setabc_->setStyleSheet("QPushButton { background: #404040;"
                             "color: #aaaaaa;"
                             "border-radius: 5px;"
                             "}");
    connect(setabc_, SIGNAL (released()), this, SLOT (setAlphabets()));

    words_ = new QLabel(window_);
    words_->setText("Word");
    words_->setFont(QFont("Rockwell", 20));
    words_->resize(200, 50);
    words_->move(760, 390);
    words_->setStyleSheet("QLabel {"
                         "background-color: #b5a8c2;"
                         "color: #eeeeee;}");

    word_ = new QLineEdit(window_);
    word_->setPlaceholderText("Word");
    word_->setFont(QFont("Italic", 16));
    word_->resize(200, 30);
    word_->move(780, 430);
    word_->setStyleSheet("QLineEdit { background: #9684a8;"
                             "color: #393939;"
                             "border-radius: 3px;"
                             "opacity: 1; }");

    setword_ = new QPushButton(window_);
    setword_->setFont(QFont("Italic", 16));
    setword_->setText("Set Word");
    setword_->resize(180, 30);
    setword_->move(990, 430);
    setword_->setStyleSheet("QPushButton { background: #404040;"
                           "color: #aaaaaa;"
                           "border-radius: 5px;"
                           "}");
    connect(setword_, SIGNAL (released()), this, SLOT (setWord()));

    States = new QLabel(window_);
    States->setText("States");
    States->setFont(QFont("Rockwell", 20));
    States->resize(200, 50);
    States->move(760, 470);
    States->setStyleSheet("QLabel {"
                          "background-color: #b5a8c2;"
                          "color: #eeeeee;}");

    del_state_ = new QPushButton(window_);
    del_state_->resize(30, 30);
    del_state_->move(780, 510);
    del_state_->setFont(QFont("Times", 30));
    del_state_->setText("-");
    del_state_->setStyleSheet("QPushButton { background: #404040;"
                              "color: #aaaaaa;"
                              "border-radius: 5px;"
                              "test-align: bottom;"
                              "}");
    connect(del_state_, SIGNAL (released()), this, SLOT (delStates()));

    states_cnt_ = new QLineEdit(window_);
    states_cnt_->resize(150, 30);
    states_cnt_->move(820, 510);
    states_cnt_->setFont(QFont("Times", 16));
    states_cnt_->setPlaceholderText("States count");
    states_cnt_->setStyleSheet("QLineEdit { background: #9684a8;"
                             "color: #393939;"
                             "border-radius: 3px;"
                             "}");

    add_state_ = new QPushButton(window_);
    add_state_->resize(30, 30);
    add_state_->move(980, 510);
    add_state_->setFont(QFont("Times", 30));
    add_state_->setText("+");
    add_state_->setStyleSheet("QPushButton { background: #404040;"
                            "color: #aaaaaa;"
                            "border-radius: 5px;"
                            "test-align: bottom;"
                            "}");
    connect(add_state_, SIGNAL (released()), this, SLOT (addStates()));

    set_states_ = new QPushButton(window_);
    set_states_->resize(150, 30);
    set_states_->move(1020, 510);
    set_states_->setFont(QFont("Times", 16));
    set_states_->setText("Save states");
    set_states_->setStyleSheet("QPushButton { background: #404040;"
                              "color: #aaaaaa;"
                              "border-radius: 5px;"
                              "test-align: bottom;"
                              "}");
    connect(set_states_, SIGNAL (released()), this, SLOT (setStates()));

    Speed = new QLabel(window_);
    Speed->setFont(QFont("Rockwell", 20));
    Speed->resize(200, 50);
    Speed->move(760, 550);
    Speed->setStyleSheet("QLabel {"
                          "background-color: #b5a8c2;"
                          "color: #eeeeee;"
                          "}");
    Speed->setText("Speed");

    speeddown_ = new QPushButton(window_);
    speeddown_->resize(30, 30);
    speeddown_->move(780, 590);
    speeddown_->setFont(QFont("Times", 30));
    speeddown_->setText("-");
    speeddown_->setStyleSheet("QPushButton { background: #404040;"
                              "color: #aaaaaa;"
                              "border-radius: 5px;"
                              "test-align: bottom;"
                              "}");
    connect(speeddown_, SIGNAL (released()), this, SLOT (speedDown()));

    cur_speed_ = new QLabel(window_);
    cur_speed_->setFont(QFont("Rockwell", 20));
    cur_speed_->resize(200, 50);
    cur_speed_->move(820, 585);
    cur_speed_->setStyleSheet("QLabel {"
                         "capacity: 0;"
                         "color: #eeeeee;"
                         "}");
    cur_speed_->setText(QString::fromStdString(std::to_string(per_speed_/100) + '.' + std::to_string(per_speed_%100) + 'x'));


    speedup_ = new QPushButton(window_);
    speedup_->resize(30, 30);
    speedup_->move(880, 590);
    speedup_->setFont(QFont("Times", 30));
    speedup_->setText("+");
    speedup_->setStyleSheet("QPushButton { background: #404040;"
                              "color: #aaaaaa;"
                              "border-radius: 5px;"
                              "test-align: bottom;"
                              "}");
    connect(speedup_, SIGNAL (released()), this, SLOT (speedUp()));

    head_ = new QLabel(window_);
    head_->resize(30, 30);
    head_->move(155, 70);
    head_->setFont(QFont("Times", 30));
    head_->setText("▼");
    head_->setStyleSheet("QLabel {"
                         "color: #5c4e68;"
                         "}");

    animation = new QPropertyAnimation(head_, "geometry");

    error_ = new QLabel(window_);
    error_->resize(700, 320);
    error_->move(50, 300);
    error_->setFont(QFont("Rockwell", 30));
    error_->setText("LOSER;3");
    error_->setStyleSheet("QLabel {"
                          "color: #af2222;"
                          "text-align: bottom;"
                          "}");

    error_->setFont(QFont("Rockwell", 100));
    error_->setAlignment(Qt::AlignCenter);
    error_->hide();

}

App::~App() {
    delete window_;
}

void App::frowError() {
    error_->show();
    error_timer_->start(2000);
    timer->stop();
    coloring_timer->stop();
    if (!is_start_emulate_)
        Stop();
}

void App::hideError() {
    error_->hide();
    error_timer_->stop();
}

void App::updTape() {
    for (int i = 0; i < 12; ++i) {
        tape_[i]->resize(80, 80);
        tape_[i]->move(130+78*i, 100);
        tape_[i]->setFont(QFont("Italic", 40));
        tape_[i]->setText(tape_data_[head_on_ + i]);
        tape_[i]->setAlignment(Qt::AlignCenter);
        if (i == cur_head_) {
            tape_[i]->setStyleSheet("QLabel {"
                                    "background-color: #707070;"
                                    "border: 2px solid #eee;"
                                    "border-radius: 10px;"
                                    "color: #aaaaaa;"
                                    "}");
        } else {
            tape_[i]->setStyleSheet("QLabel {"
                                    "background-color: #404040;"
                                    "border: 2px solid #eee;"
                                    "border-radius: 10px;"
                                    "color: #aaaaaa;"
                                    "}");
        }
    }
}

void App::stepHeadRight() {
    if (cur_head_ == 11) {
        cur_head_ = 8;
        head_->move(779, 70);
        head_on_ += 4;
    } else {
        cur_head_++;
        head_->move(head_->pos().x() + 78, 70);
    }
    updTape();
}

void App::animatedStepHeadRight() {
    if (cur_head_ == 11) {
        cur_head_ = 8;
        animation->setDuration(50);
        animation->setStartValue(head_->geometry());
        animation->setEndValue(QRect(779, 70, 30, 30));
        animation->start();
        head_on_ += 4;
    } else {
        cur_head_++;
        animation->setDuration(speed_ - speed_ / 5);
        animation->setStartValue(head_->geometry());
        animation->setEndValue(QRect(head_->pos().x() + 78, 70, 30, 30));
        animation->start();
    }
    updTape();
}

void App::stepHeadLeft() {
    if (cur_head_ == 0) {
        cur_head_ = 3;
        head_->move(389, 70);
        head_on_ -= 4;
    } else {
        cur_head_--;
        head_->move(head_->pos().x() - 78, 70);
    }
    updTape();
}

void App::animatedStepHeadLeft() {
    if (cur_head_ == 0) {
        cur_head_ = 3;
        animation->setDuration(50);
        animation->setStartValue(head_->geometry());
        animation->setEndValue(QRect(389, 70, 30, 30));
        animation->start();
        head_on_ -= 4;
    } else {
        cur_head_--;
        animation->setDuration(speed_ - speed_ / 5);
        animation->setStartValue(head_->geometry());
        animation->setEndValue(QRect(head_->pos().x() - 78, 70, 30, 30));
        animation->start();
    }
    updTape();
}

void App::setAlphabets() {
    buf_tablet_.resize(states_count_);
    for (int state = 0; state < states_count_; ++state) {
        int ind = 0;
        for (auto e : tape_abc_) {
            if (tablet_->item(state, states_[e]) != 0) {
                buf_tablet_[state][e] = (QString)tablet_->item(state, states_[e])->text();
                buf_tablet_is_exist_[e] = 1;
            }
        }
        if (tablet_->item(state, states_["λ"]) != 0) {
            auto it = tablet_->item(state, states_["λ"])->text();
            buf_tablet_[state][QString::fromStdString("λ")] = tablet_->item(state, states_["λ"])->text();
            buf_tablet_is_exist_["λ"] = 1;
        }
        for (auto e : added_abc_) {
            if (tablet_->item(state, states_[e]) != 0) {
                buf_tablet_[state][e] = tablet_->item(state, states_[e])->text();
                buf_tablet_is_exist_[e] = 1;
            }
        }
    }
    QString s1 = tapeabc_->text();
    QString s2 = addedabc_->text();
    bool check_added_tape_ = true;
    for (auto e : s1) {
        for (auto elem : s2) {
            check_added_tape_ = check_added_tape_ & (e != elem);
        }
    }
    if (!check_added_tape_) {
        frowError();
        updTape();
        updTablet();
        return;
    }
    head_on_ = 1e5;
    cur_head_ = 0;
    animation->setDuration(50);
    animation->setStartValue(head_->geometry());
    animation->setEndValue(QRect(155, 70, 30, 30));
    animation->start();
    updTape();
    states_.clear();
    int tape_abc_cnt_ = 0;
    bool check = true;
    for (auto e : s1){
        for (auto elem : tape_abc_) {
            tape_abc_cnt_ += e == elem;
        }
    }
    check = check && (tape_abc_cnt_ == tape_abc_.size());
    int ind = 0;
    tape_abc_.resize(0);
    for (auto e : s1) {
        tape_abc_.push_back(e);
        states_[(QString)e] = ind++;
    }
    states_["λ"] = ind++;
    tapeabc_->clear();
    int added_abc_cnt = 0;
    for (auto e : s2){
        for (auto elem : added_abc_) {
            added_abc_cnt += e == elem;
        }
    }
    check = check && (added_abc_cnt == added_abc_.size());
    added_abc_.resize(0);
    for (auto e : s2) {
        states_[(QString)e] = ind++;
        added_abc_.push_back(e);
    }
    addedabc_->clear();
    std::unordered_set<QString> tape_abc_set, added_abc_set;
    for (auto e : tape_abc_) {
        tape_abc_set.insert(e);
    }
    for (auto e : added_abc_) {
        added_abc_set.insert(e);
    }
    bool alp_check = (tape_abc_set.size() == tape_abc_.size());
    alp_check = alp_check && (added_abc_set.size() == added_abc_.size());
    if (!alp_check) {
        frowError();
        tape_data_.resize(2e5, "λ");
        tablet_->clear();
        buf_tablet_.clear();
        buf_tablet_is_exist_.clear();
        tape_abc_.clear();
        added_abc_.clear();
        updTablet();
        return;
    }
    if (!check) {
        tape_data_.clear();
        tape_data_.resize(2e5, "λ");
        tablet_->clear();
        buf_tablet_.clear();
        buf_tablet_is_exist_.clear();
        updTablet();
    } else {
        tablet_->clear();
        updTablet();
        for (int state = 0; state < states_count_; ++state) {
            for (int i = 0; i < tape_abc_.size(); ++i) {
                if (buf_tablet_is_exist_[tape_abc_[i]]) {
                    tablet_->setItem(state, i, new QTableWidgetItem(buf_tablet_[state][tape_abc_[i]]));
                }
            }
            if (buf_tablet_is_exist_["λ"])
                tablet_->setItem(state, tape_abc_.size(), new QTableWidgetItem(buf_tablet_[state]["λ"]));
            for (int i = 0; i < added_abc_.size(); ++i) {
                if (buf_tablet_is_exist_[added_abc_[i]])
                    tablet_->setItem(state, i + tape_abc_.size() + 1, new QTableWidgetItem(buf_tablet_[state][added_abc_[i]]));
            }
        }
    }
}

void App::setWord() {
    tape_data_.clear();
    tape_data_.resize(2e5, "λ");
    QString s = word_->text();
    for (int i = head_on_ + cur_head_; i < head_on_ + cur_head_ + s.size(); ++i) {
        bool check = false;
        for (auto e : tape_abc_) {
            check = check || (s[i - head_on_ - cur_head_] == e[0]);
        }
        if (!check) {
            frowError();
            word_->clear();
            return;
        }
    }
    hideError();
    for (int i = head_on_ + cur_head_; i < head_on_ + cur_head_ + s.size(); ++i) {
        tape_data_[i] = s[i - head_on_ - cur_head_];
    }
    updTape();
    word_->clear();
}

void App::updTablet() {
    tablet_->resize(700, 350);
    tablet_->move(50, 270);
    tablet_->setStyleSheet("QTableWidget {"
                           "background-color: #5c4e68;"
                           "gridline-color: #fff;"
                           "}");
    tablet_->setColumnCount(tape_abc_.size() + 1 + added_abc_.size());
    tablet_->setRowCount(states_count_);
    for (int i = 0; i < tablet_->columnCount();++i) {
        if (i < tape_abc_.size())
            tablet_->setHorizontalHeaderItem(i, new QTableWidgetItem(tape_abc_[i]));
        else if (i == tape_abc_.size())
            tablet_->setHorizontalHeaderItem(i, new QTableWidgetItem("λ"));
        else
            tablet_->setHorizontalHeaderItem(i, new QTableWidgetItem(added_abc_[i - tape_abc_.size() - 1]));
    }
    for (int i = 0; i < states_count_; ++i) {
        std::string  s = std::to_string(i);
        tablet_->setVerticalHeaderItem(i, new QTableWidgetItem(QString::fromStdString("q" + s)));
    }
}

void App::delStates() {
   if (states_count_ > 1) {
       --states_count_;
   }
    updTablet();
}

void App::setStates() {
    int count = states_cnt_->text().toInt();
    if (count <= 0)
        return;
    states_count_ = count;
    states_cnt_->clear();
    updTablet();
}

void App::addStates() {
    states_count_++;
    updTablet();
}

void App::Start() {
    timer->start(speed_);
}
void App::Pause() {
    timer->stop();
    tablet_->setEnabled(true);
    coloring_timer->stop();
}

void App::nextStep() {
    coloring_timer->start(speed_ - 75);
    if (is_start_emulate_) {
        if (!Check()) {
            frowError();
            return;
        }
        tablet_->setEnabled(false);
        buf_cur_head_ = cur_head_;
        buf_head_on_ = head_on_;
        buf_tape_ = tape_;
        buf_tape_data_ = tape_data_;
        buf_x = head_->pos().x();
        is_start_emulate_ = false;
    }
    if (prefrow != -1 && prefcol != -1) {
        tablet_->item(prefrow, prefcol)->setBackground(QColor("#5c4e68"));
    }
    tablet_->item(cur_state_, states_[tape_data_[head_on_ + cur_head_]])->setBackground(QColor("#a89dae"));
    prefrow = cur_state_;
    prefcol = states_[tape_data_[head_on_ + cur_head_]];
    QString s = tablet_->item(cur_state_, states_[tape_data_[head_on_ + cur_head_]])->text();
    for (auto e : s) {
        if (e == 'R') {
            animatedStepHeadRight();
        } else if (e == 'L') {
            animatedStepHeadLeft();
        } else if (e == '!') {
            cur_state_ = 0;
            Pause();
            updTape();
        } else if ('0' <= e && e <= '9') {
            cur_state_ = e.toLatin1() - '0';
        } else {
            tape_data_[head_on_ + cur_head_] = s[0];
        }
    }
}

 void App::Stop() {
    tablet_->setEnabled(true);
    animation->setDuration(50);
    animation->setStartValue(head_->geometry());
    animation->setEndValue(QRect(155 + 78 * buf_cur_head_, 70, 30, 30));
    animation->start();
    cur_head_ = buf_cur_head_;
    head_on_ = buf_head_on_;
    tape_ = buf_tape_;
    tape_data_ = buf_tape_data_;
    is_start_emulate_ = true;
    cur_state_ = 0;
    timer->stop();
    coloring_timer->stop();
    head_->move(buf_x, 70);
    stop_timer_->stop();
    if (prefrow != -1 && prefcol != -1) {
        tablet_->item(prefrow, prefcol)->setBackground(QColor("#5c4e68"));
    }
    updTablet();
    updTape();
}

void App::speedDown() {
    if (per_speed_ > 25) {
        per_speed_ -= 25;
        speed_ = 300 / (per_speed_ / 100.0);
        cur_speed_->setText(QString::fromStdString(std::to_string(per_speed_/100) + '.' + std::to_string(per_speed_%100) + 'x'));

    }
}

void App::speedUp() {
    if (speed_ > 50) {
        per_speed_ += 25;
        speed_ = 300 / (per_speed_ / 100.0);
        cur_speed_->setText(QString::fromStdString(std::to_string(per_speed_/100) + '.' + std::to_string(per_speed_%100) + 'x'));
    }
}

bool App::Check() {
    bool stop_check = false;
    bool abc_check = true;
    for (int row = 0; row < states_count_; ++row) {
        for (int col = 0; col < tape_abc_.size() + 1 + added_abc_.size(); ++col) {
            if (tablet_->item(row, col) != 0) {
                QString s = tablet_->item(row, col)->text();
                for (auto e : s) {
                    stop_check = stop_check || (e == '!');
                    bool sym_check = false;
                    for (auto elem : tape_abc_) {
                        sym_check = sym_check || (e == elem) || (e == '!') || (e == 'R') || (e == 'L');
                    }
                    sym_check = sym_check || (e == QString::fromStdString("λ") || (e == '!') || (e == 'R') || (e == 'L'));
                    for (auto elem : added_abc_) {
                        sym_check = sym_check || (e == elem) || (e == '!') || (e == 'R') || (e == 'L');
                    }
                    sym_check = sym_check || ('0' <= e && e <= '9');
                    abc_check = abc_check && sym_check;
                }
            }
        }
    }
    return abc_check && stop_check;
}
