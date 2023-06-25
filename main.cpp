#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string.h>

using namespace sf;
using namespace std;

struct Block
{
        int x, y, row, col, ran, matching;
        Block() { matching = 0; }
} board[8][8];

void initBoard(int ts)
{
        srand(time(0));

        // Assigning to 2d-array
        for (int i = 1; i <= 8; i++)
        {
                for (int j = 1; j <= 8; j++)
                {
                        board[i][j].ran = rand() % 6;
                        board[i][j].col = j;
                        board[i][j].row = i;
                        board[i][j].x = j * ts;
                        board[i][j].y = i * ts;
                }
        }
}

void switch_piece(Block p1, Block p2)
{
        int swappy;
        swappy = p1.col;
        p1.col = p2 .col;
        p2.col = swappy;

        swappy = p1.row;
        p1.row = p2.row;
        p2.row = swappy;
        // swap(p1.col, p2.col);
        // swap(p1.row, p2.row);
        // swap(p1.x, p2.x);
        // swap(p1.y, p2.y);
        board[p1.row][p1.col] = p1;
        board[p2.row][p2.col] = p2;
}

int main()
{
        int ts = 78; // tile size
        Vector2i offset(48, 24);
        // gems.setTextureRect(IntRect(p.ran * 49, 0, 49, 49));
        initBoard(ts);

        RenderWindow app(VideoMode(1100, 688), "Bubble Burst");
        app.setFramerateLimit(60);

        // MUSIC
        Music music;
        music.openFromFile("music/background.ogg");
        music.play();
        music.setLoop(true);
        // sound effects
        Music pop;
        pop.openFromFile("music/test2.ogg");

        Texture t1, t2, t3, t4, t5, t6;
        t1.loadFromFile("sprites/bg.png");
        t2.loadFromFile("sprites/bubbles.png");
        t3.loadFromFile("sprites/cursor.png");
        t4.loadFromFile("sprites/menu.png");
        t5.loadFromFile("sprites/gameover.png");
        t6.loadFromFile("sprites/win.png");

        Sprite background(t1), gems(t2), cursor(t3), menu(t4), gameover(t5), win(t6);

        int x0, y0, x, y;
        int click = 0, time = 60, moves = 15, score = 0, input = 0;
        string temp, temp1, temp2;
        Clock Clock;
        Time time_elapsed;
        Font font;
        if (!font.loadFromFile("fonts/Romans Story.otf"))
                throw("Could Not Load Font");

        Text text, text1;
        text.setFont(font);
        text.setCharacterSize(26);
        text.setColor(Color::Black);
        text.setStyle(Text::Bold);
        text.setPosition(1100 - 315, 688 - 499);

        text1.setFont(font);
        text1.setCharacterSize(26);
        text1.setColor(Color::Black);
        // text1.setStyle(Text::Bold);
        text1.setPosition(1100 - 315, 688 - 470);

        Vector2i pos;
        bool swapping = false, movement = false;

        // Making of progress bar
        Text text_progressbar;
        text_progressbar.setFont(font);
        text_progressbar.setCharacterSize(26);
        text_progressbar.setColor(Color::Black);
        text_progressbar.setStyle(Text::Bold);
        text_progressbar.setPosition(1100 - 315, 688 - 410);
        float rect_size = 0.0;
        RectangleShape progressbar;
        progressbar.setFillColor(Color::Black);
        progressbar.setOutlineThickness(2);
        progressbar.setOutlineColor(Color::White);
        progressbar.setPosition(1100 - 315, 688 - 370);
        while (app.isOpen())
        {

                Event e;
                while (app.pollEvent(e))
                {
                        if (e.type == Event::Closed)
                                app.close();

                        if (e.type == Event::MouseButtonPressed)
                                if (e.key.code == Mouse::Left)
                                {
                                        if (!swapping && !movement)
                                                click++;
                                        pos = Mouse::getPosition(app) - offset;
                                }
                        if (e.type == Event::KeyPressed && e.key.code == Keyboard::D)
                        {
                                input = 1;
                        }
                        else if (e.type == Event::KeyPressed && e.key.code == Keyboard::S)
                        {
                                input = 2;
                        }
                        else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                        {
                                app.close();
                        }
                }

                // Timer Reset
                if (input == 0 || input == 2)
                {
                        time_elapsed = Clock.restart();
                }

                if (input == 3)
                {
                        app.draw(gameover);
                        app.display();
                }
                else if(input == 4)
                {
                        app.draw(win);
                        app.display();
                }

                if (input == 1 || input == 2)
                {
                        // Mouse
                        app.clear();

                        time_elapsed = Clock.getElapsedTime();

                        // Timer
                        if (input == 1)
                        {
                                temp1 = to_string(int(time - time_elapsed.asSeconds()));
                        }

                        if (click == 1)
                        {
                                x0 = pos.x / ts + 1;
                                y0 = pos.y / ts + 1;
                        }
                        if (click == 2)
                        {
                                x = pos.x / ts + 1;
                                y = pos.y / ts + 1;
                                if (abs(x - x0) + abs(y - y0) == 1)
                                {
                                        pop.play();
                                        switch_piece(board[y0][x0], board[y][x]);
                                        swapping = 1;
                                        if (input == 2)
                                                moves--;
                                        click = 0;
                                }
                                else
                                        click = 1;
                        }

                        // Win/Lose
                        if (moves <= 0)
                        {
                                input = 3;
                        }

                        if (input == 1)
                        {
                                if (score >= 1500)
                                        input = 4;
                        }
                        else
                        {
                                if (score >= 2000)
                                        input = 4;
                        }

                        if (input == 1)
                        {
                                if (int(time - time_elapsed.asSeconds()) == 0)
                                        input = 3;
                        }

                        temp2 = to_string(moves);
                        
                        // Matching

                        for (int i = 1; i <= 8; i++)
                        {
                                for (int j = 1; j <= 8; j++)
                                {
                                        if (board[i][j].ran == board[i + 1][j].ran)
                                                if (board[i][j].ran == board[i - 1][j].ran)
                                                {
                                                        temp = to_string(++score);
                                                        rect_size += 0.1;
                                                        // cout<<rect_size<<endl;
                                                        progressbar.setSize(Vector2f(rect_size * 1, 10));
                                                        pop.play();
                                                        // app.draw(progressbar);
                                                        for (int k = -1; k <= 1; k++)
                                                                board[i + k][j].matching++;
                                                }

                                        if (board[i][j].ran == board[i][j + 1].ran)
                                                if (board[i][j].ran == board[i][j - 1].ran)
                                                {
                                                        temp = to_string(++score);
                                                        rect_size += 0.1;
                                                        // cout<<hp<<endl;
                                                        progressbar.setSize(Vector2f(rect_size * 1, 10));
                                                        pop.play();
                                                        for (int k = -1; k <= 1; k++)
                                                                board[i][j + k].matching++;
                                                }
                                }
                        }

                        // Animation swap
                        movement = false;
                        for (int i = 1; i <= 8; i++)
                        {
                                for (int j = 1; j <= 8; j++)
                                {
                                        Block &p = board[i][j];
                                        int dx, dy;
                                        dx = p.x - p.col * ts;
                                        dy = p.y - p.row * ts;
                                        if (dx)
                                                p.x -= 3 * (dx / abs(dx));
                                        if (dy)
                                                p.y -= 3 * (dy / abs(dy));

                                        if (dx || dy)
                                                movement = 1;
                                }
                        }

                        // Points
                        int points = 0;
                        for (int i = 1; i <= 8; i++)
                        {
                                for (int j = 1; j <= 8; j++)
                                {
                                        points += board[i][j].matching;
                                }
                        }

                        // If no match
                        if (swapping && !movement)
                        {
                                if (!points)
                                {
                                        switch_piece(board[y0][x0], board[y][x]);
                                }
                                swapping = 0;
                        }

                        // Updation

                        if (!movement)
                        {
                                for (int i = 8; i > 0; i--)
                                {
                                        for (int j = 1; j <= 8; j++)
                                        {
                                                if (board[i][j].matching)
                                                {
                                                        for (int n = i; n > 0; n--)
                                                        {
                                                                if (!board[n][j].matching)
                                                                {
                                                                        switch_piece(board[n][j], board[i][j]);
                                                                        break;
                                                                }
                                                        }
                                                }
                                        }
                                }
                                for (int j = 1; j <= 8; j++)
                                {
                                        for (int i = 8, n = 0; i > 0; i--)
                                        {
                                                if (board[i][j].matching)
                                                {
                                                        board[i][j].ran = rand() % 6;
                                                        board[i][j].y = -ts * n++;
                                                        board[i][j].matching = 0;
                                                }
                                        }
                                }
                        }

                        app.draw(background);

                        // Drawing
                        for (int i = 1; i <= 8; i++)
                        {
                                for (int j = 1; j <= 8; j++)
                                {
                                        Block p = board[i][j];
                                        gems.setTextureRect(IntRect(p.ran * 79, 0, 78, 78));
                                        text.setString("Score:  " + temp);
                                        if (input == 2)
                                        {
                                                text1.setString("Remaining Moves: " + temp2 + "\nRequired points: 2000");
                                        }
                                        else
                                                text1.setString("Timer: " + temp1 + "\nRequired points: 1500");

                                        gems.setPosition(p.x, p.y);
                                        gems.move(offset.x - ts, offset.y - ts);
                                        app.draw(gems);
                                        app.draw(text);
                                        app.draw(text1);
                                        text_progressbar.setString("Progress Bar : ");
                                        app.draw(text_progressbar);
                                        app.draw(progressbar);
                                        // app.draw(point);
                                        if (click == 1)
                                        {
                                                cursor.setScale(0.6, 0.6);
                                                cursor.setPosition(board[y0][x0].x - 49, board[y0][x0].y - 79);
                                                app.draw(cursor);
                                        }
                                }
                        }
                        app.display();
                }
                else if (input == 0)
                {
                        app.draw(menu);
                        app.display();
                }
        }
        return 0;
}