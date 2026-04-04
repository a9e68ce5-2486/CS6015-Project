#include "mainWidget.h"

#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    buildWidgets();
    buildLayout();

    connect(refreshB, &QPushButton::clicked, this, [this]() { fillSummary(); });
    connect(finishB, &QPushButton::clicked, this, [this]() { clearForm(); });
    connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() { clearForm(); });

    setWindowTitle("HelloWorld");
    resize(780, 950);
}

void MainWidget::buildWidgets() {
    QFont labelFont;
    labelFont.setPointSize(16);

    QFont controlFont;
    controlFont.setPointSize(15);

    firstNameL = new QLabel("First Name");
    lastNameL = new QLabel("Last Name");
    ageL = new QLabel("Age");
    genderL = new QLabel("Gender");
    firstNameL->setFont(labelFont);
    lastNameL->setFont(labelFont);
    ageL->setFont(labelFont);
    genderL->setFont(labelFont);

    firstNameE = new QLineEdit();
    lastNameE = new QLineEdit();
    firstNameE->setFont(controlFont);
    lastNameE->setFont(controlFont);
    firstNameE->setFixedSize(255, 44);
    lastNameE->setFixedSize(255, 44);

    ageS = new QSpinBox();
    ageS->setRange(0, 130);
    ageS->setFont(controlFont);
    ageS->setFixedSize(120, 44);

    maleR = new QRadioButton("Male");
    femaleR = new QRadioButton("Female");
    maleR->setFont(controlFont);
    femaleR->setFont(controlFont);

    genderBox = new QGroupBox();
    genderButtons = new QButtonGroup(this);
    genderButtons->setExclusive(true);
    genderButtons->addButton(maleR);
    genderButtons->addButton(femaleR);

    refreshB = new QPushButton("Refresh");
    finishB = new QPushButton("Finish");
    refreshB->setFont(controlFont);
    finishB->setFont(controlFont);
    refreshB->setFixedSize(140, 48);
    finishB->setMinimumHeight(48);

    summaryT = new QTextEdit();
    summaryT->setReadOnly(true);
    summaryT->setFont(controlFont);
    summaryT->setMinimumHeight(380);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(finishB, QDialogButtonBox::AcceptRole);
}

void MainWidget::buildLayout() {
    auto *genderLayout = new QVBoxLayout();
    genderLayout->setContentsMargins(10, 4, 10, 4);
    genderLayout->setSpacing(16);
    genderLayout->addWidget(maleR);
    genderLayout->addWidget(femaleR);
    genderLayout->addStretch();
    genderBox->setLayout(genderLayout);
    genderBox->setFlat(true);

    auto *gridLayout = new QGridLayout();
    gridLayout->addWidget(firstNameL, 0, 0);
    gridLayout->addWidget(firstNameE, 0, 1);
    gridLayout->addItem(new QSpacerItem(48, 10), 0, 2, 1, 1);
    gridLayout->addWidget(ageL, 0, 3);
    gridLayout->addWidget(ageS, 0, 4);

    gridLayout->addWidget(lastNameL, 1, 0);
    gridLayout->addWidget(lastNameE, 1, 1);

    gridLayout->addWidget(genderL, 2, 0);
    gridLayout->addWidget(genderBox, 2, 1);

    gridLayout->addWidget(refreshB, 3, 0);
    gridLayout->setColumnMinimumWidth(0, 150);
    gridLayout->setColumnMinimumWidth(1, 270);
    gridLayout->setColumnMinimumWidth(3, 70);
    gridLayout->setColumnMinimumWidth(4, 120);
    gridLayout->setRowMinimumHeight(2, 120);
    gridLayout->setHorizontalSpacing(20);
    gridLayout->setVerticalSpacing(22);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(34, 28, 34, 18);
    mainLayout->setSpacing(18);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(summaryT, 1);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

QString MainWidget::selectedGender() const {
    if (maleR->isChecked()) {
        return "Male";
    }
    if (femaleR->isChecked()) {
        return "Female";
    }
    return "Unspecified";
}

void MainWidget::fillSummary() {
    QString content;
    content += "First Name: " + firstNameE->text() + "\n";
    content += "Last Name: " + lastNameE->text() + "\n";
    content += "Age: " + QString::number(ageS->value()) + "\n";
    content += "Gender: " + selectedGender();
    summaryT->setText(content);
}

void MainWidget::clearForm() {
    firstNameE->clear();
    lastNameE->clear();
    ageS->setValue(0);
    summaryT->clear();

    genderButtons->setExclusive(false);
    maleR->setChecked(false);
    femaleR->setChecked(false);
    genderButtons->setExclusive(true);
}
