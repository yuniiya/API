#include "GameEngineImage.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineWindow.h>

GameEngineImage::GameEngineImage()
	: ImageDC_(nullptr)
	, BitMap_(nullptr)
	, OldBitMap_(nullptr)
	, Info_{}
{

}

GameEngineImage::~GameEngineImage()
{
	if (nullptr != BitMap_)
	{
		DeleteObject(BitMap_);
		BitMap_ = nullptr;
	}

	if (nullptr != OldBitMap_)
	{
		DeleteObject(OldBitMap_);
		OldBitMap_ = nullptr;
	}

	if (nullptr != ImageDC_)
	{
		DeleteObject(ImageDC_);
		ImageDC_ = nullptr;
	}
}

// �� ������ ó���� ������� HDC�� ���Խ�Ų��
bool GameEngineImage::Create(HDC _DC)
{
	// �� ������ DC
	ImageDC_ = _DC;
	// ��Ʈ�� ���� �޾ƿ���
	ImageScaleCheck();

	return true;
}

bool GameEngineImage::Load(const std::string& _Path)
{
	BitMap_ = static_cast<HBITMAP>(LoadImageA(
		nullptr,
		_Path.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
		));

	if (nullptr == BitMap_)
	{
		MsgBoxAssertString(_Path + "�̹��� �ε忡 �����߽��ϴ�. �����е��� ��������� ���� 1. ��δ� ����� �Ƴ���? 2. ������� ����� �ó���");
	}

	// DC�� ����
	ImageDC_ = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC_)
	{
		MsgBoxAssert("ImageDc ������ �����߽��ϴ�.");
	}

	OldBitMap_ = (HBITMAP)SelectObject(ImageDC_, BitMap_);

	ImageScaleCheck();

	return true;
}

// �� ���� ���� �뵵
bool GameEngineImage::Create(float4 _Scale)
{
	// 0,0 �̹����� ������� �ϴ��� üũ 
	if (true == _Scale.IsZero2D())
	{
		MsgBoxAssert("ũ�Ⱑ 0�� �̹����� �����Ϸ��� �߽��ϴ�");
		return false;
	}

	// �̹��� ũ��� ������ ��Ʈ���� �����
	BitMap_ = CreateCompatibleBitmap(GameEngineWindow::GetHDC(), _Scale.ix(), _Scale.iy());
	// DC���� �� ����Ʈ ��Ʈ�ʰ� ������ ���´�
	ImageDC_ = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC_)
	{
		MsgBoxAssert("ImageDC ������ �����߽��ϴ�");
	}

	// ������ ��Ʈ�ʰ� DC�� �����Ѵ� 
	OldBitMap_ = (HBITMAP)SelectObject(ImageDC_, BitMap_);

	// �ϼ��� ��Ʈ���� ������ ���´�
	ImageScaleCheck();

	return true;
}


void GameEngineImage::ImageScaleCheck()
{
	// DC�� ����� ��Ʈ���� �ڵ� ������
	HBITMAP CurrentBitMap = (HBITMAP)GetCurrentObject(ImageDC_, OBJ_BITMAP);
	GetObject(CurrentBitMap, sizeof(BITMAP), &Info_);
}


void GameEngineImage::BitCopy(GameEngineImage* _Other)
{
	// 0,0���� ���, ������, ũ�⸸ŭ
	BitCopy(_Other, { 0, 0 }, _Other->GetScale(), float4{ 0, 0 });
}

void GameEngineImage::BitCopy(GameEngineImage* _Other, const float4& _CopyPos)
{
	// ����� ��ġ �޾Ƽ� ���, ������, ũ�⸸ŭ 
	BitCopy(_Other, _CopyPos, _Other->GetScale(), float4{ 0, 0 });
}

void GameEngineImage::BitCopyCenter(GameEngineImage* _Other, const float4& _CopyPos)
{
	// �̹����� ����� ��ġ - (ũ��/2) = �߾�
	BitCopy(_Other, _CopyPos - _Other->GetScale().Half(), _Other->GetScale(), float4{ 0, 0 });
}

void GameEngineImage::BitCopyCenterPivot(GameEngineImage* _Other, const float4& _CopyPos, const float4& _CopyPivot)
{
	// �߾ӿ��� CopyPivot��ŭ ���� �÷��� ���
	BitCopy(_Other, _CopyPos - _Other->GetScale().Half() + _CopyPivot, _Other->GetScale(), float4{ 0, 0 });
}

void GameEngineImage::BitCopyBot(GameEngineImage* _Other, const float4& _CopyPos)
{
	float4 ImagePivot = _Other->GetScale().Half();
	ImagePivot.y = _Other->GetScale().y;

	// ����� ��ġ - y
	BitCopy(_Other, _CopyPos - ImagePivot, _Other->GetScale(), float4{ 0, 0 });
}

void GameEngineImage::BitCopyBotPivot(GameEngineImage* _Other, const float4& _CopyPos, const float4& _CopyPivot)
{
	float4 ImagePivot = _Other->GetScale().Half();
	ImagePivot.y = _Other->GetScale().y;

	// ����� ��ġ - y ���� CopyPivot��ŭ ���� �÷��� ��� 
	BitCopy(_Other, _CopyPos - ImagePivot + _CopyPivot, float4{ 0, 0 }, _Other->GetScale());
}

void GameEngineImage::BitCopy(GameEngineImage* _Other, const float4& _CopyPos, const float4& _CopyScale, const float4& _OtherPivot)
{	
	// �� ���� �̹��� -> ���� ������� ����
	BitBlt(
		ImageDC_,			// �̹����� ����� ��ġ�� �ڵ�
		_CopyPos.ix(),		// �̹����� ����� ��ġ
		_CopyPos.iy(),
		_CopyScale.ix(),	// ����� �̹����� ũ�� (A)
		_CopyScale.iy(),
		_Other->ImageDC_,	// ����� �̹����� �ڵ�
		_OtherPivot.ix(),	// ������ �̹����� ������ (���⼭ A��ŭ �߶�ͼ� �׸���)
		_OtherPivot.iy(),
		SRCCOPY
	);
}

// =================================================================

// ����ڰ� �־��� ũ��� �׸���
//void GameEngineImage::TransCopyCenterScale(GameEngineImage* _Other, const float4& _CopyPos, const float4& _RenderScale, unsigned int _TransColor)
//{
//	TransCopy(_Other, _CopyPos - _RenderScale.Half(), _RenderScale, float4{ 0,0 }, _Other->GetScale(), _TransColor);
//}
//
//// �̹����� ũ��� �׸���
//void GameEngineImage::TransCopyCenter(GameEngineImage* _Other, const float4& _CopyPos, unsigned int _TransColor)
//{
//	TransCopy(_Other, _CopyPos - _Other->GetScale().Half(), _Other->GetScale(), float4{ 0, 0 }, _Other->GetScale(), _TransColor);
//}

void GameEngineImage::TransCopy(GameEngineImage* _Other, const float4& _CopyPos,
	const float4& _CopyScale, 
	const float4& _OtherPivot, const float4& _OtherScale, unsigned int _TransColor)
{
	TransparentBlt(
		ImageDC_,			// �̹����� ����� ��ġ�� �ڵ�
		_CopyPos.ix(),		// �̹����� ����� ���� ��ġ 
		_CopyPos.iy(),
		_CopyScale.ix(),	// ����� �̹����� ũ�� ����
		_CopyScale.iy(),
		_Other->ImageDC_,	// ����� �̹����� �ڵ�
		_OtherPivot.ix(),	// ������ �̹����� ������ (���⼭ A��ŭ �߶�ͼ� �׸���)
		_OtherPivot.iy(),
		_OtherScale.ix(),	// �߶� ũ�� (A) -> ������ CopyScale�� ���� A�� �ø��ų� ���δ�
		_OtherScale.iy(),
		_TransColor			// �ش� ������ �����ϰ� ���
	);
}

void GameEngineImage::CutCount(int _x, int _y)
{
	float4 Scale = { GetScale().x / _x, GetScale().y / _y };
	Cut(Scale);
}

void GameEngineImage::Cut(const float4& _CutSize)
{
	/*if (0 != (GetScale().ix() % _CutSize.ix()))
	{
		MsgBoxAssert("�ڸ� �� �ִ� ��ġ�� �� �¾ƶ������� �ʽ��ϴ�.");
	}

	if (0 != (GetScale().iy() % _CutSize.iy()))
	{
		MsgBoxAssert("�ڸ� �� �ִ� ��ġ�� �� �¾ƶ������� �ʽ��ϴ�.");
	}*/

	// Sprite�� �̹����� ����, ���� ����
	int XCount = GetScale().ix() / _CutSize.ix();
	int YCount = GetScale().iy() / _CutSize.iy();

	// ����, ���� ���� ��ŭ
	for (int y = 0; y < YCount; y++)
	{
		for (int x = 0; x < XCount; x++)
		{
			CutPivot_.push_back({ static_cast<float>(x * _CutSize.ix()), static_cast<float>(y * _CutSize.iy()) });
			CutScale_.push_back(_CutSize);
		}
	}
}

int GameEngineImage::GetImagePixel(int _x, int _y)
{
	// ȭ�� ������ ������ �ش� �κ� ���������� �ν� -> ���� �÷��̾ ȭ�� ������ �� ������ 
	if (0 > _x)
	{
		return RGB(0, 0, 0);
	}

	if (0 > _y)
	{
		return RGB(0, 0, 0);
	}

	if (GetScale().ix() <= _x)
	{
		return RGB(0, 0, 0);
	}

	if (GetScale().iy() <= _y)
	{
		return RGB(0, 0, 0);
	}

	return GetPixel(ImageDC_, _x, _y);
}
