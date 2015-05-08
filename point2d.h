#ifndef Point2D1
#define Point2D1

template<typename T> struct Point2D
{
	T x;
	T y;

	Point2D::Point2D(T positionX  = 0, T positionY  = 0)
	{
		SetPosition(positionX , positionY);
	}

	void Point2D::SetPosition(T positionX, T positionY)
	{
		x = positionX;
		y = positionY;
	}
};

#endif