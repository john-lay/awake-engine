#include <gb/gb.h>
#include <stdio.h>
#include "../sprites/link.c"
#include "GameCharacter.c"
#include "../include/game_time.h"

struct GameCharacter player;
UBYTE spriteSize = 8;
UBYTE flipWalkCycle;
UBYTE spriteMirrored;
UBYTE movementSpeed = 2;
UBYTE strafeSpeed = 1;
UINT8 game_time = 0;
UINT8 previous_direction;

void moveCharacter(struct GameCharacter *character, UINT8 x, UINT8 y)
{
    move_sprite(character->spriteIds[0], x, y);
    move_sprite(character->spriteIds[1], x + spriteSize, y);
    move_sprite(character->spriteIds[2], x, y + spriteSize);
    move_sprite(character->spriteIds[3], x + spriteSize, y + spriteSize);
}

void setupPlayer()
{
    player.x = 80;
    player.y = 130;
    player.width = 16;
    player.height = 16;
    player.facing = J_DOWN;

    set_sprite_tile(0, 0);
    player.spriteIds[0] = 0;
    set_sprite_tile(1, 1);
    player.spriteIds[1] = 1;
    set_sprite_tile(2, 2);
    player.spriteIds[2] = 2;
    set_sprite_tile(3, 3);
    player.spriteIds[3] = 3;

    moveCharacter(&player, player.x, player.y);
}

void sleep(UINT8 numloops)
{
    UINT8 i;
    for (i = 0; i < numloops; i++)
    {
        wait_vbl_done();
    }
}

void mirrorSprite(struct GameCharacter *sprite)
{
    if (!spriteMirrored)
    {
        spriteMirrored = 1;
        set_sprite_prop(sprite->spriteIds[0], S_FLIPX);
        set_sprite_prop(sprite->spriteIds[1], S_FLIPX);
        set_sprite_prop(sprite->spriteIds[2], S_FLIPX);
        set_sprite_prop(sprite->spriteIds[3], S_FLIPX);
    }
}

void unmirrorSprite(struct GameCharacter *sprite)
{
    if (spriteMirrored)
    {
        spriteMirrored = 0;
        set_sprite_prop(sprite->spriteIds[0], get_sprite_prop(0) & ~S_FLIPX);
        set_sprite_prop(sprite->spriteIds[1], get_sprite_prop(1) & ~S_FLIPX);
        set_sprite_prop(sprite->spriteIds[2], get_sprite_prop(2) & ~S_FLIPX);
        set_sprite_prop(sprite->spriteIds[3], get_sprite_prop(3) & ~S_FLIPX);
    }
}

void animateDown(struct GameCharacter *sprite)
{
    unmirrorSprite(&player);

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(sprite->spriteIds[0], 4);
        set_sprite_tile(sprite->spriteIds[1], 5);
        set_sprite_tile(sprite->spriteIds[2], 6);
        set_sprite_tile(sprite->spriteIds[3], 7);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(sprite->spriteIds[0], 0);
        set_sprite_tile(sprite->spriteIds[1], 1);
        set_sprite_tile(sprite->spriteIds[2], 2);
        set_sprite_tile(sprite->spriteIds[3], 3);
    }
}

void animateUp(struct GameCharacter *sprite)
{
    unmirrorSprite(&player);

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(sprite->spriteIds[0], 12);
        set_sprite_tile(sprite->spriteIds[1], 13);
        set_sprite_tile(sprite->spriteIds[2], 14);
        set_sprite_tile(sprite->spriteIds[3], 15);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(sprite->spriteIds[0], 8);
        set_sprite_tile(sprite->spriteIds[1], 9);
        set_sprite_tile(sprite->spriteIds[2], 10);
        set_sprite_tile(sprite->spriteIds[3], 11);
    }
}

void animateRight(struct GameCharacter *sprite)
{
    unmirrorSprite(&player);

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(sprite->spriteIds[0], 20);
        set_sprite_tile(sprite->spriteIds[1], 21);
        set_sprite_tile(sprite->spriteIds[2], 22);
        set_sprite_tile(sprite->spriteIds[3], 23);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(sprite->spriteIds[0], 16);
        set_sprite_tile(sprite->spriteIds[1], 17);
        set_sprite_tile(sprite->spriteIds[2], 18);
        set_sprite_tile(sprite->spriteIds[3], 19);
    }
}

void animateLeft(struct GameCharacter *sprite)
{
    mirrorSprite(&player);

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(sprite->spriteIds[0], 21);
        set_sprite_tile(sprite->spriteIds[1], 20);
        set_sprite_tile(sprite->spriteIds[2], 23);
        set_sprite_tile(sprite->spriteIds[3], 22);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(sprite->spriteIds[0], 17);
        set_sprite_tile(sprite->spriteIds[1], 16);
        set_sprite_tile(sprite->spriteIds[2], 19);
        set_sprite_tile(sprite->spriteIds[3], 18);
    }
}

void animatePlayer()
{
    if (player.facing == J_UP) {
        animateUp(&player);
    } 
    else if (player.facing == J_DOWN) {
        animateDown(&player);
    }
    else if (player.facing == J_LEFT) {
        animateLeft(&player);
    }
    else if (player.facing == J_RIGHT) {
        animateRight(&player);
    }
}

UBYTE inputUp() {
    return joypad() & J_UP && player.y > 16;
}

UBYTE inputDown() {
    return joypad() & J_DOWN && player.y < 144;
}

UBYTE inputLeft() {
    return joypad() & J_LEFT && player.x > 8;
}

UBYTE inputRight() {
    return joypad() & J_RIGHT && player.x < 154;
}

void moveUp() {
    player.y -= movementSpeed;
    previous_direction = J_UP;
    moveCharacter(&player, player.x, player.y);
}

void moveDown() {
    player.y += movementSpeed;
    previous_direction = J_DOWN;
    moveCharacter(&player, player.x, player.y);
}

void moveLeft() {
    player.x -= movementSpeed;
    previous_direction = J_LEFT;
    moveCharacter(&player, player.x, player.y);    
}

void moveRight() {
    player.x += movementSpeed;
    previous_direction = J_RIGHT;
    moveCharacter(&player, player.x, player.y);
}

void updateFacing() {
    int key = joypad();
    if (key & J_UP && !(key & J_LEFT) && !(key & J_RIGHT)) {
        player.facing = J_UP;
    }
    else if (key & J_DOWN && !(key & J_LEFT) && !(key & J_RIGHT)) {
        player.facing = J_DOWN;
    }
    else if (key & J_LEFT && !(key & J_UP) && !(key & J_DOWN)) {
        player.facing = J_LEFT;
    }
    else if (key & J_RIGHT && !(key & J_UP) && !(key & J_DOWN)) {
        player.facing = J_RIGHT;
    }
}

void handleInput()
{
    updateFacing();

    if (player.facing == J_LEFT || player.facing == J_RIGHT) {
        if (inputLeft()) {
            if (inputUp()) player.y -= strafeSpeed;
            if (inputDown()) player.y += strafeSpeed;
            moveLeft();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputRight()) {
            if (inputUp()) player.y -= strafeSpeed;
            if (inputDown()) player.y += strafeSpeed;
            moveRight();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputUp()) {
            if (inputLeft()) player.x -= strafeSpeed;
            if (inputRight()) player.x += strafeSpeed;
            moveUp();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputDown()) {
            if (inputLeft()) player.x -= strafeSpeed;
            if (inputRight()) player.x += strafeSpeed;
            moveDown();
            if (IS_FRAME_8) animatePlayer();
        }
    } else {
        if (inputUp()) {
            if (inputLeft()) player.x -= strafeSpeed;
            if (inputRight()) player.x += strafeSpeed;
            moveUp();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputDown()) {
            if (inputLeft()) player.x -= strafeSpeed;
            if (inputRight()) player.x += strafeSpeed;
            moveDown();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputLeft()) {
            if (inputUp()) player.y -= strafeSpeed;
            if (inputDown()) player.y += strafeSpeed;
            moveLeft();
            if (IS_FRAME_8) animatePlayer();
        } else if (inputRight()) {
            if (inputUp()) player.y -= strafeSpeed;
            if (inputDown()) player.y += strafeSpeed;
            moveRight();
            if (IS_FRAME_8) animatePlayer();
        }   
    }
}

void main()
{
    // sprite data contains 8 8x8 sprites
    // tiles 0-3 are of link facing down with his right foot forwards
    // tiles 4-7 are of link facing down with his left foot forwards
    // tiles 8-11 are of link facing up with his left foot forwards
    // tiles 12-15 are of link facing up with his right foot forwards
    // tiles 16-19 are of link facing right with his right foot forwards
    // tiles 20-23 are of link facing right with his left foot forwards
    set_sprite_data(0, 24, LinkSpriteData);
    setupPlayer();
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
        handleInput();   
        game_time++;
        // sleep(1);
        wait_vbl_done();
    }
}
