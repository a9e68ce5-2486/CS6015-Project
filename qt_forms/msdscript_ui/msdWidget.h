#ifndef MSDWIDGET_H
#define MSDWIDGET_H

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QWidget>

class MsdWidget : public QWidget {
public:
    explicit MsdWidget(QWidget *parent = nullptr);

private:
    QLabel *expressionL;
    QLabel *resultL;
    QTextEdit *expressionT;
    QTextEdit *resultT;
    QRadioButton *interpR;
    QRadioButton *prettyPrintR;
    QPushButton *openFileB;
    QPushButton *submitB;
    QPushButton *resetB;
    QButtonGroup *modeButtons;

    void buildWidgets();
    void buildLayout();
    void openFile();
    void submit();
    void resetForm();
};

#endif
