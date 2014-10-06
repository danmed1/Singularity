#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <vector>

namespace soan {

	namespace utils {

		/**
			@class 	FPSCounter
			@brief 	A FPS calculator.
			@author Cengiz Terzibas
		*/
		class FPSCounter  {
			public:

				FPSCounter();

				virtual ~FPSCounter();

				/// Returns the mean FPS value.
				double getMeanFPS();

				/// Returns the sample size used to calculate the mean value.
				std::size_t getSampleSize();

				/// Sets the sample size which is used to calculate the mean value.
				void setSampleSize(std::size_t sampleSize);

				/// Update current fps.
				void update(double timeStep);

		private:
		
				/// Holds the sample size.
				unsigned int 				m_sampleSize;
				
				/// Holds the last mean FPS value.
				double							m_meanFPS;
				
				/// Array for the FPS items.
				std::vector<double>	m_fps;
		};

	}

}

#endif
