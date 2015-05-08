// This had color as a member, but it was moved to tetrimenio.
// could be converted to a struct.
namespace Tetris
{

	class Block
	{

	public:
		Block();
		Block(int x, int y);
		const int GetPositionX() const;
		const int GetPositionY() const;

		void SetPosition(int x, int y);
		void SetPositionX(int x);
		void SetPositionY(int y);

	private:
		int _positionX;
		int _positionY;
	};

}