#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(302, 325);
inline const PParam KNIGHT_MAT = S(875, 659);
inline const PParam BISHOP_MAT = S(937, 654);
inline const PParam ROOK_MAT   = S(1190, 1198);
inline const PParam QUEEN_MAT  = S(2677, 2232);
inline const PParam TEMPO_VAL  = S(58, 16);

inline const PParam BISHOP_PAIR_VAL   = S(78, 177);
inline const PParam ROOK_OPEN_VAL     = S(102, -27);
inline const PParam ROOK_SEMIOPEN_VAL = S(38, 14);

inline const PParam DOUBLED_PAWN_VAL = S(-36, -78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-75, -4);
inline const PParam OUTPOST_KNIGHT_VAL    = S(6, 51);
inline const PParam OUTPOST_BISHOP_VAL    = S(29, 27);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(53, 17);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(55, -14);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(33, 33);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(53, -51);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 20), S(62, 31), S(73, 70), S(186, 141), S(523, 261), S(674, 716)
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(63, 43), S(59, 32), S(65, 58), S(145, 120), S(620, -52)
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-68, -96), S(-58, -77), S(-32, -2), S(22, 78), S(49, 212), S(283, 308)
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(50, -44), S(37, -6), S(21, 28), S(22, 77), S(85, 157), S(159, 288)
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -46), S(5, 2), S(1, -27), S(7, -47), S(2, -93), S(-190, -144)
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(12, 100), S(-22, 87), S(-14, 36), S(0, 7), S(9, 12), S(39, 9), S(18, -3)
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-186, -55), S(28, -8), S(-14, 40), S(9, 70), S(14, 95), S(34, 94), S(-15, 114)
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-226, -229), S(-121, -66), S(-66, -12), S(-23, 22), S(23, 38), S(48, 77), S(84, 74), S(118, 77),
    S(163, 20)
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-254, -293), S(-181, -109), S(-108, -47), S(-73, 3), S(-42, 34), S(-25, 56), S(-6, 71), S(13, 77),
    S(33, 81), S(46, 77), S(72, 66), S(136, 19), S(166, -3), S(229, -39)
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-283, -276), S(-150, -95), S(-99, -30), S(-67, -20), S(-40, 3), S(-26, 25), S(-8, 37), S(10, 44),
    S(27, 56), S(45, 65), S(63, 67), S(75, 69), S(96, 72), S(107, 58), S(253, -70)
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-787, -665), S(-279, -536), S(-176, -466), S(-117, -299), S(-106, -109), S(-67, -10), S(-62, 95), S(-38, 106),
    S(-32, 159), S(-20, 183), S(-11, 206), S(-5, 220), S(15, 210), S(26, 220), S(33, 212), S(47, 206),
    S(55, 195), S(57, 198), S(85, 149), S(111, 106), S(133, 79), S(189, 0), S(193, -1), S(281, -118),
    S(262, -127), S(231, -118), S(22, -42), S(-38, -54)
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(289, 140), S(93, -120), S(-9, -33), S(-24, 5), S(-51, 8), S(-85, 13), S(-64, 15), S(-73, 9),
    S(-74, -39)
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(230, 260), S(316, 228), S(386, 182)
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(289, 328), S(325, 320), S(424, 283)
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(473, 516), S(541, 466), S(525, 450), S(576, 451), S(632, 385)
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(785, 813), S(745, 842), S(726, 854), S(780, 806), S(942, 713), S(1096, 608)
};

inline const PParam PAWN_THREAT_KNIGHT = S(236, 59);
inline const PParam PAWN_THREAT_BISHOP = S(204, 104);
inline const PParam PAWN_THREAT_ROOK   = S(196, 58);
inline const PParam PAWN_THREAT_QUEEN  = S(175, -59);

inline const PParam KNIGHT_THREAT_BISHOP = S(104, 74);
inline const PParam KNIGHT_THREAT_ROOK   = S(240, 7);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -73);

inline const PParam BISHOP_THREAT_KNIGHT = S(109, 35);
inline const PParam BISHOP_THREAT_ROOK   = S(239, 57);
inline const PParam BISHOP_THREAT_QUEEN  = S(191, 44);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(-1, -10), S(-3, -5), S(-3, -15), S(-8, -26), S(-14, -31), S(-19, -38), S(-20, -45), S(-27, -40),
    S(-36, -44)
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(112, 165), S(108, 210), S(170, 178), S(235, 59), S(183, 53), S(167, 118), S(63, 140), S(120, 118),
    S(60, 38), S(170, 67), S(148, 9), S(149, -48), S(103, -65), S(49, -17), S(10, 29), S(-40, 32),
    S(-18, 1), S(2, 5), S(19, -39), S(7, -54), S(-10, -58), S(-52, -51), S(-93, -4), S(-119, 17),
    S(-43, -47), S(-26, -19), S(-29, -52), S(-48, -49), S(-73, -58), S(-93, -50), S(-144, -1), S(-162, -13),
    S(-45, -77), S(17, -74), S(-32, -30), S(-63, -28), S(-83, -37), S(-122, -39), S(-140, -26), S(-164, -32),
    S(-35, -70), S(92, -65), S(48, -29), S(-9, -11), S(-46, -24), S(-81, -29), S(-107, -6), S(-144, -18)
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-385, -158), S(-329, 53), S(-414, 214), S(-120, 69), S(-243, 90), S(-322, 95), S(-542, 75), S(-524, -23),
    S(3, 1), S(69, 12), S(170, -56), S(117, 9), S(118, 16), S(57, -9), S(-3, 12), S(-23, -32),
    S(60, -26), S(103, 17), S(193, 8), S(145, 32), S(144, 21), S(64, 29), S(54, 4), S(-41, 12),
    S(114, 8), S(105, 29), S(134, 34), S(113, 61), S(121, 48), S(89, 41), S(64, -1), S(39, 7),
    S(103, -11), S(129, -15), S(123, 10), S(95, 27), S(87, 35), S(84, 29), S(57, 2), S(42, -52),
    S(16, -21), S(46, -34), S(42, -14), S(52, 30), S(59, 27), S(3, 4), S(7, -34), S(-32, -39),
    S(17, -5), S(39, -34), S(23, -29), S(23, -10), S(10, -16), S(-17, -37), S(-6, -50), S(-62, -118),
    S(-31, -55), S(7, -13), S(25, -37), S(33, -30), S(26, -23), S(-20, -53), S(-33, -28), S(-82, -80)
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-169, 67), S(-192, 49), S(-426, 73), S(-308, 87), S(-260, 91), S(-425, 116), S(-175, 95), S(-124, 69),
    S(0, -44), S(-19, 32), S(2, 14), S(-19, 18), S(-44, 33), S(-6, 24), S(-29, 14), S(-62, 17),
    S(28, 15), S(73, 4), S(150, 12), S(80, 11), S(56, 14), S(31, 26), S(89, -2), S(-11, 15),
    S(45, -30), S(58, 1), S(92, 4), S(91, 29), S(97, 29), S(36, 27), S(25, 3), S(-22, 10),
    S(46, -58), S(53, -21), S(60, -5), S(60, 19), S(52, 36), S(13, 26), S(-4, -11), S(-7, -53),
    S(58, -50), S(97, -36), S(102, -22), S(43, 18), S(26, 24), S(26, 19), S(55, -24), S(23, -46),
    S(46, -84), S(94, -61), S(65, -48), S(37, -19), S(29, -34), S(29, -45), S(12, -30), S(31, -93),
    S(41, -64), S(28, -19), S(32, -12), S(43, -47), S(52, -57), S(48, -15), S(40, -43), S(23, -46)
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(105, 6), S(169, 5), S(99, 35), S(100, 29), S(109, 17), S(57, 30), S(65, 33), S(73, 38),
    S(17, 62), S(103, 37), S(176, 15), S(104, 58), S(119, 45), S(67, 55), S(8, 72), S(-1, 79),
    S(6, 39), S(153, -1), S(183, -7), S(183, -11), S(138, -2), S(64, 39), S(81, 27), S(-37, 76),
    S(-25, 33), S(51, 26), S(82, 18), S(105, -19), S(73, 5), S(13, 53), S(-6, 52), S(-76, 60),
    S(-88, -16), S(-8, -11), S(-23, 7), S(-39, 7), S(-45, 4), S(-62, 43), S(-91, 40), S(-110, 29),
    S(-111, -37), S(-38, -63), S(-43, -33), S(-63, -32), S(-46, -51), S(-95, 3), S(-97, -14), S(-119, -17),
    S(-172, -26), S(-74, -86), S(-50, -71), S(-46, -68), S(-52, -64), S(-71, -47), S(-91, -70), S(-122, -54),
    S(-138, -25), S(-107, -20), S(-55, -55), S(-30, -72), S(-44, -57), S(-56, -45), S(-71, -53), S(-88, -37)
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(60, -6), S(107, -47), S(112, -35), S(8, 80), S(56, 25), S(3, 47), S(51, -20), S(-15, 5),
    S(21, 71), S(-47, 159), S(-42, 201), S(-123, 226), S(-104, 185), S(-122, 193), S(-77, 104), S(-41, 42),
    S(-22, 105), S(67, 100), S(11, 172), S(-5, 182), S(-46, 166), S(-81, 169), S(-11, 65), S(-45, 36),
    S(45, 19), S(49, 86), S(13, 123), S(0, 190), S(-22, 177), S(-31, 109), S(7, 17), S(-5, -20),
    S(10, 46), S(43, 10), S(17, 80), S(-25, 144), S(-37, 132), S(-30, 87), S(-18, 7), S(-15, -44),
    S(19, -108), S(43, -65), S(43, 5), S(-11, 33), S(0, -7), S(4, -2), S(15, -73), S(-8, -69),
    S(3, -210), S(36, -309), S(23, -176), S(40, -102), S(14, -83), S(30, -152), S(4, -89), S(-12, -90),
    S(-51, -133), S(9, -376), S(4, -368), S(31, -273), S(34, -190), S(36, -233), S(24, -194), S(-28, -122)
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-208, -360), S(55, -17), S(-62, 23), S(-151, 43), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(155, -111), S(0, 138), S(18, 124), S(129, 52), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(-43, 52), S(63, 130), S(103, 98), S(87, 53), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(-261, 76), S(26, 91), S(17, 89), S(-36, 73), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(-232, 32), S(-52, 63), S(-35, 69), S(-118, 105), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(-138, 1), S(51, 5), S(-39, 57), S(-82, 82), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(76, -89), S(128, -49), S(41, -2), S(-40, 42), S(17, -16), S(17, -16), S(17, -16), S(17, -16),
    S(-46, -119), S(82, -107), S(-17, -72), S(-34, -70), S(17, -16), S(17, -16), S(17, -16), S(17, -16)
};

// Epoch duration: 3.35737s
// Epoch 101/1000
// clang-format on
}  // namespace Clockwork
