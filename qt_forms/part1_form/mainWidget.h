#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

class MainWidget : public QWidget {
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    QLabel *firstNameL;
    QLabel *lastNameL;
    QLabel *ageL;
    QLabel *genderL;
    QLineEdit *firstNameE;
    QLineEdit *lastNameE;
    QSpinBox *ageS;
    QRadioButton *maleR;
    QRadioButton *femaleR;
    QPushButton *refreshB;
    QPushButton *finishB;
    QTextEdit *summaryT;
    QDialogButtonBox *buttonBox;
    QGroupBox *genderBox;
    QButtonGroup *genderButtons;

    void buildWidgets();
    void buildLayout();
    void fillSummary();
    void clearForm();
    QString selectedGender() const;
};

#endif
