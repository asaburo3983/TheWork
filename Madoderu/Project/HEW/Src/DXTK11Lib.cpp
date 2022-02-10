#include "DXTK11Lib.h"

namespace DXTK11LIB {

	float imageExtend = 1.0f;//�S�摜�̔{��

	std::unique_ptr<DirectX::SpriteBatch> fontBatch;//�t�H���g�p�̋��ʕϐ�
	std::unique_ptr<DirectX::SpriteFont> fontSprite;

	float alphaDefault = 1.0f;//�S�̂̃f�t�H���g�A���t�@�l

	ID3D11DeviceContext* context;
	ID3D11Device* device;

	
	//��������
	void InitDXTK11Lib() {
		context = DirectGraphics::GetInstance()->GetContext();
		device = DirectGraphics::GetInstance()->GetDevice();
		//�t�H���g�ǂݍ���
		fontBatch = std::make_unique<DirectX::SpriteBatch>(context);
		fontSprite = std::make_unique<DirectX::SpriteFont>(device, L"Font/myfile.spritefont");

	}

	//�QD�`��n//////////////////////////////

	//�����`��
	void DrawString(const wchar_t* str, int x, int y, StrColor _color, float size, float angle) {

		DirectX::XMVECTORF32 color;
		for (int i = 0; i < 4; i++) {
			color.f[i] = _color.rgba[i];
		}
		
		//�`��
		fontBatch->Begin();

		fontSprite->DrawString(fontBatch.get(), str, DirectX::XMFLOAT2(x*imageExtend, y*imageExtend), color, 0, DirectX::XMFLOAT2(0, 0), size*imageExtend);

		fontBatch->End();
	}
	void DrawString(const char* str, int x, int y, StrColor _color, float size, float angle) {
		size_t tmpsize = strlen(str);//char�^��WCHAR�ɕϊ�
		int  tmpsizei = tmpsize + 1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, str, _TRUNCATE);

		DrawString(tmpwbuf, x, y, _color, size, angle);

		delete tmpwbuf;
	}

	//FPS��`��
	int frame = 0;
	unsigned long base_time = 0.0;
	unsigned long prev = 0.0;
	unsigned long current = 0.0;
	char	buf[20];
	WCHAR wbuf[20];
	void DrawFPS() {
		// ���Ԏ擾
		base_time = timeGetTime();


		float intarval = 0.0;

		// �t���[�����[�g�o��
		if ((frame + 1) % 60 == 0)
		{
			current = timeGetTime();

			intarval = 60000.0f / (current - prev);

			sprintf_s(buf, sizeof(buf), "FPS : %.1f", intarval);
			prev = current;
		}
		frame++;
		mbstowcs_s(0, wbuf, 20, buf, _TRUNCATE);
		DrawString(wbuf, 0, 0, StrColor{ 1,0,0,1 });

	}

	//�S�̂̃f�t�H���g���l���Z�b�g
	void SetDefaultAlpha(float _alpha) {
		alphaDefault = _alpha;
	}
	//�S�̂̒ǉ��̊g�嗦���Z�b�g
	void SetDefaultImageExtend(float _extend) {
		imageExtend = _extend;
	}
	float GetDefaultImageExtend() {
		return imageExtend;
	}
	//////////////////////////////
	//2D�摜//////////////////////////////
	Image::Image(const wchar_t* filePath) {//�摜���[�h �R���X�g���N�^
		Load(filePath);
	}
	void Image::Load(const wchar_t* filePath) {//�摜���[�h�@
		//�摜�`��ǉ���
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;

		//�摜�ǂݍ���
		DirectX::CreateWICTextureFromFile(device, filePath, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		DX::ThrowIfFailed(resource.As(&texture));

		//�摜�f�[�^���Ƃ�
		CD3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);

		textureCenter = DirectX::SimpleMath::Vector2(textureDesc.Width / 2, textureDesc.Height / 2);

		width = textureDesc.Width;//�摜�T�C�Y�̊i�[
		height = textureDesc.Height;

	}

	void Image::Load(const char* filePath) {//�摜���[�h�@
		size_t tmpsize = strlen(filePath);//char�^��WCHAR�ɕϊ�
		int  tmpsizei = tmpsize + 1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, filePath, _TRUNCATE);

		Load(tmpwbuf);

		delete tmpwbuf;
	}

	void Image::Draw(int _x, int _y, float _angle, float _size, float _alpha, int _center) {
		x = _x* imageExtend;
		y = _y* imageExtend;
		sizeX = _size * imageExtend;
		sizeY = _size * imageExtend;

		center = _center;

		//�V�F�[�_�[�𖳌����ɂ��Ă��� ���ꂪ�Ȃ��ƕʂ̃V�F�[�_�[���K������ăo�O��
		// VS�ɒ��_�V�F�[�_��ݒ�
		context->VSSetShader(NULL, NULL, 0);
		// GS�ɃW�I���g���E�V�F�[�_��ݒ�
		context->GSSetShader(NULL, NULL, 0);
		// RS�Ƀ��X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
		context->RSSetState(NULL);
		// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
		context->PSSetShader(NULL, NULL, 0);

		//�����͂̍ۃf�t�H���g�̃��l������
		if (_alpha == -1000.0123f)
			_alpha = alphaDefault;
		//�摜�`��
		m_spriteBatch->Begin();
		
		m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), NULL, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter*_center, sizeX);
		m_spriteBatch->End();
	}
	void Image::DrawEX(int _x, int _y, float _angle, float _sizeX, float _sizeY, float _alpha, int _center, bool LR_TURN, bool UD_TURN,RECT drawArea) {
		x = _x * imageExtend;
		y = _y * imageExtend;

		center = _center;
		sizeX = _sizeX * imageExtend;
		sizeY = _sizeY * imageExtend;

		//�V�F�[�_�[�𖳌����ɂ��Ă��� ���ꂪ�Ȃ��ƕʂ̃V�F�[�_�[���K������ăo�O��
		// VS�ɒ��_�V�F�[�_��ݒ�
		context->VSSetShader(NULL, NULL, 0);
		// GS�ɃW�I���g���E�V�F�[�_��ݒ�
		context->GSSetShader(NULL, NULL, 0);
		// RS�Ƀ��X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
		context->RSSetState(NULL);
		// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
		context->PSSetShader(NULL, NULL, 0);

		//�����͂̍ۃf�t�H���g�̃��l������
		if (_alpha == -1000.0123f)
			_alpha = alphaDefault;

		if (drawArea.left == -1) {
			drawArea.left = 0;
			drawArea.right = width;
			drawArea.bottom = height;

		}

		const RECT* rect = &drawArea;



		m_spriteBatch->Begin();

		if (!LR_TURN && !UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_None);
		}

		else if (LR_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipHorizontally);

		}
		else if (UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipVertically);

		}
		else {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipBoth);

		}

		m_spriteBatch->End();

	}

	void Image::DrawBegin() {
		m_spriteBatch->Begin();
	}
	void Image::DrawEnd() {
		m_spriteBatch->End();
	}
	void Image::DrawEXManualBegin(int _x, int _y, float _angle, float _sizeX, float _sizeY, float _alpha, int _center, bool LR_TURN, bool UD_TURN, RECT drawArea) {
		x = _x * imageExtend;
		y = _y * imageExtend;

		center = _center;
		sizeX = _sizeX * imageExtend;
		sizeY = _sizeY * imageExtend;

		//�V�F�[�_�[�𖳌����ɂ��Ă��� ���ꂪ�Ȃ��ƕʂ̃V�F�[�_�[���K������ăo�O��
		// VS�ɒ��_�V�F�[�_��ݒ�
		context->VSSetShader(NULL, NULL, 0);
		// GS�ɃW�I���g���E�V�F�[�_��ݒ�
		context->GSSetShader(NULL, NULL, 0);
		// RS�Ƀ��X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
		context->RSSetState(NULL);
		// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
		context->PSSetShader(NULL, NULL, 0);

		//�����͂̍ۃf�t�H���g�̃��l������
		if (_alpha == -1000.0123f)
			_alpha = alphaDefault;

		if (drawArea.left == -1) {
			drawArea.left = 0;
			drawArea.right = width;
			drawArea.bottom = height;

		}

		const RECT* rect = &drawArea;

		if (!LR_TURN && !UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_None);
		}

		else if (LR_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipHorizontally);

		}
		else if (UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipVertically);

		}
		else {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipBoth);

		}

	}

	bool Image::Hit(int _x, int _y) {//�摜���ɓ����������蔻�� ��]���ĂȂ��l�p���摜���������蔻����Ƃ��
		int minX, maxX, minY, maxY;
		if (center == 1) {
			maxX = x + (width / 2 * sizeX);
			minX = x - (width / 2 * sizeX);
			maxY = y + (height / 2 * sizeY);
			minY = y - (height / 2 * sizeY);
		}
		else {
			maxX = x + (width * sizeX);
			minX = x;
			maxY = y + (height * sizeY);
			minY = y;
		}

		//�����蔻���
		if (minX <= _x && _x <= maxX && minY <= _y && _y <= maxY) {
			return true;
		}
		return false;
	}
	//////////////////////////////
	//���y//////////////////////////////

	Sound_MP3::Sound_MP3(const wchar_t* filePath) {
		Load(filePath);
	}
	void Sound_MP3::Load(const wchar_t* filePath) {

		MFPCreateMediaPlayer(filePath, FALSE, 0, NULL, NULL, &g_pPlayer);
		
	}
	void Sound_MP3::Load(const char* filePath) {
		size_t tmpsize = strlen(filePath);//char�^��WCHAR�ɕϊ�
		int  tmpsizei = tmpsize+1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, filePath, _TRUNCATE);

		MFPCreateMediaPlayer(tmpwbuf, FALSE, 0, NULL, NULL, &g_pPlayer);
		delete tmpwbuf;
	}
	void Sound_MP3::Play() {
		g_pPlayer->GetState(&state);
		if (state == MFP_MEDIAPLAYER_STATE_STOPPED || state ==MFP_MEDIAPLAYER_STATE_EMPTY)
		{
			g_pPlayer->Play();

		}
	}
	void Sound_MP3::Stop() {
		g_pPlayer->GetState(&state);
		if (state != MFP_MEDIAPLAYER_STATE_STOPPED) {
			g_pPlayer->Stop();
		}
	}
	void Sound_MP3::Volume(float vol) {
		g_pPlayer->SetVolume(vol);
	}
	//////////////////////////////
}