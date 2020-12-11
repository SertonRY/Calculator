#include "Widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QStack>
#include <QKeyEvent>
#include <QVector>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //fixed size
    Widget::setFixedSize(400,420);

    //connect signals and slots with add
    connect(ui->Button_0,&QPushButton::clicked,[this](){
        this->addToMainText("0");
    });
    connect(ui->Button_1,&QPushButton::clicked,[this](){
        this->addToMainText("1");
    });
    connect(ui->Button_2,&QPushButton::clicked,[this](){
        this->addToMainText("2");
    });
    connect(ui->Button_3,&QPushButton::clicked,[this](){
        this->addToMainText("3");
    });
    connect(ui->Button_4,&QPushButton::clicked,[this](){
        this->addToMainText("4");
    });
    connect(ui->Button_5,&QPushButton::clicked,[this](){
        this->addToMainText("5");
    });
    connect(ui->Button_6,&QPushButton::clicked,[this](){
        this->addToMainText("6");
    });
    connect(ui->Button_7,&QPushButton::clicked,[this](){
        this->addToMainText("7");
    });
    connect(ui->Button_8,&QPushButton::clicked,[this](){
        this->addToMainText("8");
    });
    connect(ui->Button_9,&QPushButton::clicked,[this](){
        this->addToMainText("9");
    });
    connect(ui->Button_plus,&QPushButton::clicked,[this](){
        this->addToMainText("+");
    });
    connect(ui->Button_subtract,&QPushButton::clicked,[this](){
        this->addToMainText("-");
    });
    connect(ui->Button_multiply,&QPushButton::clicked,[this](){
        this->addToMainText("*");
    });
    connect(ui->Button_divide,&QPushButton::clicked,[this](){
        this->addToMainText("/");
    });
    connect(ui->Button_dot,&QPushButton::clicked,[this](){
        this->addToMainText(".");
    });
    connect(ui->Button_leftBracket,&QPushButton::clicked,[this](){
        this->addToMainText("(");
    });
    connect(ui->Button_rightBracket,&QPushButton::clicked,[this](){
        this->addToMainText(")");
    });
    //connect signals and slots with delete
    connect(ui->Button_clear,&QPushButton::clicked,[this](){
        this->clearMainText();
        syntaxErrorFlag = 0;
        historyPointer = -1;
        ui->Bar_progress->setValue(0);
        ui->Bar_progress->setStyleSheet(
                    "QProgressBar::chunk {}");
        ui->Label_status->setText("Idle");
    });
    connect(ui->Button_backspace,&QPushButton::clicked,[this](){
        this->backspaceMainText();
    });
    //connect signals and slots with check and process
    connect(ui->Button_euqal, &QPushButton::clicked, this, &Widget::processSlot);
}
Widget::~Widget()
{
    delete ui;
}
void Widget::processSlot()
{
    if(ui->Text_main->displayText().compare("") != 0)
    {
        historyArray.prepend(ui->Text_main->displayText());
        historyPointer = -1;
    }
    QString tempText;
    syntaxErrorFlag = 0;
    ui->Bar_progress->setValue(0);
    ui->Bar_progress->setStyleSheet(
                "QProgressBar::chunk {}");
    ui->Label_status->setText("Busy");
    tempText = this->checkMainText();
    if(syntaxErrorFlag == 0)
    {
        ui->Label_status->setText("Processing");
        ui->Bar_progress->setValue(92);
        tempText = tempText + "=";
        this->processFormula(tempText);
    }
    else
    {
        ui->Label_status->setText("Error!");
        ui->Bar_progress->setStyleSheet(
                    "QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
        return;
    }
}
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Enter:
            processSlot();
            break;
        case Qt::Key_Return:
            processSlot();
            break;
        case Qt::Key_Up:
            upList();
            break;
        case Qt::Key_Down:
            downList();
            break;
    }
}
void Widget::upList()
{
    if(historyArray.isEmpty())
    {
        return;
    }
    else if(historyPointer == -1)
    {
        currentText = ui->Text_main->displayText();
        ++historyPointer;
        ui->Text_main->setText(historyArray[0]);
        return;
    }
    else
    {
        if(historyPointer == historyArray.size() - 1)
        {
            return;
        }
        else
        {
            ++historyPointer;
            ui->Text_main->setText(historyArray[historyPointer]);
            return;
        }
    }
}
void Widget::downList()
{
    if(historyArray.isEmpty())
    {
        return;
    }
    else if(historyPointer == -1)
    {
        return;
    }
    else
    {
        if(historyPointer == 0)
        {
            ui->Text_main->setText(currentText);
            --historyPointer;
            return;
        }
        else
        {
            --historyPointer;
            ui->Text_main->setText(historyArray[historyPointer]);
            return;
        }
    }
}
QVector<QString> historyArray;
QString currentText;
int historyPointer;
//display with add
void Widget::addToMainText(QString addText)
{
    QString tempText;
    tempText = ui->Text_main->displayText() + addText;
    ui->Text_main->setText(tempText);
}
//display with delete
void Widget::clearMainText()
{
    ui->Text_main->clear();
}
void Widget::backspaceMainText()
{
    ui->Text_main->backspace();
}
//check the formula with a mark on extern flag
int syntaxErrorFlag = 0;
QString Widget::checkMainText()
{
    //1st auto fix syntax error(symbol dismatch)
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(0);
    QString tempText;
    tempText = ui->Text_main->displayText();
    while(tempText.indexOf(" ") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf(" "), 1, "");
    }
    ui->Bar_progress->setValue(4);
    while(tempText.indexOf("（") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("（"), 1, "(");
    }
    ui->Bar_progress->setValue(8);
    while(tempText.indexOf("）") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("）"), 1, ")");
    }
    ui->Bar_progress->setValue(12);
    while(tempText.indexOf("。") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("。"), 1, ".");
    }
    ui->Bar_progress->setValue(16);
    while(tempText.indexOf("、") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("、"), 1, ".");
    }
    ui->Bar_progress->setValue(20);
    while(tempText.indexOf("＋") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("＋"), 1, "+");
    }
    ui->Bar_progress->setValue(24);
    while(tempText.indexOf("－") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("－"), 1, "-");
    }
    ui->Bar_progress->setValue(28);
    while(tempText.indexOf("×") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("×"), 1, "*");
    }
    ui->Bar_progress->setValue(32);
    while(tempText.indexOf("x") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("x"), 1, "*");
    }
    ui->Bar_progress->setValue(36);
    while(tempText.indexOf("÷") != -1)
    {
        ui->Label_status->setText("Auto fixing");
        tempText.replace(tempText.indexOf("÷"), 1, "/");
    }
    ui->Bar_progress->setValue(40);
    //2nd auto convert constant symbols
    while(tempText.indexOf("pi", 0, Qt::CaseInsensitive) != -1)
    {
        ui->Label_status->setText("Auto converting");
        tempText.replace(tempText.indexOf("pi", 0, Qt::CaseInsensitive), 2, "3.14159");
    }
    ui->Bar_progress->setValue(44);
    while(tempText.indexOf("e", 0, Qt::CaseInsensitive) != -1)
    {
        ui->Label_status->setText("Auto converting");
        tempText.replace(tempText.indexOf("e", 0, Qt::CaseInsensitive), 1, "2.71828");
    }
    //3rd limit all characters are selected
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(48);
    QChar *data_1 = tempText.data();
    while(!data_1->isNull())
    {
        int checkCode = data_1->unicode();
        int subFlag = -1;
        QString allSupportedCharacters = "0123456789.^()+-*/";
        QChar *data_2 = allSupportedCharacters.data();
        while(!data_2->isNull())
        {
            int supportedCode = data_2->unicode();
            if(checkCode == supportedCode)
            {
                subFlag = 0;
                break;
            }
            ++data_2;
        }
        if(subFlag == -1)
        {
            QMessageBox::critical(this, "ERROR", "Unsupported characters!");
            tempText.clear();
            tempText = tempText + "0";
            syntaxErrorFlag = -1;
            return tempText;
        }
        ++data_1;
    }
    ui->Bar_progress->setValue(52);
    //4th missing bracket detection
    QStack <int> leftBracketStorage;
    QChar *data_3 = tempText.data();
    while(!data_3->isNull())
    {
        int checkCode = data_3->unicode();
        if(checkCode == 40)
        {
            leftBracketStorage.push(1);
        }
        if(checkCode == 41)
        {
            if(leftBracketStorage.empty())
            {
                QMessageBox::critical(this, "ERROR", "Missing brackets!");
                tempText.clear();
                tempText = tempText + "0";
                syntaxErrorFlag = -1;
                return tempText;
            }
            leftBracketStorage.pop();
        }
        ++data_3;
        if(data_3->isNull())
        {
            if(!leftBracketStorage.empty())
            {
                QMessageBox::critical(this, "ERROR", "Missing brackets!");
                tempText.clear();
                tempText = tempText + "0";
                syntaxErrorFlag = -1;
                return tempText;
            }
        }
    }
    ui->Bar_progress->setValue(56);
    //5th continus operators delete
    int catchOperator = 0;
    int tempDeletePosition = 0;
    QChar *data_4 = tempText.data();
    while(!data_4->isNull())
    {
        int checkCode = data_4->unicode();
        if(checkCode == 43 || checkCode == 45 || checkCode == 42 || checkCode == 47
                || checkCode == 94)
        {
            if(catchOperator == 0)
            {
                catchOperator = -1;
            }
            else if(catchOperator == -1)
            {
                tempText.remove(tempDeletePosition - 1, 1);
                ui->Label_status->setText("Auto fixing");
                continue;
            }
        }
        else if(catchOperator == -1)
        {
            catchOperator = 0;
        }
        ++data_4;
        ++tempDeletePosition;
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(60);
    //6th negtive number auto fix
    QChar *data_5 = tempText.data();
    if(!data_5->isNull())
    {
        int checkCode = data_5->unicode();
        if(checkCode == 45)
        {
            int subFlag = -1;
            ui->Label_status->setText("Auto fixing");
            tempText = "(0" + tempText;
            data_5 = tempText.data();
            data_5 +=3;
            int tempPosition = 3;
            while(!data_5->isNull())
            {
                checkCode = data_5->unicode();
                if(checkCode != 46 && (checkCode < 48 || checkCode > 57))
                {
                    tempText.insert(tempPosition, ")");
                    subFlag = 0;
                    break;
                }
                ++data_5;
                ++tempPosition;
            }
            if(subFlag == -1)
            {
                tempText = tempText.mid(1); //like "-5"-->"(0-5"-->"0-5"
            }
        }
    }
    data_5 = tempText.data();
    int tempPosition = 0;
    while(!data_5->isNull())
    {
        int checkCode = data_5->unicode();
        if(checkCode == 40)
        {
            ++data_5;
            ++tempPosition;
            checkCode = data_5->unicode();
            if(checkCode == 45)
            {
                ui->Label_status->setText("Auto fixing");
                tempText.insert(tempPosition, "(0");
                tempPosition+=3;
                data_5 = tempText.data();
                for(int i = 0; i < tempPosition; i++)
                {
                    ++data_5;
                }
                while(!data_5->isNull())
                {
                    checkCode = data_5->unicode();
                    if(!(checkCode == 46 || (checkCode >= 48 && checkCode <= 57)))
                    { 
                        tempText.insert(tempPosition, ")");
                        data_5 = tempText.data();
                        for(int i = 0; i < tempPosition; i++)
                        {
                            ++data_5;
                        }
                        break;
                    }
                    ++data_5;
                    ++tempPosition;
                }
            }
        }
        ++data_5;
        ++tempPosition;
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(64);
    //7th multiple dots auto fix
    int catchDot = 0;//0->out of range | -1->active for delete | 1->active in range
    int tempRemovePosition = 0;
    QChar *data_6 = tempText.data();
    while(!data_6->isNull())
    {
        int checkCode = data_6->unicode();
        if(checkCode >= 48 && checkCode <= 57)
        {
            if(catchDot == 0)
            {
                catchDot = 1;
            }
        }
        else if(checkCode == 46)
        {
            if(catchDot == 1)
            {
                catchDot = -1;
            }
            else if(catchDot == -1)
            {
                tempText.remove(tempRemovePosition, 1);
                ui->Label_status->setText("Auto fixing");
                continue;
            }
        }
        else
        {
            catchDot = 0;
        }
        ++data_6;
        ++tempRemovePosition;
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(68);
    //8th dot abbreviate(before and after) auto fix
    QChar *data_7 = tempText.data();
    int tempDotRemovePosition = 0;
    while(!data_7->isNull())
    {
        int checkCode = data_7->unicode();
        if(checkCode == 46)
        {
            if(tempDotRemovePosition == 0)
            {
                tempText = "0" + tempText;
                ui->Label_status->setText("Auto fixing");
                ++tempDotRemovePosition;
                data_7 = tempText.data();
                for(int i = 0; i < tempDotRemovePosition; i++)
                {
                    ++data_7;
                }
            }
            else
            {
                --data_7;
                checkCode = data_7->unicode();
                if(!(checkCode >= 48 && checkCode <= 57))
                {
                    tempText.insert(tempDotRemovePosition, "0");
                    ui->Label_status->setText("Auto fixing");
                    data_7 = tempText.data();
                    for(int i = 0; i < tempDotRemovePosition; i++)
                    {
                        ++data_7;
                    }
                    ++tempDotRemovePosition;
                }
                ++data_7;
            }
            ++data_7;
            checkCode = data_7->unicode();
            if(!(checkCode >= 48 && checkCode <= 57))
            {
                tempText.remove(tempDotRemovePosition, 1);
                ui->Label_status->setText("Auto fixing");
                data_7 = tempText.data();
                for(int i = 0; i < tempDotRemovePosition; i++)
                {
                    ++data_7;
                }
                --tempDotRemovePosition;
            }
            --data_7;
        }
        ++tempDotRemovePosition;
        ++data_7;
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(72);
    //9th empty brackets delete
    QStack <int> leftBracketStack;
    QStack <int> statusBetweenBracketsStack;
    QChar *data_8 = tempText.data();
    int tempBracketPosition = 0;
    int statusBetweenBrackets = -2;
    while(!data_8->isNull())
    {
        int checkCode = data_8->unicode();
        int subFlag = 0;
        if(checkCode == 40)
        {
            leftBracketStorage.push(tempBracketPosition);
            statusBetweenBracketsStack.push(statusBetweenBrackets);
            statusBetweenBrackets = -1;
        }
        else if(checkCode == 41)
        {
            if(statusBetweenBrackets == -1)
            {
                ui->Label_status->setText("Auto fixing");
                tempText.remove(tempBracketPosition, 1);
                tempText.remove(leftBracketStorage.pop(), 1);
                --tempBracketPosition;
                data_8 = tempText.data();
                for(int i = 0; i < tempBracketPosition; i++)
                {
                    ++data_8;
                }
                subFlag = -1;
                statusBetweenBrackets = statusBetweenBracketsStack.pop();
            }
            else
            {
                statusBetweenBrackets = statusBetweenBracketsStack.pop();
                leftBracketStorage.pop();
            }
        }
        else
        {
            statusBetweenBrackets = 0;
        }
        if(subFlag == 0)
        {
            ++data_8;
            ++tempBracketPosition;
        }
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(76);
    //10th missing operand detection
    QChar *data_9 = tempText.data();
    while(!data_9->isNull())
    {
        int checkCode = data_9->unicode();
        if(checkCode == 43 || checkCode == 45 || checkCode == 42
                || checkCode == 47 || checkCode == 94)
        {
            --data_9;
            checkCode = data_9->unicode();
            if(!(checkCode >= 48 && checkCode <= 57) && checkCode != 41)
            {
                QMessageBox::critical(this, "ERROR", "Missing operands!");
                tempText.clear();
                tempText = tempText + "0";
                syntaxErrorFlag = -1;
                return tempText;
            }
            ++data_9;
            ++data_9;
            checkCode = data_9->unicode();
            if(!(checkCode >= 48 && checkCode <= 57) && checkCode != 40)
            {
                QMessageBox::critical(this, "ERROR", "Missing operands!");
                tempText.clear();
                tempText = tempText + "0";
                syntaxErrorFlag = -1;
                return tempText;
            }
            --data_9;
        }
        ++data_9;
    }
    ui->Label_status->setText("Checking");
    ui->Bar_progress->setValue(80);
    //11th missing operator detection like "5(10)"
    QChar *data_10 = tempText.data();
    int tempInsertPosition = 0;
    while(!data_10->isNull())
    {
        int checkCode = data_10->unicode();
        if(checkCode >= 48 && checkCode <= 57)
        {
            ++data_10;
            ++tempInsertPosition;
            checkCode = data_10->unicode();
            if(checkCode == 40)
            {
                tempText.insert(tempInsertPosition, "*");
                ui->Label_status->setText("Auto fixing");
                ++tempInsertPosition;
                data_10 = tempText.data();
                for(int i = 0; i < tempInsertPosition; i++)
                {
                    ++data_10;
                }
            }
            else
            {
                --data_10;
                --tempInsertPosition;
            }
        }
        else if(checkCode == 41)
        {
            ++data_10;
            ++tempInsertPosition;
            checkCode = data_10->unicode();
            if(checkCode == 40 || (checkCode >= 48 && checkCode <= 57))
            {
                tempText.insert(tempInsertPosition, "*");
                ++tempInsertPosition;
                data_10 = tempText.data();
                for(int i = 0; i < tempInsertPosition; i++)
                {
                    ++data_10;
                }
            }
            else
            {
                --data_10;
                --tempInsertPosition;
            }
        }
        ++data_10;
        ++tempInsertPosition;
    }
    ui->Bar_progress->setValue(84);
    ui->Label_status->setText("Checking");
    //12th change empty status to zero like "" --> "0"
    QChar *data_11 = tempText.data();
    if(data_11->isNull())
    {
        tempText = tempText + "0";
    }
    ui->Bar_progress->setValue(88);
    ui->Label_status->setText("Checking");
    return tempText;
}
int priorityTable[6][8] = {1, 1, 0, 0, 0, 0, 1, 1,
                           1, 1, 0, 0, 0, 0, 1, 1,
                           1, 1, 1, 1, 0, 0, 1, 1,
                           1, 1, 1, 1, 0, 0, 1, 1,
                           1, 1, 1, 1, 0, 0, 1, 1,
                           0, 0, 0, 0, 0, 0, -1, -2};
bool Widget::judgePriority(QString operatorOld, QString operatorNew)
{
    int operatorOldNumber = judgePriorityNumber(operatorOld);
    int operatorNewNumber = judgePriorityNumber(operatorNew);
    if(operatorOldNumber == -1 || operatorNewNumber == -1 || operatorOldNumber >=6)
    {
        QMessageBox::critical(this, "Oops", "Unexpected Error!");
        syntaxErrorFlag = -1;
        return false;
    }
    if(priorityTable [operatorOldNumber][operatorNewNumber] == 1)
    {
        return true;
    }
    else if(priorityTable [operatorOldNumber][operatorNewNumber] == 0)
    {
        return false;
    }
    else
    {
        QMessageBox::critical(this, "Oops", "Unexpected Error!");
        syntaxErrorFlag = -1;
        return false;
    }
}
int Widget::judgePriorityNumber(QString tempOperator)
{
    if(tempOperator.compare("+") == 0)
    {
        return 0;
    }
    if(tempOperator.compare("-") == 0)
    {
        return 1;
    }
    if(tempOperator.compare("*") == 0)
    {
        return 2;
    }
    if(tempOperator.compare("/") == 0)
    {
        return 3;
    }
    if(tempOperator.compare("^") == 0)
    {
        return 4;
    }
    if(tempOperator.compare("(") == 0)
    {
        return 5;
    }
    if(tempOperator.compare(")") == 0)
    {
        return 6;
    }
    if(tempOperator.compare("=") == 0)
    {
        return 7;
    }
    return -1;
}
void Widget::displayFormula(QString tempText)
{
    ui->Label_history->setText(tempText);
}
//calculate the formula
void Widget::processFormula(QString tempText)
{
    QStack <double> operandStack;
    QStack <QString> operatorStack;
    double  operandLeft = 0;
    double  operandRight = 0;
    QString operandTemp = "";
    QString operatorBetween = "";
    QChar *data_1 = tempText.data();
    int processStatus = -1;
    while(processStatus != 0)
    {
        int checkCode = data_1->unicode();
        if(checkCode == 46 || (checkCode >= 48 && checkCode <= 57))
        {
            operandTemp.append(*data_1);
            ++data_1;
            checkCode = data_1->unicode();
            if(!(checkCode == 46 || (checkCode >= 48 && checkCode <= 57)))
            {
                operandRight = operandTemp.toDouble();
                operandStack.push(operandRight);
                operandTemp = "";
            }
            --data_1;
        }
        else
        {
            operatorBetween = *data_1;
            if(operatorBetween.compare(")") == 0)
            {
                if(operatorStack.top().compare("(") == 0)
                {
                    operatorStack.pop();
                }
                else
                {
                    operandRight = operandStack.pop();
                    operandLeft = operandStack.pop();
                    operatorBetween = operatorStack.pop();
                    int operationType = judgePriorityNumber(operatorBetween);
                    if(operationType == 0)
                    {
                        operandLeft = operandLeft + operandRight;
                        operandStack.push(operandLeft);
                    }
                    else if(operationType == 1)
                    {
                        operandLeft = operandLeft - operandRight;
                        operandStack.push(operandLeft);
                    }
                    else if(operationType == 2)
                    {
                        operandLeft = operandLeft * operandRight;
                        operandStack.push(operandLeft);
                    }
                    else if(operationType == 3)
                    {
                        if(operandRight == 0)
                        {
                            QMessageBox::critical(this, "Error", "Divided By Zero Error!");
                            syntaxErrorFlag = -1;
                            processStatus = 0;
                            return;
                        }
                        operandLeft = operandLeft / operandRight;
                        operandStack.push(operandLeft);
                    }
                    else if(operationType == 4)
                    {
                        if(operandLeft == 0)
                        {
                            operandStack.push(0);
                        }
                        else if(operandRight == 0)
                        {
                            operandStack.push(1);
                        }
                        else if(operandRight >= 0)
                        {
                            double tempPow = operandLeft;
                            for(int i = 1; i < operandRight; i++)
                            {
                                operandLeft = operandLeft * tempPow;
                            }
                            operandStack.push(operandLeft);
                        }
                        else
                        {
                            double tempPow = operandLeft;
                            for(int i = 1; i < operandRight; i++)
                            {
                                operandLeft = operandLeft * tempPow;
                            }
                            operandLeft = 1 / operandLeft;
                            operandStack.push(operandLeft);
                        }
                    }
                    continue;
                }
            }
            else if(operatorBetween.compare("=") == 0)
            {
                if(operatorStack.empty())
                {
                    if(operandStack.size() != 1)
                    {
                        QMessageBox::critical(this, "Oops", "Unexpected Error!");
                        syntaxErrorFlag = -1;
                        processStatus = 0;
                        return;
                    }
                    ui->Text_main->setText(QString::number(operandStack.pop(), 'g', 6));
                    processStatus = 0;
                    ui->Bar_progress->setValue(96);
                    displayFormula(tempText);
                    ui->Label_status->setText("Done");
                    ui->Bar_progress->setValue(100);
                    return;
                }
                else if(operandStack.size() == 1)
                {
                    QMessageBox::critical(this, "Oops", "Unexpected Error!");
                    syntaxErrorFlag = -1;
                    processStatus = 0;
                    return;
                }
                operandRight = operandStack.pop();
                operandLeft = operandStack.pop();
                operatorBetween = operatorStack.pop();
                int operationType = judgePriorityNumber(operatorBetween);
                if(operationType == 0)
                {
                    operandLeft = operandLeft + operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 1)
                {
                    operandLeft = operandLeft - operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 2)
                {
                    operandLeft = operandLeft * operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 3)
                {
                    if(operandRight == 0)
                    {
                        QMessageBox::critical(this, "Error", "Divided By Zero Error!");
                        syntaxErrorFlag = -1;
                        processStatus = 0;
                        return;
                    }
                    operandLeft = operandLeft / operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 4)
                {
                    if(operandLeft == 0)
                    {
                        operandStack.push(0);
                    }
                    else if(operandRight == 0)
                    {
                        operandStack.push(1);
                    }
                    else if(operandRight >= 0)
                    {
                        double tempPow = operandLeft;
                        for(int i = 1; i < operandRight; i++)
                        {
                            operandLeft = operandLeft * tempPow;
                        }
                        operandStack.push(operandLeft);
                    }
                    else
                    {
                        double tempPow = operandLeft;
                        for(int i = 1; i < operandRight; i++)
                        {
                            operandLeft = operandLeft * tempPow;
                        }
                        operandLeft = 1 / operandLeft;
                        operandStack.push(operandLeft);
                    }
                }
                continue;
            }
            else if(operatorStack.empty())
            {
                operatorStack.push(operatorBetween);
            }
            else if(!(judgePriority(operatorStack.top(), operatorBetween)))
            {
                operatorStack.push(operatorBetween);
            }
            else if(judgePriority(operatorStack.top(), operatorBetween))
            {
                operandRight = operandStack.pop();
                operandLeft = operandStack.pop();
                operatorBetween = operatorStack.pop();
                int operationType = judgePriorityNumber(operatorBetween);
                if(syntaxErrorFlag != 0)
                {
                    processStatus = 0;
                    return;
                }
                if(operationType == 0)
                {
                    operandLeft = operandLeft + operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 1)
                {
                    operandLeft = operandLeft - operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 2)
                {
                    operandLeft = operandLeft * operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 3)
                {
                    if(operandRight == 0)
                    {
                        QMessageBox::critical(this, "Error", "Divided By Zero Error!");
                        syntaxErrorFlag = -1;
                        processStatus = 0;
                        return;
                    }
                    operandLeft = operandLeft / operandRight;
                    operandStack.push(operandLeft);
                }
                else if(operationType == 4)
                {
                    if(operandLeft == 0)
                    {
                        operandStack.push(0);
                    }
                    else if(operandRight == 0)
                    {
                        operandStack.push(1);
                    }
                    else if(operandRight >= 0)
                    {
                        double tempPow = operandLeft;
                        for(int i = 1; i < operandRight; i++)
                        {
                            operandLeft = operandLeft * tempPow;
                        }
                        operandStack.push(operandLeft);
                    }
                    else
                    {
                        double tempPow = operandLeft;
                        for(int i = 1; i < (-operandRight); i++)
                        {
                            operandLeft = operandLeft * tempPow;
                        }
                        operandLeft = 1 / operandLeft;
                        operandStack.push(operandLeft);
                    }
                }
                continue;
            }
        }
        ++data_1;
    }
    return;
}
