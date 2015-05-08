#ifndef color1
#define color1

#include <string>

namespace Tetris
{

	class Color
	{


	public:
		Color();
		Color(int r, int g, int b);
		const int GetRed() const;
		void SetRed(int r);
		const int GetGreen()const;
		void SetGreen(int g);
		const int GetBlue() const;
		void SetBlue(int b);
		const bool IsEmpty() const;
		void SetEmpty();
		const std::string ToHex() const;

	private:
		unsigned _red : 8;
		unsigned _green : 8;
		unsigned _blue : 8;
		bool _isEmpty;
	};

}
#endif