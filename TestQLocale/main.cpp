#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QMetaEnum>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  QMetaEnum metaEnum = QMetaEnum::fromType<QLocale::Language>();
  for (int i = 0; i < metaEnum.keyCount(); i++) {
    QString name = metaEnum.valueToKey(metaEnum.value(i));
    QLocale loc((QLocale::Language)(metaEnum.value(i)));
    qDebug() << name << " : " << loc.country() << " : "
             << loc.nativeCountryName() << ": " << loc.nativeLanguageName()
             << " : " << loc.bcp47Name();
  }

  qDebug() << QLocale(QLocale::Chinese, QLocale::China).bcp47Name();

  return a.exec();
}
