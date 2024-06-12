#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "fileprocessor.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  FileProcessor file_processor;

  QQmlContext* context = engine.rootContext();
  context->setContextProperty("fileProcessor", &file_processor);

  const QUrl url(QStringLiteral("qrc:/WordCount/qml/Main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
