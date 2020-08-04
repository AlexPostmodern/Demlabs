#ifndef SERIALNUMBERLINEEDIT_H
#define SERIALNUMBERLINEEDIT_H

#include <QDebug>
#include <QValidator>
#include <QTimer>
#include <QClipboard>
#include <QQueue>
#include <QKeyEvent>
#include <QGraphicsOpacityEffect>
#include "CustomLineEditBase.h"

#ifndef MAX_COUNT_CHAR
#define MAX_COUNT_CHAR 4
#endif

#ifndef MAX_COUNT_PART
#define MAX_COUNT_PART 4
#endif

#ifndef DASH
#define DASH "-"
#endif

#ifndef VALIDATOR
#define VALIDATOR "[A-Z0-9]"
#endif

class SecondLineEdit;
class LabelPaste;
class SerialNumberLineEdit:public QLineEdit
{
    Q_OBJECT
    friend class SecondLineEdit;

public:

    SerialNumberLineEdit(QWidget* parent = Q_NULLPTR);

    void setPlaceholderText(const QString& text="Serial number");
    void setFont(const QFont& font);
    bool setSerial(const QString& serialNumber);

    QString serial()const{return m_serialNumber;}
    bool isFilledOut()const{return m_isFilledOut;}

    void clear();

signals:

    void filledOut();
    void serialChanged(const QString&);
    void serialEdited(const QString&);
    void visiblePaste(bool);

private:

    void init();

    SecondLineEdit* createLineEdit();
    QLabel* createDashLabel();

    void paint_outFocusWithContent();
    void paint_outFocusWithoutContent();
    void paint_inFocus();

private slots:
    void slot_textEdited(QString text);
    void slot_textChanged(QString);

    void slot_checkOutOfLineEdit();
    void slot_checkInLineEdit();

    void slot_changeFocusBackspace();
    void slot_paste(QString text);
protected:
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
private:
    QHBoxLayout* m_layout{Q_NULLPTR};
    QVector <SecondLineEdit*>m_vecLineEdit{};
    QVector <QLabel*>m_vecLabel{};
    QLabel* m_placeholder{Q_NULLPTR};
    QString m_serialNumber{};
    QRegExp m_regExp{};
    bool m_isFilledOut{false};
};


class SecondLineEdit:public QLineEdit
{
    Q_OBJECT
    friend class SerialNumberLineEdit;
public slots:
    void paste();
signals:
    void signal_outOfLineEdit();
    void signal_inLineEdit();
    void signal_lineEditIsEmpty();
    void signal_pasteEvent(QString);
protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void pasteEvent();
private:
    SecondLineEdit(QWidget* parent = Q_NULLPTR)
        :QLineEdit(parent){}
    static bool inFocus;
};


class LabelPaste:public QLabel
{
    Q_OBJECT
    friend class SerialNumberLineEdit;
public:
    LabelPaste(QWidget* parent = Q_NULLPTR);
    void setFont(const QFont& font);
private slots:
    void setVisibility(bool);
private:
    QGraphicsOpacityEffect* m_op{Q_NULLPTR};
    QLabel* m_labelPaste{Q_NULLPTR};
    QVBoxLayout* m_layout{Q_NULLPTR};
};

#endif // SERIALNUMBERLINEEDIT_H
