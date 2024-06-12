#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QFile>
#include <QMap>
#include <QObject>
#include <QVariantList>

#include "fileprocessorthread.h"

class FileProcessor : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList wordCountList READ wordCountList NOTIFY wordCountChanged)
  Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

 public:
  explicit FileProcessor(QObject* parent = nullptr);
  ~FileProcessor();

  QVariantList wordCountList() const;
  double progress() const;

 signals:
  void wordCountChanged();
  void progressChanged();
  void processedFinished();

 public slots:
  void openFile(const QString& filePath);
  void startProcessing();
  void pauseProcessing();
  void cancelProcessing();

 private:
  FileProcessorThread* worker_thread_;
  QVariantList word_count_list_;
  double progress_;
};

#endif  // FILEPROCESSOR_H
