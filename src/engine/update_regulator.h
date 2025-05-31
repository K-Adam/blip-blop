#pragma once

// The game updates in fixed steps, this decides how many steps to run per drawn frame

#include "engine/chrono.h"
#include "engine/moving_average.h"

class UpdateRegulator {
   public:
	UpdateRegulator(int delay_goal = UPDATE_PERIOD_MS, int margin = 1, int hist_size = 50)
		: last_frames_spare_time_(hist_size), dtime_(0), delay_goal_(delay_goal), margin_(margin) {}

	// Call once per drawn frame, returns how many update steps to run
	int Step() {
		if (fixedStep()) return 1;
		int since_last = RegisterFrameTime();
		dtime_ += since_last;
		int mean_FST = last_frames_spare_time_.average();

		frame_time_.Reset();

		if (-margin_ <= mean_FST && mean_FST <= margin_) {
			// Close enough to schedule, drop the drift so it never turns into extra steps
			dtime_ = 0;
			return 1;
		} else {
			int cnt = 0;
			while (dtime_ > delay_goal_) {
				++cnt;
				dtime_ -= delay_goal_;
			}
			return cnt;
		}
	}

	void Skip() { frame_time_.Reset(); }

   private:
	int RegisterFrameTime() {
		int since_last = frame_time_.elapsed();
		// Clock glitches and long stalls would skew the average
		if (since_last <= 0) {
			last_frames_spare_time_.Add(0);
		} else if (since_last >= 500) {
			last_frames_spare_time_.Add(0);
		} else {
			last_frames_spare_time_.Add(since_last - delay_goal_);
		}
		return since_last;
	}

	MovingAverage<int> last_frames_spare_time_;
	Chrono frame_time_;
	int dtime_;
	int delay_goal_;
	int margin_;
};
