# HKSFramework
DirectX9を中心とした、ライブラリです。

## Classes
### Actor
ゲーム内オブジェクトの基底クラスです。

### ActorManager
Actorを管理するクラスです。

### Animation
Textureをアニメーションすることが出来ます。

### FPSControler
FPSの管理をします。

### GameApp
メインループを持ったゲームの雛形です。新規でゲームを作る際には、GameAppを継承します。

### GraphicDevice
Direct3DDeviceを扱います。

### InputManager
Keyboard、Mouse、XInputなどの入力機器を扱います。

### Point
整数型の2Dベクトルクラスです。

### Random
mt19937のラッパーです。

### Shape
簡易的な図形を描画します。

### SoundManager
XAudio2を使って、Waveファイルの読み込み、再生などをします。

### Texture
テクスチャの描画をします。

### TextureManager
テクスチャの管理をします。

## Build
VS2013 + DirectX SDK (June2010)

## License
MIT