#include "msdWidget.h"

#include <QFileDialog>
#include <QFont>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTextStream>
#include <QVBoxLayout>

#include "msdscript_core/env.h"
#include "msdscript_core/expr.h"
#include "msdscript_core/parse.h"
#include "msdscript_core/val.h"

MsdWidget::MsdWidget(QWidget *parent) : QWidget(parent) {
    buildWidgets();
    buildLayout();

    connect(openFileB, &QPushButton::clicked, this, [this]() { openFile(); });
    connect(submitB, &QPushButton::clicked, this, [this]() { submit(); });
    connect(resetB, &QPushButton::clicked, this, [this]() { resetForm(); });

    setWindowTitle("MSDscript UI");
    resize(1200, 900);
}

void MsdWidget::buildWidgets() {
    QFont labelFont;
    labelFont.setPointSize(15);

    QFont controlFont;
    controlFont.setPointSize(15);

    expressionL = new QLabel("Expression:");
    resultL = new QLabel("Result:");
    expressionL->setFont(labelFont);
    resultL->setFont(labelFont);
    expressionL->setMinimumWidth(110);
    resultL->setMinimumWidth(110);

    expressionT = new QTextEdit();
    expressionT->setPlaceholderText("Enter an MSDscript expression, or paste one from a file.");
    expressionT->setFont(controlFont);
    expressionT->setMinimumHeight(310);

    resultT = new QTextEdit();
    resultT->setReadOnly(true);
    resultT->setFont(controlFont);
    resultT->setMinimumHeight(310);

    interpR = new QRadioButton("Interp");
    prettyPrintR = new QRadioButton("Pretty Print");
    interpR->setFont(controlFont);
    prettyPrintR->setFont(controlFont);
    interpR->setChecked(true);

    modeButtons = new QButtonGroup(this);
    modeButtons->addButton(interpR);
    modeButtons->addButton(prettyPrintR);

    openFileB = new QPushButton("Open File");
    submitB = new QPushButton("Submit");
    resetB = new QPushButton("Reset");
    openFileB->setFont(controlFont);
    submitB->setFont(controlFont);
    resetB->setFont(controlFont);
    openFileB->setFixedSize(170, 42);
    submitB->setFixedSize(170, 42);
    resetB->setFixedSize(170, 42);
}

void MsdWidget::buildLayout() {
    auto *modeLayout = new QVBoxLayout();
    modeLayout->setContentsMargins(18, 16, 18, 16);
    modeLayout->setSpacing(12);
    modeLayout->addWidget(interpR);
    modeLayout->addWidget(prettyPrintR);
    modeLayout->addStretch();

    auto *modeBox = new QGroupBox();
    modeBox->setFlat(true);
    modeBox->setLayout(modeLayout);
    modeBox->setMinimumHeight(96);

    auto *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(18);
    buttonRow->addWidget(openFileB);
    buttonRow->addWidget(submitB);
    buttonRow->addStretch();

    auto *resetRow = new QHBoxLayout();
    resetRow->setSpacing(18);
    resetRow->addWidget(resetB);
    resetRow->addStretch();

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(26, 22, 26, 22);
    mainLayout->setSpacing(16);

    auto *expressionRow = new QHBoxLayout();
    expressionRow->setSpacing(12);
    expressionRow->addWidget(expressionL);
    expressionRow->addWidget(expressionT, 1);

    auto *resultRow = new QHBoxLayout();
    resultRow->setSpacing(12);
    resultRow->addWidget(resultL);
    resultRow->addWidget(resultT, 1);

    mainLayout->addLayout(expressionRow, 3);
    mainLayout->addWidget(modeBox);
    mainLayout->addLayout(buttonRow);
    mainLayout->addLayout(resultRow, 3);
    mainLayout->addLayout(resetRow);
    setLayout(mainLayout);
}

void MsdWidget::openFile() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open MSDscript Expression",
        QString(),
        "Text Files (*.txt *.msd);;All Files (*)");
    if (path.isEmpty()) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        resultT->setPlainText("Unable to open file.");
        return;
    }

    QTextStream in(&file);
    expressionT->setPlainText(in.readAll());
}

void MsdWidget::submit() {
    try {
        QString input = expressionT->toPlainText();
        std::string exprString = input.toUtf8().constData();
        PTR(Expr) expr = parse_str(exprString);

        QString output;
        if (interpR->isChecked()) {
            output = QString::fromStdString(expr->interp(Env::empty)->to_string());
        } else {
            output = QString::fromStdString(expr->to_pretty_string());
        }

        resultT->setPlainText(output);
    } catch (std::runtime_error &ex) {
        resultT->setPlainText(QString::fromStdString(ex.what()));
    }
}

void MsdWidget::resetForm() {
    expressionT->clear();
    resultT->clear();
    interpR->setChecked(true);
}
