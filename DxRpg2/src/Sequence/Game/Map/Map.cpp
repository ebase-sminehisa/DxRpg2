#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Parent.h"
#include "Map/AbstractMapCharacter.h"
#include "Util/ResourceLoader.h"
#include "Util/CsvMapReader.h"

namespace Sequence
{
	namespace Game
	{
		namespace Map
		{
			// �}�b�v��ʂ̏���������
			Map::Map(Sequence::Game::Parent *parent, int stage)
				: hasChanged_(true), mapStage_(stage), nextSequence_(NextMap),
				 rl_(Util::ResourceLoader::getInstance()),
				 cmr_(std::make_unique<Util::CsvMapReader>())
			{
				for (int i = 0; i < YBlock * XBlock; i++)
				{
					isPassable_[i] = Through;
				}
				parent_ = parent;
			}

			Map::~Map()
			{
			}

			void Map::update(Sequence::Game::Parent *parent)
			{
				UNREFERENCED_PARAMETER(parent);
				if (hasChanged_)
				{
					initMapState();
					initCharState();
				}
				else
				{
					// �}�b�v�̕`��
					drawMap();
					// �L�����N�^�̕`��
					drawCharacter();
				}
				// �}�b�v�ʒu�̌v�Z�y�ѕ��s��Ԃ̌���
				moveCharacter();
				// �}�b�v�ύX����
				changeMap();
			}

			// �}�b�v��ʂ̏���������
			void Map::initMapState()
			{
				cmr_->load(MapFile[mapStage_]);
				// �}�b�v�̏�Ԃ���ʉ߃t���O���쐬
				for (int y = 0; y < YBlock; y++)  // �c��15���
				{
					for (int x = 0; x < XBlock; x++)  // ����20���
					{
						if (cmr_->getMapData(x, y) > (0 | 1))  // �ʂ�Ȃ��ꏊ�Ȃ�
						{
							isPassable_[y * XBlock + x] = NoThrough;  // �ʂ�Ȃ��t���O�𗧂Ă�
						}
						// �ʂ��ꏊ�Ȃ�
						else
						{
							// �l�� + ���̈�����Ȃ�(���ꂪ�ړ����ɖ�����d�Ȃ�o�O����������i)
							// ��肷���������邪�܂�������
							if (y == 0 || y == 1 || y == YBlock - 2 || y == YBlock - 1
								|| x == 0 || x == 1 || x == XBlock - 2 || x == XBlock - 1)
							{
								// ���L��������ʑJ�ڂ��Ȃ��悤�ɂ���
								isPassable_[y * XBlock + x] = MainCharOnly;
							}
							else
							{
								isPassable_[y * XBlock + x] = Through;
							}
						}
					}
				}
			}

			void Map::initCharState()
			{
				// �L�����N�^�[��z�u����B
				int tx = parent_->mapMainChar_->getX();  // �v�Z�pY���W
				int ty = parent_->mapMainChar_->getY();   // �v�Z�pX���W
				int dir = parent_->mapMainChar_->getDir();  // ����
				int kind = 0;  // �L�����N�^�̎�� 0�͎�l��
				parent_->mapMainChar_->initMapState(tx, ty, dir, kind);
				// ��l�������̐l������Ƃ���͒ʂ�Ȃ��ꏊ�Ƃ���
				isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
				for (int i = 0; i < CompCharMax;)
				{
					// �l���Ƃ��̂Q����(-4���炳���-1���Ă��邱�Ƃɒ���[GetRand�̓���])�������
					int rx = DxLib::GetRand(XBlock - 5) + 2;
					int ry = DxLib::GetRand(YBlock - 5) + 2;
					// ���̐l�ŗ����Ō��߂��ꏊ���ʂ�Ȃ��ꏊ�Ȃ�
					if (isPassable_[ry * XBlock + rx] >= MainCharOnly)
					{
						continue;	// ��蒼��
					}
					tx = rx * BlockLen;
					ty = ry * BlockLen;
					dir = DxLib::GetRand(3);		// �����������_���Ɍ��� 0-3
					kind = i % (CharKindMax - 1) + 1;		// �l�̎�ނ����� ��0�͎�l��  1-3
					parent_->mapCityChar_[i]->initMapState(tx, ty, dir, kind);
					// ��l�������̐l������Ƃ���͒ʂ�Ȃ��ꏊ�Ƃ���
					isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
					i++;	// �ʒu���߂����������Ƃ��̂݃J�E���^����
				}
				hasChanged_ = false;
			}

			// �L�����̈ړ�����
			void Map::moveCharacter()
			{
				// ��l�� �G�ɑ���
				if (parent_->mapMainChar_->move(isPassable_))
				{
					// �G�ɑ����������ʃL���v�`�������y��~�����[�h�J��
					// ���̃^�C�~���O�ŌĂ΂Ȃ��ƃL���v�`���ł��Ȃ�(ClearDrawScreen�̑O)
					rl_.captureImgMap();
					// �J�ڐ�ݒ�
					parent_->moveTo(Parent::NextBattle);
				}
				parent_->mapMainChar_->setImg(
					rl_.getHdlImgChar(
						parent_->mapMainChar_->getKind(),
						parent_->mapMainChar_->getAnimePicPos()
					)
				);

				// ���̐l
				for (int i = 0; i < CompCharMax; i++)
				{
					parent_->mapCityChar_[i]->move(isPassable_);
					// �����C���[�W���Z�b�g
					parent_->mapCityChar_[i]->setImg(
						rl_.getHdlImgChar(
							parent_->mapCityChar_[i]->getKind(),
							parent_->mapCityChar_[i]->getAnimePicPos()
						)
					);
				}
			}

			// �}�b�v�؂�ւ�
			void Map::changeMap()
			{
				if (parent_->mapMainChar_->getX() <= 0
					|| parent_->mapMainChar_->getY() <= 0
					|| parent_->mapMainChar_->getX() >= MapWidth - BlockLen
					|| parent_->mapMainChar_->getY() >= MapHeight - BlockLen)
				{
					// �E�ɏo����
					if (parent_->mapMainChar_->getX() >= MapWidth - BlockLen)
					{
						// y���̂܂܂ň�ԍ��̈�E��
						parent_->mapMainChar_->setX(BlockLen);
						// �}�b�v�ύX
						mapStage_++;
					}
					// ���ɏo����
					if (parent_->mapMainChar_->getX() <= 0)
					{
						// y���̂܂܂ň�Ԃ̉E�����
						parent_->mapMainChar_->setX(MapWidth - BlockLen * 2);
						// �}�b�v�ύX
						mapStage_--;
					}
					// �K�v������Ύ���
					// ��A��
					// �}�b�v�ύX
					hasChanged_ = true;
					parent_->setMapStage(mapStage_);
				}
			}

			// �}�b�v�̕`��
			void Map::drawMap()
			{
				for (int y = 0; y < YBlock; y++)  // �c�̋�Ԍ������[�v
				{
					// ��l���𒆐S�Ƃ�����W�ŉ摜�̍��[/��[�J�n�_�����߂�B
					int ofsY = WindHeight / 2 - BlockLen / 2 - parent_->mapMainChar_->getY();  // ��l�����΍��W
					for (int x = 0; x < XBlock; x++)  // ���̋�Ԍ�
					{
						int ofsX = WindWidth / 2 - BlockLen / 2 - parent_->mapMainChar_->getX();  // ��l�����΍��W
						// ��������Ő���`��
						DxLib::DrawGraph(
							x * BlockLen + ofsX,	// �`�摊�Έʒu
							y * BlockLen + ofsY,				   // �`�摊�Έʒu
							rl_.getHdlImgMapTip(0), TRUE);
						// �{���A�Ő��łȂ����̉����̕`��ł����
						if (cmr_->getMapData(x, y) != 0)
						{								// ���̉摜��`��
							DxLib::DrawGraph(
								x * BlockLen + ofsX,
								y * BlockLen + ofsY,
								rl_.getHdlImgMapTip(cmr_->getMapData(x, y)), TRUE);
						}
					}
				}
			}

			// �L�����N�^�̕`��
			void Map::drawCharacter()
			{
				// �L�������X���C�h���Ă��镪�A�d�Ȃ���l�����ďォ��`�� �������A���̍l����32x48�̃L�����`�b�v�̎��̂�
				for (int y = 0; y < YBlock; y++)
				{
					int ty = parent_->mapMainChar_->getY();
					// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
					// �������Ȃ��Ǝ�O�̃L�������O�ɕ`�悳��Ȃ��i�d�Ȃ肪���������Ȃ�j
					// ��ʉ����珇�ɕ`�悷�邱�̕��@��Z�\�[�g�Ƃ���
					if (y == ty / BlockLen)
					{
							// ��l��
							// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
							// �^�񒆂���̑��Έʒu
							DxLib::DrawGraph(
								WindWidth / 2 - BlockLen / 2,	// ���S
								// �L�������c����48pixcel�̂��߁A
								// 32pixel����͂ݏo��16pixel����ɂ��炵�Ă���B
								WindHeight / 2 - BlockLen / 2 - SlidePicVal,  // ���S + �X���C�h��
								parent_->mapMainChar_->getImg(), TRUE);
					}
					for (int i = 0; i < CompCharMax; i++)
					{
						ty = parent_->mapCityChar_[i]->getY();
						// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
						// �������Ȃ��Ǝ�O�̃L�������O�ɕ`�悳��Ȃ��i�d�Ȃ肪���������Ȃ�j
						// ��ʉ����珇�ɕ`�悷�邱�̕��@��Z�\�[�g�Ƃ���
						if (y == ty / BlockLen)
						{
							// ��l���ȊO
							// �^�񒆂���̑��Έʒu
							int tx = parent_->mapCityChar_[i]->getX();
							int ofsY = WindHeight / 2 - BlockLen / 2 - parent_->mapMainChar_->getY();  // ��l�����΍��W
							int ofsX = WindWidth / 2 - BlockLen / 2 - parent_->mapMainChar_->getX();  // ��l�����΍��W
							// x,y�͍��㒸�_�̍��W�ŁA
							// x:�}�b�v��1��x���W�ɒǉ����Ă��邽�߂̂��炵
							// ��l�����������������炷
							DxLib::DrawGraph(
								tx + ofsX,
								// �L�������c����48pixcel�̂��߁A
								// 32pixel����͂ݏo��16pixel����ɂ��炵�Ă���B
								ty + ofsY - SlidePicVal,	 // �u���b�N���̂��炵�̓L�����ɂ���
								parent_->mapCityChar_[i]->getImg(), TRUE);
						}
					}
				}
			}
		}
	}
}
