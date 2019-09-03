#pragma once
#include "StdCommon.h"

namespace
{
	const int	MapTipNum = 4;  // �}�b�v�`�b�v��
	const int	BackGroundNum = 20;  // �w�i�̐�
	const int	MonsterNum = 10;
	const int	ImgNumberKind = 5;
	const int	ImgNumberDivNum = 10;
	const int	ImgAnimationKind = 10;
	const int	ImgAnimationDivNum = 25;
	const int	ImgEtcNum = 5;
}

namespace Util
{
	class ResourceLoader
	{
	public:
		// ������R�s�[���֎~
		ResourceLoader(const ResourceLoader&) = delete;
		ResourceLoader& operator=(const ResourceLoader&) = delete;
		ResourceLoader(ResourceLoader&&) = delete;
		ResourceLoader& operator=(ResourceLoader&&) = delete;

		static ResourceLoader &getInstance();

		int load();
		void captureImgMap();
		int getHdlImgChar(int kind, int pos) const;
		int getHdlImgMapTip(int kind) const;
		int getHdlImgBackGround(int kind) const;
		int getHdlImgMonster(int kind) const;
		int getHdlImgStartBattle(int kind) const;
		void setImageStartBattle(int idx, int width, int height);

		int getCureImage(int frame) const;

		int getSlashImage(int frame) const;
		int getWindImage(int frame) const;
		int getFireImage(int frame) const;
		int getIceImage(int frame) const;
		int getThunderImage(int frame) const;

		int getHdlImgNumber(int kind, int pos) const;

		int getHdlImgEtc(int kind) const;
		int getHdlImgCursor() const;
		int getHdlFont(int kind) const;
		int getHdlNullImg() const;

	private:
		ResourceLoader() {};
		~ResourceLoader() {};
		void dispProgress();
		void loadMapTip(char *resourcePath, int *handle);
		void loadCharacterTip(char *resourcePath, int *handle);
		void loadImage(char *resourcePath, int *handle);
		
		int hdlImgMapTip_[MapTipNum];  // �}�b�v�`�b�v(32x32pixel)�i�[�p
		int hdlImgChar_[CharKindMax][ImgCharDivNum];   // �L�����摜
		int hdlImgBackGround_[BackGroundNum];
		int hdlImgMonster_[MonsterNum];
		int hdlImgStartBattle_[ImgStartBattleDivNum];

		int hdlImgNumber_[ImgNumberKind][ImgNumberDivNum];
		int hdlImgAnimation_[ImgAnimationKind][ImgAnimationDivNum];
		int hdlImgEtc_[ImgEtcNum];
		int hdlImgCursor_;
		int hdlFont_[2];
	};
}