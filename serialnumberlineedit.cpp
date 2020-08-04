#include "serialnumberlineedit.h"

bool SecondLineEdit::inFocus=false;

//public
SerialNumberLineEdit::SerialNumberLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    init();
}

void SerialNumberLineEdit::setPlaceholderText(const QString& text)
{
    m_placeholder=new QLabel(text);
    m_placeholder->setObjectName("obj_placeholderLabel");
    m_layout->addWidget(m_placeholder);
}

void SerialNumberLineEdit::setFont(const QFont &font)
{
    m_placeholder->setFont(font);
    for(auto& el:m_vecLabel)
        el->setFont(font);
    for(auto& el:m_vecLineEdit)
        el->setFont(font);
}

bool SerialNumberLineEdit::setSerial(const QString &serialNumber)
{
    QString temp{};
    QString strRegExp{static_cast<QString>(VALIDATOR)+
                "{1,"+QString::number(MAX_COUNT_CHAR*MAX_COUNT_PART)+"}"};
    QRegExp regExp(strRegExp);
    if (regExp.exactMatch(serialNumber))
    {
        QQueue<QString>qq;
        for(const auto& el:serialNumber)
            qq.enqueue(el);
        for(int i=0;i<MAX_COUNT_PART;i++)
        {
            for(int j=0;j<MAX_COUNT_CHAR;j++)
            {
                if (!qq.isEmpty()) temp.append(qq.dequeue());
                else break;
            }
            m_vecLineEdit[i]->setText(temp);
            temp.clear();
        }
        paint_outFocusWithContent();
        emit serialChanged(serialNumber);
        return true;
    }

    else
    {
        qDebug()<<"SerialNumberLineEdit::setSerial() serialNumber is not valid"
               <<strRegExp;
        return false;
    }
}

void SerialNumberLineEdit::clear()
{
    m_serialNumber.clear();
    for(auto& el:m_vecLineEdit)
        el->clear();
    m_isFilledOut=false;
    paint_outFocusWithoutContent();
}

//private
void SerialNumberLineEdit::init()
{
    m_layout=new QHBoxLayout;
    setObjectName("obj_SerialNumberLineEdit");
    m_regExp=QRegExp(static_cast<QString>(VALIDATOR)+
                     "{1,"+QString::number(MAX_COUNT_CHAR)+"}");

    m_vecLabel.reserve(MAX_COUNT_PART-1);
    m_vecLineEdit.reserve(MAX_COUNT_PART);
    m_vecLineEdit.push_back(createLineEdit());

    m_layout->addWidget(m_vecLineEdit[0]);
    for(int i=1;i<MAX_COUNT_PART;++i)
    {
        m_vecLabel.push_back(createDashLabel());
        m_layout->addWidget(m_vecLabel[i-1]);
        m_vecLineEdit.push_back(createLineEdit());
        m_layout->addWidget(m_vecLineEdit[i]);
    }
    setPlaceholderText();
    m_layout->setContentsMargins(0,10,0,10);
    setLayout(m_layout);
}

SecondLineEdit* SerialNumberLineEdit::createLineEdit()
{
    static int i=1;
    SecondLineEdit* temp=new SecondLineEdit;
    temp->setObjectName("obj_SecondLineEdit_"+QString::number(i));
    ++i;
    QValidator* validator=new QRegExpValidator(m_regExp);
    temp->setValidator(validator);
    temp->setMaxLength(MAX_COUNT_CHAR);
    temp->setVisible(false);
    connect(temp,SIGNAL(signal_outOfLineEdit()),SLOT(slot_checkOutOfLineEdit()));
    connect(temp,SIGNAL(signal_inLineEdit()),SLOT(slot_checkInLineEdit()));
    connect(temp,SIGNAL(textEdited(QString)),this,SLOT(slot_textEdited(QString)));
    connect(temp,SIGNAL(signal_lineEditIsEmpty()),SLOT(slot_changeFocusBackspace()));
    connect(temp,SIGNAL(textChanged(QString)),SLOT(slot_textChanged(QString)));
    connect(temp,SIGNAL(signal_pasteEvent(QString)),SLOT(slot_paste(QString)));
    return temp;
}
QLabel* SerialNumberLineEdit::createDashLabel()
{
    QLabel* temp=new QLabel(DASH);
    temp->setObjectName("obj_DashLabel");
    temp->setAlignment(Qt::AlignCenter);
    temp->setVisible(false);
    temp->setStyleSheet("QLabel{color:transparent;}");
    return temp;
}

void SerialNumberLineEdit::paint_outFocusWithContent()
{
    for(auto& el:m_vecLineEdit)
    {
        el->setStyleSheet("QLineEdit{color:black;}");
        el->setVisible(true);
    }
    for(auto& el:m_vecLabel)
    {
        el->setStyleSheet("QLabel{color:#BFC9C8;}");
        el->setVisible(true);
    }
    m_placeholder->setVisible(false);
    setStyleSheet("QLineEdit{border-color:#E1E1E1;}");
}

void SerialNumberLineEdit::paint_outFocusWithoutContent()
{
    m_placeholder->setVisible(true);
    for(auto& el:m_vecLineEdit)
        el->setVisible(false);
    for(auto& el:m_vecLabel)
        el->setVisible(false);
    setStyleSheet("QLineEdit{border-color:#E1E1E1;}");
}

void SerialNumberLineEdit::paint_inFocus()
{
    m_placeholder->setVisible(false);
    for(auto& el:m_vecLineEdit)
    {
        el->setVisible(true);
        el->setStyleSheet("QLineEdit{color:#00A86C;}");
    }
    for(auto& el:m_vecLabel)
        el->setVisible(true);
    setStyleSheet("QLineEdit{border-color:#00A86C;}");
}


void SerialNumberLineEdit::slot_textEdited(QString text)
{

    /*нужно что-то придумать с сигналами,
    чтобы они не дублировались*/
    emit serialEdited(text);
    //qDebug()<<__FUNCTION__<<m_serialNumber;


    QString name = qobject_cast<QLineEdit*>(sender())->objectName();
    int obj=static_cast<QString>(name.back()).toInt();

    if (text.count()==MAX_COUNT_CHAR && obj!=MAX_COUNT_PART)
    {
        m_vecLineEdit[obj]->setFocus();
    }
}

void SerialNumberLineEdit::slot_textChanged(QString)
{
    m_serialNumber.clear();
    m_isFilledOut=false;
    for(const auto& el:m_vecLineEdit)
        m_serialNumber.append(el->text());

    QString style{};
    if(m_serialNumber.count()>0)
    {
        style="QLabel{color:#BFC9C8;}";
        emit visiblePaste(false);
    }
    else
    {
        style="QLabel{color:transparent;}";
        emit visiblePaste(true);
    }
    for(auto& el:m_vecLabel)
        el->setStyleSheet(style);

    if (m_serialNumber.count()==MAX_COUNT_CHAR * MAX_COUNT_PART)
    {
        qDebug()<<"filledOut\t"<<m_serialNumber;
        emit filledOut();
        m_isFilledOut=true;
    }


    /*работает не так как хотел,
    нужно подумать, сигналы дублируются*/
    QTimer::singleShot(0,this,[this]()
    {
        emit serialChanged(m_serialNumber);
        //qDebug()<<__FUNCTION__<<m_serialNumber;
    });

}

void SerialNumberLineEdit::slot_checkOutOfLineEdit()
{
    QTimer::singleShot(0,this,[this]()
    {
        if(!SecondLineEdit::inFocus)
        {
            bool empty{true};
            emit visiblePaste(false);
            for(const auto& el:m_vecLineEdit)
            {
                if (el->text().count()!=0)
                    empty=false;
            }
            if(empty)
            {
                paint_outFocusWithoutContent();
            }
            else paint_outFocusWithContent();

    #ifdef Q_OS_ANDROID
        QApplication::inputMethod()->hide();

    #endif

        }
    });
}

void SerialNumberLineEdit::slot_checkInLineEdit()
{
    paint_inFocus();
}

void SerialNumberLineEdit::slot_changeFocusBackspace()
{
    QString name = qobject_cast<QLineEdit*>(sender())->objectName();
    int obj=static_cast<QString>(name.back()).toInt();

    if(obj==1) return;

    m_vecLineEdit[obj-2]->setFocus();
    QTimer::singleShot(0,m_vecLineEdit[obj-2],&QLineEdit::deselect);
    QString text{m_vecLineEdit[obj-2]->text()};
    text.chop(1);
    m_vecLineEdit[obj-2]->setText(text);
}

void SerialNumberLineEdit::slot_paste(QString text)
{
    QString temp{text}, strRegExp{static_cast<QString>(VALIDATOR)+
                "{1,"+QString::number(MAX_COUNT_CHAR*MAX_COUNT_PART)+"}"};
    QRegExp regExp(strRegExp);
    if (regExp.exactMatch(text)&&text.count())
    {
        for (auto& el:m_vecLineEdit)
        {
            el->setText(temp.left(MAX_COUNT_CHAR));
            temp.remove(0,MAX_COUNT_CHAR);
        }
    }
}

//protected
void SerialNumberLineEdit::focusInEvent(QFocusEvent*)
{
    SecondLineEdit::inFocus=true;
    paint_inFocus();
    m_vecLineEdit[0]->setFocus();
    if (m_serialNumber.count()==0) emit visiblePaste(true);
#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->show();
#endif
}
void SerialNumberLineEdit::focusOutEvent(QFocusEvent*)
{
    //empty
}


//********************SecondLineEdit********************

void SecondLineEdit::paste()
{
    QString text = QApplication::clipboard()->text();
    if(text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        pasteEvent();
    else QLineEdit::paste();
}

void SecondLineEdit::focusInEvent(QFocusEvent *event)
{
    inFocus=true;
    QLineEdit::focusInEvent(event);
    QTimer::singleShot(0, this, &QLineEdit::selectAll);
    emit signal_inLineEdit();

#ifdef Q_OS_ANDROID
    if(!QApplication::inputMethod()->isVisible())
        QApplication::inputMethod()->setVisible(true);
#endif
}
void SecondLineEdit::focusOutEvent(QFocusEvent *event)
{
    inFocus=false;
    QLineEdit::focusOutEvent(event);
    emit signal_outOfLineEdit();
}

void SecondLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Backspace)
    {
        if (text().count()==0) emit signal_lineEditIsEmpty();
        else QLineEdit::keyPressEvent(event);
        return;
    }

    if(event==QKeySequence::Paste)
    {
        QString text = QApplication::clipboard()->text();
        if (text.count()==MAX_COUNT_CHAR*MAX_COUNT_PART)
        {
            pasteEvent();
            return;
        }
    }

    QLineEdit::keyPressEvent(event);
}

void SecondLineEdit::pasteEvent()
{
    QString text = QApplication::clipboard()->text();
    emit signal_pasteEvent(text);
}


//********************LabelPaste********************

LabelPaste::LabelPaste(QWidget *parent)
    :QLabel(parent)
{
    setObjectName("obj_labelPaste");
    setPixmap(QPixmap(":/Popover.svg"));
    m_op=new QGraphicsOpacityEffect;
    m_op->setOpacity(0);
    setGraphicsEffect(m_op);
    m_layout=new QVBoxLayout;
    m_labelPaste=new QLabel("Paste");
    m_layout->addWidget(m_labelPaste);
    m_labelPaste->setAlignment(Qt::AlignCenter);
    m_layout->setContentsMargins(0,10,0,15);
    setLayout(m_layout);
}

void LabelPaste::setFont(const QFont &font)
{
    m_labelPaste->setFont(font);
}

void LabelPaste::setVisibility(bool vis)
{
    if(vis) m_op->setOpacity(1);
    else m_op->setOpacity(0);
    setGraphicsEffect(m_op);
}
