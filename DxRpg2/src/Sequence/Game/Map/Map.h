#pragma once
#include <memory>
#include <vector>
#include "StdCommon.h"
#include "Sequence/Game/IChild.h"
#include "Map/MapMainCharacter.h"
#include "Map/MapCityCharacter.h"

class SharedCharacterStatus;
namespace Util
{
	class CsvMapReader;
	class ResourceLoader;
}

// �萔�֘A�̏��
namespace Sequence
{
	class Parent;
	namespace Game
	{
		namespace Map
		{
#ifdef CHARCHIP32X48  // 32x48�̃L�����`�b�v�̏ꍇ
			const int SlidePicVal = 16;			// �L�����c�`��ʒu���炵��
#else  // 32x32�̃L�����`�b�v�̏ꍇ
			const int SlidePicVal = 0;			// �L�����c�`��ʒu���炵��
#endif
			const int MapNum = 2;
			const char MapFile[MapNum][MAX_PATH + 1] =
			{
				"resources/csv/map/20x16/0.txt",
				"resources/csv/map/20x16/1.txt"
			};

			// ���[���h�}�b�v����̃N���X
			class Map : public Sequence::Game::IChild
			{
			public:
				enum NextSequence
				{
					NextMap,
					NextBattle,
				};
				Map(SharedCharacterStatus *scs, int stage);
				~Map();
				void update(Parent *parent) override;

			private:
				SharedCharacterStatus *scs_;
				// ::��t���Ă���̂͂ǂ�����Map���킩��Ȃ�����
				std::unique_ptr<::Map::MapMainCharacter> mapMainChar_;
				std::vector<std::unique_ptr<::Map::MapCityCharacter>> mapCityChar_;
				
				Util::ResourceLoader &rl_;
				std::unique_ptr<Util::CsvMapReader> cmr_;
				int isPassable_[YBlock * XBlock];	// �ʍs�\���ǂ����̏��(�d�v�I)
				int mapStage_;
				bool hasChanged_;
				NextSequence nextSequence_;
				void initMapState();
				void initCharState();
				void drawMap();
				void moveCharacter(Parent *parent);
				void changeMap(Parent *parent);
				void drawCharacter();
			};
		}
	}
}