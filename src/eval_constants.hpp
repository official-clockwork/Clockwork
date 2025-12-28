#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(301, 307);
inline const PParam KNIGHT_MAT = S(916, 615);
inline const PParam BISHOP_MAT = S(835, 526);
inline const PParam ROOK_MAT   = S(1359, 1179);
inline const PParam QUEEN_MAT  = S(2765, 1705);
inline const PParam TEMPO_VAL  = S(65, 13);

inline const PParam BISHOP_PAIR_VAL   = S(84, 168);
inline const PParam ROOK_OPEN_VAL     = S(111, -29);
inline const PParam ROOK_SEMIOPEN_VAL = S(40, 17);

inline const PParam DOUBLED_PAWN_VAL = S(-40, -75);

inline const PParam POTENTIAL_CHECKER_VAL = S(-67, -10);
inline const PParam OUTPOST_KNIGHT_VAL    = S(53, 36);
inline const PParam OUTPOST_BISHOP_VAL    = S(61, 28);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(52, 3);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(59, -26);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(34, 34);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(69, -58);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(26, 15), S(62, 32), S(77, 69), S(188, 146), S(497, 246), S(721, 709)
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(73, 38), S(64, 33), S(75, 66), S(178, 115), S(632, -29)
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-84, -98), S(-82, -75), S(-57, 5), S(24, 73), S(102, 197), S(316, 281)
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(44, -36), S(33, -1), S(27, 24), S(20, 77), S(112, 126), S(217, 224)
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(5, -33), S(-1, 5), S(-6, -20), S(-7, -41), S(-22, -87), S(-265, -111)
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(1, 113), S(-9, 88), S(-3, 33), S(-1, 8), S(11, 9), S(47, 7), S(21, -9)
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-280, -12), S(-32, 25), S(-10, 48), S(32, 62), S(38, 84), S(51, 90), S(17, 92)
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-234, -218), S(-125, -59), S(-69, -1), S(-29, 31), S(18, 40), S(47, 75), S(88, 65), S(126, 65),
    S(175, 4)
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-251, -289), S(-178, -101), S(-107, -43), S(-71, 0), S(-44, 36), S(-21, 55), S(-7, 69), S(14, 73),
    S(33, 81), S(52, 72), S(76, 59), S(137, 16), S(147, 12), S(216, -36)
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-279, -358), S(-165, -63), S(-108, -18), S(-70, -13), S(-43, 8), S(-28, 34), S(-10, 48), S(8, 51),
    S(25, 61), S(41, 69), S(61, 69), S(76, 72), S(91, 74), S(119, 53), S(277, -87)
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-833, -482), S(-295, -631), S(-193, -534), S(-140, -337), S(-131, -131), S(-100, 7), S(-95, 108), S(-69, 129),
    S(-61, 178), S(-50, 200), S(-39, 229), S(-30, 232), S(-9, 219), S(-7, 238), S(-1, 236), S(11, 226),
    S(26, 206), S(23, 214), S(39, 181), S(68, 143), S(80, 123), S(134, 49), S(144, 42), S(258, -89),
    S(356, -174), S(439, -263), S(125, -86), S(343, -233)
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(516, -126), S(185, -152), S(51, -46), S(6, 2), S(-45, 15), S(-106, 40), S(-155, 78), S(-206, 100),
    S(-246, 85)
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(269, 228), S(357, 199), S(424, 158)
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(478, 377), S(295, 239), S(164, 74)
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(425, 395), S(536, 393), S(560, 409), S(645, 461), S(767, 434)
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(1124, 911), S(777, 741), S(457, 560), S(201, 319), S(81, 12), S(-51, -326)
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-113, 60), S(-1, 0), S(-224, -129), S(42, 43), S(-307, -189)
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-26, 23), S(-23, 21), S(-23, 15), S(-15, 9), S(-21, -11)
};

inline const PParam PAWN_THREAT_KNIGHT = S(247, 58);
inline const PParam PAWN_THREAT_BISHOP = S(232, 122);
inline const PParam PAWN_THREAT_ROOK   = S(209, 84);
inline const PParam PAWN_THREAT_QUEEN  = S(189, -39);

inline const PParam KNIGHT_THREAT_BISHOP = S(121, 69);
inline const PParam KNIGHT_THREAT_ROOK   = S(257, 13);
inline const PParam KNIGHT_THREAT_QUEEN  = S(166, -56);

inline const PParam BISHOP_THREAT_KNIGHT = S(121, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(248, 66);
inline const PParam BISHOP_THREAT_QUEEN  = S(199, 51);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(1, -17), S(-4, -5), S(-4, -16), S(-9, -23), S(-16, -29), S(-22, -34), S(-25, -43), S(-29, -48),
    S(-39, -52)
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(185, 136), S(126, 199), S(191, 162), S(166, 103), S(218, 36), S(177, 93), S(93, 128), S(168, 98),
    S(36, 29), S(109, 75), S(87, 19), S(52, -11), S(45, -44), S(12, -8), S(-10, 24), S(-55, 34),
    S(-16, -8), S(-11, 10), S(4, -33), S(-9, -42), S(-23, -49), S(-56, -39), S(-91, -4), S(-104, 9),
    S(-31, -56), S(-15, -26), S(-12, -49), S(-32, -48), S(-59, -54), S(-76, -47), S(-123, -9), S(-143, -18),
    S(-38, -81), S(25, -74), S(-29, -29), S(-42, -31), S(-75, -33), S(-107, -36), S(-127, -24), S(-145, -36),
    S(-37, -72), S(88, -65), S(37, -26), S(-5, -10), S(-41, -20), S(-70, -28), S(-91, -12), S(-128, -18)
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-433, -145), S(-401, 80), S(-499, 264), S(-89, 43), S(-159, 39), S(-265, 7), S(-594, 122), S(-501, -80),
    S(-26, 8), S(37, -5), S(126, -42), S(126, -10), S(117, 0), S(52, -11), S(23, 1), S(-14, -38),
    S(59, -44), S(101, 6), S(155, 20), S(133, 37), S(99, 39), S(57, 43), S(49, -5), S(-31, 5),
    S(96, 8), S(99, 18), S(113, 45), S(89, 77), S(108, 69), S(79, 53), S(70, 11), S(43, -11),
    S(90, -5), S(133, -18), S(110, 29), S(104, 44), S(83, 54), S(83, 38), S(72, 0), S(42, -14),
    S(26, -28), S(60, -38), S(44, 3), S(66, 28), S(70, 24), S(18, 17), S(21, -33), S(-21, -40),
    S(30, -35), S(55, -53), S(40, -38), S(42, -13), S(24, -8), S(0, -46), S(11, -46), S(-35, -109),
    S(-24, -64), S(11, -1), S(35, -40), S(51, -43), S(40, -33), S(-9, -51), S(-19, -24), S(-76, -85)
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-164, 57), S(-178, 51), S(-407, 90), S(-319, 79), S(-264, 90), S(-428, 125), S(-211, 109), S(-141, 71),
    S(4, -34), S(-30, 31), S(-27, 10), S(-70, 42), S(-88, 47), S(-34, 34), S(-20, 15), S(-41, 6),
    S(42, -5), S(79, -5), S(100, 17), S(52, 29), S(31, 19), S(36, 19), S(61, 3), S(10, -18),
    S(38, -30), S(45, 2), S(77, 6), S(62, 46), S(88, 39), S(29, 21), S(26, -9), S(-6, -8),
    S(41, -48), S(60, -27), S(63, 2), S(72, 18), S(60, 28), S(21, 26), S(-6, -4), S(-5, -44),
    S(50, -41), S(97, -40), S(111, -23), S(60, 13), S(51, 3), S(51, -2), S(61, -22), S(12, -24),
    S(42, -91), S(117, -77), S(82, -58), S(57, -35), S(38, -26), S(40, -47), S(35, -42), S(34, -87),
    S(44, -71), S(34, -25), S(44, -18), S(55, -49), S(66, -57), S(61, -12), S(50, -43), S(28, -49)
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(106, 8), S(163, 7), S(86, 43), S(143, 5), S(123, 6), S(66, 29), S(82, 24), S(100, 24),
    S(0, 64), S(78, 42), S(144, 27), S(93, 49), S(115, 36), S(78, 36), S(-3, 70), S(-13, 76),
    S(-8, 45), S(114, 8), S(155, -2), S(144, 0), S(120, -3), S(63, 30), S(71, 27), S(-25, 76),
    S(-38, 35), S(37, 33), S(82, 19), S(89, -3), S(68, 11), S(21, 37), S(-7, 47), S(-63, 54),
    S(-84, -12), S(-8, -6), S(-29, 12), S(-30, 4), S(-32, 2), S(-43, 30), S(-77, 29), S(-94, 22),
    S(-109, -40), S(-47, -44), S(-43, -22), S(-59, -29), S(-45, -44), S(-77, 2), S(-82, -20), S(-123, -9),
    S(-181, -26), S(-75, -87), S(-50, -66), S(-45, -64), S(-47, -66), S(-63, -49), S(-78, -70), S(-116, -49),
    S(-139, -31), S(-113, -20), S(-49, -59), S(-23, -72), S(-39, -58), S(-52, -42), S(-70, -45), S(-88, -26)
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(19, 19), S(70, -20), S(77, -14), S(-29, 118), S(18, 41), S(-14, 64), S(20, -19), S(-56, 50),
    S(-1, 88), S(-37, 165), S(-57, 192), S(-176, 235), S(-152, 206), S(-102, 174), S(-64, 80), S(-42, 73),
    S(-36, 101), S(41, 105), S(-39, 178), S(-48, 186), S(-96, 177), S(-90, 161), S(-13, 42), S(-49, 40),
    S(34, 15), S(32, 73), S(-13, 126), S(-11, 159), S(-34, 165), S(-46, 117), S(14, 17), S(17, -40),
    S(11, 15), S(36, 20), S(11, 64), S(-11, 107), S(-10, 98), S(-11, 56), S(8, -8), S(15, -51),
    S(18, -97), S(47, -55), S(51, 16), S(20, 2), S(21, -17), S(30, 0), S(34, -59), S(10, -71),
    S(8, -201), S(52, -270), S(55, -183), S(69, -125), S(51, -113), S(55, -146), S(20, -79), S(19, -107),
    S(-37, -133), S(24, -360), S(31, -372), S(52, -255), S(62, -191), S(57, -239), S(47, -209), S(-6, -137)
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-651, -130), S(-65, 85), S(-51, 70), S(-173, 33), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(62, -68), S(62, 119), S(148, 75), S(162, -2), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-35, 72), S(178, 66), S(222, 44), S(172, 5), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-287, 84), S(158, 18), S(100, 19), S(66, 11), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-278, 55), S(79, -9), S(103, -12), S(28, 17), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-168, 30), S(154, -46), S(111, -21), S(84, -2), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-10, -35), S(179, -69), S(128, -41), S(61, -7), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9),
    S(-210, -22), S(-25, -30), S(-108, -1), S(-127, -2), S(-2, -9), S(-2, -9), S(-2, -9), S(-2, -9)
};

// Epoch duration: 8.05849s
// Epoch duration: 6.87929s
// clang-format on
}  // namespace Clockwork
