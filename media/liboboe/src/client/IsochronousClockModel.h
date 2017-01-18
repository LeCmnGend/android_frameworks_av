/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OBOE_ISOCHRONOUSCLOCKMODEL_H
#define OBOE_ISOCHRONOUSCLOCKMODEL_H

#include <stdint.h>
#include <oboe/OboeAudio.h>

namespace oboe {

/**
 * Model an isochronous data stream using occasional timestamps as input.
 * This can be used to predict the position of the stream at a given time.
 *
 * This class is not thread safe and should only be called from one thread.
 */
class IsochronousClockModel {

public:
    IsochronousClockModel();
    virtual ~IsochronousClockModel();

    void start(oboe_nanoseconds_t nanoTime);
    void stop(oboe_nanoseconds_t nanoTime);

    void processTimestamp(oboe_position_frames_t framePosition, oboe_nanoseconds_t nanoTime);

    /**
     * @param sampleRate rate of the stream in frames per second
     */
    void setSampleRate(oboe_sample_rate_t sampleRate);

    oboe_sample_rate_t getSampleRate() const {
        return mSampleRate;
    }

    /**
     * This must be set accurately in order to track the isochronous stream.
     *
     * @param framesPerBurst number of frames that stream advance at one time.
     */
    void setFramesPerBurst(oboe_size_frames_t framesPerBurst);

    oboe_size_frames_t getFramesPerBurst() const {
        return mFramesPerBurst;
    }

    /**
     * Calculate an estimated time when the stream will be at that position.
     *
     * @param framePosition position of the stream in frames
     * @return time in nanoseconds
     */
    oboe_nanoseconds_t convertPositionToTime(oboe_position_frames_t framePosition) const;

    /**
     * Calculate an estimated position where the stream will be at the specified time.
     *
     * @param nanoTime time of interest
     * @return position in frames
     */
    oboe_position_frames_t convertTimeToPosition(oboe_nanoseconds_t nanoTime) const;

    /**
     * @param framesDelta difference in frames
     * @return duration in nanoseconds
     */
    oboe_nanoseconds_t convertDeltaPositionToTime(oboe_position_frames_t framesDelta) const;

    /**
     * @param nanosDelta duration in nanoseconds
     * @return frames that stream will advance in that time
     */
    oboe_position_frames_t convertDeltaTimeToPosition(oboe_nanoseconds_t nanosDelta) const;

private:
    enum clock_model_state_t {
        STATE_STOPPED,
        STATE_STARTING,
        STATE_SYNCING,
        STATE_RUNNING
    };

    oboe_sample_rate_t     mSampleRate;
    oboe_size_frames_t     mFramesPerBurst;
    int32_t                mMaxLatenessInNanos;
    oboe_position_frames_t mMarkerFramePosition;
    oboe_nanoseconds_t     mMarkerNanoTime;
    int32_t                mTimestampCount;
    clock_model_state_t     mState;

    void update();
};

} /* namespace oboe */

#endif //OBOE_ISOCHRONOUSCLOCKMODEL_H
