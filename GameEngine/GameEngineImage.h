#pragma once
#include <GameEngineBase/GameEngineNameObject.h>
#include <GameEngineBase/GameEngineMath.h>
#include <Windows.h>
#include <vector>

class GameEngineImage : public GameEngineNameObject
{
public:
	// constrcuter destructer
	GameEngineImage();
	~GameEngineImage();

	// delete Function
	GameEngineImage(const GameEngineImage& _Other) = delete;
	GameEngineImage(GameEngineImage&& _Other) noexcept = delete;
	GameEngineImage& operator=(const GameEngineImage& _Other) = delete;
	GameEngineImage& operator=(GameEngineImage&& _Other) noexcept = delete;

	bool Create(float4 _Scale);
	bool Create(HDC _DC);

	bool Load(const std::string& _Path);

	inline float4 GetScale()
	{
		return float4(static_cast<float>(Info_.bmWidth), static_cast<float>(Info_.bmHeight));
	}

	inline HDC ImageDC()
	{
		return ImageDC_;
	}

	// �� ���� �̹����� ���� ������� �����ϴ� �Լ���
	void BitCopy(GameEngineImage* _Other);

	void BitCopy(GameEngineImage* _Other, const float4& _CopyPos);

	void BitCopyCenter(GameEngineImage* _Other, const float4& _CopyPos);

	void BitCopyCenterPivot(GameEngineImage* _Other, const float4& _CopyPos, const float4& _CopyPivot);

	void BitCopyBot(GameEngineImage* _Other, const float4& _CopyPos);

	void BitCopyBotPivot(GameEngineImage* _Other, const float4& _CopyPos, const float4& _CopyPivot);

	// BitBlt�� ����� ���� ����
	void BitCopy(GameEngineImage* _Other, const float4& _CopyPos,
		const float4& _CopyScale, const float4& _OtherPivot);


	// ===========================================================
	//void TransCopyCenterScale(GameEngineImage* _Other, const float4& _CopyPos, const float4& _RenderScale, unsigned int _TransColor);
	//void TransCopyCenter(GameEngineImage* _Other, const float4& _CopyPos, unsigned int _TransColor);


	void TransCopy(GameEngineImage* _Other, const float4& _CopyPos,
		const float4& _CopyScale,
		const float4& _OtherPivot, const float4& _OtherScale, unsigned int _TransColor);

	void Cut(const float4& _CutSize);

	void CutCount(int _x, int _y);

	// �̹����� �߷� �ִ��� -> ���� ���� �����Ͱ� 0�� �ƴ϶�� 
	inline bool IsCut()
	{
		return 0 != CutPivot_.size();
	}

	inline size_t GetCutCount()
	{
		return CutPivot_.size();
	}

	inline float4 GetCutPivot(size_t _Index)
	{
		return CutPivot_[_Index];
	}

	// �̹������� �߶� ũ�⸦ �ٸ��� �ϰ� �ʹ�?
	inline float4 GetCutScale(size_t _Index)
	{
		return CutScale_[_Index];
	}

	void Cut(const float4& _CutScale, const float4& _CutPos)
	{
		CutPivot_.push_back(_CutPos);
		CutScale_.push_back(_CutScale);
	}

	inline int GetImagePixel(const float4& _Pos)
	{
		return GetImagePixel(_Pos.ix(), _Pos.iy());
	}

	// �ش� �ȼ��� ���� �˷���
	int GetImagePixel(int _x, int _y);

protected:

private:
	// �� ���ۿ� �׸� DC
	HDC ImageDC_;
	HBITMAP BitMap_;
	HBITMAP OldBitMap_;
	BITMAP Info_;

	// sprite���� �ڸ� �̹����� ��ǥ, ũ��
	std::vector<float4> CutPivot_;
	std::vector<float4> CutScale_;

	void ImageScaleCheck();
};
