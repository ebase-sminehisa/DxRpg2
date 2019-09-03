#pragma once
#include <memory>
#include "Sequence/Child.h"
#include "Map/MapMainCharacter.h"
#include "Map/MapCityCharacter.h"  // �z�񂪂��邩�炱��Ȃ��ƃ_��
#include "SharedCharacterStatus.h"  // �s���S�N���X�^�ւ̃|�C���^�[���Q�Ƃł��Ȃ������Ȃ̂�

namespace Sequence
{
	namespace Game
	{
		class Child;
		typedef Sequence::Parent GrandParent;

		class Parent : public Sequence::Child
		{
		public:
			enum NextSequence{
				NextMapMain,
				NextBattle,
				NextRestart,
				NextNone,
			};
			Parent();
			~Parent();
			void update(GrandParent* grandParent);
			void moveTo(NextSequence nextSequence);
			void setMapStage(int mapStage);
			std::unique_ptr<::Map::MapMainCharacter> mapMainChar_;
			::Map::MapCityCharacter *mapCityChar_[CompCharMax];  // �z���unique_ptr�͊ȒP�ɂ͂ł��Ȃ��̂Œ��߂�

		private:
			NextSequence nextSequence_;
			std::unique_ptr <Game::Child> child_;
			std::unique_ptr <::SharedCharacterStatus> sharedStatus_;
			int  mapStage_;
			void initializeParameter();
		};
	}
}
