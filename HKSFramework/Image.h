#pragma once
#include <string>
#include <memory>
#include <array>
#include <d3d9.h>
#include <d3dx9.h>

class Texture {
public:
	Texture(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice);
	~Texture();
	LPDIRECT3DTEXTURE9 getTexture() const { return m_d3dTex; }
	D3DXVECTOR2 getSize() const { return m_size; }
private:
	LPDIRECT3DTEXTURE9 m_d3dTex;
	D3DXVECTOR2 m_size;
};

struct RectF {
	RectF(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	float x, y, w, h;
};

class Image {
public:
	/// <summary>�摜���쐬���܂�</summary>
	/// <param name="fileName">�t�@�C����</param>
	Image(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice);

	/// <summary>�摜���쐬���܂�</summary>
	/// <param name="texture">�e�N�X�`���ւ̃X�}�[�g�|�C���^</param>
	Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice);

	/// <summary>�摜��`�悵�܂�</summary>
	/// <param name="pos">���S���W</param>
	/// <param name="rad">��]�p�x(���W�A��)</param>
	/// <param name="scale">�g��䗦(1.0���W��)</param>
	void draw(D3DXVECTOR2 pos, float rad = 0.0, float scale = 1.0);

	/// <summary>uv���W�Ńe�N�X�`���̈ꕔ��؂������摜��`�悵�܂�</summary>
	/// <param name="uvRect">�؂���͈�</param>
	/// <param name="pos">���S���W</param>
	/// <param name="rad">��]�p�x(���W�A��)</param>
	/// <param name="scale">�g��䗦(1.0���W��)</param>
	void draw(RectF uvRect, D3DXVECTOR2 pos, float rad = 0.0, float scale = 1.0);
private:
	struct ImageVertex {
		D3DXVECTOR3 p;
		float rhw;
		D3DXVECTOR2 t;
	};
	std::shared_ptr<Texture> m_texture;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};

class AnimationImage {
public:
	/// <summary>�A�j���[�V��������摜���쐬���܂�</summary>
	/// <param name="image">Image�ւ̃X�}�[�g�|�C���^</param>
	/// <param name="col">��̕�����</param>
	/// <param name="row">�s�̕�����</param>
	/// <param name="currentRow">���s�ڂ��Đ����邩</param>
	/// <param name="time">�A�j���[�V�����̊Ԋu</param>
	/// <param name="autoLineBreak">�s�������ő���</param>
	AnimationImage(std::shared_ptr<Image> image, int col, int row, int currentRow, int time, bool autoLineBreak = false);

	/// <summary>�A�j���[�V�������X�V���܂�</summary>
	void update();

	/// <summary>�A�j���[�V����������s��ύX���܂�</summary>
	/// <param name="currentRow">�ύX����s�ԍ�</param>
	void switchRow(int currentRow) { m_currentRow = currentRow; }

	/// <summary>���݂̃t���[����`�悵�܂�</summary>
	void draw(D3DXVECTOR2 pos, float rad = 0.0, float scale = 1.0);
private:
	std::shared_ptr<Image> m_image;
	int m_col, m_row, m_currentRow, m_time;
	int m_cnt;
	bool m_autoLineBreak;
	RectF m_uvRect;
};
