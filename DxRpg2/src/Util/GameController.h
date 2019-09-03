#pragma once
#include <climits>
namespace
{
	const int	MetricTimes = 60;		// FPS�v����
	const int	OneFrameMillsec = 16;	// 1�t���[���̃~���b(16ms)
	const int	GCountMax = INT_MAX;	// �O���[�o���J�E���^MAX
	const int	KeyKindNum = 256;
}

namespace Util
{
	class GameController
	{
	public:
		// ������R�s�[���֎~
		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;
		GameController(GameController&&) = delete;
		GameController& operator=(GameController&&) = delete;

		enum KeyState {
			NotPressed = 0,
			Pressed = 1,
			PressedNow = 2
		};
		static GameController &getInstance();

		bool keyNotPressed(int input) const;
		bool escapeNotPressed() const;
		bool eNotPressed() const;

		bool keyPressed(int input) const;
		bool bPressed() const;
		bool fPressed() const;

		bool upPressedNow() const;
		bool downPressedNow() const;
		bool xPressedNow() const;
		bool zPressedNow() const;
		
		int getAllKeyPressed();

		void increaseGCount();
		int getGCount() const;

		void adjustKeyState();
		void control();

	private:
		GameController() {};
		~GameController() {};
		char getKey(int input) const;
		void calcFps();
		void graphFps();
		void controlFps();

		int gCount_;
		int frameSpdAvg;  // ���ς�1�t���[������
		int waitTime_;  // �e�o�r�Ŏg�p����ϐ�
		int prevTime_;
		int fps_[MetricTimes];
		char key_[KeyKindNum];
		char prevKey_[KeyKindNum];
	};
}

