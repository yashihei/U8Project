#pragma once

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual Scene *update() = 0;
	virtual void draw() = 0;
};

class Title : public Scene {
public:
	Title();
	Scene *update() override;
	void draw() override;
private:
};

class Play : public Scene {
public:
private:
};

class Result : public Scene {
public:
private:
};
