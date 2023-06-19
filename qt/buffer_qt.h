#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <QTextEdit>
#include <iostream>
#include <streambuf>

#include <sstream>

class QLabelStreamBuf : public std::streambuf {
public:
    explicit QLabelStreamBuf(QLabel* label) : m_label(label) {
    }

protected:
    virtual int_type overflow(int_type c) {
        if (c != EOF) {
            // Append the character to the label's text
            m_buffer << static_cast<char>(c);
            updateLabel();
        }
        return c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) {
        // Append the string to the label's text
        m_buffer.write(s, n);
        updateLabel();
        return n;
    }

private:
    void updateLabel() {
        // Update the label's text with the buffered content
        m_label->setText(QString::fromStdString(m_buffer.str()));
        m_label->repaint();
    }

    QLabel* m_label;
    std::ostringstream m_buffer;
};

class InputStreamBuf : public std::streambuf {
public:
    explicit InputStreamBuf(QLineEdit* lineEdit) : m_lineEdit(lineEdit) {
    }
protected:
    virtual int_type underflow() {
        QString input = m_lineEdit->text();
        if (!input.isEmpty()) {
            m_lineEdit->clear();
            // Append a newline to simulate pressing Enter
            input.append('\n');
            // Output the entered text to the standard input
            for (QChar c : input) {
                if (c != '\r') {
                    *gptr() = c.toLatin1();
                    gbump(1);
                }
            }
        }
        return traits_type::eof();
    }
private:
    QLineEdit* m_lineEdit;
};

// #include <QWaitCondition>
// #include <QMutex>

// class InputStreamBuf : public std::streambuf {
// public:
//     explicit InputStreamBuf(QLineEdit* inputLineEdit) : inputLineEdit(inputLineEdit) {
//     }

// protected:
//     virtual int_type underflow() {
//         static QMutex mutex;
//         static QWaitCondition waitCondition;
//         QMutexLocker locker(&mutex);

//         QString input;
//         while (input.isEmpty()) {
//             // Wait until new input is available
//             waitCondition.wait(&mutex);

//             // Read the input from the QLineEdit
//             input = inputLineEdit->text();
//             inputLineEdit->clear();
//         }

//         // Append a newline to simulate pressing Enter
//         input.append('\n');

//         // Output the entered text to the standard input
//         for (QChar c : input) {
//             if (c != '\r') {
//                 *gptr() = c.toLatin1();
//                 gbump(1);
//             }
//         }

//         return traits_type::eof();
//     }

//     virtual std::streamsize xsgetn(char_type* s, std::streamsize count) override {
//         // Prevent reading multiple characters at once
//         return 0;
//     }

// private:
//     QLineEdit* inputLineEdit;
// };
