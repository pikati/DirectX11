#pragma once
class Editor
{
private:
	static bool m_isDrawEditor;
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	static bool IsPlay();
};

