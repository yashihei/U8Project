#pragma once

enum class SceneState {
	Title, Play, Result
};

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual SceneState run() = 0;
};

class Title : public Scene {
public:
	Title();
	SceneState run();
private:
};

class Play : public Scene {
public:
	Play();
	SceneState run();
private:;
};

class Result : public Scene {
public:
private:
};
