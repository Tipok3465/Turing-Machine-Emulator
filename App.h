#include "header.h"

class App : public QMainWindow {
    Q_OBJECT

public:
    App(int width, int height);
    ~App() override;

    void render() {
        window_->show();
    }

private slots:
    void stepHeadRight();
    void stepHeadLeft();
    void updTape();
    void setAlphabets();
    void setWord();
    void frowError();
    void hideError();
    void updTablet();
    void delStates();
    void setStates();
    void addStates();
    void Start();
    void nextStep();
    void Pause();
    void Stop();
    void speedUp();
    void speedDown();
    void animatedStepHeadLeft();
    void animatedStepHeadRight();
    bool Check();
private:
    int head_on_ = 1e5, buf_head_on_ = 1e5;
    int cur_head_ = 0, buf_cur_head_ = 0;
    int cur_state_ = 0;
    int buf_x = 155;
    int states_count_ = 1;
    int speed_ = 300;
    int per_speed_ = 100;
    int prefrow = -1, prefcol = -1;
    bool is_start_emulate_ = true;
    std::vector<QString> tape_data_, buf_tape_data_;
    std::vector<QString> tape_abc_, added_abc_;
    std::map<QString, int> states_;
    std::vector <std::map<QString, QString>> buf_tablet_;
    std::map <QString, int> buf_tablet_is_exist_;
    std::vector<QLabel*> tape_, buf_tape_;
    QWidget* window_;
    QLabel* bg_;
    QLabel* error_;
    QLabel* name_;
    QPushButton* rstephead_;
    QPushButton* lstephead_;

    QPushButton* start_;
    QPushButton* nextstep_;
    QPushButton* pause_;
    QPushButton* stop_;

    QTimer* error_timer_;
    QTimer* stop_timer_;

    QTableWidget* tablet_;
    QLabel* abcs_;
    QLineEdit* tapeabc_;
    QLineEdit* addedabc_;
    QPushButton* setabc_;
    QLabel* words_;
    QLineEdit* word_;
    QLabel* States;
    QPushButton* add_state_;
    QLineEdit* states_cnt_;
    QPushButton* del_state_;
    QPushButton* set_states_;
    QPushButton* setword_;
    QTimer* timer;
    QPropertyAnimation* animation;
    QLabel* Speed;
    QPushButton* speeddown_;
    QLabel* cur_speed_;
    QPushButton* speedup_;
    QTimer* coloring_timer;
    QLabel* head_;
};