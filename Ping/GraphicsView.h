#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "GameSnapshot.h"

/// The GraphicsView provides the ability to draw GameSnapshots and other
/// information to the screen.
class GraphicsView
{
public:
	GraphicsView(void);
	virtual ~GraphicsView(void);

	void initialize(sf::RenderWindow *window);
	bool loadContent();

	void drawSnapshot(GameSnapshot *snapshot);
	void drawBounds(GameSnapshot *snapshot, sf::Color color);
	void drawScore(int *score);

protected:
	void drawCountdown(float time);

	std::string resPath;

	sf::Font font;

	sf::RenderWindow *window;

	sf::Texture backgroundTex;
	sf::Texture paddleTex;
	sf::Texture paddleShadowTex;
	sf::Texture ballTex;
	sf::Texture ballShadowTex;

	sf::Sprite backgroundSprite;
	sf::Sprite ballSprite;
	sf::Sprite ballShSprite;
	sf::Sprite p0Sprite;
	sf::Sprite p0ShSprite;
	sf::Sprite p1Sprite;
	sf::Sprite p1ShSprite;
};

