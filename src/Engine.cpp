#include <iostream>
#include<cstring>
#include <array>
#include <vector>
#include "../include/Engine.h"; 

using namespace std;

#define ll unsigned long long
#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define pop_bit(b, i) ((b) &= ~(1ULL << i))
#define MASK(x) (1ULL << x)

static inline int count_bits(ll x) {
    int cnt = 0;
    while (x) {
        cnt++;
        x &= (x - 1);
    }
    return cnt;
}

static inline int get_LSB(long long b) {
    if (b == 0) return -1;
    return count_bits((b & (-b)) - 1);
}

#define encode_move(source, target, piece, promoted, capture, rdouble, enpassant, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (rdouble << 21) |    \
    (enpassant << 22) | \
    (castling << 23)    \

// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)

// extract double pawn push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)

// extract castling flag
#define get_move_castling(move) (move & 0x800000)

#define copy_board()                                                      \
    ll bitboards_copy[12], occupancies_copy[3];                          \
    int side_copy, enpassant_copy, castle_copy;                           \
    memcpy(bitboards_copy, bitboards, 96);                                \
    memcpy(occupancies_copy, occupancies, 24);                            \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle;   \

// restore board state
#define take_back()                                                       \
    memcpy(bitboards, bitboards_copy, 96);                                \
    memcpy(occupancies, occupancies_copy, 24);                            \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy;   \

struct moves {
    int Moves[256];
    int cnt;
};

static inline void add_move(moves* move_list, int Move) {
    move_list->Moves[move_list->cnt] = Move;
    move_list->cnt++;
}

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

const char* square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

enum { P, N, B, R, Q, K, p, n, b, r, q, k };

enum { white, black, both };

enum { rook, bishop };

enum { wk = 1, wq = 2, bk = 4, bq = 8 };

char ascii_pieces[] = "PNBRQKpnbrqk";

// unicode pieces
// convert ASCII character pieces to encoded constants
int char_pieces[400];


ll rook_magic_numbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

// bishop magic numbers
ll bishop_magic_numbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};


// [side][square]
ll pawn_attacks[2][64];
ll knight_attacks[64];
ll king_attacks[64];
ll bishop_masks[64];
ll rook_masks[64];
/// [square][occupancies]
ll bishop_attacks[64][512];
ll rook_attacks[64][4096];
ll bitboards[12];
ll occupancies[64];
int side = -1;
int enpassant = no_sq;
int castle;

inline int pop_LSB(ll& b) {
    int i = get_LSB(b);
    b &= b - 1;
    return i;
}

enum { all_moves, only_captures };

const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};


void print_bitboard(ll bitboard) {
    printf("\n");

    for (int Rank = 0; Rank < 8; Rank++) {
        for (int file = 0; file < 8; file++) {
            int square = Rank * 8 + file;
            if (!file) {
                cout << ' ' << 8 - Rank << ' ';
            }
            cout << ' ' << (get_bit(bitboard, square) ? 1 : 0);
        }

        cout << '\n';
    }

    // print board files
    printf("\n    a b c d e f g h\n\n");

    // print bitboard as unsigned decimal number
    printf("Bitboard: %llu\n\n", bitboard);
}

const ll not_a_file = 18374403900871474942ULL;

// not H file constant
const ll not_h_file = 9187201950435737471ULL;

// not HG file constant
const ll not_hg_file = 4557430888798830399ULL;

// not AB file constant
const ll not_ab_file = 18229723555195321596ULL;


ll mask_pawn_attacks(bool side, int square) {
    ll attacks = 0;
    ll bitboard = 0;

    set_bit(bitboard, square);

    if (!side)
    {
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    else
    {
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }

    return attacks;
}

ll mask_knight_attacks(int square) {
    ll attacks = 0;
    ll bitboard = 0;

    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    return attacks;
}

ll mask_king_attacks(int square) {
    ll attacks = 0;
    ll bitboard = 0;

    set_bit(bitboard, square);

    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

    return attacks;
}

ll mask_bishop_attacks(int square) {
    ll attacks = 0;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

    return attacks;
}

ll mask_rook_attacks(int square) {
    ll attacks = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

    return attacks;
}

ll bishop_attacks_on_the_fly(int square, ll block) {
    ll attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    // return attack map
    return attacks;
}

ll rook_attacks_on_the_fly(int square, ll block) {
    ll attacks = 0ULL;
    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }

    return attacks;
}

ll set_occupancy(int index, int bits_in_mask, ll attack_mask) {
    ll occupancy = 0;
    for (int cnt = 0; cnt < bits_in_mask; cnt++) {
        int square = get_LSB(attack_mask);
        clear_bit(attack_mask, square);
        if (index & (1 << cnt))
            occupancy |= MASK(square);
    }

    return occupancy;
}


void init_sliders_attacks(int bishop) {
    for (int square = 0; square < 64; square++) {
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        ll attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        int relevant_bits_count = count_bits(attack_mask);

        int occupancy_indicies = (1 << relevant_bits_count);

        for (int index = 0; index < occupancy_indicies; index++) {
            if (bishop) {
                ll occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

                // init bishop attacks
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
            else {
                ll occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

                // init bishop attacks
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);

            }
        }
    }
}

static inline ll get_bishop_attacks(int square, ll occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];
    return bishop_attacks[square][occupancy];
}

static inline ll get_rook_attacks(int square, ll occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancy];
}

static inline ll get_queen_attacks(int square, ll occupancy) {
    return get_rook_attacks(square, occupancy) | get_bishop_attacks(square, occupancy);
}

static inline bool is_square_attacked(int square, int side) {
    if ((side == white) && (pawn_attacks[black][square] & bitboards[P])) return 1;

    if ((side == black) && (pawn_attacks[white][square] & bitboards[p])) return 1;

    if (knight_attacks[square] & ((side == white) ? bitboards[N] : bitboards[n])) return 1;

    if (get_bishop_attacks(square, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b])) return 1;

    if (get_rook_attacks(square, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r])) return 1;

    if (get_queen_attacks(square, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q])) return 1;

    if (king_attacks[square] & ((side == white) ? bitboards[K] : bitboards[k])) return 1;

    return 0;
}

static inline int make_move(int Move, int move_flag) {
    if (move_flag == all_moves) {
        copy_board();

        int source_square = get_move_source(Move);
        int target_square = get_move_target(Move);
        int piece = get_move_piece(Move);
        int promoted_piece = get_move_promoted(Move);
        int capture = get_move_capture(Move);
        int double_push = get_move_double(Move);
        int enpass = get_move_enpassant(Move);
        int castling = get_move_castling(Move);

        pop_bit(bitboards[piece], source_square);
        set_bit(bitboards[piece], target_square);

        if (capture) {
            int start_piece, end_piece;

            if (side == white) {
                start_piece = p;
                end_piece = k;
            }
            else {
                start_piece = P;
                end_piece = K;
            }

            for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
                if (get_bit(bitboards[bb_piece], target_square)) {
                    pop_bit(bitboards[bb_piece], target_square);
                    break;
                }
            }
        }

        if (promoted_piece) {
            pop_bit(bitboards[(side == white) ? P : p], target_square);

            set_bit(bitboards[promoted_piece], target_square);
        }

        if (enpass) {
            (side == white) ? pop_bit(bitboards[p], target_square + 8) :
                pop_bit(bitboards[P], target_square - 8);
        }

        enpassant = no_sq;

        if (double_push) {
            (side == white) ? (enpassant = target_square + 8) :
                (enpassant = target_square - 8);
        }

        if (castling) {
            switch (target_square) {
            case (g1):
                pop_bit(bitboards[R], h1);
                set_bit(bitboards[R], f1);
                break;
            case (c1):
                pop_bit(bitboards[R], a1);
                set_bit(bitboards[R], d1);
                break;
            case (g8):
                pop_bit(bitboards[r], h8);
                set_bit(bitboards[r], f8);
                break;
            case (c8):
                pop_bit(bitboards[r], a8);
                set_bit(bitboards[r], d8);
                break;
            }
        }

        castle &= castling_rights[source_square];
        castle &= castling_rights[target_square];

        memset(occupancies, 0ULL, 24);

        for (int bb_piece = P; bb_piece <= K; bb_piece++)
            occupancies[white] |= bitboards[bb_piece];

        for (int bb_piece = p; bb_piece <= k; bb_piece++)
            occupancies[black] |= bitboards[bb_piece];

        occupancies[both] |= occupancies[white];
        occupancies[both] |= occupancies[black];

        side ^= 1;

        if (is_square_attacked((side == white) ? get_LSB(bitboards[k]) : get_LSB(bitboards[K]), side)) {
            take_back();
            return 0;
        }
        else
            return 1;

    }
    else {
        if (get_move_capture(Move))
            make_move(Move, all_moves);
        else
            return 0;
    }
}

static inline void generate_moves(moves* move_list) {
    move_list->cnt = 0;

    int source_square, target_square;
    ll bitboard, attacks;

    for (int piece = P; piece <= k; piece++) {
        bitboard = bitboards[piece];

        if (side == white)
        {
            // pick up white pawn bitboards index
            if (piece == P)
            {
                // loop over white pawns within white pawn bitboard
                while (bitboard)
                {
                    // init source square
                    source_square = get_LSB(bitboard);

                    // init target square
                    target_square = source_square - 8;

                    // generate quiet pawn moves
                    if (!(target_square < a8) && !get_bit(occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(occupancies[both], target_square - 8))
                                add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
                        }
                    }

                    // init pawn attacks bitboard
                    attacks = pawn_attacks[side][source_square] & occupancies[black];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_LSB(attacks);

                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        }

                        else
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        // pop ls1b of the pawn attacks
                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant square (bit)
                        ll enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        // make sure enpassant capture available
                        if (enpassant_attacks)
                        {
                            // init enpassant capture target square
                            int target_enpassant = get_LSB(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            // castling moves
            if (piece == K)
            {
                // king side castling is available
                if (castle & wk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1))
                    {
                        // make sure king and the f1 squares are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                    }
                }

                // queen side castling is available
                if (castle & wq)
                {
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], b1))
                    {
                        // make sure king and the d1 squares are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        // generate black pawns & black king castling moves
        else
        {
            // pick up black pawn bitboards index
            if (piece == p)
            {
                // loop over white pawns within white pawn bitboard
                while (bitboard)
                {
                    // init source square
                    source_square = get_LSB(bitboard);

                    // init target square
                    target_square = source_square + 8;

                    // generate quiet pawn moves
                    if (!(target_square > h1) && !get_bit(occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(occupancies[both], target_square + 8))
                                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                        }
                    }

                    // init pawn attacks bitboard
                    attacks = pawn_attacks[side][source_square] & occupancies[white];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_LSB(attacks);

                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        }

                        else
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        // pop ls1b of the pawn attacks
                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant square (bit)
                        ll enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        // make sure enpassant capture available
                        if (enpassant_attacks)
                        {
                            // init enpassant capture target square
                            int target_enpassant = get_LSB(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            // castling moves
            if (piece == k)
            {
                // king side castling is available
                if (castle & bk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8))
                    {
                        // make sure king and the f8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }

                // queen side castling is available
                if (castle & bq)
                {
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], b8))
                    {
                        // make sure king and the d8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        // genarate knight moves
        if ((side == white) ? piece == N : piece == n)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_LSB(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = knight_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_LSB(attacks);

                    // quiet move
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate bishop moves
        if ((side == white) ? piece == B : piece == b)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_LSB(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_bishop_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_LSB(attacks);

                    // quiet move
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate rook moves
        if ((side == white) ? piece == R : piece == r)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_LSB(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_rook_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_LSB(attacks);

                    // quiet move
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate queen moves
        if ((side == white) ? piece == Q : piece == q)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_LSB(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_queen_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_LSB(attacks);

                    // quiet move
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }


                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate king moves
        if ((side == white) ? piece == K : piece == k)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_LSB(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = king_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_LSB(attacks);

                    // quiet move
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }
    }
}

void init_all() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }

    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    char_pieces['P'] = P,
        char_pieces['N'] = N,
        char_pieces['B'] = B,
        char_pieces['R'] = R,
        char_pieces['Q'] = Q,
        char_pieces['K'] = K,
        char_pieces['p'] = p,
        char_pieces['n'] = n,
        char_pieces['b'] = b,
        char_pieces['r'] = r,
        char_pieces['q'] = q,
        char_pieces['k'] = k;
}


// MVV LVA [attacker][victim]
static int mvv_lva[12][12] = {
    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

int material_score[12] = {
    100,      // white pawn score
    300,      // white knight scrore
    350,      // white bishop score
    500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
};

// pawn positional score
const int pawn_score[64] =
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] =
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rook_score[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int king_score[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

// mirror positional score tables for opposite side
const int mirror_score[128] =
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

static inline int evaluate() {
    int score = 0;

    ll bitboard;

    int piece, square;

    for (int bb_piece = P; bb_piece <= k; bb_piece++)
    {
        bitboard = bitboards[bb_piece];

        while (bitboard)
        {
            piece = bb_piece;

            square = get_LSB(bitboard);

            score += material_score[piece];

            switch (piece)
            {
            case P: score += pawn_score[square]; break;
            case N: score += knight_score[square]; break;
            case B: score += bishop_score[square]; break;
            case R: score += rook_score[square]; break;
            case K: score += king_score[square]; break;

            case p: score -= pawn_score[mirror_score[square]]; break;
            case n: score -= knight_score[mirror_score[square]]; break;
            case b: score -= bishop_score[mirror_score[square]]; break;
            case r: score -= rook_score[mirror_score[square]]; break;
            case k: score -= king_score[mirror_score[square]]; break;
            }

            pop_bit(bitboard, square);
        }
    }

    return (side == white) ? score : -score;
}

int nodes;
ll best_move;
int ply;

pair<int, int> move_scores[250];
int temp_move[250];
int killer_moves[2][64];
int history_moves[12][70];


static inline int score_move(int move) {
    if (get_move_capture(move)) {
        int l = (side == white ? p : P);
        int r = (side == white ? k : K);
        int target_piece = P;

        for (int piece = l; piece <= r; piece++) {
            if (get_bit(bitboards[piece], get_move_target(move))) {
                target_piece = piece;
                break;
            }
        }

        return mvv_lva[get_move_piece(move)][target_piece];
    }
    else {
        if (killer_moves[0][ply] == move)
            return 9000;
        else if (killer_moves[1][ply] == move)
            return 8000;
        else
            return history_moves[get_move_piece(move)][get_move_target(move)];
    }

    return 0;
}

static inline void sort_moves(moves* move_list) {
    for (int cnt = 0; cnt < move_list->cnt; cnt++) {
        move_scores[cnt] = { score_move(move_list->Moves[cnt]), cnt };
        temp_move[cnt] = move_list->Moves[cnt];
    }

    sort(move_scores, move_scores + move_list->cnt, greater<pair<int, int>>());

    for (int cnt = 0; cnt < move_list->cnt; cnt++) {
        move_list->Moves[cnt] = temp_move[move_scores[cnt].second];
    }
}

static inline int quiescence(int alpha, int beta) {
    nodes++;

    int evaluation = evaluate();

    if (evaluation >= beta)
        return beta;

    if (evaluation > alpha)
        alpha = evaluation;

    moves move_list[1];

    generate_moves(move_list);

    sort_moves(move_list);

    for (int cnt = 0; cnt < move_list->cnt; cnt++) {
        copy_board();
        ply++;

        if (make_move(move_list->Moves[cnt], only_captures) == 0) {
            ply--;
            continue;
        }

        int score = -quiescence(-beta, -alpha);

        ply--;
        take_back();

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;

    }

    return alpha;
}

static inline int negamax(int alpha, int beta, int depth) {
    if (depth == 0)
        return quiescence(alpha, beta);

    nodes++;

    bool in_check = is_square_attacked((side == white) ? get_LSB(bitboards[K]) :
        get_LSB(bitboards[k]),
        side ^ 1);

    if (in_check) depth++;

    int legal_moves = 0;

    int best_sofar = -1e9;

    int old_alpha = alpha;

    moves move_list[1];

    generate_moves(move_list);

    sort_moves(move_list);

    for (int cnt = 0; cnt < move_list->cnt; cnt++) {
        copy_board();

        ply++;

        if (make_move(move_list->Moves[cnt], all_moves) == 0) {
            ply--;
            continue;
        }

        legal_moves++;

        int score = -negamax(-beta, -alpha, depth - 1);

        ply--;

        take_back();

        if (score >= beta) {

            if (get_move_capture(move_list->Moves[cnt]) == 0) {
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = move_list->Moves[cnt];
            }

            return beta;
        }

        if (score > alpha) {
            if (get_move_capture(move_list->Moves[cnt]) == 0) history_moves[get_move_piece(move_list->Moves[cnt])][get_move_target(move_list->Moves[cnt])] += depth;

            alpha = score;

            if (ply == 0)
                best_sofar = move_list->Moves[cnt];
        }
    }

    if (legal_moves == 0) {
        if (in_check)
            return -49000 + ply;

        else
            return 0;
    }

    if (old_alpha != alpha && best_sofar > 0)
        best_move = best_sofar;

    return alpha;
}


Engine::Engine() {
    init_all();
    castle = 15;
    best_move = nodes = ply = 0;
}

void Engine::MakeMove() {
    memset(occupancies, 0ULL, 24);

    for (int bb_piece = P; bb_piece <= K; bb_piece++)
        occupancies[white] |= bitboards[bb_piece];

    for (int bb_piece = p; bb_piece <= k; bb_piece++)
        occupancies[black] |= bitboards[bb_piece];

    occupancies[both] |= occupancies[white];
    occupancies[both] |= occupancies[black];
}

static inline int minimax(int alpha, int beta, int depth);

array <int, 4> Engine::getDecision(bool t) {

    if (t == 0)
        minimax(-50000, 50000, 4);
    else
        minimax(-50000, 50000, 5);

    array<int, 4> best;

    best[0] = get_move_source(best_move);
    best[1] = get_move_target(best_move);
    best[2] = get_move_promoted(best_move);

    if (best[2] == B || best[2] == b) best[2] = bishop_main;
    if (best[2] == N || best[2] == n) best[2] = knight_main;
    if (best[2] == Q || best[2] == q) best[2] = queen_main;
    if (best[2] == R || best[2] == r) best[2] = rook_main;

    best[3] = get_move_piece(best_move);

    return best;
}

void print_board();

void Engine::print() {
    print_board();
}


void Engine::Reset(bool view) {
    for (int piece = P; piece <= k; piece++) bitboards[piece] = 0;
    side = view;  castle = 15;
    best_move = nodes = ply = 0;
}

void Engine::UpdBoard(int square, int view, int type) {
    if (square < 0 || square >= 64) return;

    int piece = -1;

    if (type == pawn_main) piece = (view == white ? P : p);
    if (type == knight_main) piece = (view == white ? N : n);
    if (type == bishop_main) piece = (view == white ? B : b);
    if (type == rook_main) piece = (view == white ? R : r);
    if (type == king_main) piece = (view == white ? K : k);
    if (type == queen_main) piece = (view == white ? Q : q);

    if (piece < 0) return;

    set_bit(bitboards[piece], square);
}

void Engine::upd_castle(bool side, int type, int played, bool dir) {
    if (type == king_main && played > 0) castle &= (side == white ? 12 : 3);
    if (type == rook_main && played > 0) castle &= (side == white ? (dir ? 13 : 14) : (dir ? 7 : 11));
}


void print_board() {
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop ober board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            // define piece variable
            int piece = -1;

            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // if there is a piece on current square
                if (get_bit(bitboards[bb_piece], square))
                    // get piece code
                    piece = bb_piece;
            }

            // print different piece set depending on OS
            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print side to move
    printf("     Side:     %s\n", !side ? "white" : "black");

    // print enpassant square
    printf("     Enpassant:   %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");

    // print castling rights
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
        (castle & wq) ? 'Q' : '-',
        (castle & bk) ? 'k' : '-',
        (castle & bq) ? 'q' : '-');
}

static inline int minimax(int alpha, int beta, int depth) {
    if (depth == 0)
        return evaluate();

    nodes++;

    int in_check = is_square_attacked((side == white) ? get_LSB(bitboards[K]) :
        get_LSB(bitboards[k]),
        side ^ 1);

    int legal_moves = 0;

    int best_sofar = -1e9;

    int old_alpha = alpha;

    moves move_list[1];

    generate_moves(move_list);
    sort_moves(move_list);

    for (int cnt = 0; cnt < move_list->cnt; cnt++) {
        copy_board();

        ply++;

        if (make_move(move_list->Moves[cnt], all_moves) == 0) {
            ply--;
            continue;
        }

        legal_moves++;

        int score = -minimax(-beta, -alpha, depth - 1);

        ply--;

        take_back();

        if (score >= beta)
            return beta;


        if (score > alpha) {
            alpha = score;

            if (ply == 0)
                best_sofar = move_list->Moves[cnt];
        }
    }

    if (legal_moves == 0) {
        if (in_check)
            return -49000 + ply;
        else
            return 0;
    }

    if (old_alpha != alpha && best_sofar > 0)
        best_move = best_sofar;

    return alpha;
}
