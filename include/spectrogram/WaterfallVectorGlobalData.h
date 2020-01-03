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

#ifndef WATERFALL_VECTOR_GLOBAL_DATA_H
#define WATERFALL_VECTOR_GLOBAL_DATA_H

#include <spectrogram/api.h>
#include <inttypes.h>
#include <qwt_raster_data.h>

#if QWT_VERSION >= 0x060000
// clang-format off
#include <qwt_point_3d.h> // doesn't seem necessary, but is...
#include <qwt_compat.h>
// clang-format on
#endif

class SPECTROGRAM_API WaterfallVectorData : public QwtRasterData
{
public:
    WaterfallVectorData(const double, const double, const uint64_t, const unsigned int);
    virtual ~WaterfallVectorData();

    virtual void reset();
    virtual void copy(const WaterfallVectorData *);

    virtual void
    resizeData(const double, const double, const uint64_t, const int history = 0);

    virtual QwtRasterData *copy() const;

#if QWT_VERSION < 0x060000
    virtual QwtDoubleInterval range() const;
    virtual void setRange(const QwtDoubleInterval &);
#endif

    virtual double value(double x, double y) const;

    virtual uint64_t getNumVecPoints() const;
    virtual void addVecData(const double *, const uint64_t, const int);

    virtual double *getSpectrumDataBuffer() const;
    virtual void setSpectrumDataBuffer(const double *);

    virtual int getNumLinesToUpdate() const;
    virtual void setNumLinesToUpdate(const int);
    virtual void incrementNumLinesToUpdate();

protected:
    double *_spectrumData;
    uint64_t _vecPoints;
    uint64_t _historyLength;
    int _numLinesToUpdate;

#if QWT_VERSION < 0x060000
    QwtDoubleInterval _intensityRange;
#else
    QwtInterval _intensityRange;
#endif

private:
};

#endif /* WATERFALL_VECTOR_GLOBAL_DATA_H */
