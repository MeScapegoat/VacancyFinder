#include <QComboBox>

#include <QList>
#include <QWidget>


template<typename Function>
class CallableComboBox: public QComboBox
{
public:
    explicit CallableComboBox(QWidget* parent = nullptr) :
        QComboBox(parent),
        m_functions()
    {
    }

public:
    void insertItem(int index, const QString& text, Function function, const QVariant& userData = QVariant())
    {
        QComboBox::insertItem(index, text, userData);
        insertFunction(index, std::move(function));
    }

    void insertItem(int index, const QIcon& icon, const QString& text, Function function, const QVariant& userData = QVariant())
    {
        QComboBox::insertItem(index, icon, text, userData);
        insertFunction(index, std::move(function));
    }

    const Function& currentFunction() const
    {
        return m_functions[currentIndex()];
    }

private:
    void insertFunction(int index, Function function)
    {
        if (index >= m_functions.size())
        {
            m_functions.resize(index);
        }

        m_functions.insert(index, std::move(function));
    }

private:
    QList<Function> m_functions;
};