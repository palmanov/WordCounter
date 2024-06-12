#include "fileprocessor.h"

#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>
#include <QUrl>

FileProcessor::FileProcessor(QObject* parent) : QObject(parent), progress_(0) {
  worker_thread_ = new FileProcessorThread(this);

  QObject::connect(worker_thread_, &FileProcessorThread::wordCountChanged,
                   this, [&](const WordCounts& wordCountList) {
                     word_count_list_.clear();
                     for (const auto& word : wordCountList) {
                       QVariantMap word_map;
                       word_map["word"] = word.word;
                       word_map["count"] = word.count;
                       word_count_list_.push_back(word_map);
                     }

                     emit wordCountChanged();
                   });
  QObject::connect(worker_thread_, &FileProcessorThread::progressChanged, 
                   this, [&](double progress) {
                     progress_ = progress;
                     emit progressChanged();
                   });

  QObject::connect(worker_thread_, &QThread::finished, 
                   this, &FileProcessor::processedFinished);
}

FileProcessor::~FileProcessor() {
  worker_thread_->cancelProcessing();
  worker_thread_->quit();
  worker_thread_->wait();
}

QVariantList FileProcessor::wordCountList() const {
  return word_count_list_;
}

double FileProcessor::progress() const {
  return progress_;
}

void FileProcessor::openFile(const QString& filePath) {
  worker_thread_->processFile(filePath);
}

void FileProcessor::startProcessing() {
  if (!worker_thread_->isRunning()) {
    worker_thread_->start();
  }
}

void FileProcessor::pauseProcessing() {
  worker_thread_->pauseProcessing();
}

void FileProcessor::cancelProcessing() {
  if (worker_thread_->isRunning()) {
    worker_thread_->cancelProcessing();
  } else {
    word_count_list_.clear();
    progress_ = 0;

    emit wordCountChanged();
    emit progressChanged();
  }
}
