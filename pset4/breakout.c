//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

//maximum points
#define POINTMX 50

//height and width of paddle
#define PADDLEW 60
#define PADDLEH 10
#define BRICKW 35
#define BRICKH 10
#define SPACEB 4

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    //set speed of the ball randomly
    double velocityX = drand48() + 1.5;
    double velocityY = velocityX + 1.0;
    
    // keep playing until game over
    waitForClick();
    while (lives > 0 && bricks > 0 && points < POINTMX)
    {
        
         move(ball,velocityX, velocityY);
         GObject object = detectCollision(window, ball);
         
             
         if (object != NULL)
           { 
            if (object == paddle && velocityY > 0) 
             {
                  velocityY = -velocityY;
             }
            
             else if(strcmp(getType(object), "GRect") == 0 && object != paddle )
             {
             removeGWindow(window,object);
             points++;
             updateScoreboard( window,  label,  points);
             velocityY = -velocityY;
            }
           } 
 
        //bounce of right edge of window 
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }

        // bounce off left edge of window
        if (getX(ball) <= 0)
        {
        velocityX = -velocityX;
        }
        //If ball hits bottom subtract live and check for end
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
        lives--;
        setLocation(ball,190,300);
        waitForClick();
            if(lives > 0)
            {
             
            move(ball,velocityX, velocityY);
            }
        }
        
        //bounce off top of window
        if (getY(ball) <= 0)
        {
        velocityY = -velocityY;
        }
         pause(10);

         //Check for mouse event to move paddle
    
    
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
        
            // if the event was movement
        if (getEventType(event) == MOUSE_MOVED)
            {
                if (getX(event) > getWidth(paddle)){
                double x = getX(event) - getWidth(paddle);
                setLocation(paddle, x, 550);
                }else{
                double x = 0;
                setLocation(paddle, x, 550);
                }
                
              }
            
        }
        
      //Game Over print to screen
        if (lives == 0 || points == POINTMX){
             GLabel end = newGLabel("Game Over!");
             setFont(end, "SansSerif-28");
             setColor(end,"BLACK");
             double x = (getWidth(window) - getWidth(end)) /2;
            setLocation(end, x, 450);
            add(window, end);   
        }       
        
    }
    
    
    
    // game over
   

    // wait for click before exiting
    waitForClick();

    
    
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
   int x = 60;
   int y = 6;
   for (int i = 0; i < ROWS; i++)
   {
            string c = "BLACK";
             switch (i)
            {
                case 0:
                    c = "BLUE";
                    break;
                case 1:
                    c = "YELLOW";
                    break;
                case 2:
                    c = "RED";
                    break;
                case 3:
                    c = "ORANGE";
                    break;
                case 4:
                    c = "GREEN";
                    break;
                 default:
                    c = "BLACK";
               }                
   
        for (int j = 0; j < COLS; j++)
            if(j == COLS - 1) {
                GRect brick = newGRect(357,x,BRICKW,BRICKH);    
                setFilled(brick, true);
                setColor(brick, c);
                add(window, brick);
                x = x + BRICKH + SPACEB;
                y = 6;
             }else{
                GRect brick = newGRect(y,x,BRICKW,BRICKH);
                setFilled(brick, true);
                setColor(brick, c);
                add(window, brick);
                y = y + BRICKW + SPACEB;
            }
   }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GRect ball = newGOval(190,300,20,20);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(170,550,PADDLEW,PADDLEH);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-28");
    setColor(label, "Dark Gray");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
