#ifndef SOAN_MOTION_STATE
#define SOAN_MOTION_STATE

#include <btBulletDynamicsCommon.h>

namespace soan {

	class SoanMotionState : public btMotionState {

		public:
			SoanMotionState();
			virtual ~SoanMotionState();
		private:
			btTransform mInitialPosition;
	};

}

#endif // SOAN_MOTION_STATE
