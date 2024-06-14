#include "fileprocessorthread.h"

#include <QRegularExpression>

#ifdef Q_OS_ANDROID
#include <QFileInfo>
#else
#include <QUrl>
#endif

FileProcessorThread::FileProcessorThread(QObject* parent)
    : QThread(parent),
      progress_(0),
      file_(nullptr),
      cancelled_(false),
      paused_(false) {}

FileProcessorThread::~FileProcessorThread() {
  if (file_) {
    file_->close();
    delete file_;
  }
}

void FileProcessorThread::processFile(const QString& file_path) {
#ifdef Q_OS_ANDROID
  file_path_ = QFileInfo(file_path).filePath();
#else
  file_path_ = QUrl(file_path).toLocalFile();
#endif
}

void FileProcessorThread::pauseProcessing() {
  QMutexLocker locker(&mutex_);
  paused_ = !paused_;
  if (!paused_) {
    pause_condition_.wakeAll();
  }
}

void FileProcessorThread::cancelProcessing() {
  QMutexLocker locker(&mutex_);
  cancelled_ = true;
  pause_condition_.wakeAll();
}

void FileProcessorThread::run() {
  if (file_) {
    file_->close();
    delete file_;
  }
  file_ = new QFile(file_path_);
  if (!file_->open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  word_count_.clear();
  progress_ = 0;
  cancelled_ = false;
  paused_ = false;

  QTextStream in(file_);
  qint64 total_size = file_->size();

  while (!in.atEnd() && !cancelled_) {
    {
      QMutexLocker locker(&mutex_);
      if (paused_) {
        pause_condition_.wait(&mutex_);
        if (cancelled_) {
          word_counts_v_.clear();
          progress_ = 0;

          emit wordCountChanged(word_counts_v_);
          emit progressChanged(progress_);
          break;
        }
      }
    }

    static QRegularExpression reg_exp("\\W+");

    QString line = in.readLine().toLower();
    QStringList words = line.split(reg_exp, Qt::SkipEmptyParts);
    for (const auto& word : words) {
      word_count_[word]++;
      updateTopWords(word);
    }

    const auto remains = file_->bytesAvailable();
    progress_ = static_cast<double>((total_size - remains)) / total_size;

    emit wordCountChanged(word_counts_v_);
    emit progressChanged(progress_);
  }

  if (cancelled_) {
    word_counts_v_.clear();
    progress_ = 0;

    emit wordCountChanged(word_counts_v_);
    emit progressChanged(progress_);
  }

  file_->close();
}

void FileProcessorThread::updateTopWords(const QString& word) {
  auto it = std::find_if(word_counts_v_.begin(), word_counts_v_.end(),
                         [&](const WordCount& wc) { return wc.word == word; });

  if (it != word_counts_v_.end()) {
    it->count = word_count_[word];
  } else {
    word_counts_v_.push_back({word, word_count_[word]});
  }

  std::sort(
      word_counts_v_.begin(), word_counts_v_.end(),
      [](const WordCount& a, const WordCount& b) { return a.count > b.count; });

  if (word_counts_v_.size() > 15) {
    word_counts_v_.removeLast();
  }
}
