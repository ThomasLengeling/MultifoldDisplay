#pragma once

/*
Thomas Sanchez Lengeling Mapping
MIT 2020
*/

#include "ofMain.h"
#include "VideoWarp.h"

namespace inn {

	class VideoWindow;

	typedef std::shared_ptr<VideoWindow> VideoWindowRef;

	class VideoWindow {
	public:
		VideoWindow() {

		}

		//create memory
		static VideoWindowRef create() {

		}

	private:


	};
}
