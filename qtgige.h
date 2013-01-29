// Copyright (c) 2011 University of Southern Denmark. All rights reserved.
// Use of this source code is governed by the MIT license (see license.txt).
#ifndef QTGIGE_H
#define QTGIGE_H
#include <arvtypes.h>
#include <arvstream.h>
#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QElapsedTimer>
#include <QTreeWidgetItem>

#include <time.h>
#include <sys/time.h>
#include <QThread>
#include <qvarlengtharray.h>
#include <opencv2/opencv.hpp>
  class QTGIGE : public QThread {
    Q_OBJECT
    public:
    QTGIGE(char* serial);
    ~QTGIGE();
    int setROI(int x, int y, int width, int height);
    int setExposure(float period); //Exposure time in µs
    int setGain(float gain); //Unit currently unknown (have to look it up from datasheet)
    void newImageCallback(ArvStreamCallbackType type, ArvBuffer* buffer);
    static void newImageCallbackWrapper(void *user_data, ArvStreamCallbackType type, ArvBuffer *buffer);
    int startAquisition(void);
    int stopAquisition(void);
    void unpack12BitPacked(const ArvBuffer* img, char* unpacked16);
    void setptimer(itimerval timer);
    void PrintParms(void);
    static void convert16to8bit(cv::InputArray in, cv::OutputArray out);
  signals:
    void newBayerBGImage(const cv::Mat img);
    void newBayerGRImage(const cv::Mat img);
    void measuredFPS(float fps);
    void measuredFrameStats(int success, int failed);
  private:
      void run();
      ArvCamera * camera;
      ArvStream* stream;
      QQueue<ArvBuffer*> bufferQue;
      QSemaphore bufferSem;
      itimerval ptimer;
      bool updateptimer;
      bool abort;
      static const int frameAvg = 20;
      QElapsedTimer framePeriod;
      int nFrames;
      int successFrames;
      int failedFrames;
      void gigE_list_features(ArvGc* genicam, const char* feature, gboolean show_description, QTreeWidgetItem* parent);
  };

#endif  // QTGIGE_H
