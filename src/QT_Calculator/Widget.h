#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
extern int syntaxErrorFlag;
extern QVector<QString> historyArray;
extern QString currentText;
extern int historyPointer;
namespace Ui {
class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void keyPressEvent(QKeyEvent *event);
    void addToMainText(QString addText);
    void clearMainText();
    void backspaceMainText();
    QString checkMainText();
    void processFormula(QString tempText);
    bool judgePriority(QString operatorOld, QString operatorNew);
    int judgePriorityNumber(QString tempOperator);
    void displayFormula(QString tempText);
    void upList();
    void downList();
public slots:
    void processSlot();
private:
    Ui::Widget *ui;
}; 
#endif // WIDGET_H
