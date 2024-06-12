#ifndef FILEPROCESSORTHREAD_H
#define FILEPROCESSORTHREAD_H

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QThread>
#include <QVariant>
#include <QWaitCondition>

struct WordCount {
  QString word;
  int count;
};

using WordCounts = QVector<WordCount>;

class FileProcessorThread : public QThread {
  Q_OBJECT

 public:
  explicit FileProcessorThread(QObject* parent = nullptr);
  ~FileProcessorThread();

  void processFile(const QString& file_path);
  void pauseProcessing();
  void cancelProcessing();

 signals:
  void wordCountChanged(const WordCounts& word_counts_v);
  void progressChanged(double progress);

 protected:
  void run() override;

 private:
  void updateTopWords(const QString& word);

 private:
  QMap<QString, int> word_count_;
  double progress_;
  WordCounts word_counts_v_;

  QString file_path_;
  QFile* file_;

  bool cancelled_;
  bool paused_;
  QMutex mutex_;
  QWaitCondition pause_condition_;
};

#endif  // FILEPROCESSORTHREAD_H
