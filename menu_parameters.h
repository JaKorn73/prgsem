/*
 * filename: menu.c
 * date: 2024/05/15 16:03 
 * author: Jakub Kornel
 * email: kornejak@fel.cvut.cz
*/

#ifndef __MENU_PARAMETERS__
#define __MENU_PARAMETERS__

// Global menu variables
#define FONT_PATH "./fonts/Roboto-Black.ttf"
#define TEXT_FONT_SIZE 30
#define INPUT_FONT_SIZE 40
#define MENU_HEIGHT 800
#define MENU_WIDTH 1000

#define BOX_WIDTH 300
#define BOX_HEIGHT 50

#define Y_FIRST_ROW 10
#define Y_SEC_ROW 130
#define Y_THIRD_ROW 250
#define Y_FOURTH_ROW 370
#define Y_FIFTH_ROW 490

#define X_FIRST_COL 10
#define X_SEC_COL 320

#define INPUT_LEN 5

// Text before inputs
#define TEXT_C_RE "Re. cast konstanty:"
#define TEXT_C_IM "Im. cast konstanty:"

#define TEXT_RE_MIN "Min re. cast:"
#define TEXT_RE_MAX "Max re. cast:"

#define TEXT_IM_MIN "Min im. cast:"
#define TEXT_IM_MAX "Max im. cast:"

#define TEXT_N "Iterations n:"
#define TEXT_SAVE "Save"

// Messages for user
#define WRONG_INPUT_MESS "Wrong input!"
#define SUCCESS_INPUT "Input entered successfuly"
#define SUCCESS_SAVE "Variables saved"

// ASCII 
#define ASCII_MINUS 45
#define ASCII_DOT 46
#define ASCII_ZERO 48
#define ASCII_NINE 57

// Button IDs
enum {
    EMPTY_ID,
    C_RE_ID,
    C_IM_ID,
    RE_MIN_ID,
    RE_MAX_ID,
    IM_MIN_ID,
    IM_MAX_ID,
    N_ID,
    SAVE_ID,
    MESSAGE_ID
};
#endif

/* end of menu_parameters.h */
