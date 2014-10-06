#include "Utils/FPSCounter.h"

namespace soan {

	namespace utils {

		FPSCounter::FPSCounter() :	m_sampleSize(20),
																m_meanFPS(0.0) {
			m_fps.reserve(m_sampleSize);
		}

		FPSCounter::~FPSCounter() {

		}
		
		std::size_t FPSCounter::getSampleSize() {
			return m_sampleSize;
		}
		
		void FPSCounter::setSampleSize(std::size_t sampleSize) {
			m_sampleSize = sampleSize;
			m_fps.reserve(sampleSize);
		}

		double FPSCounter::getMeanFPS() {
			return m_meanFPS;
		}

		void FPSCounter::update(double timestep) {
			m_fps.push_back(1.0/timestep);
			
			// Check if the array is full. If yes than calculate the mean FPS value
			// and store that value.
			if(m_fps.size() == getSampleSize()) {
				m_meanFPS = 0.0;
				for(unsigned int a = 0; a < getSampleSize(); a++) {
					m_meanFPS += m_fps[a];
				}
				m_meanFPS /= (double)getSampleSize();
				m_fps.clear();
			}
		}

	}

}
