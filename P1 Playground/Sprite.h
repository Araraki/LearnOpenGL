#pragma once

class Sprite
{
public:
	Sprite()
	{
		
	}

	void Init()
	{
		add(1, 2);
		add(1.2, 2.3);
		add2<float, int>(1, 2);
	}

	void Draw()
	{
		
	}

	template <typename T>
	T add(T a, T b)
	{
		return a + b;
	}
	
	template <typename R, typename T>
	R add2(T a, T b)
	{
		R r = a + b;
		return r;
	}

private:
	int VAO;
	int VBO;
};