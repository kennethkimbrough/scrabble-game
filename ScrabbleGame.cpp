#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>

// Constants
const int BOARD_SIZE = 15;
const int TILE_SIZE = 30;
const int FONT_SIZE = 24;
const int SCORE_FONT_SIZE = 36;
const int MARGIN = 10;

// Colors
const sf::Color BOARD_COLOR = sf::Color(255, 255, 255);
const sf::Color TILE_COLOR = sf::Color(128, 128, 128);
const sf::Color SELECTED_TILE_COLOR = sf::Color(255, 0, 0);
const sf::Color TEXT_COLOR = sf::Color(0, 0, 0);

// Font
sf::Font font;

// Board
std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' '));

// Tiles
std::vector<sf::RectangleShape> tiles;
std::vector<char> tileLetters;
std::vector<bool> tileSelected;

// Score
int score = 0;

// Function to draw the board
void drawBoard(sf::RenderWindow& window) {
  sf::RectangleShape boardRect(sf::Vector2f(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE));
  boardRect.setFillColor(BOARD_COLOR);
  boardRect.setPosition(MARGIN, MARGIN);
  window.draw(boardRect);

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      sf::RectangleShape tileRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
      tileRect.setFillColor(TILE_COLOR);
      tileRect.setPosition(MARGIN + i * TILE_SIZE, MARGIN + j * TILE_SIZE);
      window.draw(tileRect);

      sf::Text tileText;
      tileText.setFont(font);
      tileText.setCharacterSize(FONT_SIZE);
      tileText.setFillColor(TEXT_COLOR);
      tileText.setString(std::string(1, board[i][j]));
      tileText.setPosition(MARGIN + i * TILE_SIZE + TILE_SIZE / 2 - tileText.getLocalBounds().width / 2, MARGIN + j * TILE_SIZE + TILE_SIZE / 2 - tileText.getLocalBounds().height / 2);
      window.draw(tileText);
    }
  }
}

// Function to draw the tiles
void drawTiles(sf::RenderWindow& window) {
  for (int i = 0; i < tiles.size(); i++) {
    sf::RectangleShape tileRect = tiles[i];
    tileRect.setFillColor(tileSelected[i] ? SELECTED_TILE_COLOR : TILE_COLOR);
    window.draw(tileRect);

    sf::Text tileText;
    tileText.setFont(font);
    tileText.setCharacterSize(FONT_SIZE);
    tileText.setFillColor(TEXT_COLOR);
    tileText.setString(std::string(1, tileLetters[i]));
    tileText.setPosition(tileRect.getPosition().x + TILE_SIZE / 2 - tileText.getLocalBounds().width / 2, tileRect.getPosition().y + TILE_SIZE / 2 - tileText.getLocalBounds().height / 2);
    window.draw(tileText);
  }
}

// Function to draw the score
void drawScore(sf::RenderWindow& window) {
  sf::Text scoreText;
  scoreText.setFont(font);
  scoreText.setCharacterSize(SCORE_FONT_SIZE);
  scoreText.setFillColor(TEXT_COLOR);
  scoreText.setString("Score: " + std::to_string(score));
  scoreText.setPosition(MARGIN, MARGIN + BOARD_SIZE * TILE_SIZE + SCORE_FONT_SIZE);
  window.draw(scoreText);
}

// Function to handle mouse click
void handleMouseClick(sf::RenderWindow& window, sf::Event& event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    for (int i = 0; i < tiles.size(); i++) {
      if (tiles[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        tileSelected[i] = !tileSelected[i];
      }
    }
  }
}

// Function to handle key press
void handleKeyPress(sf::RenderWindow& window, sf::Event& event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Space) {
      // Place selected tiles on the board
      for (int i = 0; i < tiles.size(); i++) {
        if (tileSelected[i]) {
          // Find an empty space on the board
          for (int j = 0; j < BOARD_SIZE; j++) {
            for (int k = 0; k < BOARD_SIZE; k++) {
              if (board[j][k] == ' ') {
                board[j][k] = tileLetters[i];
                tileSelected[i] = false;
                break;
              }
            }
          }
        }
      }
    } else if (event.key.code == sf::Keyboard::Enter) {
      // Calculate score
      int wordScore = 0;
      for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
          if (board[i][j] != ' ') {
            wordScore += getLetterScore(board[i][j]);
          }
        }
      }
      score += wordScore;
    }
  }
}

// Function to get the score of a letter
int getLetterScore(char letter) {
  // TO DO: implement letter scoring system
  return 1;
}

int main() {
  // Initialize SFML
  sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * TILE_SIZE + MARGIN * 2, BOARD_SIZE * TILE_SIZE + MARGIN * 2 + SCORE_FONT_SIZE), "Scrabble");

  // Load font
  font.loadFromFile("arial.ttf");

  // Initialize tiles
  for (int i = 0; i < 7; i++) {
    sf::RectangleShape tileRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    tileRect.setPosition(MARGIN + i * TILE_SIZE, MARGIN + BOARD_SIZE * TILE_SIZE);
    tiles.push_back(tileRect);
    tileLetters.push_back('A' + i);
    tileSelected.push_back(false);
  }

  // Game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonPressed) {
        handleMouseClick(window, event);
      } else if (event.type == sf::Event::KeyPressed) {
        handleKeyPress(window, event);
      }
    }

    window.clear();

    drawBoard(window);
    drawTiles(window);
    drawScore(window);

    window.display();
  }

  return 0;
}
