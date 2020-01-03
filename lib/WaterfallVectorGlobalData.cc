/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef WATERFALL_GLOBAL_DATA_CPP
#define WATERFALL_GLOBAL_DATA_CPP

#include <spectrogram/WaterfallVectorGlobalData.h>
#include <cstdio>

WaterfallVectorData::WaterfallVectorData(const double minimumFrequency,
                             const double maximumFrequency,
                             const uint64_t vecPoints,
                             const unsigned int historyExtent)
#if QWT_VERSION < 0x060000
    : QwtRasterData(QwtDoubleRect(minimumFrequency /* X START */,
                                  0 /* Y START */,
                                  maximumFrequency - minimumFrequency /* WIDTH */,
                                  static_cast<double>(historyExtent) /* HEIGHT */))
#else
    : QwtRasterData()
#endif
{
    _intensityRange = QwtDoubleInterval(-200.0, 0.0);

    _vecPoints = vecPoints;
    _historyLength = historyExtent;

    _spectrumData = new double[_vecPoints * _historyLength];

#if QWT_VERSION >= 0x060000
    setInterval(Qt::XAxis, QwtInterval(minimumFrequency, maximumFrequency));
    setInterval(Qt::YAxis, QwtInterval(0, historyExtent));
    setInterval(Qt::ZAxis, QwtInterval(-200, 0.0));
#endif

    reset();
}

WaterfallVectorData::~WaterfallVectorData() { delete[] _spectrumData; }

void WaterfallVectorData::reset()
{
    memset(_spectrumData, 0x0, _vecPoints * _historyLength * sizeof(double));

    _numLinesToUpdate = -1;
}

void WaterfallVectorData::copy(const WaterfallVectorData* rhs)
{
#if QWT_VERSION < 0x060000
    if ((_vecPoints != rhs->getNumVecPoints()) ||
        (boundingRect() != rhs->boundingRect())) {
        _vecPoints = rhs->getNumVecPoints();
        setBoundingRect(rhs->boundingRect());
        delete[] _spectrumData;
        _spectrumData = new double[_vecPoints * _historyLength];
    }
#else
    if (_vecPoints != rhs->getNumVecPoints()) {
        _vecPoints = rhs->getNumVecPoints();
        delete[] _spectrumData;
        _spectrumData = new double[_vecPoints * _historyLength];
    }
#endif

    reset();
    setSpectrumDataBuffer(rhs->getSpectrumDataBuffer());
    setNumLinesToUpdate(rhs->getNumLinesToUpdate());

#if QWT_VERSION < 0x060000
    setRange(rhs->range());
#else
    setInterval(Qt::XAxis, rhs->interval(Qt::XAxis));
    setInterval(Qt::YAxis, rhs->interval(Qt::YAxis));
    setInterval(Qt::ZAxis, rhs->interval(Qt::ZAxis));
#endif
}

void WaterfallVectorData::resizeData(const double startFreq,
                               const double stopFreq,
                               const uint64_t vecPoints,
                               const int history)
{
    if (history > 0) {
        _historyLength = history;
    }

#if QWT_VERSION < 0x060000
    if ((vecPoints != getNumVecPoints()) ||
        (boundingRect().width() != (stopFreq - startFreq)) ||
        (boundingRect().left() != startFreq)) {

        setBoundingRect(QwtDoubleRect(
            startFreq, 0, stopFreq - startFreq, static_cast<double>(_historyLength)));
        _vecPoints = vecPoints;
        delete[] _spectrumData;
        _spectrumData = new double[_vecPoints * _historyLength];
    }

#else
    if ((vecPoints != getNumVecPoints()) ||
        (interval(Qt::XAxis).width() != (stopFreq - startFreq)) ||
        (interval(Qt::XAxis).minValue() != startFreq)) {

        setInterval(Qt::XAxis, QwtInterval(startFreq, stopFreq));
        setInterval(Qt::YAxis, QwtInterval(0, _historyLength));

        _vecPoints = vecPoints;
        delete[] _spectrumData;
        _spectrumData = new double[_vecPoints * _historyLength];
    }
#endif

    reset();
}

QwtRasterData* WaterfallVectorData::copy() const
{
#if QWT_VERSION < 0x060000
    WaterfallVectorData* returnData = new WaterfallVectorData(
        boundingRect().left(), boundingRect().right(), _vecPoints, _historyLength);
#else
    WaterfallVectorData* returnData = new WaterfallVectorData(interval(Qt::XAxis).minValue(),
                                                  interval(Qt::XAxis).maxValue(),
                                                  _vecPoints,
                                                  _historyLength);
#endif

    returnData->copy(this);
    return returnData;
}


#if QWT_VERSION < 0x060000
QwtDoubleInterval WaterfallVectorData::range() const { return _intensityRange; }

void WaterfallVectorData::setRange(const QwtDoubleInterval& newRange)
{
    _intensityRange = newRange;
}

#endif


double WaterfallVectorData::value(double x, double y) const
{
    double returnValue = 0.0;

#if QWT_VERSION < 0x060000
    const unsigned int intY = static_cast<unsigned int>(
        (1.0 - (y / boundingRect().height())) * static_cast<double>(_historyLength - 1));
    const unsigned int intX = static_cast<unsigned int>(
        (((x - boundingRect().left()) / boundingRect().width()) *
         static_cast<double>(_vecPoints - 1)) +
        0.5);
#else
    double height = interval(Qt::YAxis).maxValue();
    double left = interval(Qt::XAxis).minValue();
    double right = interval(Qt::XAxis).maxValue();
    double ylen = static_cast<double>(_historyLength - 1);
    double xlen = static_cast<double>(_vecPoints - 1);
    const unsigned int intY = static_cast<unsigned int>((1.0 - y / height) * ylen);
    const unsigned int intX =
        static_cast<unsigned int>((((x - left) / (right - left)) * xlen) + 0.5);
#endif

    const int location = (intY * _vecPoints) + intX;
    if ((location > -1) &&
        (location < static_cast<int64_t>(_vecPoints * _historyLength))) {
        returnValue = _spectrumData[location];
    }

    return returnValue;
}

uint64_t WaterfallVectorData::getNumVecPoints() const { return _vecPoints; }

void WaterfallVectorData::addVecData(const double* vecData,
                               const uint64_t vecDataSize,
                               const int droppedFrames)
{
    if (vecDataSize == _vecPoints) {
        int64_t heightOffset = _historyLength - 1 - droppedFrames;
        uint64_t drawingDroppedFrames = droppedFrames;

        // Any valid data rolled off the display so just fill in zeros and write new data
        if (heightOffset < 0) {
            heightOffset = 0;
            drawingDroppedFrames = static_cast<uint64_t>(_historyLength - 1);
        }

        // Copy the old data over if any available
        if (heightOffset > 0) {
            memmove(_spectrumData,
                    &_spectrumData[(drawingDroppedFrames + 1) * _vecPoints],
                    heightOffset * _vecPoints * sizeof(double));
        }

        if (drawingDroppedFrames > 0) {
            // Fill in zeros data for dropped data
            memset(&_spectrumData[heightOffset * _vecPoints],
                   0x00,
                   static_cast<int64_t>(drawingDroppedFrames) * _vecPoints *
                       sizeof(double));
        }

        // add the new buffer
        memcpy(&_spectrumData[(_historyLength - 1) * _vecPoints],
               vecData,
               _vecPoints * sizeof(double));
    }
}

double* WaterfallVectorData::getSpectrumDataBuffer() const { return _spectrumData; }

void WaterfallVectorData::setSpectrumDataBuffer(const double* newData)
{
    memcpy(_spectrumData, newData, _vecPoints * _historyLength * sizeof(double));
}

int WaterfallVectorData::getNumLinesToUpdate() const { return _numLinesToUpdate; }

void WaterfallVectorData::setNumLinesToUpdate(const int newNum) { _numLinesToUpdate = newNum; }

void WaterfallVectorData::incrementNumLinesToUpdate() { _numLinesToUpdate++; }

#endif /* WATERFALL_GLOBAL_DATA_CPP */
