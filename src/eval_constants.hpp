#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(309, 324);
inline const PParam KNIGHT_MAT = S(1135, 912);
inline const PParam BISHOP_MAT = S(1260, 972);
inline const PParam ROOK_MAT   = S(1707, 1704);
inline const PParam QUEEN_MAT  = S(3730, 2874);
inline const PParam TEMPO_VAL  = S(59, 15);

inline const PParam BISHOP_PAIR_VAL   = S(80, 178);
inline const PParam ROOK_OPEN_VAL     = S(104, -29);
inline const PParam ROOK_SEMIOPEN_VAL = S(39, 13);

inline const PParam DOUBLED_PAWN_VAL = S(-37, -78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-75, -3);
inline const PParam OUTPOST_KNIGHT_VAL    = S(7, 51);
inline const PParam OUTPOST_BISHOP_VAL    = S(29, 27);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(54, 17);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(56, -14);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(33, 33);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(55, -53);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 20), S(63, 31), S(74, 70), S(190, 140), S(561, 241), S(925, 1149)
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(64, 43), S(61, 31), S(67, 57), S(147, 120), S(688, -90)
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-68, -98), S(-58, -80), S(-32, -4), S(22, 76), S(45, 217), S(288, 305)
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(50, -44), S(36, -6), S(21, 27), S(24, 76), S(92, 152), S(144, 296)
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -46), S(4, 3), S(0, -27), S(7, -47), S(5, -96), S(-191, -145)
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(13, 101), S(-22, 88), S(-14, 37), S(0, 8), S(9, 12), S(39, 10), S(18, -2)
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-187, -54), S(29, -8), S(-13, 41), S(10, 71), S(15, 96), S(35, 96), S(-15, 115)
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-232, -226), S(-124, -64), S(-67, -11), S(-24, 22), S(23, 37), S(49, 75), S(86, 72), S(120, 74),
    S(166, 17)
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-259, -292), S(-185, -108), S(-110, -46), S(-74, 4), S(-43, 35), S(-25, 56), S(-6, 71), S(13, 77),
    S(33, 81), S(47, 77), S(72, 65), S(137, 19), S(167, -3), S(228, -37)
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-304, -243), S(-151, -96), S(-99, -32), S(-66, -22), S(-40, 1), S(-25, 23), S(-7, 35), S(12, 41),
    S(29, 53), S(47, 62), S(66, 64), S(78, 66), S(99, 69), S(110, 55), S(256, -72)
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-1108, -835), S(-320, -633), S(-223, -500), S(-169, -291), S(-160, -83), S(-122, 21), S(-117, 132), S(-93, 145),
    S(-88, 201), S(-76, 227), S(-67, 251), S(-61, 266), S(-42, 258), S(-32, 270), S(-25, 264), S(-12, 260),
    S(-5, 251), S(-6, 258), S(20, 214), S(43, 177), S(57, 158), S(101, 92), S(113, 84), S(273, -90),
    S(308, -133), S(551, -289), S(423, -234), S(826, -450)
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(426, 10), S(78, -106), S(-27, -16), S(-41, 22), S(-69, 25), S(-104, 31), S(-82, 32), S(-91, 26),
    S(-92, -23)
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(0, 0), S(88, -33), S(158, -79)
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(0, 0), S(36, -7), S(136, -45)
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(0, 0), S(68, -50), S(53, -65), S(104, -64), S(161, -130)
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(0, 0), S(-39, 27), S(-57, 38), S(-1, -9), S(161, -97), S(357, -238)
};

inline const PParam PAWN_THREAT_KNIGHT = S(240, 58);
inline const PParam PAWN_THREAT_BISHOP = S(208, 102);
inline const PParam PAWN_THREAT_ROOK   = S(199, 56);
inline const PParam PAWN_THREAT_QUEEN  = S(179, -63);

inline const PParam KNIGHT_THREAT_BISHOP = S(106, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(244, 5);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -69);

inline const PParam BISHOP_THREAT_KNIGHT = S(111, 35);
inline const PParam BISHOP_THREAT_ROOK   = S(244, 55);
inline const PParam BISHOP_THREAT_QUEEN  = S(192, 49);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(-1, -11), S(-3, -5), S(-3, -15), S(-8, -26), S(-14, -31), S(-19, -38), S(-21, -45), S(-27, -40),
    S(-37, -44)
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(114, 164), S(105, 211), S(174, 177), S(237, 58), S(182, 53), S(168, 118), S(62, 141), S(122, 117),
    S(63, 33), S(176, 61), S(152, 4), S(154, -54), S(107, -70), S(51, -22), S(12, 25), S(-39, 29),
    S(-18, 1), S(2, 5), S(19, -39), S(6, -54), S(-11, -58), S(-54, -51), S(-95, -4), S(-121, 18),
    S(-44, -47), S(-26, -19), S(-30, -52), S(-48, -49), S(-74, -58), S(-94, -50), S(-146, -1), S(-164, -12),
    S(-46, -77), S(17, -74), S(-32, -30), S(-63, -28), S(-84, -37), S(-123, -39), S(-142, -25), S(-167, -31),
    S(-36, -70), S(94, -66), S(49, -30), S(-9, -12), S(-46, -24), S(-82, -29), S(-108, -6), S(-146, -18)
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-395, -158), S(-347, 61), S(-456, 236), S(-121, 69), S(-250, 93), S(-334, 100), S(-569, 87), S(-540, -17),
    S(5, -2), S(71, 11), S(173, -57), S(119, 9), S(121, 16), S(58, -9), S(-2, 11), S(-21, -35),
    S(62, -29), S(105, 17), S(196, 8), S(148, 31), S(146, 21), S(66, 29), S(55, 4), S(-41, 12),
    S(117, 6), S(107, 28), S(137, 33), S(115, 61), S(124, 48), S(91, 41), S(66, -1), S(41, 5),
    S(106, -14), S(132, -17), S(125, 9), S(98, 26), S(89, 34), S(86, 29), S(59, 1), S(45, -54),
    S(18, -23), S(49, -35), S(43, -14), S(54, 29), S(61, 26), S(4, 4), S(9, -35), S(-31, -41),
    S(20, -8), S(41, -36), S(25, -30), S(25, -11), S(12, -17), S(-16, -38), S(-5, -51), S(-62, -120),
    S(-29, -59), S(9, -16), S(27, -39), S(35, -32), S(28, -25), S(-20, -55), S(-32, -31), S(-82, -83)
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-174, 69), S(-195, 50), S(-437, 77), S(-316, 90), S(-267, 93), S(-435, 119), S(-178, 96), S(-129, 72),
    S(-2, -44), S(-20, 32), S(0, 14), S(-20, 19), S(-45, 34), S(-8, 25), S(-30, 15), S(-65, 18),
    S(29, 14), S(74, 3), S(152, 11), S(82, 10), S(56, 14), S(31, 26), S(90, -2), S(-12, 15),
    S(45, -31), S(58, 0), S(93, 3), S(92, 28), S(98, 28), S(36, 26), S(25, 3), S(-23, 10),
    S(46, -59), S(54, -22), S(61, -5), S(61, 19), S(53, 35), S(13, 26), S(-3, -11), S(-8, -54),
    S(59, -50), S(98, -37), S(103, -23), S(44, 18), S(26, 24), S(27, 19), S(55, -25), S(23, -47),
    S(46, -85), S(95, -61), S(66, -49), S(38, -19), S(29, -34), S(30, -45), S(12, -30), S(31, -94),
    S(41, -66), S(28, -20), S(33, -13), S(43, -47), S(52, -58), S(49, -16), S(40, -44), S(23, -47)
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(108, 4), S(174, 3), S(103, 33), S(103, 28), S(111, 16), S(59, 29), S(66, 33), S(74, 37),
    S(17, 62), S(104, 37), S(178, 15), S(103, 58), S(119, 45), S(67, 55), S(7, 73), S(-2, 79),
    S(5, 39), S(154, -2), S(184, -8), S(184, -12), S(139, -2), S(64, 39), S(81, 27), S(-38, 77),
    S(-26, 33), S(51, 26), S(82, 17), S(105, -19), S(73, 5), S(11, 53), S(-7, 52), S(-77, 60),
    S(-90, -16), S(-9, -11), S(-24, 6), S(-40, 7), S(-46, 4), S(-63, 44), S(-93, 41), S(-112, 30),
    S(-114, -36), S(-39, -63), S(-44, -33), S(-64, -32), S(-47, -50), S(-97, 4), S(-99, -14), S(-122, -17),
    S(-176, -25), S(-75, -85), S(-51, -71), S(-47, -68), S(-54, -64), S(-73, -47), S(-93, -70), S(-125, -53),
    S(-141, -24), S(-110, -19), S(-56, -55), S(-31, -72), S(-45, -57), S(-58, -45), S(-73, -53), S(-91, -37)
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(36, 33), S(76, -1), S(80, 4), S(-40, 134), S(31, 52), S(-25, 80), S(41, -5), S(-23, 19),
    S(23, 72), S(-50, 166), S(-54, 221), S(-143, 253), S(-114, 199), S(-125, 198), S(-76, 103), S(-40, 43),
    S(-19, 104), S(71, 100), S(10, 177), S(-6, 187), S(-47, 170), S(-80, 168), S(-9, 63), S(-43, 35),
    S(49, 17), S(53, 85), S(16, 121), S(5, 186), S(-18, 171), S(-27, 105), S(11, 11), S(-2, -21),
    S(14, 44), S(48, 5), S(22, 75), S(-18, 133), S(-30, 120), S(-24, 77), S(-12, -2), S(-10, -51),
    S(24, -115), S(48, -71), S(49, -2), S(-4, 25), S(7, -18), S(11, -13), S(21, -84), S(-2, -77),
    S(9, -221), S(42, -321), S(30, -185), S(46, -112), S(20, -92), S(37, -164), S(11, -101), S(-6, -99),
    S(-47, -141), S(14, -391), S(10, -379), S(37, -284), S(40, -202), S(43, -247), S(30, -208), S(-24, -131)
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-145, -383), S(90, -26), S(-68, 27), S(-178, 52), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(185, -118), S(1, 139), S(17, 125), S(121, 55), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(-42, 53), S(62, 131), S(101, 100), S(86, 55), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(-262, 77), S(27, 92), S(19, 89), S(-35, 74), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(-235, 34), S(-52, 65), S(-35, 70), S(-118, 106), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(-140, 4), S(52, 7), S(-39, 58), S(-82, 83), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(75, -87), S(129, -47), S(41, 0), S(-41, 43), S(16, -16), S(16, -16), S(16, -16), S(16, -16),
    S(-48, -116), S(82, -106), S(-19, -70), S(-36, -69), S(16, -16), S(16, -16), S(16, -16), S(16, -16)
};

// Epoch duration: 3.39711
// clang-format on
}  // namespace Clockwork
