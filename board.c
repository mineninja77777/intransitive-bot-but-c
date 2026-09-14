typedef struct {
    unsigned long long high; // 64 to 80
    unsigned long long low; // squares 0 to 63
} BitBoard;
// typedef unsigned __int128 BitBoard;
typedef struct {
    BitBoard blueR;
    BitBoard blueS;
    BitBoard blueP;
    BitBoard redR;
    BitBoard redS;
    BitBoard redP;
} Board;

Board board;

void init_board() {
    BitBoard blueR = {4, 72127962782105600};
    BitBoard blueS = {0, 288512126006329344};
    BitBoard blueP = {8, 144256063003164672};
    BitBoard redR = {0, 17196662784};
    BitBoard redS = {0, 4402345672704};
    BitBoard redP = {0, 8804691353600};
    board = (Board){blueR, blueS, blueP, redR, redS, redP};
}

int get_bit(BitBoard board, int bit) {
    if (bit > 63) {
        return (board.high >> (bit - 64)) & 1;
    }
    return (board.low >> bit) & 1;
}

void set_bit(BitBoard *board, int bit, int value) {
    if (get_bit(*board, bit) == value) {
        return;
    }
    if (bit > 63) {
        (*board).high ^= (unsigned long long)1 << (bit - 64);
    } else {
        (*board).low ^= (unsigned long long)1 << bit;
    }
}

void output_bitboard(BitBoard board) {
    // complications due to wanting line breaks

    char out[90] = {};
    for (int i = 0; i < 81; i++) {
        if (get_bit(board, i) == 0) {
            out[i + i/9] = '0';
        } else {
            out[i + i/9] = '1';
        }
    }
    for (int i = 1; i < 9; i++) {
        out[i*10-1] = '\n';
    }
    printf("%s \n", out);
}

void output_board() {
    // complications due to wanting line breaks

    char out[90] = {};
    for (int i = 0; i < 81; i++) {
        if (get_bit(board.blueR, i) == 1) {
            out[i + i/9] = 'r';
        } else if (get_bit(board.blueS, i) == 1) {
            out[i + i/9] = 's';
        } else if (get_bit(board.blueP, i) == 1) {
            out[i + i/9] = 'p';
        } else if (get_bit(board.redR, i) == 1) {
            out[i + i/9] = 'R';
        } else if (get_bit(board.redS, i) == 1) {
            out[i + i/9] = 'S';
        } else if (get_bit(board.redP, i) == 1) {
            out[i + i/9] = 'P';
        } else {
            out[i + i/9] = '.';
        }
    }
    for (int i = 1; i < 9; i++) {
        out[i*10-1] = '\n';
    }
    printf("%s \n", out);
}