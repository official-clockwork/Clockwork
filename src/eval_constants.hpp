#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(298, 321);
inline const PParam KNIGHT_MAT = S(1089, 895);
inline const PParam BISHOP_MAT = S(1206, 950);
inline const PParam ROOK_MAT   = S(1640, 1672);
inline const PParam QUEEN_MAT  = S(3372, 2936);
inline const PParam TEMPO_VAL  = S(58, 15);

inline const PParam BISHOP_PAIR_VAL   = S(77, 174);
inline const PParam ROOK_OPEN_VAL     = S(101, -24);
inline const PParam ROOK_SEMIOPEN_VAL = S(38, 14);

inline const PParam DOUBLED_PAWN_VAL = S(-36, -78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-75, 0);
inline const PParam OUTPOST_KNIGHT_VAL    = S(7, 51);
inline const PParam OUTPOST_BISHOP_VAL    = S(42, 43);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(54, 18);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(55, -14);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(33, 33);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(52, -45);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 19), S(62, 31), S(72, 69), S(185, 139), S(520, 249), S(641, 696)
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(63, 42), S(59, 31), S(65, 57), S(143, 117), S(607, -50)
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-68, -93), S(-57, -75), S(-31, -2), S(22, 75), S(50, 207), S(281, 301)
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(49, -42), S(36, -4), S(20, 29), S(21, 78), S(83, 157), S(163, 282)
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -45), S(4, 2), S(0, -26), S(6, -45), S(0, -90), S(-190, -140)
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(12, 98), S(-20, 86), S(-13, 36), S(0, 7), S(9, 12), S(39, 9), S(18, -3)
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-183, -53), S(27, -6), S(-12, 40), S(10, 69), S(15, 94), S(35, 93), S(-12, 113)
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-223, -225), S(-120, -64), S(-65, -11), S(-23, 23), S(23, 38), S(48, 76), S(84, 72), S(117, 75),
    S(162, 17)
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-251, -286), S(-179, -105), S(-106, -44), S(-72, 6), S(-42, 37), S(-25, 57), S(-6, 71), S(12, 77),
    S(31, 80), S(45, 76), S(69, 63), S(133, 17), S(162, -3), S(226, -40)
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-279, -287), S(-149, -94), S(-98, -30), S(-66, -20), S(-40, 4), S(-26, 26), S(-8, 38), S(10, 44),
    S(26, 56), S(44, 65), S(62, 67), S(74, 69), S(95, 71), S(107, 57), S(249, -68)
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-996, -893), S(-275, -534), S(-179, -405), S(-120, -242), S(-108, -58), S(-68, 34), S(-63, 137), S(-38, 145),
    S(-31, 195), S(-19, 218), S(-9, 239), S(-3, 250), S(17, 238), S(29, 245), S(38, 234), S(54, 224),
    S(64, 208), S(69, 205), S(101, 150), S(131, 101), S(159, 65), S(227, -31), S(226, -30), S(330, -166),
    S(286, -159), S(247, -162), S(10, -98), S(-91, -115)
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(286, 141), S(91, -117), S(-10, -31), S(-24, 5), S(-50, 7), S(-83, 13), S(-63, 14), S(-72, 8),
    S(-73, -39)
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(0, 0), S(85, -28), S(154, -73)
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(0, 0), S(35, -4), S(134, -40)
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(0, 0), S(66, -45), S(48, -58), S(96, -55), S(143, -115)
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(0, 0), S(-52, 64), S(-84, 99), S(-48, 78), S(81, 24), S(211, -61)
};

inline const PParam PAWN_THREAT_KNIGHT = S(234, 57);
inline const PParam PAWN_THREAT_BISHOP = S(200, 99);
inline const PParam PAWN_THREAT_ROOK   = S(194, 57);
inline const PParam PAWN_THREAT_QUEEN  = S(175, -63);

inline const PParam KNIGHT_THREAT_BISHOP = S(102, 71);
inline const PParam KNIGHT_THREAT_ROOK   = S(238, 6);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -77);

inline const PParam BISHOP_THREAT_KNIGHT = S(108, 34);
inline const PParam BISHOP_THREAT_ROOK   = S(237, 55);
inline const PParam BISHOP_THREAT_QUEEN  = S(192, 35);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(1, -6), S(-1, 0), S(0, -10), S(-5, -21), S(-11, -26), S(-16, -32), S(-17, -39), S(-23, -37),
    S(-33, -42)
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(115, 160), S(110, 205), S(168, 175), S(231, 59), S(181, 52), S(164, 117), S(63, 139), S(120, 116),
    S(60, 37), S(169, 65), S(147, 9), S(146, -46), S(101, -63), S(48, -16), S(10, 30), S(-39, 32),
    S(-18, 0), S(1, 4), S(18, -40), S(6, -53), S(-11, -58), S(-52, -51), S(-92, -4), S(-118, 17),
    S(-43, -47), S(-26, -20), S(-31, -53), S(-49, -49), S(-73, -58), S(-93, -50), S(-142, -1), S(-160, -13),
    S(-45, -77), S(16, -74), S(-34, -31), S(-64, -29), S(-84, -38), S(-121, -39), S(-139, -25), S(-162, -31),
    S(-35, -69), S(91, -64), S(46, -29), S(-10, -11), S(-47, -24), S(-81, -28), S(-106, -4), S(-141, -17)
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-377, -151), S(-317, 48), S(-393, 198), S(-118, 66), S(-237, 87), S(-313, 91), S(-522, 68), S(-511, -22),
    S(1, 1), S(66, 12), S(165, -54), S(115, 7), S(116, 15), S(56, -8), S(-4, 12), S(-23, -31),
    S(58, -26), S(103, 13), S(189, 9), S(142, 32), S(141, 22), S(62, 31), S(51, 5), S(-42, 13),
    S(112, 7), S(103, 27), S(132, 34), S(112, 60), S(119, 48), S(87, 42), S(62, 0), S(38, 7),
    S(101, -12), S(125, -15), S(120, 9), S(92, 27), S(84, 36), S(81, 30), S(55, 3), S(41, -52),
    S(15, -21), S(45, -34), S(39, -14), S(50, 30), S(57, 27), S(1, 5), S(7, -34), S(-33, -39),
    S(17, -6), S(38, -35), S(22, -29), S(22, -10), S(9, -16), S(-18, -36), S(-7, -49), S(-63, -115),
    S(-31, -53), S(7, -13), S(24, -38), S(32, -31), S(25, -23), S(-21, -53), S(-34, -27), S(-82, -76)
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-167, 65), S(-188, 47), S(-418, 71), S(-302, 85), S(-255, 88), S(-415, 112), S(-171, 92), S(-121, 65),
    S(-1, -43), S(-17, 34), S(3, 14), S(-17, 18), S(-41, 33), S(-6, 25), S(-26, 16), S(-62, 16),
    S(26, 13), S(72, 3), S(149, 12), S(79, 11), S(55, 14), S(32, 26), S(88, -2), S(-12, 15),
    S(43, -32), S(55, 1), S(91, 4), S(89, 29), S(95, 29), S(35, 27), S(23, 3), S(-23, 9),
    S(44, -59), S(51, -21), S(59, -4), S(58, 20), S(50, 37), S(11, 27), S(-4, -10), S(-9, -53),
    S(58, -50), S(103, -31), S(104, -20), S(47, 24), S(28, 29), S(28, 25), S(59, -17), S(22, -46),
    S(44, -85), S(93, -62), S(63, -49), S(36, -20), S(27, -35), S(28, -46), S(10, -31), S(29, -94),
    S(39, -65), S(26, -21), S(31, -15), S(41, -48), S(50, -59), S(46, -18), S(38, -44), S(21, -46)
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(101, 7), S(164, 6), S(95, 35), S(97, 29), S(106, 17), S(55, 30), S(63, 33), S(71, 38),
    S(18, 60), S(102, 36), S(174, 15), S(104, 55), S(119, 43), S(68, 52), S(9, 70), S(0, 77),
    S(6, 38), S(151, -1), S(181, -8), S(181, -12), S(137, -3), S(65, 38), S(80, 26), S(-35, 75),
    S(-24, 33), S(50, 26), S(82, 17), S(105, -19), S(73, 4), S(13, 52), S(-5, 52), S(-74, 59),
    S(-87, -16), S(-8, -11), S(-23, 6), S(-39, 6), S(-44, 3), S(-62, 43), S(-90, 40), S(-109, 29),
    S(-110, -37), S(-37, -63), S(-43, -34), S(-63, -32), S(-46, -51), S(-95, 3), S(-96, -15), S(-118, -17),
    S(-170, -26), S(-72, -86), S(-49, -72), S(-45, -69), S(-52, -64), S(-70, -48), S(-90, -70), S(-120, -54),
    S(-137, -23), S(-106, -19), S(-54, -54), S(-30, -70), S(-43, -56), S(-56, -44), S(-71, -52), S(-87, -35)
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(86, -57), S(136, -99), S(145, -89), S(42, 27), S(85, -20), S(30, 3), S(75, -62), S(3, -30),
    S(29, 49), S(-37, 136), S(-26, 167), S(-98, 183), S(-87, 155), S(-112, 173), S(-75, 97), S(-37, 31),
    S(-9, 77), S(75, 80), S(25, 145), S(5, 160), S(-39, 150), S(-79, 166), S(-12, 66), S(-44, 35),
    S(47, 8), S(48, 81), S(14, 115), S(-2, 190), S(-27, 182), S(-35, 115), S(-2, 33), S(-10, -10),
    S(6, 47), S(37, 15), S(10, 88), S(-36, 161), S(-49, 152), S(-41, 105), S(-27, 22), S(-24, -27),
    S(12, -99), S(35, -55), S(32, 21), S(-23, 56), S(-12, 17), S(-10, 24), S(4, -52), S(-16, -54),
    S(-6, -194), S(24, -287), S(12, -153), S(26, -74), S(0, -55), S(17, -125), S(-8, -64), S(-21, -74),
    S(-60, -111), S(-2, -352), S(-7, -344), S(18, -244), S(20, -159), S(23, -203), S(12, -169), S(-38, -99)
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-216, -336), S(47, -14), S(-77, 27), S(-152, 44), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(141, -102), S(2, 135), S(20, 121), S(135, 50), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(-45, 52), S(67, 128), S(105, 97), S(91, 52), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(-258, 75), S(29, 89), S(21, 88), S(-30, 72), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(-229, 31), S(-49, 63), S(-30, 68), S(-111, 103), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(-134, 0), S(52, 6), S(-36, 57), S(-77, 82), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(76, -87), S(127, -45), S(41, 0), S(-37, 42), S(17, -17), S(17, -17), S(17, -17), S(17, -17),
    S(-43, -118), S(83, -104), S(-14, -70), S(-31, -69), S(17, -17), S(17, -17), S(17, -17), S(17, -17)
};
// Epoch duration: 61.8411s
// clang-format on
}  // namespace Clockwork
