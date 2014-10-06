#ifndef SIGNED_DISTANCE_FIELD_GENERATOR
#define SIGNED_DISTANCE_FIELD_GENERATOR

#include <cstdint>

namespace soan {

	namespace utils {

		struct Point {
			int dx, dy;

			int DistSq() const {
				return dx*dx + dy*dy;
			}
		};

		struct Grid {
			Grid(int width, int height) : w(width), h(height) {
				grid = new Point[w*h];
			}
			~Grid() {
				delete [] grid;
			}

			Point* grid;
			int w;
			int h;
		};

		class SignedDistanceFieldGenerator {
			public:
				SignedDistanceFieldGenerator();
				~SignedDistanceFieldGenerator();

				int generateFromFile(const char* filename, const char* outputFilename, int rescaleWidth = -1, int rescaleHeight = -1);
			protected:
				Point Get(Grid &g, int x, int y);
				void Put(Grid &g, int x, int y, const Point &p);
				void Compare(Grid &g, Point &p, int x, int y, int offsetx, int offsety);
				void generateSDF(Grid &g);
			private:
				unsigned int m_rescaleWidth;
				unsigned int m_rescaleHeight;
		};

	}

}

#endif // SIGNED_DISTANCE_FIELD_GENERATOR
