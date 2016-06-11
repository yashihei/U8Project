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
	/// <summary>画像を作成します</summary>
	/// <param name="fileName">ファイル名</param>
	Image(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice);

	/// <summary>画像を作成します</summary>
	/// <param name="texture">テクスチャへのスマートポインタ</param>
	Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice);

	/// <summary>画像を描画します</summary>
	/// <param name="pos">中心座標</param>
	/// <param name="rad">回転角度(ラジアン)</param>
	/// <param name="scale">拡大比率(1.0が標準)</param>
	void draw(D3DXVECTOR2 pos, float rad = 0.0, float scale = 1.0);

	/// <summary>uv座標でテクスチャの一部を切り取った画像を描画します</summary>
	/// <param name="uvRect">切り取る範囲</param>
	/// <param name="pos">中心座標</param>
	/// <param name="rad">回転角度(ラジアン)</param>
	/// <param name="scale">拡大比率(1.0が標準)</param>
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
	/// <summary>アニメーションする画像を作成します</summary>
	/// <param name="image">Imageへのスマートポインタ</param>
	/// <param name="col">列の分割数</param>
	/// <param name="row">行の分割数</param>
	/// <param name="currentRow">何行目を再生するか</param>
	/// <param name="time">アニメーションの間隔</param>
	/// <param name="autoLineBreak">行を自動で送る</param>
	AnimationImage(std::shared_ptr<Image> image, int col, int row, int currentRow, int time, bool autoLineBreak = false);

	/// <summary>アニメーションを更新します</summary>
	void update();

	/// <summary>アニメーションさせる行を変更します</summary>
	/// <param name="currentRow">変更する行番号</param>
	void switchRow(int currentRow) { m_currentRow = currentRow; }

	/// <summary>現在のフレームを描画します</summary>
	void draw(D3DXVECTOR2 pos, float rad = 0.0, float scale = 1.0);
private:
	std::shared_ptr<Image> m_image;
	int m_col, m_row, m_currentRow, m_time;
	int m_cnt;
	bool m_autoLineBreak;
	RectF m_uvRect;
};
